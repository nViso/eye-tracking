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
    
    if (argc<3) {
        cout<<argv[0]<<" userProfileDir camera_profile"<<endl;
        return 0;
    }

    cam.open(0);
    
    if(!cam.isOpened()){
        cout<<"cannot open camera."<<endl;
        return 0;
    }
    
    ASM_Gaze_Tracker poseTracker(fs::path(argv[1])/ "trackermodel.yaml",fs::path(argv[2]));
    windowName += ( " by profile ["+string(argv[1])+"]");
    namedWindow(windowName);
    moveWindow(windowName, 43, 0);
    
    Mat origin, im ;
    float zoomRatio = 1.0f;
    
    float currentDistance;
    
    while(true){
        bool success = captureImage(cam, origin, true);
        if (success == false) {
            break;
        }
        
        imresize(origin,zoomRatio,im);
        bool succeeded = poseTracker.featureTracking(im);
        if (succeeded) {
            poseTracker.estimateFacePose();
            currentDistance = poseTracker.distanceToCamera();
        }
        
        
        line(im, Point(im.cols/2,0), Point(im.cols/2,im.rows-1), Scalar(0,255,0),3);
        line(im, Point(0,im.rows/3), Point(im.cols-1,im.rows/3), Scalar(0,255,0),3);
        circle(im, Point(im.cols/2,im.rows/3), 5, Scalar(0,0,255),-1);
        drawStringAtPoint(im, "distance:"+boost::lexical_cast<string>(currentDistance), Point(im.cols/2,im.rows/2));

        
        
        
        imshow(windowName,im);
        int c = waitKey(1);
        if(c == 'q')
            break;
        if(c == 'd')
            poseTracker.reDetectFace();
    }


}
