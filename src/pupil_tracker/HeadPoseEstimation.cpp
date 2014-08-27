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
vector<Point2f> findBestFrontalFaceShape2D(ft_data smodel);

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
    
    vector<Point2f> frontPerspective2D = findBestFrontalFaceShape2D(ftdata);
    
    VideoCapture cam;
    if(argc > 2)cam.open(argv[2]); else cam.open(0);
    if(!cam.isOpened()){
        return 0;
    }
    Mat rvec, tvec;
    Mat im;
    captureImage(cam,im);
    cout<<"original cm:"<<endl<<cameraMatrix<<endl;
    
//    Mat cameraMatrix2 = cameraMatrixByCropNResize(cameraMatrix,im.size(),findBiggestSquare(im),0.4f);
//    cout<<"now cm: "<<endl<<cameraMatrix2<<endl;
    while(cam.get(CV_CAP_PROP_POS_AVI_RATIO) < 0.999999){
        captureImage(cam,im);
//        im = imresize(im, 0.4f);
        tracker.track(im);
        
        vector<Point2f> featuresTruPts = tracker.points;
        fliplr(featuresTruPts, im.size());
        Mat hM = findHomography(featuresTruPts, frontPerspective2D, CV_RANSAC);
        
        Mat frontim;
        Mat gray;
        warpPerspective(im.clone(), frontim, hM, im.size());
        imshow("front", frontim);
        
        
        
        
        solvePnP(faceFeatures, featuresTruPts, cameraMatrix, distCoeffs, rvec, tvec);
//        cout<<"distance:"<<norm(tvec)<<endl;
        Mat reprjPtsMat, reprjCrdRefMat;
        projectPoints(faceFeatures, rvec, tvec, cameraMatrix, distCoeffs, reprjPtsMat);
        projectPoints(faceCrdRefVecs, rvec, tvec, cameraMatrix, distCoeffs, reprjCrdRefMat);
        vector<Point2f> reprjPts = Mat2PointsVector(reprjPtsMat);
        vector<Point2f> reprjCrdRefPts = Mat2PointsVector(reprjCrdRefMat);
        fliplr(reprjPts, im.size());
        fliplr(reprjCrdRefPts, im.size());
        
        vector<Point2f> canthusPts(tracker.points.begin(),tracker.points.begin()+4);
        vector<Point2f> nosePts(tracker.points.begin()+4,tracker.points.begin()+6);
        
        drawPoints(im, reprjPts);
        line(im, reprjCrdRefPts[0], reprjCrdRefPts[1], Scalar(255,0,0),2);
        line(im, reprjCrdRefPts[0], reprjCrdRefPts[2], Scalar(0,255,0),2);
        line(im, reprjCrdRefPts[0], reprjCrdRefPts[3], Scalar(0,0,255),2);
        
        tracker.timer.display_fps(im,Point(1,im.rows-1));
        imshow("head pose",im);
        
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

vector<Point2f> findBestFrontalFaceShape2D(ft_data ft)  {
    return ft.points[0];
    
}

