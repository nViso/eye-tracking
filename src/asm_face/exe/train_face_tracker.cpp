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
  train_face_tracker: build a face_tracker object from its constituents
  Jason Saragih (2012)
*/

#include "asm_face/ft.hpp"
#define fl at<float>
const char* usage = 
"usage: ./train_face_tracker host_dir (which contains the annotations.yaml)";
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
  //parse cmdline input
  if(parse_help(argc,argv)){ cout << usage << endl; return 0;}
  if(argc < 2){ cout << usage << endl; return 0;}

    string fname =string(argv[1]);
    ft_data ftdata = load_ft_jzp(fname);
    
    
  //create face tracker model
  face_tracker tracker;
    load_ft<shape_model>(string(ftdata.baseDir+"shapemodel.yaml").c_str());
  tracker.smodel = load_ft<shape_model>(string(ftdata.baseDir+"shapemodel.yaml").c_str());
  tracker.pmodel = load_ft<patch_models>(string(ftdata.baseDir+"patchmodels.yaml").c_str());
  tracker.detector = load_ft<face_detector>(string(ftdata.baseDir+"detectormodel.yaml").c_str());
  
  //save face tracker
  save_ft<face_tracker>(string(ftdata.baseDir+"trackermodel.yaml").c_str(),tracker); return 0;
}
//==============================================================================
