//
//  Camcorder.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#include "Camcorder.h"
#include <jzp_lib/jzplib_all.h>

// Include Boost headers for system time and threading
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread.hpp"
#include <boost/atomic.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace cv;
using namespace boost;

boost::mutex mutexLock;

// Code for capture thread
void captureFunc(Mat *frame,VideoCapture *capture){

    Mat capframe , flip2;
    //loop infinitely
    for(;;){
        
        mutexLock.lock();
		(*capture) >> (*frame);
        flip(*frame, flip2, 1);
        *frame = flip2;
        mutexLock.unlock();
	}
}

//main
int main (int argc, char *argv[])
{
	//vars
	time_duration td, td1;
	ptime nextFrameTimestamp, currentFrameTimestamp, initialLoopTimestamp, finalLoopTimestamp;
	int delayFound = 0;
    
	// initialize capture on default source
	VideoCapture capture(0);
    
	// set framerate to record and capture at
	int framerate = 25;
    
	// Get the properties from the camera
	double width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    
	// print camera frame size
	cout << "Camera properties\n";
	cout << "width = " << width << endl <<"height = "<< height << endl;
    
	// Create a matrix to keep the retrieved frame
	Mat frame;
    
    
	// Create the video writer
	VideoWriter video("/Users/ZhipingJiang/capture.avi",CV_FOURCC('D','I','V','X'), framerate, cvSize((int)width,(int)height) );
    
	// initialize initial timestamps
	nextFrameTimestamp = microsec_clock::local_time();
	currentFrameTimestamp = nextFrameTimestamp;
	td = (currentFrameTimestamp - nextFrameTimestamp);
    
	// start thread to begin capture and populate Mat frame
	boost::thread captureThread(captureFunc, &frame, &capture);
    
	// loop infinitely
	for(;;)
	{
        
		      
        mutexLock.lock();
        while(td.total_microseconds() < 1000000/framerate){
			currentFrameTimestamp = microsec_clock::local_time();
			td = (currentFrameTimestamp - nextFrameTimestamp);
		}
        
		initialLoopTimestamp = microsec_clock::local_time();
        

		video << frame;
        
        
        cout << nextFrameTimestamp << " " << currentFrameTimestamp << " ";
        
		// add 1second/framerate time for next loop pause
		nextFrameTimestamp = nextFrameTimestamp + microsec(1000000/framerate);
        
		// reset time_duration so while loop engages
		td = (currentFrameTimestamp - nextFrameTimestamp);
        
		finalLoopTimestamp = microsec_clock::local_time();
		td1 = (finalLoopTimestamp - initialLoopTimestamp);
		delayFound = td1.total_milliseconds();
        
        
        mutexLock.unlock();
        

		cout << delayFound << endl;
        
	}
    
	// Exit
	return 0;
}