//
//  HeadPoseEstimation.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-22.
//
//


#include <cmath>
#include "asm_face/ft.hpp"
#include "jzp_lib/jzplib_all.h"

vector<Point3f> findBestFrontalFaceShape(shape_model smodel) ;

// test pushing to github by xcode
int main(int argc, const char * argv[])
{
    
    ft_data ftdata;
    if (argc<2) {
        return 0;
    }
    string fname =string(argv[1]);
    ftdata = load_ft_jzp(fname);
    
    face_tracker tracker = load_ft<face_tracker>(string(ftdata.baseDir+"trackermodel.yaml").c_str());
    tracker.detector.baseDir = ftdata.baseDir;
    
    FileStorage fileLoader(string(ftdata.baseDir+"cameraparams.yaml"),FileStorage::READ);
    Mat cameraMatrix, distCoeffs;
    fileLoader["cameraMatrix"]>>cameraMatrix;
    fileLoader["distCoeffs"] >> distCoeffs;
    fileLoader.release();
    
    shape_model smodel = load_ft<shape_model>(string(ftdata.baseDir+"shapemodel.yaml").c_str());
    vector<Point3f> faceFeatures = findBestFrontalFaceShape(smodel);
    vector<Point3f> faceCrdRefVecs;
    faceCrdRefVecs.push_back(Point3f(0,0,0));
    faceCrdRefVecs.push_back(Point3f(50,0,0));
    faceCrdRefVecs.push_back(Point3f(0,50,0));
    faceCrdRefVecs.push_back(Point3f(0,0,50));
    
    VideoCapture cam;
    if(argc > 2)cam.open(argv[2]); else cam.open(0);
    if(!cam.isOpened()){
        return 0;
    }
    Mat rvec, tvec, rmatrix;
    while(cam.get(CV_CAP_PROP_POS_AVI_RATIO) < 0.999999){
        Mat im = captureImage(cam);
        tracker.track(im);
        
        vector<Point2f> featuresTruPts = tracker.points;
        fliplr(featuresTruPts, im.size());
        
        
        solvePnP(faceFeatures, featuresTruPts, cameraMatrix, distCoeffs, rvec, tvec);
        cout<<"distance:"<<norm(tvec)<<endl;
        Mat reprjPtsMat, reprjCrdRefMat;
        projectPoints(faceFeatures, rvec, tvec, cameraMatrix, distCoeffs, reprjPtsMat);
        projectPoints(faceCrdRefVecs, rvec, tvec, cameraMatrix, distCoeffs, reprjCrdRefMat);
        vector<Point2f> reprjPts = Mat2PointsVector(reprjPtsMat);
        vector<Point2f> reprjCrdRefPts = Mat2PointsVector(reprjCrdRefMat);
        fliplr(reprjPts, im.size());
        fliplr(reprjCrdRefPts, im.size());
        Rodrigues(rvec, rmatrix);
        
        vector<Point2f> canthusPts(tracker.points.begin(),tracker.points.begin()+4);
        vector<Point2f> nosePts(tracker.points.begin()+4,tracker.points.begin()+6);
        
        float eyePairTileAngle = calculateEyePairTileAngle(canthusPts);
        Point2f eyePairCenter = caculateEyePairCenter(canthusPts);
        
        Mat M = getRotationMatrix2D(eyePairCenter, eyePairTileAngle, 1.0);
        Mat Mback = getRotationMatrix2D(eyePairCenter, -eyePairTileAngle, 1.0);
        
        vector<Point2f> rotatedCanthusPts = rotatePointsByRotationMatrix(canthusPts, M);
        vector<Point2f> rotatedNosePts = rotatePointsByRotationMatrix(nosePts, M);
        vector<Point2f> rotatedReprjPts = rotatePointsByRotationMatrix(reprjPts, M);
        vector<Point2f> rotatedReprjCrdRefPts = rotatePointsByRotationMatrix(reprjCrdRefPts, M);
        float eyePairRectWidth =abs(rotatedCanthusPts[2].x - rotatedCanthusPts[3].x)+1;
        Size2f eyePairRectSize(eyePairRectWidth,eyePairRectWidth/7);
        Rect cropRect(Point2f(eyePairCenter.x-eyePairRectWidth/2,eyePairCenter.y -eyePairRectWidth/14.0f),eyePairRectSize);
        Mat rotated_img, cropped;
        warpAffine(im, rotated_img, M, im.size(),INTER_CUBIC);
        getRectSubPix(rotated_img, eyePairRectSize, eyePairCenter, cropped);
        
        Rect leftEyeRect = Rect(0,0,rotatedCanthusPts[0].x-rotatedCanthusPts[2].x,eyePairRectSize.height);
        Rect rightEyeRect = Rect(rotatedCanthusPts[1].x-rotatedCanthusPts[2].x,0,rotatedCanthusPts[3].x-rotatedCanthusPts[1].x,eyePairRectSize.height);
        
        Mat leftEyeImg , rightEyeImg;
        leftEyeImg = cropped(leftEyeRect);
        rightEyeImg = cropped(rightEyeRect);
        Point leftEyeCenter =   findEyeCenterByColorSegmentation(leftEyeImg);
        Point rightEyeCenter ; //=  findEyeCenterByColorSegmentation(rightEyeImg);
        leftEyeCenter += leftEyeRect.tl();
        leftEyeCenter += cropRect.tl();
        rightEyeCenter += rightEyeRect.tl();
        rightEyeCenter += cropRect.tl();
        
        Point leftEC_unrotated = rotatePointByRotationMatrix(leftEyeCenter, Mback);
        Point rightEC_unroated = rotatePointByRotationMatrix(rightEyeCenter, Mback);
        
        circle(rotated_img, leftEyeCenter, 3, Scalar(0,255,0));
        circle(rotated_img, rightEyeCenter, 3, Scalar(0,255,0));
        
        circle(im, leftEC_unrotated, 3, Scalar(0,255,0));
        circle(im, rightEC_unroated, 3, Scalar(0,255,0));
        drawPoints(rotated_img, rotatedReprjPts);
        line(rotated_img, rotatedReprjCrdRefPts[0], rotatedReprjCrdRefPts[1], Scalar(255,0,0),2,CV_AA);
        line(rotated_img, rotatedReprjCrdRefPts[0], rotatedReprjCrdRefPts[2], Scalar(0,255,0),2,CV_AA);
        line(rotated_img, rotatedReprjCrdRefPts[0], rotatedReprjCrdRefPts[3], Scalar(0,0,255),2,CV_AA);
        
        tracker.timer.display_fps(cropped,Point(1,cropped.rows-1));
        imshow("rotated",rotated_img);
        
        int c = waitKey(1);
        if(c == 'q')break;
        else if(c == 'd') tracker.reset();
        
    }
    
}

vector<Point3f> findBestFrontalFaceShape(shape_model smodel)  {
    int currentIndex = -1;
    vector<vector<Point2f> > pointsSeries = smodel.matY2pts();
    
    currentIndex = 0;
    vector<Point2f> points = pointsSeries[currentIndex];
    Point2f center = points[0]*0.5f + points[1]*0.5f;
    float normVaue = 83.0f/norm(points[3]-points[2]);
    for (int i = 0 ; i < points.size(); i++) {
        points[i] =points[i]-  center;
        points[i] *=normVaue;
    }
    vector<Point3f> faceFeatures;
    for (int i =0 ; i < points.size() ; i ++) {
        faceFeatures.push_back(Point3f(points[i].x,points[i].y,0));
    }
    faceFeatures[4].z = 8;
    faceFeatures[5].z = 8;
    
    return faceFeatures;
}

