//
//  HeadPoseEstimation.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-22.
//
//


#include <cmath>
#include "ASMGazeTracker.h"


int main(int argc, const char * argv[])
{
    
    ft_data ftdata;
    if (argc<3) {
        cout<<argv[0]<<" user_profile_dir camera_profile.yaml";
        return 0;
    }
//    string fname =string(argv[1]);
//    ftdata = load_ft_jzp(fname);
//    face_tracker tracker = load_ft<face_tracker>(string(ftdata.baseDir+"trackermodel.yaml").c_str());
//    tracker.detector.baseDir = ftdata.baseDir;
//    
//    Mat cameraMatrix, distCoeffs;
//    readCameraProfile(fs::path(argv[2]), cameraMatrix, distCoeffs);
    
    fs::path baseDirPath(argv[1]);
    ASM_Gaze_Tracker poseTracker(baseDirPath / "trackermodel.yaml", fs::path(argv[2]));
    
    
    
//    vector<Point3f> faceFeatures ; //findBestFrontalFaceShape(smodel);
    vector<Point3f> faceCrdRefVecs;
    faceCrdRefVecs.push_back(Point3f(0,0,0));
    faceCrdRefVecs.push_back(Point3f(50,0,0));
    faceCrdRefVecs.push_back(Point3f(0,50,0));
    faceCrdRefVecs.push_back(Point3f(0,0,50));
    
//    vector<Point2f> frontPerspective2D = findBestFrontalFaceShape2D(ftdata);
    
    VideoCapture cam;
    cam.open(0);
    if(!cam.isOpened()){
        return 0;
    }
    Mat rvec, tvec;
    Mat im;
    captureImage(cam,im);
    

    while(true){
        bool success = captureImage(cam, im, true);
        if (success == false) {
            break;
        }
        
        bool succeeded = poseTracker.featureTracking(im);
        if (succeeded)
            poseTracker.estimateFacePose();

//        Mat hM = findHomography(featuresTruPts ,frontPerspective2D, 0);
//        Mat frontim;
//        Mat gray;
//        warpPerspective(im.clone(), frontim, hM, im.size());
//        imshow("front", frontim);
        
        
        
        
        vector<Point2f> reprjCrdRefPts;
        vector<Point2f> reprjFeaturePts;
        poseTracker.projectPoints(poseTracker.facialPointsIn3D, reprjFeaturePts);
        poseTracker.projectPoints(faceCrdRefVecs, reprjCrdRefPts);
        line(im, reprjCrdRefPts[0], reprjCrdRefPts[1], Scalar(255,0,0),2);
        line(im, reprjCrdRefPts[0], reprjCrdRefPts[2], Scalar(0,255,0),2);
        line(im, reprjCrdRefPts[0], reprjCrdRefPts[3], Scalar(0,0,255),2);
        drawPoints(im, reprjFeaturePts);
        drawStringAtTopLeftCorner(im, "distance to camera:" + boost::lexical_cast<string>(poseTracker.distanceToCamera()));
        imshow("head pose",im);
        
        int c = waitKey(1);
        if(c == 'q')break;
        else if(c == 'd') poseTracker.reDetectFace();
        
    }
    
}
