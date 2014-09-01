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
 train_face_detector: learn a face_detector object from training data
 Jason Saragih (2012)
 */

#include "asm_face/ft.hpp"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
namespace fs = boost::filesystem;

#define fl at<float>
const char* usage =
"usage: ./train_face_detector haarcascadefile host_dir (which contains the annotations.yaml)";
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
int
parse_frac(int argc,char** argv)
{
    for(int i = 1; i < argc; i++){
        string str = argv[i];
        if(str.length() != 2)continue;
        if(strcmp(str.c_str(),"-f") == 0){
            if(argc > i+1)return atof(argv[i+1]);
        }
    }return 0.8;
}
//==============================================================================
bool
parse_mirror(int argc,char** argv)
{
    for(int i = 1; i < argc; i++){
        string str = argv[i];
        if(str.length() != 8)continue;
        if(strcmp(str.c_str(),"--mirror") == 0)return true;
    }return false;
}
//==============================================================================
int main(int argc,char** argv)
{
    //parse cmdline input
    if(argc <=1){ cout << usage << endl; return 0;}
    float frac = parse_frac(argc,argv);
    bool mirror = parse_mirror(argc,argv);
    
    fs::path workingDirPath(argv[1]);
    ft_data  ftdata = load_ft_jzp(workingDirPath.string());
    shape_model smodel = load_ft<shape_model>(fs::path(workingDirPath / "shapemodel.yaml").string().c_str());
    smodel.set_identity_params();
    vector<Point2f> r = smodel.calc_shape();
    Mat ref = Mat(r).reshape(1,2*r.size());
    
    fs::path detectorFilePath;
    if (argc==2) {
        
        fs::directory_iterator iterator(fs::current_path());
        vector<fs::path> detectorFiles;
        for (; iterator != fs::directory_iterator(); iterator++) {
            if (boost::starts_with(iterator->path().filename().string(),"haar")) {
                detectorFiles.push_back(iterator->path());
            }
        }
        cout<<"There are "<<detectorFiles.size()<<" haar detector files in your CWD. Which one do you choose ?"<<endl;
        for (int i = 0; i < detectorFiles.size(); i++) {
            cout<<"("<<i+1<<") "<<detectorFiles[i].string()<<endl;
        }
        cout<<"your choice:";
        string input;
        cin >> input;
        int index = boost::lexical_cast<int>(input)-1;
        detectorFilePath =workingDirPath / detectorFiles[index].filename();
        if (index >=0 && index < detectorFiles.size()) {
            fs::copy_file(detectorFiles[index], detectorFilePath,  boost::filesystem::copy_option::overwrite_if_exists);
        }
    }
    
    if (argc==3) {
        fs::path srcDetectorFilePath(argv[2]);
        detectorFilePath =workingDirPath / srcDetectorFilePath.filename();
        fs::copy_file(srcDetectorFilePath, detectorFilePath,  boost::filesystem::copy_option::overwrite_if_exists);
    }

    cout<<"Your current detector file is :"<<detectorFilePath<<endl;
    
    //train face detector
    face_detector detector;
    detector.train(ftdata,detectorFilePath.string(),ref,mirror,true,frac);
    //save detector
    save_ft<face_detector>(string(ftdata.baseDir+"detectormodel.yaml").c_str(),detector);
    return 0;
}
//==============================================================================
