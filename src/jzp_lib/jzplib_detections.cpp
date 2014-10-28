//
//  jzplib_detections.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//
#include "jzplib_draw.h"
#include "jzplib_detections.h"
#include <boost/lexical_cast.hpp>
#include <queue>




Rect haarPatternDetection(CascadeClassifier classifier, Mat image, int imageWidthforDetection, Rect parentRect=Rect()) {
    double imageSizeScale = 1.0*image.size().width*1.0/image.size().height;
    Mat color_img = image.clone();
    Mat face_img, gray_img;
    imresize(color_img, imageWidthforDetection,face_img);
    cvtColor(face_img, gray_img, CV_BGR2GRAY);
    equalizeHist(gray_img, gray_img);
    vector<Rect> faceRectsVector;
    classifier.detectMultiScale(gray_img, faceRectsVector,1.2,3,
                                CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE );
    
    Rect faceRect(0,0,0,0);
    
    for (int i = 0; i < faceRectsVector.size(); i++) {
        drawBox(face_img, faceRectsVector[i]);
        if (faceRect.area() < faceRectsVector[i].area())
            faceRect = faceRectsVector[i];
    }
    drawBox(face_img, faceRect);
    //    imshow("rect",face_img);
    double x = 1.0*faceRect.x/(double)imageWidthforDetection;
    double y = 1.0*faceRect.y/(double)cvRound(imageWidthforDetection/imageSizeScale);
    double w = 1.0*faceRect.width/(double)imageWidthforDetection;
    double h = 1.0*faceRect.height/(double)cvRound(imageWidthforDetection/imageSizeScale);
    
    int bx = cvRound(x*image.size().width) ;;//+ parentRect.tl().x;
    int by = cvRound(y*image.size().height) ;//+ parentRect.tl().y;
    int bw = cvRound(w*image.size().width);
    int bh = cvRound(h*image.size().height);
    
    Rect biggerRect(bx,by,bw,bh);
    biggerRect += parentRect.tl();
    
    return biggerRect;
}

Point2f findMassCenter_BinaryBiggestBlob(const Mat& bw_img) {
    Mat full_bw_img = removeSmallBlobs(bw_img);
    threshold(bw_img, bw_img, 254, 255, CV_THRESH_BINARY);
    
    vector<vector<Point> > contours,onlyContours(1);
    
    vector<Vec4i> hierarchy;
    
    findContours( bw_img.clone(), contours, hierarchy,
                 CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    
    if (contours.size() >0) {
        vector<int> areas(contours.size());
        for(size_t i= 0 ; i < contours.size() ; i ++) {
            areas[i] = contourArea(contours[i]);
            //        cout<<areas[i]<<",";
        }
        
        long biggestIndex = distance(areas.begin(), max_element(areas.begin(),areas.end()));
        //    cout<<biggestIndex<<":"<<areas[biggestIndex]<<endl;
        onlyContours[0] =contours[biggestIndex];
        
        /// Get the moments
        vector<Moments> mu(contours.size() );
        for( int i = 0; i < contours.size(); i++ )
        { mu[i] = moments( contours[i], false ); }
        
        ///  Get the mass centers:
        vector<Point2f> mc( contours.size() );
        for( int i = 0; i < contours.size(); i++ )
        { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
        
        return mc[0];
    } else
        return Point(0,0);
}

SymmetryScore_tbb::SymmetryScore_tbb(Mat & gray, Mat & score) : score(score), gray_img(gray){
    flip(gray_img, flipped, 1);
}

void SymmetryScore_tbb::operator() (const cv::Range& range) const {
    Mat temp, base, colsum;
    for (int i = range.start ; i < range.end; i++) {
        
        if (i<gray_img.cols/2) {
            flipped.colRange(flipped.cols-i-1, flipped.cols).copyTo(temp);
            gray_img.colRange(i+1, gray_img.cols).copyTo(base);
        } else {
            gray_img.colRange(i, gray_img.cols).copyTo(temp);
            flipped.colRange(flipped.cols-i, flipped.cols).copyTo(base);
        }
        base.colRange(0, temp.cols) += temp;
        reduce(base, colsum, 0, CV_REDUCE_SUM,CV_32FC1);
        colsum =  colsum.t();
        colsum.copyTo(score.colRange(i, i+1).rowRange(0, colsum.rows));
    }
}


Mat calculateImageSymmetryScore(const Mat& image) {
    Mat gray_img ;
    
    if (image.channels() == 3) {
        cvtColor(image, gray_img, CV_BGR2GRAY);
    } else {
        gray_img = image.clone();
    }
    
    if (gray_img.cols %2 == 1) {
        hconcat(gray_img, gray_img.col(gray_img.cols-1), gray_img);
    }
    gray_img.convertTo(gray_img, CV_32FC1);
    Mat score(gray_img.cols,gray_img.cols,CV_32FC1,Scalar::all(0));
    
    parallel_for_(Range(0,gray_img.cols), SymmetryScore_tbb(gray_img, score));
    
    return score;
}
