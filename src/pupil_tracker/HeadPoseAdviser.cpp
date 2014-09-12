//
//  HeadPoseAdviser.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-9-10.
//
//

#include "ASMGazeTracker.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, const char * argv[])
{
    string windowName;
    VideoCapture cam;
    
    if (argc<2) {
        cout<<argv[0]<<" userProfileDir"<<endl;
        return 0;
    } else if (argc == 2) {
        windowName = "Head Position Advizer";
        cam.open(0);
    }
    
    if(!cam.isOpened()){
        cout<<"cannot open camera."<<endl;
        return 0;
    }
    
    fs::path baseDirPath(argv[1]);
    ASM_Gaze_Tracker pupilTracker(baseDirPath / "trackermodel.yaml");
    windowName += ( " by profile ["+baseDirPath.string()+"]");
    
    Mat origin, im ;
    float zoomRatio = 1.0f;
    while(true){
        bool success = captureImage(cam, origin, true);
        if (success == false) {
            break;
        }
        
        imresize(origin,zoomRatio,im);
        bool succeeded = pupilTracker.featureTracking(im);
        if (succeeded)
            pupilTracker.calculatePupilCenter();


        drawPoints(im, pupilTracker.canthusPts);
        drawPoints(im, pupilTracker.nosePts);
        circle(im, pupilTracker.leftEyePoint, 3, Scalar(0,255,0));
        circle(im, pupilTracker.rightEyePoint, 3, Scalar(0,255,0));
        
        imshow(windowName,im);
        int c = waitKey(1);
        if(c == 'q')
            break;
        if(c == 'd')
            pupilTracker.reDetectFace();
    }


}
