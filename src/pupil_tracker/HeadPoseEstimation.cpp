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

    fs::path baseDirPath(argv[1]);
    ASM_Gaze_Tracker poseTracker(baseDirPath / "trackermodel.yaml", fs::path(argv[2]));
    
    
    vector<Point3f> faceCrdRefVecs;
    faceCrdRefVecs.push_back(Point3f(0,0,0));
    faceCrdRefVecs.push_back(Point3f(50,0,0));
    faceCrdRefVecs.push_back(Point3f(0,50,0));
    faceCrdRefVecs.push_back(Point3f(0,0,50));
    
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

        
        
        
        Mat frontim,flipback;
        flip(im,flipback,1);
        
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
        
        vector<Point2f> transformedPoints = poseTracker.tracker.points;
        fliplr(transformedPoints, im.size());
        Mat part;
        
        Mat hM = findHomography(poseTracker.facialPointsIn2D ,transformedPoints, 0);
        warpPerspective(flipback(boundingRect(transformedPoints)), frontim, hM, im.size());
        imshow("front", im);

        
        int c = waitKey(1)%256;
        if(c == 'q')break;
        else if(c == 'd') poseTracker.reDetectFace();
        
    }
    
}
