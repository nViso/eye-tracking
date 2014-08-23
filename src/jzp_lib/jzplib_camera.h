//
//  jzplib_camera.h
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-23.
//
//

#ifndef __JZP_EYE_TRACKING__jzplib_camera__
#define __JZP_EYE_TRACKING__jzplib_camera__

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

Mat cameraMatrixByCropNResize(Mat originalCameraMatrix, Size originalSize, Rect currentRect, float resizeFactor);

#endif /* defined(__JZP_EYE_TRACKING__jzplib_camera__) */
