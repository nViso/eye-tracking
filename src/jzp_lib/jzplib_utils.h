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
#include <boost/lexical_cast.hpp>
#include <boost/timer/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
#include <boost/asio/time_traits.hpp>
#include <deque>
#include <numeric> 

using namespace std;
using namespace boost::timer;
using namespace boost::posix_time;
using namespace boost::asio;

bool is_number(const std::string& s);

class LowpassFPSTimer {
private:
    ptime tickTime, tockTime;
    deque<double> durationValues;
    int lowpassLength;
public:
    LowpassFPSTimer(int lowpassLength = 50);
    void tick();
    double tock();
};

class FPScontroller {
public:
    FPScontroller (int fps);
    void controlledDelay();
    
private:
    boost::asio::deadline_timer * blockTimer;
    int fps;
    time_duration td;
    ptime currentFrameTimestamp;
    ptime nextFrameTimestamp ;
};

class BlockDelayer {
public:
    static void delay_milisec(double milisec);
    static void delay_microsec(double microsec);
    static void delay_sec(double sec);
private:
    static boost::asio::deadline_timer * blockTimer;
    static void delay_microsec_private(double microsec);
    
};

#endif /* defined(__JZP_EYE_TRACKING__jzplib_utils__) */
