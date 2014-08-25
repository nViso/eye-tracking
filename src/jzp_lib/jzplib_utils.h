//
//  jzplib_utils.h
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-23.
//
//

#ifndef __JZP_EYE_TRACKING__jzplib_utils__
#define __JZP_EYE_TRACKING__jzplib_utils__

#include <iostream>
#include <vector>
#include <boost/timer/timer.hpp>
using namespace std;
using namespace boost::timer;

class LowpassFPSTimer {
private:
    cpu_timer timer;
    vector<double> durationValues;
    int lowpassLength;
public:
    LowpassFPSTimer(int lowpassLength = 50);
    void tick();
    double tock();
};

#endif /* defined(__JZP_EYE_TRACKING__jzplib_utils__) */
