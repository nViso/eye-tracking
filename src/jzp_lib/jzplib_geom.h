//
//  jzplib_geom.h
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#ifndef __OPENCV_HOTSHOTS__jzplib_geom__
#define __OPENCV_HOTSHOTS__jzplib_geom__

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

float rad2deg(float radian);
float deg2rad(float degree);
vector<Point2f> Mat2PointsVector(Mat mat);
float tileRadianBtwn2Pts(Point2f left, Point2f right);
float calculateEyePairTileAngle(vector<Point2f> canthusPts) ;
Point2f caculateEyePairCenter(vector<Point2f> canthusPts);
Point2f rotatePointByRotationMatrix(Point2f src,Mat M);
vector<Point2f> rotatePointsByRotationMatrix(vector<Point2f> original, Mat RM);
Mat rotatePointsByRotationMatrix_C2(Mat pointPerRow, Mat RM) ;
void fliplr(vector<Point2f>& points,Size imageSize);

Point rectBL(Rect rect);
Point rectTR(Rect rect);
Point rectBR(Rect rect);

Rect findBiggestSquare(Mat original_img);

#endif /* defined(__OPENCV_HOTSHOTS__jzplib_geom__) */


