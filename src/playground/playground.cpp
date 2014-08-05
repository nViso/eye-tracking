//
//  playground.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#include "playground.h"


#include "opencv_hotshots/ft/ft.hpp"


float ppi = 135.0f;
float widthInch = 3.0f , heightInch = 4.0f;
//==============================================================================
class Trajectory{
public:
    float widthInch, heightInch;
    float ppi;
    vector<Point2f> pointsVector;
    vector<float> speedVector;
    
    
    void addSegment(Point2f p, float speed) {
        if (pointsVector.size() >0) {
            pointsVector.push_back(p);
            speedVector.push_back(speed);
        } else{
            pointsVector.push_back(p);
        }
        
    }
    
    void drawTrajectory(Mat blank) {
        for (int i = 0 ; i < pointsVector.size(); i++) {
            circle(blank, pointsVector[i]*ppi, 3, Scalar(0,0,255));
        }
    }
    
    

 } trajectory;


void mouseCallback(int event, int x, int y, int /*flags*/, void* /*param*/)
{
    if(event == CV_EVENT_LBUTTONDOWN){
        trajectory.addSegment(Point2f(x/trajectory.ppi,y/trajectory.ppi), 10);
//        trajectory.drawTrajectory(blank);
    }else if(event == CV_EVENT_MOUSEMOVE){
        
    }
}


//==============================================================================
int main(int argc,char** argv)
{
    
    
    string windowName = "PlayGround";
    namedWindow(windowName);
//    imshow(windowName,curImg);
    setMouseCallback(windowName,mouseCallback,0);
    while (true) {
//        imshow(windowName,curImg);
        int c = waitKey(10);
        if(c == 'q')break;

    }
        
    
    
    
}
//==============================================================================
