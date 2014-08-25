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