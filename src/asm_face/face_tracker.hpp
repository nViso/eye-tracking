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
  face_tracker: face tracking classes
  Jason Saragih (2012)
*/
#ifndef _FT_FACE_TRACKER_HPP_
#define _FT_FACE_TRACKER_HPP_
#include "asm_face/ft_data.hpp"
#include "asm_face/shape_model.hpp"
#include "asm_face/patch_model.hpp"
#include "asm_face/face_detector.hpp"
class face_tracker_params{                 //face tracking parameters
public:
  vector<Size> ssize;                      //search region size/level
  bool robust;                             //use robust fitting?
  int itol;                                //maximum number of iterations to try
  float ftol;                              //convergence tolerance
  float scaleFactor;                       //OpenCV Cascade detector parameters
  int minNeighbours;                       //...
  Size minSize;                            //...

  face_tracker_params();                   //sets default parameter settings

  void 
  write(FileStorage &fs) const;            //file storage object to write to

  void 
  read(const FileNode& node);              //file storage node to read from
};
//==============================================================================
class face_tracker{                        //face tracking class
public:
  vector<Point2f> points;                  //current tracked points
  face_detector detector;                  //detector for initialisation
  shape_model smodel;                      //shape model
  patch_models pmodel;                     //feature detectors
  ft_data annotations;
  

  int                                      //0 = failure
  track(const Mat &im,                     //image containing face
    const face_tracker_params &p =     //fitting parameters 
    face_tracker_params());            //default tracking parameters

  void
  draw(Mat &im,
       const Scalar pts_color = CV_RGB(255,0,0),
       const Scalar con_color = CV_RGB(0,255,0));
  void 
  write(FileStorage &fs) const;            //file storage object to write to

  void 
  read(const FileNode& node);              //file storage node to read from

protected:
  vector<Point2f>                          //points for fitted face in image
  fit(const Mat &image,                    //image containing face
      const vector<Point2f> &init,         //initial point estimates
      const Size ssize = Size(21,21),      //search region size
      const bool robust = false,           //use robust fitting?
      const int itol = 10,                 //maximum number of iterations to try
      const float ftol = 1e-3);            //convergence tolerance
};
//==============================================================================
#endif
