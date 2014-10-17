//
//  ASMPupilTracker.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-31.
//
//

#include "ASMGazeTracker.h"

ASM_Gaze_Tracker::ASM_Gaze_Tracker(const fs::path & trackermodel, const fs::path & cameraProfile) {
    tracker = load_ft<face_tracker>(trackermodel.string());
    tracker.detector.baseDir = trackermodel.parent_path().string() + fs::path("/").make_preferred().native();
    
    if (cameraProfile.empty() == false) {
        readCameraProfile(cameraProfile, cameraMatrix, distCoeffs);
        findBestFrontalFaceShapeIn3D();
    }
}

vector<float> ASM_Gaze_Tracker::toDataSlot() {
    vector<float> slot;
    
    if (isTrackingSuccess == false) {
        for (int i = 0; i < 8; i++) {
            slot.push_back(0.0f);
            slot.push_back(0.0f);
        }
    }
    
    slot.push_back(leftEyePoint.x);
    slot.push_back(leftEyePoint.y);
    slot.push_back(rightEyePoint.x);
    slot.push_back(rightEyePoint.y);
    for (int i = 0; i < tracker.points.size(); i++) {
        slot.push_back(tracker.points[i].x);
        slot.push_back(tracker.points[i].y);
    }
    for (int i = 0 ; i < 3 ; i ++) {
        slot.push_back((float)tvec.at<double>(i));
    }
    for (int i = 0 ; i < 3 ; i ++) {
        slot.push_back((float)rvec.at<double>(i));
    }
    
    return slot;
}

bool ASM_Gaze_Tracker::featureTracking(const cv::Mat & im) {
    if(tracker.track(im) == 0) {// failed
        isTrackingSuccess = false;
        return false;
    }
    glabellaPoint = tracker.points[0]*0.5 + tracker.points[1]*0.5;
    im.copyTo(this->im);
    isTrackingSuccess = true;
    return true;
    
}

void ASM_Gaze_Tracker::reDetectFace() {
    tracker.reset();
}

bool ASM_Gaze_Tracker::calculatePupilCenter(){
    Mat  leftEyeImg,rightEyeImg,cropped;

    if (isTrackingSuccess == false) {
        return false;
    }
    
    canthusPts = vector<Point2f>(tracker.points.begin(),tracker.points.begin()+4);
    nosePts = vector<Point2f>(tracker.points.begin()+4,tracker.points.begin()+6);
    
    if (canthusPts[3].x < canthusPts[1].x && canthusPts[1].x < canthusPts[0].x && canthusPts[0].x < canthusPts[2].x) {
    } else {
        return false;
    }
    
    eyePairTileAngle = calculateEyePairTileAngle(canthusPts);
    glabellaPoint= caculateEyePairCenter(canthusPts);
    
    rotationMatrix = getRotationMatrix2D(glabellaPoint, eyePairTileAngle, 1.0);
    Mat Mback = getRotationMatrix2D(glabellaPoint, -eyePairTileAngle, 1.0);
    
    vector<Point2f> rotatedCanthusPts = rotatePointsByRotationMatrix(canthusPts, rotationMatrix);
    vector<Point2f> rotatedNosePts = rotatePointsByRotationMatrix(nosePts, rotationMatrix);
    
    float eyePairRectWidth =abs(rotatedCanthusPts[2].x - rotatedCanthusPts[3].x)+1;
    Size2f eyePairRectSize(eyePairRectWidth,eyePairRectWidth/7);
    Rect cropRect(Point2f(glabellaPoint.x-eyePairRectWidth/2,glabellaPoint.y -eyePairRectWidth/14.0f),eyePairRectSize);
    
    warpAffine(im, rotated_img, rotationMatrix, im.size(),CV_INTER_LINEAR);
    getRectSubPix(rotated_img, eyePairRectSize, glabellaPoint, cropped);
    
    Rect rightEyeRect = Rect(0,0,rotatedCanthusPts[1].x-rotatedCanthusPts[3].x,eyePairRectSize.height);
    Rect leftEyeRect = Rect(rotatedCanthusPts[0].x-rotatedCanthusPts[3].x,0,rotatedCanthusPts[2].x-rotatedCanthusPts[0].x,eyePairRectSize.height);
    
    if (leftEyeRect.area() < 50 || rightEyeRect.area()< 50) {
        return false;
    }
    
    Point2f leftEyeCenter, rightEyeCenter;
    
//    findEyeCenterByColorSegmentation(cropped(leftEyeRect), leftEyeCenter);
//    findEyeCenterByColorSegmentation(cropped(rightEyeRect), rightEyeCenter);
//    cout<<"debug"<<endl;
    
    boost::thread leftEyeThread(findEyeCenterByColorSegmentation, cropped(leftEyeRect), boost::ref(leftEyeCenter), 0.4,4,3,5);
    boost::thread  rightEyeThread(findEyeCenterByColorSegmentation, cropped(rightEyeRect), boost::ref(rightEyeCenter), 0.4,4,3,5);
    leftEyeThread.join();
    rightEyeThread.join();
    
    leftEyeCenter += Point2f(leftEyeRect.tl().x,leftEyeRect.tl().y);
    leftEyeCenter += Point2f(cropRect.tl().x, cropRect.tl().y);
    rightEyeCenter += Point2f(rightEyeRect.tl().x,rightEyeRect.tl().y);
    rightEyeCenter += Point2f(cropRect.tl().x,cropRect.tl().y);
    
    leftEyePoint= rotatePointByRotationMatrix(leftEyeCenter, Mback);
    rightEyePoint= rotatePointByRotationMatrix(rightEyeCenter, Mback);
    isTrackingSuccess = true;
    return true;
}

bool ASM_Gaze_Tracker::estimateFacePose() {
    if (isTrackingSuccess == false) {
        return false;
    }
    vector<Point2f> imagePoints = tracker.points;
    solvePnP(facialPointsIn3D, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
    return true;
}

void ASM_Gaze_Tracker::projectPoints(const vector<Point3f> & sourcePoints, vector<Point2f> & destPoints) {
    cv::projectPoints(sourcePoints, rvec, tvec, cameraMatrix, distCoeffs, destPoints);
}

float ASM_Gaze_Tracker::distanceToCamera() {
    return norm(tvec);
}

void ASM_Gaze_Tracker::findBestFrontalFaceShapeIn3D()  {
    vector<vector<Point2f> > pointsSeries = tracker.smodel.matY2pts();
    
    int bestIndex = 0;
//    float largestArea = 0.0f;
//    for (int i = 0; i < pointsSeries.size(); i++) {
//        vector<Point2f> points = pointsSeries[i];
//        Point2f center = points[0]*0.5f + points[1]*0.5f;
//        float normVaue = tracker.annotations.getDistanceBetweenOuterCanthuses()/norm(points[3]-points[2]);
//        for (int j = 0 ; j < points.size(); j++) {
//            points[j] =points[j]-  center;
//            points[j] *=normVaue;
//        }
//        points.erase(points.begin()+4, points.begin()+6);
//        points.erase(points.begin()  , points.begin()+2);
//        float area =contourArea(points);
////        cout<<i<<" "<<area<<endl;
//        if (contourArea(points) > largestArea) {
//            bestIndex = i;
//            largestArea = area;
//        }
//    }
//    cout<<"bestIndex"<<bestIndex<<endl;
    bestIndex = 0;
    vector<Point2f> points = pointsSeries[bestIndex];
    
    Point2f center = points[0]*0.5f + points[1]*0.5f;
    float normVaue = tracker.annotations.getDistanceBetweenOuterCanthuses()/norm(points[3]-points[2]);
    for (int j = 0 ; j < points.size(); j++) {
        points[j] =center - points[j] ;
        points[j].y = - points[j].y;
        points[j] *=normVaue;
    }
    vector<Point3f> faceFeatures;
    vector<Point2f> faceFeatures2;
    for (int i =0 ; i < points.size() ; i ++) {
        faceFeatures.push_back(Point3f(points[i].x,points[i].y,0));
        faceFeatures2.push_back(Point2f(points[i].x,points[i].y));
    }
    faceFeatures[4].z = 8;
    faceFeatures[5].z = 8;
    
    facialPointsIn3D = faceFeatures;
    facialPointsIn2D = faceFeatures2;
//    cout<<"facial points 2d:"<<endl<<faceFeatures2<<endl;
}