
//#include "asm_face/ft.hpp"
//#include "jzp_lib/jzplib_all.h"
#include "ASMGazeTracker.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, const char * argv[])
{
    string windowName;
    VideoCapture cam;
    bool dumpFile = false;
    bool noShow = false;
    fs::path userProfilePath,cameraProfilePath,inputFilePath;
    int videoFrameRotation = 0;
    if (argc<3) {
        cout<<argv[0]<<" userProfileDir"<<" cameraProfile"<<" [dumping Video file]"<<" [noshow]"<<endl;
        return 0;
    } else if (argc == 3) {
        windowName = "Pupil tracking from camera";
        cam.open(0);
        userProfilePath = fs::path(argv[1]);
        cameraProfilePath = fs::path(argv[2]);
    } else if(argc == 4) {
        dumpFile = true;
        userProfilePath = fs::path(argv[1]);
        cameraProfilePath = fs::path(argv[2]);
        inputFilePath = fs::path(argv[3]);
        cam.open(inputFilePath.string());
        videoFrameRotation = readRotationMetadataForVideo(inputFilePath);
        windowName = "Pupil tracking from video [" + inputFilePath.string() +"]";
    } else if(argc == 5 && boost::iequals(string(argv[4]), "noshow")) {
        dumpFile = true;
        noShow = true;
        userProfilePath = fs::path(argv[1]);
        cameraProfilePath = fs::path(argv[2]);
        inputFilePath = fs::path(argv[3]);
        cam.open(inputFilePath.string());
        videoFrameRotation = readRotationMetadataForVideo(inputFilePath);
        windowName = "Pupil tracking from video [" + inputFilePath.string() +"]";
    }
    
    if(!cam.isOpened()){
        cout<<"cannot open camera or video file."<<endl;
        return 0;
    }
    
    ASM_Gaze_Tracker pupilTracker(userProfilePath / "trackermodel.yaml",cameraProfilePath);
    windowName += ( " by profile ["+userProfilePath.string()+"]");
    
    vector<Point3f> faceCrdRefVecs;
    faceCrdRefVecs.push_back(Point3f(0,0,0));
    faceCrdRefVecs.push_back(Point3f(50,0,0));
    faceCrdRefVecs.push_back(Point3f(0,50,0));
    faceCrdRefVecs.push_back(Point3f(0,0,50));
    
    Mat origin, im ;
    float zoomRatio = 1.0f;
    CSVFileWriter csvlogger;
    LowpassFPSTimer timer(20);
    int frameCount = 0;
    while(true){
        timer.tick();
        bool success = captureImage(cam, origin);
        if (success == false) {
            break;
        }
        
        imageOrientationFix(origin,videoFrameRotation);
        imresize(origin,zoomRatio,im);
        bool succeeded = pupilTracker.featureTracking(im);
        if (succeeded) {
            pupilTracker.calculatePupilCenter();
            pupilTracker.estimateFacePose();
        }
        
        printf("\b\rfps: %f, frame count: %d",1.0/timer.tock(), ++frameCount);
        fflush(stdout);
        
        if (dumpFile)
            csvlogger.addSlot(pupilTracker.toDataSlot());
        
        if (noShow) {
            continue;
        }
        if (succeeded) {
//            plotLiveData("tvec[0]", pupilTracker.tvec.at<double>(0));
//            plotLiveData("tvec[1]", pupilTracker.tvec.at<double>(1));
//            plotLiveData("tvec[2]", pupilTracker.tvec.at<double>(2));
            drawPoints(im, pupilTracker.canthusPts);
            drawPoints(im, pupilTracker.nosePts);
            drawPoints(im, pupilTracker.reprojectedFacialPointsInImage,Scalar(0,255,0));
            circle(im, pupilTracker.leftEyePoint, 1, Scalar(0,255,0),-1);
            circle(im, pupilTracker.rightEyePoint, 1, Scalar(0,255,0),-1);
            vector<Point2f> reprjCrdRefPts;
            pupilTracker.projectPoints(faceCrdRefVecs, reprjCrdRefPts);
            line(im, reprjCrdRefPts[0], reprjCrdRefPts[1], Scalar(255,0,0),2);
            line(im, reprjCrdRefPts[0], reprjCrdRefPts[2], Scalar(0,255,0),2);
            line(im, reprjCrdRefPts[0], reprjCrdRefPts[3], Scalar(0,0,255),2);
            drawStringAtTopLeftCorner(im, "d:" + boost::lexical_cast<string>(pupilTracker.distanceToCamera()));
        }
        imshow(windowName,im);
        int c = waitKey(1)%256;
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
