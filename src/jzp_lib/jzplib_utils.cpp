//
//  jzplib_utils.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-23.
//
//

#include "jzplib_utils.h"

LowpassFPSTimer::LowpassFPSTimer(int lowpassLength) {
    this->lowpassLength = lowpassLength;
    durationValues = vector<double>(lowpassLength);
    for (int i = 0 ; i < lowpassLength; i++) {
        durationValues.push_back(999);
    }
}

void LowpassFPSTimer::tick() {
    if (timer.is_stopped()) {
        timer.start();
    } else {
        timer.resume();
    }
}

double LowpassFPSTimer::tock() {
    cpu_times t = timer.elapsed();
    
    durationValues.erase(durationValues.begin());
    durationValues.push_back((double)t.wall/1000000000.0L);
    
    double sum = 0.0;
    for (int i = 0 ; i < durationValues.size(); i ++) {
        sum += durationValues[i];
    }
    sum /= durationValues.size();
    timer.stop();
//    timer.start();
    return sum;
}


FPScontroller::FPScontroller(int fps) {
    this->fps = fps;
    boost::asio::io_service ioservice;
    blockTimer = new deadline_timer(ioservice);
    nextFrameTimestamp = boost::asio::time_traits<boost::posix_time::ptime>::now();
}

void FPScontroller::controlledDelay() {
    if (fps <= 0 ) {
        return;
    }
    nextFrameTimestamp = nextFrameTimestamp + microsec(1000000/fps);
    blockTimer->expires_at(nextFrameTimestamp);
    blockTimer->wait();
//    cout<<" pass blocking "<<endl;
//    do {
//        currentFrameTimestamp = microsec_clock::local_time();
//        td = (currentFrameTimestamp - nextFrameTimestamp);
//    } while(td.total_microseconds() < 1000000/fps);
    
}

boost::asio::deadline_timer * BlockDelayer::blockTimer;

void BlockDelayer::delay_microsec_private(double microsec) {
     if (blockTimer == NULL) {
         boost::asio::io_service ioservice;
         blockTimer = new deadline_timer(ioservice);
     }
    blockTimer->expires_from_now(boost::posix_time::microseconds(microsec));
    blockTimer->wait();
}

void BlockDelayer::delay_microsec(double microsec) {
    BlockDelayer::delay_microsec_private(microsec);
}

void BlockDelayer::delay_milisec(double milisec) {
    BlockDelayer::delay_microsec_private(milisec*1000);
}

void BlockDelayer::delay_sec(double sec) {
    BlockDelayer::delay_microsec_private(sec*1e6);
}