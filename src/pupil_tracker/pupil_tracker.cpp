//
//  main.cpp
//  Pupil_ASM_Merge
//
//  Created by Zhiping Jiang on 14-7-11.
//  Copyright (c) 2014年 edu.xjtu. All rights reserved.
//
#include <cmath>
#include "opencv_hotshots/ft/ft.hpp"
#include "jzp_lib/jzplib_all.h"

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
    
    VideoCapture cam;
    if(argc > 2)cam.open(argv[2]); else cam.open(0);
    if(!cam.isOpened()){
        return 0;
    }
    
    while(cam.get(CV_CAP_PROP_POS_AVI_RATIO) < 0.999999){
        Mat im = captureImage(cam);
        im = imresize(im, 0.7f);
        tracker.track(im);

        
        vector<Point2f> canthusPts(tracker.points.begin(),tracker.points.begin()+4);
        vector<Point2f> nosePts(tracker.points.begin()+4,tracker.points.begin()+6);
        
        float eyePairTileAngle = calculateEyePairTileAngle(canthusPts);
        Point2f eyePairCenter = caculateEyePairCenter(canthusPts);
        
        Mat M = getRotationMatrix2D(eyePairCenter, eyePairTileAngle, 1.0);
        Mat Mback = getRotationMatrix2D(eyePairCenter, -eyePairTileAngle, 1.0);
        
        vector<Point2f> rotatedCanthusPts = rotatePointsByRotationMatrix(canthusPts, M);
        vector<Point2f> rotatedNosePts = rotatePointsByRotationMatrix(nosePts, M);
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
        Point rightEyeCenter =  findEyeCenterByColorSegmentation(rightEyeImg);
        leftEyeCenter += leftEyeRect.tl();
        leftEyeCenter += cropRect.tl();
        rightEyeCenter += rightEyeRect.tl();
        rightEyeCenter += cropRect.tl();
        
        Point leftEC_unrotated = rotatePointByRotationMatrix(leftEyeCenter, Mback);
        Point rightEC_unroated =rotatePointByRotationMatrix(rightEyeCenter, Mback);
        
        circle(rotated_img, leftEyeCenter, 3, Scalar(0,255,0));
        circle(rotated_img, rightEyeCenter, 3, Scalar(0,255,0));
        
        circle(im, leftEC_unrotated, 3, Scalar(0,255,0));
        circle(im, rightEC_unroated, 3, Scalar(0,255,0));
        drawPoints(rotated_img, rotatedCanthusPts);
        drawPoints(rotated_img, rotatedNosePts);
        drawPoints(im, canthusPts);
        
        tracker.timer.display_fps(cropped,Point(1,cropped.rows-1));
        imshow("cropped",imresize(cropped,2.0f));
        imshow("rotated",rotated_img);
        
        
        
        
        
        int c = waitKey(1);
        if(c == 'q')break;
        else if(c == 'd') tracker.reset();
        
    }
    
}

