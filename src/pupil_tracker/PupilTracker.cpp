
//#include "asm_face/ft.hpp"
//#include "jzp_lib/jzplib_all.h"
#include "ASMPupilTracker.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, const char * argv[])
{
    string windowName;
    VideoCapture cam;
    bool dumpFile = false;
    bool noShow = false;
    fs::path inputFilePath;
    if (argc<2) {
        cout<<argv[0]<<" userProfileDir"<<" [dumping Video file]"<<" [noshow]"<<endl;
        return 0;
    } else if (argc == 2) {
        windowName = "Pupil tracking from camera";
        cam.open(0);
    } else if(argc == 3) {
        dumpFile = true;
        cam.open(argv[2]);
        inputFilePath = fs::path(argv[2]);
        windowName = "Pupil tracking from video [" + string(argv[2]) +"]";
    } else if(argc == 4 && boost::iequals(string(argv[3]), "noshow")) {
        dumpFile = true;
        noShow = true;
        cam.open(argv[2]);
        inputFilePath = fs::path(argv[2]);
        windowName = "Pupil tracking from video [" + string(argv[2]) +"]";
    }
    
    if(!cam.isOpened()){
        cout<<"cannot open camera or video file."<<endl;
        return 0;
    }
    
    fs::path baseDirPath(argv[1]);
    ASM_Pupil_Tracker pupilTracker(baseDirPath / "trackermodel.yaml");
    windowName += ( " by profile ["+baseDirPath.string()+"]");
    
    Mat origin, im ;
    float zoomRatio = 1.0f;
    CSVFileWriter csvlogger;
    LowpassFPSTimer timer(20);
    int frameCount = 0;
    while(true){
        timer.tick();
        bool success = captureImage(cam, origin, !dumpFile);
        if (success == false) {
            break;
        }
        
        imresize(origin,zoomRatio,im);
        pupilTracker.processFrame(im);

        printf("\b\rfps: %f, frame count: %d",1.0/timer.tock(), ++frameCount);
        fflush(stdout);
        
        if (dumpFile)
        csvlogger.addSlot(pupilTracker.toDataSlot());
        
        if (noShow) {
            continue;
        }
        drawPoints(im, pupilTracker.canthusPts);
        drawPoints(im, pupilTracker.nosePts);
        circle(im, pupilTracker.leftEyePoint, 3, Scalar(0,255,0));
        circle(im, pupilTracker.rightEyePoint, 3, Scalar(0,255,0));
        imshow(windowName,im);
        int c = waitKey(1);
        if(c == 'q' && ! dumpFile)
            break;
        if(c == 'd')
            pupilTracker.reDetectFace();
    }
    
    if (dumpFile) {
        csvlogger.writeToFile(inputFilePath.parent_path() / (inputFilePath.stem().string() + ".test"));
    }
    
    return 0;
}
