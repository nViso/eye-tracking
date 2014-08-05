//
//  Trajectory_data.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#include "Trajectory_data.h"

void Trajectory::addSegments(Point2f endPoint, float speed) {
    
    if (points.size() >0) {
        points.push_back(endPoint);
        speeds.push_back(speed);
    } else{
        points.push_back(endPoint);
    }
    
}

void Trajectory::drawTrajectory(cv::Mat image) {
    for (int i = 0 ; i < points.size(); i++) {
        circle(image, toPixelPoint(i), 3, Scalar(0,255,0));
    }
    
    for (int i = 1; i < points.size(); i++) {
        line(image, toPixelPoint(i-1), toPixelPoint(i), Scalar(255,0,0));
    }
}

int Trajectory::getCloestPointIndex(Point2f pixelPoint,float thresh)  {
    int imin = -1; double dmin = -1;
    for(int i = 0; i < points.size(); i++){
        double d = norm(fromPixelPoint(pixelPoint)-points[i]);
        if((imin < 0) || (d < dmin)){imin = i; dmin = d;}
    }
    if((dmin >= 0) && (dmin < thresh))
        return imin; else return -1;
}

Point2f Trajectory::toPixelPoint(int index) {
    return Point2f(points[index].x*ppi, points[index].y*ppi);
}

Point2f Trajectory::fromPixelPoint(Point2f point) {
    return Point2f(point.x/ppi, point.y/ppi);
}