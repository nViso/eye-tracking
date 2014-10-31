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
 train_patch_model: learn a patch_model object from training data
 Jason Saragih (2012)
 */

#include "asm_face/ft.hpp"

#define fl at<float>
const char* usage =
"usage: ./train_patch_model host_dir (which contains the annotations.yaml)";
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
parse_face_width(int argc,char** argv)
{
    for(int i = 1; i < argc; i++){
        string str = argv[i];
        if(str.length() != 2)continue;
        if(strcmp(str.c_str(),"-w") == 0){
            if(argc > i+1)return atoi(argv[i+1]);
        }
    }return 150;
}
//==============================================================================
int
parse_patch_size(int argc,char** argv)
{
    for(int i = 1; i < argc; i++){
        string str = argv[i];
        if(str.length() != 2)continue;
        if(strcmp(str.c_str(),"-p") == 0){
            if(argc > i+1){
                int p = atoi(argv[i+1]); if(p%2 == 0)return p+1; else return p;
            }
        }
    }return 20;
}
//==============================================================================
int
parse_search_window_size(int argc,char** argv)
{
    for(int i = 1; i < argc; i++){
        string str = argv[i];
        if(str.length() != 2)continue;
        if(strcmp(str.c_str(),"-s") == 0){
            if(argc > i+1){
                int s = atoi(argv[i+1]); if(s%2 == 0)return s+1; else return s;
            }
        }
    }return 20;
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
float                                      //scaling factor
calc_scale(const Mat &X,                   //scaling basis vector
           const float width)              //width of desired shape
{
    int n = X.rows/2; float xmin = X.at<float>(0),xmax = X.at<float>(0);
    for(int i = 0; i < n; i++){
        xmin = min(xmin,X.at<float>(2*i));
        xmax = max(xmax,X.at<float>(2*i));
    }return width/(xmax-xmin);
}
//==============================================================================
int main(int argc,char** argv)
{
    //parse cmdline input
    if(argc <= 1){ cout << usage << endl; return 0;}
    int width  = parse_face_width(argc,argv);
    int psize  = parse_patch_size(argc,argv);
    int ssize  = parse_search_window_size(argc,argv);
    bool mirror = parse_mirror(argc,argv);
    Size wsize(psize+ssize,psize+ssize);
    
    string fname =string(argv[1]);
    ft_data  ftdata = load_ft_jzp(fname);
    
    ftdata.rm_incomplete_samples();
    if(ftdata.imnames.size() == 0){
        cerr << "Data file does not contain any annotations."<< endl; return 0;
    }
    //load shape model
    shape_model smodel = load_ft<shape_model>(string(ftdata.baseDir+"shapemodel.yaml").c_str());
    
    
    //generate reference shape
    smodel.p = Scalar::all(0.0);
    smodel.p.fl(0) = calc_scale(smodel.V.col(0),width);
    vector<Point2f> r = smodel.calc_shape();
    
    //train patch models
    patch_models pmodel;
    pmodel.train(ftdata,r,Size(psize,psize),Size(ssize,ssize),mirror);
//    save_ft<patch_models>(argv[3],pmodel);
    save_ft<patch_models>(string(ftdata.baseDir+"patchmodels.yaml").c_str(), pmodel);
    return 0;
}
//==============================================================================
