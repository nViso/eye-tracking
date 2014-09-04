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

void ASM_Pupil_Tracker::processFrame(const cv::Mat & im) {
//    LowpassFPSTimer timer(1);
//    timer.tick();
    Mat  leftEyeImg,rightEyeImg,cropped;
    tracker.track(im);
    canthusPts = vector<Point2f>(tracker.points.begin(),tracker.points.begin()+4);
    nosePts = vector<Point2f>(tracker.points.begin()+4,tracker.points.begin()+6);
    
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
    
    leftEyeImg = cropped(leftEyeRect);
    rightEyeImg = cropped(rightEyeRect);
    Point leftEyeCenter = findEyeCenterByColorSegmentation(leftEyeImg);
    Point rightEyeCenter = findEyeCenterByColorSegmentation(rightEyeImg);
    leftEyeCenter += leftEyeRect.tl();
    leftEyeCenter += cropRect.tl();
    rightEyeCenter += rightEyeRect.tl();
    rightEyeCenter += cropRect.tl();
    
    leftEyePoint= rotatePointByRotationMatrix(leftEyeCenter, Mback);
    rightEyePoint= rotatePointByRotationMatrix(rightEyeCenter, Mback);
//    cout<<timer.tock()<<endl;
}