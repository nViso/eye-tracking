//
//  Camcorder.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//

#include <jzp_lib/jzplib_all.h>
#include <boost/lexical_cast.hpp>


using namespace boost::posix_time;


int main (int argc, char *argv[])
{

	VideoCapture capture(0);
    
//	int framerate = 0;
//	double width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
//	double height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
//	cout << "Camera properties\n";
//	cout << "width = " << width << endl <<"height = "<< height << endl;
    
	Mat frame;
    
//    string ccstr = "MJPG";
//	VideoWriter video("/Users/ZhipingJiang/capture.avi",CV_FOURCC(ccstr[0],ccstr[1],ccstr[2],ccstr[3]), framerate, cvSize((int)width,(int)height) );
//    
//    bool captureFinishSign = false, writeEnabled = true;
//	boost::thread captureThread(videoCaptureThreadFun,&capture,&frame,framerate,&captureFinishSign);
//    BlockDelayer::delay_sec(10);
//    captureFinishSign = true;
//    BlockDelayer::delay_milisec(100);
//    
//    int frameCount = 0;
    LowpassFPSTimer timer(10);
    while (true) {
        timer.tick();
        captureImage(capture, frame);
        imshow("im",frame);
        waitKey(1);
        cout<<1.0/timer.tock()<<endl;
    }
    
	return 0;
}