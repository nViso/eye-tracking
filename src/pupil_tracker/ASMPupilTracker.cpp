//
//  ASMPupilTracker.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-31.
//
//

#include "ASMPupilTracker.h"

ASM_Pupil_Tracker::ASM_Pupil_Tracker(fs::path path) {
    trackerFilePath = path;
    tracker = load_ft<face_tracker>(path.string());
    tracker.detector.baseDir = path.parent_path().string() + fs::path("/").make_preferred().native();
}

vector<float> ASM_Pupil_Tracker::toDataSlot() {
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
    
    return slot;
}

void ASM_Pupil_Tracker::reDetectFace() {
    tracker.reset();
}

bool ASM_Pupil_Tracker::processFrame(const cv::Mat & im) {
    Mat  leftEyeImg,rightEyeImg,cropped;
    if(tracker.track(im) == 0) {// failed
        isTrackingSuccess = false;
        return false;
    }
    canthusPts = vector<Point2f>(tracker.points.begin(),tracker.points.begin()+4);
    nosePts = vector<Point2f>(tracker.points.begin()+4,tracker.points.begin()+6);
    
    if (canthusPts[2].x < canthusPts[0].x && canthusPts[0].x < canthusPts[1].x && canthusPts[1].x < canthusPts[3].x) {
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
    
    Rect leftEyeRect = Rect(0,0,rotatedCanthusPts[0].x-rotatedCanthusPts[2].x,eyePairRectSize.height);
    Rect rightEyeRect = Rect(rotatedCanthusPts[1].x-rotatedCanthusPts[2].x,0,rotatedCanthusPts[3].x-rotatedCanthusPts[1].x,eyePairRectSize.height);
    
    if (leftEyeRect.area() < 50 || rightEyeRect.area()< 50) {
        return false;
    }
    
    Point2f leftEyeCenter, rightEyeCenter;
    
//    findEyeCenterByColorSegmentation(cropped(leftEyeRect), leftEyeCenter);
//    findEyeCenterByColorSegmentation(cropped(rightEyeRect), rightEyeCenter);
//    cout<<"debug"<<endl;
    
    boost::thread leftEyeThread(findEyeCenterByColorSegmentation, cropped(leftEyeRect), boost::ref(leftEyeCenter), 0.4,3,3,5);
    boost::thread  rightEyeThread(findEyeCenterByColorSegmentation, cropped(rightEyeRect), boost::ref(rightEyeCenter), 0.4,3,3,5);
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