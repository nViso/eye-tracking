 //
//  SymmetryDetectionTest.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-20.
//
//

#include "SymmetryDetectionTest.h"



int main(int argc, const char * argv[])
{
    Mat img = imread("/Users/ZhipingJiang/eye2.jpg",CV_LOAD_IMAGE_COLOR);
    imshow("original",img);
    findEyeCenterByColorSegmentation(img);
    
    waitKey();
}