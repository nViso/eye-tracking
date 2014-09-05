//
//  CameraCalibrator.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-9-5.
//
//

#include <jzp_lib/jzplib_all.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
namespace fs = boost::filesystem;
using namespace boost::posix_time;


int main (int argc, char *argv[])
{
    if (argc < 2) {
        cout<<argv[0]<<" cameraBaseDir"<<endl;
    }
    
    fs::path cameraBaseDir(argv[1]);
    string input;
    cout<<"type your camera name:";
    cin >> input;
    string cameraname  = input;
    fs::path imagesDir = cameraBaseDir / input;
    if (!fs::create_directories(imagesDir)) {
        cout<<"cannot create dir. exit"<<endl;
        return 0;
    }
    int gridw, gridh;
    float gridSize;
    for(;;) {
        cout<<"type the grid width ( how many blocks in the horiton ?) :";
        cin >> input;
        if (is_number(input) && boost::lexical_cast<int>(input) > 0) {
            gridw = boost::lexical_cast<int>(input);
            break;
        } else {
            cout<<"error input. (should be int number, > 0)";
        }
    }
    
    for(;;) {
        cout<<"type the grid height ( how many blocks in the vertical ?) :";
        cin >> input;
        if (is_number(input) && boost::lexical_cast<int>(input) > 0) {
            gridh = boost::lexical_cast<int>(input);
            break;
        } else {
            cout<<"error input. (should be int number, > 0)";
        }
    }
    
    for(;;) {
        cout<<"type the length of the one block (in mm) :";
        cin >> input;
        if (is_number(input) && boost::lexical_cast<float>(input) > 0) {
            gridSize = boost::lexical_cast<float>(input);
            break;
        } else {
            cout<<"error input. (should be int number, > 0)";
        }
    }
    
    
    
    VideoCapture cam;
    cam.open(0);
    if(!cam.isOpened()){
        cout<<"cannot open camera or video file."<<endl;
        return 0;
    }
    
    int imageCounter = 0;
    Mat origin;
    while(true){
        bool success = captureImage(cam, origin,false);
        imshow("Image Intake",origin);
        
        int c = waitKey(1);
        if(c == 'q')
            break;
        if (c == 32) {
            fs::path filepath = imagesDir / string("img"+boost::lexical_cast<string>(imageCounter++)+".jpg");
            imwrite(filepath.string(), origin);
        }

    }

    vector<fs::path> images = listFilesWithExtension(imagesDir, "img", "jpg");
    
    if (images.empty()) {
        cout<<"no image files."<<endl;
        return 0;
    }
    
    Mat cameraMatrix, distCoeffs;
    bool isSuccess = chessboardCameraCalibration(gridw, gridh, gridSize, images, cameraMatrix, distCoeffs,true);
    if (isSuccess == false) {
        cout<<" calibation not successful."<<endl;
        return 0;
    }
    fs::path configpath = imagesDir /("CM_"+cameraname+".yaml");
    
    FileStorage fs(configpath.string(), FileStorage::WRITE);
    fs <<"cameraMatrix"<<cameraMatrix
    <<"distCoeffs"<<distCoeffs;
    fs.release();
}