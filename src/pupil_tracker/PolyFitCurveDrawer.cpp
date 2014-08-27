//
//  PolyFitCurveDrawer.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-26.
//
//

#include <jzp_lib/jzplib_all.h>
#include <boost/filesystem.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/thread.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
namespace fs = boost::filesystem;
using namespace boost::posix_time;

string windowName = "Curve Drawer";
Size fullSize(1366,720);
Mat bg(fullSize,CV_8UC3,Scalar::all(255));
vector<Point2f> controlPoints;
vector<int> pointsBezierOrders;
vector<Point2f> curvePoints;
vector<Point2f> animatePoints;
int divs = 50;
int currentBezierOrder = 4;
int distancePerFrame = 5;
int frameRate = 30;
Scalar lineColor(0,0,0);
Scalar endPointColor(0,0,0);
Scalar controlPointColor(255,0,0);

void drawInstructions() {
    drawStringAtPoint(bg, "-, = for bezier curve order : "+ boost::lexical_cast<string>(currentBezierOrder), Point(0,20));
    drawStringAtPoint(bg, "[, ] for distance/frame : "+ boost::lexical_cast<string>(distancePerFrame), Point(0,40));
    drawStringAtPoint(bg, ";, ' for frame rate : "+ boost::lexical_cast<string>(frameRate), Point(0,60));
    drawStringAtPoint(bg, "., / for curve resolution : "+ boost::lexical_cast<string>(divs), Point(0,80));
    cout<<controlPoints.size()<<" points,"<<"resolution:"<<divs<<", distance/frame:"<<distancePerFrame<<",frame rate:"<<frameRate<<",bezier order:"<<currentBezierOrder<<endl;
}




void adjustPointsOrder() {
    int ptIndex = 0;
    
    if (pointsBezierOrders.size() == 0) {
        return;
    }
    
    while (ptIndex+pointsBezierOrders[ptIndex] < controlPoints.size()) {
        ptIndex += pointsBezierOrders[ptIndex];
    }
    
    for (int i = ptIndex; i < pointsBezierOrders.size(); i++) {
        pointsBezierOrders[i] = currentBezierOrder;
    }
}

boost::mutex animationLock;

void animateFuc(Mat* bg) {
//    boost::mutex animationLock;
    animationLock.lock();
    animatePoints = curveAnimateSequence(curvePoints, distancePerFrame);
    
    for (int i = 0; i<animatePoints.size(); i++) {
        ptime startTimestamp = microsec_clock::local_time();
        ptime presentTimestamp = startTimestamp;
        time_duration td = (presentTimestamp - startTimestamp);
        while (td.total_microseconds() < 1000000/frameRate) {
            presentTimestamp = microsec_clock::local_time();
			td = (presentTimestamp - startTimestamp);
        }
        cout<<td<<endl;
        Mat(fullSize,CV_8UC3,Scalar::all(255)).copyTo((*bg));
        circle((*bg), animatePoints[i], 10, Scalar(0,0,255),-1,CV_AA);
    }
//    cout<<"animation finished"<<endl;
    animationLock.unlock();
}

void showAnimation() {
    animatePoints = curveAnimateSequence(curvePoints, distancePerFrame);
    Mat bgcopy(fullSize,CV_8UC3,Scalar::all(255));
    boost::thread animateThread(animateFuc,&bgcopy);
    
    for(;;){
        imshow(windowName, bgcopy);
        int key = waitKey(1);
        if (animationLock.try_lock()) {
            animationLock.unlock();
            break;
        }
        if (key == 's') {
            animationLock.unlock();
            break;
        }
    }
//    cout<<"stop "<<endl;
}


void drawCanvas() {
    bg = Mat(fullSize,CV_8UC3,Scalar::all(255));
    curvePoints = generalizedBezierCurve(controlPoints,pointsBezierOrders,1.0/divs);
    animatePoints = curveAnimateSequence(curvePoints, distancePerFrame);
    drawConnectedPoints(bg,curvePoints,lineColor);
    drawPoints(bg, controlPoints,controlPointColor);
    drawInstructions();
    imshow(windowName, bg);
}

void addPointMouseCallback(int event, int x, int y, int /*flags*/, void* /*param*/)
{
    if(event == CV_EVENT_LBUTTONDOWN){
        controlPoints.push_back(Point2f(x,y));
        pointsBezierOrders.push_back(currentBezierOrder);
        adjustPointsOrder();
        drawCanvas();
        
    }
}



int main (int argc, char *argv[])
{
    if (argc < 2) { // without arguments
        cout<<argv[0]<<" "<<"filepath"<<endl;
        return 0;
    }
    
    fs::path filePath(argv[1]);
    if (fs::exists(filePath)) {
        FileStorage storage;
        storage.open(filePath.string(), FileStorage::READ);
        storage["points"] >> controlPoints;
        storage["pointsOrders"] >> pointsBezierOrders;
        storage["distancePerFrame"] >> distancePerFrame;
        storage["frameRate"] >> frameRate;
    }

    namedWindow(windowName);
    moveWindow(windowName, 0, 0);
    setWindowProperty(windowName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    setMouseCallback(windowName,addPointMouseCallback,0);
    while(true) {
        adjustPointsOrder();
        drawCanvas();
        int key =  waitKey();
        switch (key) {
            case 'a':
                showAnimation();
                break;
            case '[':
                distancePerFrame ++;
                break;
            case ']':
                if (distancePerFrame>1) {
                    distancePerFrame--;
                }
                break;
            case '\'':
                frameRate ++;
                break;
            case ';':
                if (frameRate>1) {
                    frameRate--;
                }
                break;
            case '/':
                divs ++;
                break;
            case '.':
                if (divs>1) {
                    divs--;
                }
                break;
            case '=':
                currentBezierOrder++;
                break;
            case '-':
                if (currentBezierOrder>1) {
                    currentBezierOrder--;
                }
                break;
            case 127: // backspace
                if (controlPoints.size() >0) {
                    controlPoints.erase(controlPoints.end()-1);
                    pointsBezierOrders.erase(pointsBezierOrders.end()-1);
                }
                break;
        }
        
        if (key == 'q') {
            break;
        }
    }
    
    FileStorage storage;
    if (fs::exists(filePath.parent_path()) == 0) {
        fs::create_directories(filePath.parent_path());
    }
    storage.open(filePath.string(), FileStorage::WRITE);
    storage << "points"<< controlPoints;
    storage << "pointsOrders"<<pointsBezierOrders;
    storage << "distancePerFrame" << distancePerFrame;
    storage << "frameRate"<< frameRate;
    storage.release();
}
