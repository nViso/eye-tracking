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
#include <boost/algorithm/string.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
namespace fs = boost::filesystem;
using namespace boost::posix_time;

string windowName = "Curve Drawer";
Size fullSize(1366,720);
Mat whitebg(fullSize,CV_8UC3,Scalar::all(255));
Mat bg;
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

void setupTrajectories(fs::path pathDirPath, vector<fs::path>& trajectoryFiles) {
    if (!fs::exists(pathDirPath)) {
        fs::create_directories(pathDirPath);
    }
    
    fs::directory_iterator iterator(pathDirPath);
    
    for (; iterator != fs::directory_iterator(); iterator++) {
        if (boost::iequals(".yaml", iterator->path().extension().string())) {
            trajectoryFiles.push_back(iterator->path());
        }
    }
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

bool animationFinished = false;
bool animationFinishSign = false;
void animateFuc(Mat* bg) {
    animationFinished = false;
    cout<<"totally "<<animatePoints.size()<<" animation frames, frame rate: "<<frameRate<<endl;
    
    boost::posix_time::ptime startTime = microsec_clock::local_time();
    FPScontroller fpsDelayer(frameRate);
    for (int i = 0; i<animatePoints.size() && animationFinishSign == false; i++) {
        Mat temp ;
        whitebg.copyTo(temp);
        circle(temp, animatePoints[i], 10, Scalar(0,0,255),-1,CV_AA);
        temp.copyTo((*bg));
        fpsDelayer.controlledDelay();
    }
    animationFinished = true;
    boost::posix_time::ptime endTime = microsec_clock::local_time();
    boost::posix_time::time_duration td = endTime - startTime;
    cout<<"animation duration"<<td<<endl;
}

void showAnimation() {
    animatePoints = curveAnimateSequence(curvePoints, distancePerFrame);
    Mat bgcopy(fullSize,CV_8UC3,Scalar::all(255));
    animationFinishSign = false;
    boost::thread animateThread(animateFuc,&bgcopy);
    
    for(;;){
        imshow(windowName, bgcopy);
        int key = waitKey(1);
        if (key == 's') {
            animationFinishSign = true;
            animateThread.join();
            break;
        }
        if (animationFinished == true) {
            break;
        }
    }
//    cout<<"stop "<<endl;
}


void drawCanvas() {
    whitebg.copyTo(bg);
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

void readPathData(fs::path filePath) {
    if (fs::exists(filePath)) {
        FileStorage storage;
        storage.open(filePath.string(), FileStorage::READ);
        storage["points"] >> controlPoints;
        storage["pointsOrders"] >> pointsBezierOrders;
        storage["distancePerFrame"] >> distancePerFrame;
        storage["frameRate"] >> frameRate;
    }
}

void writePathData(fs::path filePath) {
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

void showCurveCanvas() {
    
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
            case ']':
                distancePerFrame ++;
                break;
            case '[':
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
        
        if (key == 'q' ) {
            destroyAllWindows();
            break;
        }
    }
    
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void showSelection(int & currentIndex, vector<fs::path> & trajectoryFiles) {
    Mat bg = whitebg.clone();
    while (true) {
        cout<<"\n------------\nThere are " + boost::lexical_cast<string>(trajectoryFiles.size())+" trajectory file(s)"<<endl;
        for (int i = 0 ; i < trajectoryFiles.size(); i++) {
            cout<<"("+boost::lexical_cast<string>(i)+") "+ trajectoryFiles[i].string();
            if (currentIndex == i) {
                cout<<" (current selection)"<<endl;
            } else
                cout<<endl;
        }
        cout<<"([n]ew) Add new trajectory"<<endl;
        cout<<"([q]uit) quit."<<endl;
        cout<<"Your selection ?";
        string choice;
        cin >> choice;
        
        if (boost::starts_with(choice,"n")) {
            currentIndex = trajectoryFiles.size();
            break;
        }
        if (boost::starts_with(choice,"q")) {
            currentIndex =-1;
            break;
        }
        
        if (is_number(choice)) {
            int value = boost::lexical_cast<int>(choice);
            if (value >= 0 && value < trajectoryFiles.size()) {
                currentIndex = value;
                break;
            }
        }
    }
    
}


int main (int argc, char *argv[])
{
    
    if (argc < 2) {
        cout<<argv[0]<<" trajectoies_set_dir ( contains all trajectories .yaml files)"<<endl;
        return 0;
    }
    fs::path pathDirPath(argv[1]);
    vector<fs::path> trajectoryFiles;
    setupTrajectories(pathDirPath,trajectoryFiles);
    int currentIndex = 0;
    while (true) {
        showSelection(currentIndex,trajectoryFiles);
        if (currentIndex == -1) {
            break;
        }
        if (currentIndex < trajectoryFiles.size()) {
            readPathData(trajectoryFiles[currentIndex]);
            showCurveCanvas();
            writePathData(trajectoryFiles[currentIndex]);
        } else {
            cout<<"enter the new trajectory name (without filename extension):";
            string name;
            cin >> name;
            fs::path newpath = pathDirPath / string(name+".yaml");
            cout<<newpath<<endl;
            trajectoryFiles.push_back(newpath);
        }
    }
}
