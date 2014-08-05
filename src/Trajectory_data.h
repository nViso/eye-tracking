//
//  Trajectory_data.h
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#ifndef __OPENCV_HOTSHOTS__Trajectory_data__
#define __OPENCV_HOTSHOTS__Trajectory_data__

#include <iostream>
#include <opencv2/opencv.hpp>
#include <map>

using namespace std;
using namespace cv;


class Trajectory {
public:
    const map<string,int> ppi_map = {{"MacBook_Air_11",135},{"iPhone4Above",326}, {"MacBook_Retina_13",227}, {"MacBook_Retina_15",220}};
    vector<Point2f> points;
    vector<float> speeds;
    float ppi;
    
    void addSegments(Point2f endPoint, float speed) ;
    int getCloestPointIndex(Point2f pt, float thresh) ;
    void drawTrajectory(Mat image);
    
    Point2f toPixelPoint(int index);
    Point2f fromPixelPoint(Point2f point);
    
};

#endif /* defined(__OPENCV_HOTSHOTS__Trajectory_data__) */
