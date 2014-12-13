//
//  ASMPupilTracker.h
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-31.
//
//

#include <iostream>
#include "asm_face/ft.hpp"
#include "jzp_lib/jzplib_all.h"
#include <boost/thread.hpp>

class ASM_Gaze_Tracker {
public:
    ASM_Gaze_Tracker(const fs::path & trackermodel, const fs::path & cameraProfile = fs::path());
    bool featureTracking(const cv::Mat & im);
    void reDetectFace();
    
    bool calculatePupilCenter();
    
    void estimatePupilCenterFaceCoordinates();
    bool estimateFacePose();
    void faceImageRectification();
    void projectPoints(const vector<Point3f> & sourcePoints, vector<Point2f> & destPoints);
    float distanceToCamera();
    vector<float> toDataSlot();
    
    
    
    Point2f glabellaPoint;
    float eyePairTileAngle;
    Point2f leftEyePoint, rightEyePoint;
    Point3f leftCenterInFace, rightCenterInFace;
    vector<Point2f> canthusPts;
    vector<Point2f> nosePts;
    vector<Point2f> rectifiedPts;
    Mat rectifyTransformation;
    Mat rectifyTransformationInv;
    Mat poseRMatrix;
    bool isTrackingSuccess;
    
    Mat projectionJacobian;
    Mat rvec;
    Mat tvec;
    Mat cameraMatrix;
    Mat distCoeffs;
    Mat leftEyeImageRectified, rightEyeImageRectified;
    vector<Point3f> facialPointsIn3D;
    vector<Point2f> facialPointsIn2D;
    vector<Point2f> reprojectedFacialPointsInImage;
    fs::path trackerFilePath;
    face_tracker tracker;
    
    
private:
    
    Mat im;
    Mat rectifiedImage;
    Mat rotated_img;
    void findBestFrontalFaceShapeIn3D() ;
    static void eyeCenterLocalizationImpl(const Mat& image, Point2f & eyeCord, float coordinateWeight= 0.35, int kmeansIterations = 4, int kmeansRepeats= 3, float blurSizeRatio=0.26);
};
