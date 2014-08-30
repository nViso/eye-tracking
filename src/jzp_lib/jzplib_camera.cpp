//
//  jzplib_camera.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-23.
//
//

#include "jzplib_camera.h"
#include <boost/lockfree/spsc_queue.hpp>

using namespace boost::lockfree;

Mat cameraMatrixByCropNResize(const Mat originalCameraMatrix, Size originalSize, Rect currentRect, float resizeFactor) {
    Mat nowcm = originalCameraMatrix;
    cout<<nowcm.channels()<<" "<<nowcm.size()<<endl;
    Point tl = currentRect.tl();
    nowcm.at<double>(0, 2) -= double(tl.x);
    nowcm.at<double>(1, 2) -= double(tl.y);
    float s = nowcm.at<double>(0,1);
    nowcm *= (resizeFactor);
    nowcm.at<double>(0,1) = s;
    nowcm.at<double >(2,2) = 1.0;
    cout<<nowcm<<endl;
    return nowcm;
}

void captureImage(VideoCapture& capture, Mat& color_img) {
    Rect squareRect;
    Mat origin;
    if (&capture && capture.isOpened()) {
        capture >> origin;
        //    squareRect =findBiggestSquare(color_img);
        //    color_img(squareRect).copyTo(color_img);
        flip(origin, color_img, 1);
    }
    
}

void videoCaptureThreadFun(VideoCapture *capture, Mat *frame, int framerate, bool * finishSign){
    //loop infinitely
    int frameCount = 0;
    cout<<"start capturing @ "<<framerate<<" fps..."<<endl;
    LowpassFPSTimer timer(10);
    FPScontroller delayer(framerate);
    while (!(*finishSign) && capture->isOpened()) {
        timer.tick();
		captureImage((*capture), (*frame));
        frameCount ++;
        delayer.controlledDelay();
	}
    cout<<"capture stopped. totally "<<frameCount<<" frames , or "<<(double)frameCount/framerate<<" seconds."<<endl;
}

void videoBufferThreadFun(spsc_queue<Mat,boost::lockfree::capacity<512> > * buffer, Mat *frame, int framerate, int totalFrames, bool * writeEnabled,bool * finishSign) {
    using namespace boost::posix_time;
    int frameCount = 0;
    cout<<"start recording @ "<<framerate<<" fps..."<<endl;
    LowpassFPSTimer timer(1);
    FPScontroller * delayer;
    Mat temp;
	while ((*finishSign)==false && (totalFrames <=0 || frameCount <totalFrames)) {
        
        if ((*writeEnabled)) {
            buffer->push((*frame));
            if(frameCount ==0) {
                delayer = new FPScontroller(framerate);
            }
            
            frameCount ++;
            timer.tick();
            delayer->controlledDelay();
        }
        
	}
    cout<<"buffer finished, totally "<<frameCount<<" frames , or "<<(double)frameCount/framerate<<" seconds."<<endl;
}

void videoWriterThreadFun(VideoWriter * writer, spsc_queue<Mat,boost::lockfree::capacity<512> >* buffer, int framerate, bool * writeEnabled, bool * finishSign) {
    using namespace boost::posix_time;
    int frameCount = 0;
    cout<<"start writing @ "<<framerate<<" fps..."<<endl;
    FPScontroller delayer(framerate*2);
	while ((*finishSign)==false || !buffer->empty()) {
        if (!buffer->empty()) {
            Mat frame;
            buffer->pop(frame);
            (*writer) << frame;
            frameCount ++;
        }
        delayer.controlledDelay();
	}
    cout<<"writer finished, totally "<<frameCount<<" frames."<<endl;
}