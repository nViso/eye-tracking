//
//  jzplib_camera.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-23.
//
//

#include "jzplib_camera.h"

Mat cameraMatrixByCropNResize(const Mat originalCameraMatrix, Size originalSize, Rect currentRect, float resizeFactor) {
    Mat nowcm = originalCameraMatrix;
    cout<<nowcm.channels()<<" "<<nowcm.size()<<endl;
    Point tl = currentRect.tl();
    nowcm.at<double>(0, 2) -= double(tl.x);
    nowcm.at<double>(1, 2) -= double(tl.y);
    float s = nowcm.at<double>(0,1);
    nowcm *= (resizeFactor);
    nowcm.at<double>(0,1) = s;
    nowcm.at<double >(2,2) = 1.0;
    cout<<nowcm<<endl;
    return nowcm;
}