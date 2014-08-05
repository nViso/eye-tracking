//
//  jzplib_detections.h
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#ifndef __OPENCV_HOTSHOTS__jzplib_detections__
#define __OPENCV_HOTSHOTS__jzplib_detections__

#include <iostream>
#include <opencv2/opencv.hpp>
#include "jzplib_img.h"
#include "jzplib_draw.h"
#include "jzplib_bwOps.h"
#include "jzplib_math.h"
#include "skinColorDetection.h"
#include "GraphUtils.h"

using namespace std;
using namespace cv;


Rect haarPatternDetection(CascadeClassifier classifier, Mat image, int imageWidthforDetection, Rect parentRect);

Point findMassCenter_BinaryBiggestBlob(Mat bw_img);

Point findEyeCenterByMassCenter(Mat image);
Point findEyeCenterByMassCenter2(Mat image);
Point findEyeCenterBySymmetryDetection(Mat image, float pupilRadius) ;
Point findEyeCenterByColorSegmentation(Mat image, float coordinateWeight= 0.35, int kmeansIterations = 4, int kmeansRepeats= 5, int blurSize= 5);

Mat calculateImageSymmetryScore(Mat image);

Point findEyeCenterByGradient(Mat image);


#endif /* defined(__OPENCV_HOTSHOTS__jzplib_detections__) */
