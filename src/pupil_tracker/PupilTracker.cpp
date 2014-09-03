
//#include "asm_face/ft.hpp"
//#include "jzp_lib/jzplib_all.h"
#include "ASMPupilTracker.h"
#include <boost/lexical_cast.hpp>

int main(int argc, const char * argv[])
{
    
    ft_data ftdata;
    if (argc<2) {
        return 0;
    }
    fs::path baseDirPath(argv[1]);
    ASM_Pupil_Tracker pupilTracker(baseDirPath / "trackermodel.yaml");
    
    VideoCapture cam;
    if(argc > 2)cam.open(argv[2]); else cam.open(0);
    if(!cam.isOpened()){
        return 0;
    }
    Mat origin, im ;
    Mat rotated_img, cropped;
    Mat leftEyeImg , rightEyeImg;
    LowpassFPSTimer timer(20);
    float zoomRatio = 1.0f;
    CSVFileWriter csvlogger;
    while(true){
        timer.tick();
        captureImage(cam, origin);
        imresize(origin,zoomRatio,im);
        pupilTracker.processFrame(im);
        csvlogger.addSlot(pupilTracker.toDataSlot());
        drawPoints(im, pupilTracker.canthusPts);
        drawPoints(im, pupilTracker.nosePts);
        circle(im, pupilTracker.leftEyePoint, 3, Scalar(0,255,0));
        circle(im, pupilTracker.rightEyePoint, 3, Scalar(0,255,0));
        
        imshow("im",im);
        
        int c = waitKey(1);
        if(c == 'q')break;
        else if(c == 'd') pupilTracker.reDetectFace();
    }
    csvlogger.writeToFile(fs::path("/Users/ZhipingJiang/test.txt"));
    return 0;
}
