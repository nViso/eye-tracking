
//#include "asm_face/ft.hpp"
//#include "jzp_lib/jzplib_all.h"
#include "ASMPupilTracker.h"
#include <boost/lexical_cast.hpp>

int main(int argc, const char * argv[])
{
    string windowName;
    VideoCapture cam;
    bool dumpFile = false;
    fs::path inputFilePath;
    if (argc<2) {
        cout<<argv[0]<<" userProfileDir"<<" [dumping Video file]"<<endl;
        return 0;
    } else if (argc == 2) {
        windowName = "Pupil tracking from camera";
        cam.open(0);
    } else if(argc > 2) {
        dumpFile = true;
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
    while(true){
        bool success = captureImage(cam, origin, !dumpFile);
        if (success == false) {
            break;
        }
        imresize(origin,zoomRatio,im);
        bool processSuccess = pupilTracker.processFrame(im);
        csvlogger.addSlot(pupilTracker.toDataSlot());
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
        csvlogger.writeToFile(inputFilePath.parent_path() / (inputFilePath.stem().string() + ".txt"));
    }
    
    return 0;
}
