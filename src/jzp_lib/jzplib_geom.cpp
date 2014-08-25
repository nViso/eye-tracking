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

vector<Point2f> Mat2PointsVector(const Mat& mat) {
    vector<Point2f> pts;
    mat.copyTo(pts);
    return pts;
}

//Point pointsCenter()

float tileRadianBtwn2Pts(Point2f left, Point2f right) {
    return atan((right.y - left.y)/abs(right.x-left.x));
}

float calculateEyePairTileAngle(const vector<Point2f>& canthusPts) {
    
    float tile1 = rad2deg(tileRadianBtwn2Pts(canthusPts[0], canthusPts[1]));
    float tile2 = rad2deg(tileRadianBtwn2Pts(canthusPts[2], canthusPts[3]));
    float tile3 = rad2deg(tileRadianBtwn2Pts(canthusPts[2], canthusPts[1]));
    float tile4 = rad2deg(tileRadianBtwn2Pts(canthusPts[0], canthusPts[3]));
    float tile5 = (tile3+tile4)/2.0f;
    float tile6 = tile1*0.5f + tile2*0.25f + tile5*0.25f;
    
    return tile6;
}


Point2f caculateEyePairCenter(const vector<Point2f>& canthusPts) {
    return Point ((canthusPts[2].x+canthusPts[3].x)/2,(canthusPts[2].y+canthusPts[3].y)/2);
}

Point2f rotatePointByRotationMatrix(Point2f src,Mat M) {
    Point2f result;
    result.x = M.at<double>(0,0)*src.x + M.at<double>(0,1)*src.y + M.at<double>(0,2);
    result.y = M.at<double>(1,0)*src.x + M.at<double>(1,1)*src.y + M.at<double>(1,2);
    
    return result;
}

vector<Point2f> rotatePointsByRotationMatrix(const vector<Point2f>& original,const Mat& RM) {
    vector<Point2f> pts(original);
    for (int i = 0 ; i < pts.size() ; i ++) {
        pts[i] = rotatePointByRotationMatrix(pts[i], RM);
    }
    
    return pts;
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


Rect findBiggestSquare(const Mat& original_img) {
    int width = original_img.size().width;
    int height =original_img.size().height;
    int length = width > height ? height : width;
    int x = width > height ? (width - length)/2 : 0;
    int y = width > height ? 0 :(height - length)/2 ;
    Rect squareRect(x,y,length,length);
    
    return squareRect;
}


void fliplr(vector<Point2f>& points,Size imageSize) {
    for (int i = 0 ; i < points.size(); i++) {
        points[i].x = imageSize.width - points[i].x;
    }
}