//
//  jzplib_utils.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-23.
//
//

#include "jzplib_utils.h"

bool is_number(const std::string& s)
{
    try
    {
        boost::lexical_cast<double>(s);
        return true;
    }
    catch(boost::bad_lexical_cast &)
    {
        return false;
    }
}

LowpassFPSTimer::LowpassFPSTimer(int lowpassLength) {
    this->lowpassLength = lowpassLength;
    durationValues = deque<double>();
    for (int i = 0 ; i < lowpassLength; i++) {
        durationValues.push_back(999.0);
    }
}

void LowpassFPSTimer::tick() {
    tickTime = microsec_clock::local_time();
}

double LowpassFPSTimer::tock() {
    tockTime = microsec_clock::local_time();
    durationValues.pop_front();
    durationValues.push_back((double)time_duration(tockTime-tickTime).total_milliseconds()/1000.0);
    
    double sum = 0.0;
    for (int i = 0; i < durationValues.size(); i++) {
        sum += durationValues[i];
    }
    sum /= durationValues.size();
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

void CSVFileWriter::addSlot(vector<float> slot) {
    slots.push_back(slot);
}

void CSVFileWriter::setDelimeter(const string delimeter) {
    this->delimeter = delimeter;
}

void CSVFileWriter::writeToFile(boost::filesystem::path filePath) {
    namespace fs = boost::filesystem;
    io::stream_buffer<io::file_sink> outbuf(filePath.string());
    std::ostream out(&outbuf);
    int i = 0, j =0 ;
    for (i= 0; i< slots.size(); i++) {
        for (j = 0; j < slots[i].size()-1; j++) {
            out<<setprecision(10) <<slots[i][j]<<delimeter;
        }
        out<<setprecision(10) <<slots[i][j]<<endl;
    }
    out.flush();
    outbuf.close();
}