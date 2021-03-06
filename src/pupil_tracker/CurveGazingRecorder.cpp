#include <jzp_lib/jzplib_all.h>
#include <boost/filesystem.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/thread.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <deque>
namespace fs = boost::filesystem;
using namespace boost::posix_time;

string windowName = "Gaze Training Recorder";
Size fullSize(1366,720);
Mat whitebg(fullSize,CV_8UC3,Scalar::all(255));
Mat bg = whitebg.clone();
vector<Point2f> controlPoints;
vector<int> pointsBezierOrders;
vector<Point2f> curvePoints;
vector<Point2f> animatePoints;
int divs = 40;
int currentBezierOrder = 4;
int distancePerFrame = 5;
int frameRate = 30;
int currentAnimationPoint = -1;
Scalar lineColor(0,0,0);
Scalar endPointColor(0,0,0);
Scalar controlPointColor(255,0,0);

int captureRate = 60;
int writeRate = 40;
double cameraWidth, cameraHeight;
Mat capturedFrame;

boost::lockfree::spsc_queue<Mat, boost::lockfree::capacity<512> > matQueue;

bool aminationFinishSign = false;
bool animationFinished = false;
bool captureFinishSign = false;
bool writeFinishSign = false;
bool writeEnabled = false;


void readPathData(fs::path filePath) {
    FileStorage storage;
    storage.open(filePath.string(), FileStorage::READ);
    storage["points"] >> controlPoints;
    storage["pointsOrders"] >> pointsBezierOrders;
    storage["distancePerFrame"] >> distancePerFrame;
    storage["frameRate"] >> frameRate;
    writeRate = frameRate;
    storage.release();
    
    curvePoints = generalizedBezierCurve(controlPoints,pointsBezierOrders,1.0/divs);
    animatePoints = curveAnimateSequence(curvePoints, distancePerFrame);
}

int setupTrajectories(fs::path pathDirPath, vector<fs::path>& trajectoryFiles) {
    if (!fs::exists(pathDirPath) || !fs::is_directory(pathDirPath)) {
        cout<<" trajectoies_set_dir doesn't exist."<<endl;
        return 0;
    }
    
    trajectoryFiles = listFilesWithExtension(pathDirPath, "", "yaml");
    
    if (trajectoryFiles.empty()) {
        cout<<" trajectoies_set_dir doesn't contain trajectory files (.yaml file)."<<endl;
        return 0;
    } else {
        return 1;
    }
}

void  setupCameraCapture(VideoCapture *capture, Mat* frame) {
	cameraWidth = capture->get(CV_CAP_PROP_FRAME_WIDTH);
	cameraHeight = capture->get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "camera width = " << cameraWidth << endl <<"height = "<< cameraHeight << endl;
    boost::thread captureThread(videoCaptureThreadFun,capture, frame, captureRate*2, &captureFinishSign);
}


void animateFuc(Mat* bg) {
    
    cout<<"totally "<<animatePoints.size()<<" animation frames, frame rate: "<<frameRate<<endl;
    
    boost::posix_time::ptime startTime = microsec_clock::local_time();
    FPScontroller fpsDelayer(frameRate);
    writeEnabled = true;
    for (int i = 0; i<animatePoints.size(); i++) {
        Mat temp ;
        whitebg.copyTo(temp);
        circle(temp, animatePoints[i], 15, Scalar(0,0,255),-1,CV_AA);
        temp.copyTo((*bg));
        currentAnimationPoint = i;
        fpsDelayer.controlledDelay();
    }
    writeEnabled = false;
    boost::posix_time::ptime endTime = microsec_clock::local_time();
    boost::posix_time::time_duration td = endTime - startTime;
    cout<<"animation duration"<<td<<endl;
    
    animationFinished = true;
}

void showPrelude() {
    Mat current;
    whitebg.copyTo(current);
    circle(current, animatePoints[0], 10, Scalar(0,0,255),-1,CV_AA);
    drawStringAtPoint(current, "Ready to go? Press [y]es !", Point(100,100));
    while(true) {
        imshow(windowName,current);
        int k = waitKey()%256;
        if (k == 'y' || k=='Y') {
            break;
        }
    }
    
}

void invoke_HeadPoseAdviser(fs::path userProfileDir,fs::path cameraFile) {
    cout<<"------- Invoking ./HeadPoseAdviser ----------"<<endl;
    string cmdpath = (fs::current_path()/"HeadPoseAdviser").string();
    string cmd = cmdpath+" "+userProfileDir.string()+" "+cameraFile.string();
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}

void showResultPreviewAndSave(fs::path videoFilePath, fs::path userProfilePath,fs::path cameraFile) {
    cout<<"------- Invoking ./PupilTracker ----------------"<<endl;
    string cmdpath = (fs::current_path()/"PupilTracker").string();
    string cmd = cmdpath+" "+userProfilePath.string()+" "+cameraFile.string()+" "+videoFilePath.string();
    system(cmd.c_str());
    cout<<"------- Invocation Done ------------------------"<<endl;
}
bool showResult() {
    Mat current;
    whitebg.copyTo(current);
    drawStringAtPoint(current, "Proceed to next one? press [y]es! Wanna repeat last one? press [r]epeat!", Point(100,100));
    bool result = true;
    while(true) {
        imshow(windowName,current);
        int k = waitKey()%256;
        if (k == 'y' || k=='Y') {
            break;
        }
        if (k == 'r' || k=='R') {
            result = false;
            break;
        }
    }
    return result;
}

void writeAnimationTraceFile(string path) {

    Mat points = Mat(animatePoints).reshape(1);
    CSVFileWriter logger;
    for (int j = 0; j < points.rows; j++) {
        logger.addSlot(points.row(j));
    }
    
    logger.writeToFile(fs::path(path));
}

void showAnimationAndRecordVideo(fs::path trajectoryfile, fs::path userProfilePath, fs::path cameraProfilePath, fs::path outputfilePrefix) {
    readPathData(trajectoryfile);
    showPrelude();
    // start up camera
    captureFinishSign = false;
    writeFinishSign = false;
    VideoCapture cam(0);
    setupCameraCapture(&cam, &capturedFrame);
    
    //setup video writer
    string videoOutputFileName = outputfilePrefix.string()+".avi";
    string animationTraceOutputFileName =outputfilePrefix.string()+".groundtruth";
    writeAnimationTraceFile(animationTraceOutputFileName);
    // mp4v is small , acceptable.
    // mjpg is big, slow, but clear.
    // divx is small, slower than mp4v, less acceptable.
    string ccstr = "mp4v";
	VideoWriter videoWriter(videoOutputFileName,CV_FOURCC(ccstr[0],ccstr[1],ccstr[2],ccstr[3]), writeRate, cvSize((int)cameraWidth,(int)cameraHeight));
    BlockDelayer::delay_milisec(50);
    boost::thread frameBufferThread(videoBufferThreadFun,&matQueue,&capturedFrame,writeRate,0,&writeEnabled,&writeFinishSign);
    BlockDelayer::delay_milisec(50);
    boost::thread frameWriteThread(videoWriterThreadFun,&videoWriter,&matQueue,writeRate,&writeEnabled,&writeFinishSign);
    
    // then start the animation.
    Mat bgcopy = whitebg.clone();
    animationFinished = false;
    BlockDelayer::delay_milisec(50);
    boost::thread animateThread(animateFuc,&bgcopy);
    
    for(;;){
        imshow(windowName, bgcopy);
        waitKey(1);
        if (animationFinished) {
            break;
        }
    }
    writeFinishSign = true;
    frameBufferThread.join();
    frameWriteThread.join();
    waitKey(50);
    captureFinishSign = true;
    waitKey(50);
    
    showResultPreviewAndSave(fs::path(videoOutputFileName),userProfilePath,cameraProfilePath);
}

int main (int argc, char *argv[])
{
    if (argc < 4) {
        cout<<argv[0]<<" "<<"trajectoies_set_dir user_profile_dir camera_profile output_dir"<<endl;
        return 0;
    }
    
    fs::path pathDirPath(argv[1]);
    fs::path userProfilePath(argv[2]);
    fs::path cameraProfilePath(argv[3]);
    fs::path resultsBasePath(argv[4]);
    vector<fs::path> trajectoryFiles;
    if (setupTrajectories(pathDirPath,trajectoryFiles) == 0) {
        return 0;
    }
    
    ptime presentPtime = second_clock::local_time();
    string dateString = simpleDataFormat(presentPtime, "%Y_%m_%d");
    
    fs::path resultDirPath = resultsBasePath / string(userProfilePath.filename().string());
    fs::create_directories(resultDirPath);
    copyDirRecursively(userProfilePath, resultDirPath/"user_profile");
    
    //setup
    namedWindow(windowName,CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO |CV_GUI_NORMAL);
    moveWindow(windowName, 0, 0);
//    setWindowProperty(windowName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    deque<fs::path> trainQueue;
    for (int i = 0; i < trajectoryFiles.size(); i++) {
        trainQueue.push_back(trajectoryFiles[i]);
    }
    while (trainQueue.empty() == false) {
        invoke_HeadPoseAdviser(userProfilePath,cameraProfilePath);
        fs::path currentPath =trainQueue.front();
        trainQueue.pop_front();
        fs::path outputfilepath = resultDirPath / currentPath.stem().string();
        cout<<outputfilepath<<endl;
        showAnimationAndRecordVideo(currentPath, userProfilePath, cameraProfilePath, outputfilepath);
        if (showResult() == false) {
            trainQueue.push_front(currentPath);
        }
    }
    
    
}
