//
//  jzplib_geom.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#include "jzplib_geom.h"


float rad2deg(float radian) {
    return radian/M_PI*180;
}

float deg2rad(float degree) {
    return degree/180*M_PI;
}

vector<Point2f> Mat2PointsVector(Mat mat) {
    vector<Point2f> pts;
    mat.copyTo(pts);
    return pts;
}

//Point pointsCenter()

float tileRadianBtwn2Pts(Point2f left, Point2f right) {
    return atan((right.y - left.y)/abs(right.x-left.x));
}

float calculateEyePairTileAngle(vector<Point2f> canthusPts) {
    
    float tile1 = rad2deg(tileRadianBtwn2Pts(canthusPts[0], canthusPts[1]));
    float tile2 = rad2deg(tileRadianBtwn2Pts(canthusPts[2], canthusPts[3]));
    float tile3 = rad2deg(tileRadianBtwn2Pts(canthusPts[2], canthusPts[1]));
    float tile4 = rad2deg(tileRadianBtwn2Pts(canthusPts[0], canthusPts[3]));
    float tile5 = (tile3+tile4)/2.0f;
    float tile6 = (tile1+tile2+tile5)/3;
    
    return tile6;
}


Point2f caculateEyePairCenter(vector<Point2f> canthusPts) {
    return Point ((canthusPts[2].x+canthusPts[3].x)/2,(canthusPts[2].y+canthusPts[3].y)/2);
}

Point2f rotatePointByRotationMatrix(Point2f src,Mat M) {
    Point2f result;
    result.x = M.at<double>(0,0)*src.x + M.at<double>(0,1)*src.y + M.at<double>(0,2);
    result.y = M.at<double>(1,0)*src.x + M.at<double>(1,1)*src.y + M.at<double>(1,2);
    
    return result;
}

vector<Point2f> rotatePointsByRotationMatrix(vector<Point2f> original, Mat RM) {
    vector<Point2f> pts(original);
    for (int i = 0 ; i < pts.size() ; i ++) {
        pts[i] = rotatePointByRotationMatrix(pts[i], RM);
    }
    
    return pts;
}

Mat rotatePointsByRotationMatrix_C2(Mat pointPerRow, Mat RM) {
    vector<Point2f> pts = Mat2PointsVector(pointPerRow);
    rotatePointsByRotationMatrix(pts,RM);
    return Mat(pts);
    
}



Point rectBL(Rect rect) {
    return Point(rect.x,rect.y+rect.height);
}

Point rectTR(Rect rect) {
    return Point(rect.x+rect.width,rect.y);
}

Point rectBR(Rect rect) {
    return Point(rect.x+rect.width,rect.y+rect.height);
}


Rect findBiggestSquare(Mat original_img) {
    int length = original_img.size().width > original_img.size().height ? original_img.size().height : original_img.size().width;
    int x = original_img.size().width > original_img.size().height ? (original_img.size().width - length)/2 : 0;
    int y = original_img.size().width > original_img.size().height ? 0 :(original_img.size().height - length)/2 ;
    Rect squareRect(x,y,length,length);
    cv::Size squareSize(length,length);
    
    return squareRect;
}
