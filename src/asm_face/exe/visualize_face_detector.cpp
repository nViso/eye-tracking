/*****************************************************************************
 *   Non-Rigid Face Tracking
 ******************************************************************************
 *   by Jason Saragih, 5th Dec 2012
 *   http://jsaragih.org/
 ******************************************************************************
 *   Ch6 of the book "Mastering OpenCV with Practical Computer Vision Projects"
 *   Copyright Packt Publishing 2012.
 *   http://www.packtpub.com/cool-projects-with-opencv/book
 *****************************************************************************/
/*
 visualize_face_detector: Visualize the placement of points on the face
 Jason Saragih (2012)
 */

#include "asm_face/ft.hpp"
#define fl at<float>
const char* usage = "usage: ./visualise_face detector host_dir (which contains the annotations.yaml)";
//==============================================================================
bool
parse_help(int argc,char** argv)
{
    for(int i = 1; i < argc; i++){
        string str = argv[i];
        if(str.length() == 2){if(strcmp(str.c_str(),"-h") == 0)return true;}
        if(str.length() == 6){if(strcmp(str.c_str(),"--help") == 0)return true;}
    }return false;
}
//==============================================================================
int main(int argc,char** argv)
{
    //parse command line arguments
    if(argc < 2){cout << usage << endl; return 0;}
    if(parse_help(argc,argv)){cout << usage << endl; return 0;}
    
    if (argc<1) {
        return 0;
    }
    string fname =string(argv[1]);
    ft_data  ftdata = load_ft_jzp(fname);
    
    //load detector model
    face_detector detector = load_ft<face_detector>(string(ftdata.baseDir+"detectormodel.yaml").c_str());
    detector.baseDir =ftdata.baseDir;
    //open video stream
    VideoCapture cam;
    if(argc > 2)cam.open(argv[2]); else cam.open(0);
    if(!cam.isOpened()){
        cout << "Failed opening video file." << endl
        << usage << endl; return 0;
    }
    //detect until user quits
    namedWindow("face detector");
    while(cam.get(CV_CAP_PROP_POS_AVI_RATIO) < 0.999999){
        Mat im; cam >> im;    flip(im,im,1);
        vector<Point2f> p = detector.detect(im);
        if(p.size() > 0){
            for(int i = 0; i < int(p.size()); i++)
                circle(im,p[i],1,CV_RGB(0,255,0),2,CV_AA);
        }
        imshow("face detector",im);
        if(waitKey(10)%256 == 'q')break;
    }
    destroyWindow("face detector"); cam.release(); return 0;
}
//==============================================================================
