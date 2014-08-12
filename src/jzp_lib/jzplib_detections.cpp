//
//  jzplib_detections.cpp
//  OPENCV_HOTSHOTS
//
//  Created by Zhiping Jiang on 14-7-18.
//
//
#include "jzplib_draw.h"
#include "jzplib_detections.h"
#include <queue>
#include "EyeLike.h"




Rect haarPatternDetection(CascadeClassifier classifier, Mat image, int imageWidthforDetection, Rect parentRect=Rect()) {
    double imageSizeScale = 1.0*image.size().width*1.0/image.size().height;
    Mat color_img = image.clone();
    Mat face_img, gray_img;
    face_img = imresize(color_img, imageWidthforDetection);
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

Point findMassCenter_BinaryBiggestBlob(Mat bw_img) {
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


Point findEyeCenterByMassCenter(Mat image) {
//    int maxRadius = image.size().width/5;
//    int minRadius = image.size().width/7;
    
    int width = image.cols, height = image.rows;
    
    Mat gray_img , bila_img, masked;
    
    int kernel_length = width;
    int sigmaColor = width/3;
    int sigmaSpace =width;

//    if (image.cols* image.rows >0) {
//        bilateralFilter(image, bila_img, kernel_length, sigmaColor, sigmaSpace);
//        bilateralFilter(bila_img, image, kernel_length, sigmaColor, sigmaSpace);
//        imshow("bila",image);
//    }
    
    
    if (image.cols* image.rows >0) {
        bilateralFilter(image, bila_img, kernel_length, sigmaColor, sigmaSpace);
//        bilateralFilter(bila_img, image, kernel_length, sigmaColor, sigmaSpace);
        image = bila_img;
//        Mat eye_skin_mask = cvSkinYUV(image, true);
//        erode(eye_skin_mask, eye_skin_mask, Mat(13,13,CV_8UC1));
//        eye_skin_mask = Scalar::all(255) - eye_skin_mask;
//        
//        threshold(eye_skin_mask, eye_skin_mask, 1, 255, CV_THRESH_BINARY);
//        
//        Mat masked(eye_skin_mask.size(), CV_8UC3, Scalar(255,255,255)) ;
//        image.copyTo(masked, eye_skin_mask);
        
        cvtColor(image, gray_img, CV_BGR2GRAY);
        Mat sobel = sobelEdgeDerivatives(gray_img);
        gray_img = Scalar::all(255) - gray_img;
        gray_img = magicEqualHist(gray_img);
//        imshow("eye gradients",masked);
        
        //        GaussianBlur( gray_img, gray_img, Size(3, 3), 2,2 );
        Mat bw_img, edge_img ;
        threshold(gray_img, bw_img, 255*0.75, 255, CV_THRESH_BINARY);
        Mat full_bw_img = removeSmallBlobs(bw_img);
        threshold(bw_img, bw_img, 254, 255, CV_THRESH_BINARY);
        dilate(bw_img, bw_img, Mat(6,6,CV_8UC1));
        erode(bw_img, bw_img, Mat(4,4,CV_8UC1));
        bw_img  =fillHoleInBinary(bw_img);
        
        
        
        Mat flip_bw ;
        //        flip(bw_img, flip_bw, 1);
        Point eyePoint = findMassCenter_BinaryBiggestBlob(bw_img);
        
        
        circle(image,eyePoint,3,Scalar(0,255,0));
        circle(bw_img,eyePoint,3,Scalar(0,0,0));
        //        imshow("iris gray",irisMasked);
        imshow("eye_bw",bw_img);
        imshow("eyecenter",image);
        
        
        return eyePoint;
    }
    return Point(numeric_limits<int>::min(),numeric_limits<int>::min());
    
}

Point findEyeCenterByMassCenter2(Mat image) {
    //    int maxRadius = image.size().width/5;
    //    int minRadius = image.size().width/7;
    
    int width = image.cols, height = image.rows;
    
    Mat gray_img , bila_img, masked;
    
    int kernel_length = width;
    int sigmaColor = width/3;
    int sigmaSpace =width;
    
    if (image.cols* image.rows >0) {
        bilateralFilter(image, bila_img, kernel_length, sigmaColor, sigmaSpace);

        image = bila_img;
        findEyeCenterBySymmetryDetection(image,image.cols/8);
        cvtColor(image, gray_img, CV_BGR2GRAY);
//        Mat sobel = sobelEdgeDerivatives(gray_img);
        gray_img = Scalar::all(255) - gray_img;
        gray_img = magicEqualHist(gray_img);
        imshow("eye gradients",gray_img);
        
        //        GaussianBlur( gray_img, gray_img, Size(3, 3), 2,2 );
        Mat bw_img, edge_img ;
        threshold(gray_img, bw_img, 255*0.75, 255, CV_THRESH_BINARY);
        Mat full_bw_img = removeSmallBlobs(bw_img);
        threshold(bw_img, bw_img, 254, 255, CV_THRESH_BINARY);
        dilate(bw_img, bw_img, Mat(6,6,CV_8UC1));
        erode(bw_img, bw_img, Mat(4,4,CV_8UC1));
        bw_img  =fillHoleInBinary(bw_img);
        
        
        
        Mat flip_bw ;
        //        flip(bw_img, flip_bw, 1);
        Point eyePoint = findMassCenter_BinaryBiggestBlob(bw_img);
        
        
        circle(image,eyePoint,3,Scalar(0,255,0));
        circle(bw_img,eyePoint,3,Scalar(0,0,0));
        //        imshow("iris gray",irisMasked);
//        imshow("eye_bw",bw_img);
//        imshow("eyecenter",image);
        
        
        return eyePoint;
    }
    return Point(numeric_limits<int>::min(),numeric_limits<int>::min());
    
}

Mat calculateImageSymmetryScore(Mat image) {
    Mat gray_img ;
    
    if (image.channels() == 3) {
        cvtColor(image, gray_img, CV_BGR2GRAY);
    } else {
        gray_img = image.clone();
    }
    
//    gray_img = magicEqualHist(gray_img);
//    imshow("histed",gray_img);
    if (gray_img.cols %2 == 1) {
        hconcat(gray_img, gray_img.col(gray_img.cols-1), gray_img);
    }
    
    gray_img.convertTo(gray_img, CV_32FC1);

    Mat score(gray_img.cols,gray_img.cols,CV_32FC1,Scalar::all(0));
    Mat temp(1,1,CV_32FC1), base(1,1,CV_32FC1),colsum(1,1,CV_32FC1);
    
    
    for (int i = 0 ; i < gray_img.cols/2; i++) {
        temp = gray_img.colRange(0, i+1).clone();
        flip(temp, temp, 1);
        base = gray_img.colRange(i+1, gray_img.cols).clone();
        base.colRange(0, temp.cols) += temp;
        reduce(base, colsum, 0, CV_REDUCE_SUM,CV_32FC1);
        colsum =  colsum.t();
        colsum.copyTo(score.colRange(i, i+1).rowRange(0, colsum.rows));
    }
    
    flip(score, score, 1);
    flip(gray_img, gray_img, 1);
    
    for (int i = 0 ; i < gray_img.cols/2; i++) {
        temp = gray_img.colRange(0, i+1).clone();
        flip(temp, temp, 1);
        base = gray_img.colRange(i+1, gray_img.cols).clone();
        base.colRange(0, temp.cols) += temp;
        reduce(base, colsum, 0, CV_REDUCE_SUM,CV_32FC1);
        colsum =  colsum.t();
        colsum.copyTo(score.colRange(i, i+1).rowRange(0, colsum.rows));
    }
    
    flip(score, score, 1);
    gray_img.release();
    base.release();
    temp.release();
    return score;
}

Point findEyeCenterBySymmetryDetection(Mat image,float pupilRadius) {
    const int symmetryWidth = pupilRadius;
    Mat score = calculateImageSymmetryScore(image);
    imagesc("score",imresize(mat2gray(score),4.0f));
    Mat scoresum;
    reduce(score.rowRange(0, symmetryWidth), scoresum, 0, CV_REDUCE_SUM,CV_32FC1);
    plotVectors("scoresum", scoresum.t());
    
    double minVal , maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(scoresum,&minVal,&maxVal,&minLoc,&maxLoc);
    
    int horizonCenter = maxLoc.x;
    cout<<"horizontalCenter"<<horizonCenter<<endl;
    line(image, Point(horizonCenter,0), Point(horizonCenter,image.rows), Scalar::all(255));
    imshow("hc",imresize(image,3.0f));
    
    Mat diff2line = (diff(score.col(horizonCenter)));
    plotVectors("diff2", diff2line);
    minMaxLoc(diff2line.rowRange(0,symmetryWidth),&minVal,&maxVal,&minLoc,&maxLoc);
    int irisRadius = maxLoc.y;
//    cout<<diff2line<<endl;
    cout<<"iris radius"<<irisRadius<<endl;
//    if (horizonCenter > irisRadius+2 && irisRadius > 0 ) {
//        gray_img = imcomplement(gray_img);
//        Mat irisSearchBG = gray_img.colRange(horizonCenter-irisRadius-1, horizonCenter+irisRadius+1);
//        imshow("irisBG", mat2gray(irisSearchBG));
//        cout<<"irisBG size"<<irisSearchBG.size()<<endl;
//        Mat diskTemplate(irisRadius*2-1,irisRadius*2-1,CV_32FC1,Scalar::all(0));
//        circle(diskTemplate, Point(irisRadius,irisRadius), irisRadius, Scalar::all(1),-1);
//        imshow("disk",diskTemplate);
//        cout<<"disk size"<<diskTemplate.size()<<endl;
//        
//        if (irisSearchBG.rows > diskTemplate.rows) {
//            Mat xc;
//            matchTemplate(irisSearchBG, diskTemplate, xc, CV_TM_CCORR);
//            //    imshow("xc",imresize(mat2gray(xc),100));
//            cout<<xc.size()<<endl;
//            minMaxLoc(xc,&minVal,&maxVal,&minLoc,&maxLoc);
//            int verticalCenter = maxLoc.y+irisRadius;
//            
//            Point eyeCenter(horizonCenter,verticalCenter);
//            circle(image, eyeCenter, 2, Scalar(0,255,255),-1);
//            imshow("eye center",image);
//            //    Mat m  = image.clone();
//            //    line(m, Point(horizonCenter,0), Point(horizonCenter,image.rows), Scalar(0,255,255));
//            //    imshow("hc",m);
//            return Point(horizonCenter,verticalCenter);
//
//        } else
//            return Point(0,0);
//    }
    return Point(horizonCenter,0);
}

Point findEyeCenterByColorSegmentation(Mat image, float coordinateWeight, int kmeansIterations, int kmeansRepeats, int blurSize)  {
    
    Mat img, gray_img;
    Mat colorpoints, kmeansPoints;
    
    img = equalizeImage(image);
    
    medianBlur(img, img, blurSize);
//    imshow("equlized",img);
    cvtColor(image, gray_img, CV_BGR2GRAY);
    gray_img = imcomplement(gray_img);
//    imagesc("grayeye",mat2gray(gray_img));
    vector<Mat> layers(3);
    split(img, layers);
    for (int i = 0 ; i < layers.size(); i++) {
        layers[i] = layers[i].reshape(1,1).t();
    }
    hconcat(layers, colorpoints);
    
    // add coordinates
    colorpoints.convertTo(colorpoints, CV_32FC1);
    Mat coordinates = matrixPointCoordinates(img.rows,img.cols,false) *coordinateWeight;
    hconcat(colorpoints, coordinates, kmeansPoints);
    
    Mat locIndex(img.size().area(),kmeansIterations,CV_32FC1,Scalar::all(-1));
    linspace(0, img.size().area(), 1).copyTo(locIndex.col(0));
    Mat index_img(img.rows,img.cols,CV_32FC1,Scalar::all(0));
    Mat bestLabels, centers, clustered , colorsum , minColorPtIndex;
    for(int it = 1 ; it < kmeansIterations ; it++) {
        if (kmeansPoints.rows < 2) {
            break;
        }
        kmeans(kmeansPoints,2,bestLabels,TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, kmeansRepeats, 0.001),kmeansRepeats,KMEANS_PP_CENTERS,centers);
        reduce(centers.colRange(0, 3), colorsum, 1, CV_REDUCE_SUM);

        if (colorsum.at<float>(0) < colorsum.at<float>(1)) {
            
            findNonZero(bestLabels==0, minColorPtIndex);
        }
        else {
            findNonZero(bestLabels==1, minColorPtIndex);
        }
        
        minColorPtIndex = minColorPtIndex.reshape(1).col(1);
        
        for (int  i = 0; i <minColorPtIndex.rows ; i ++) {
            locIndex.at<float>(i,it) = locIndex.at<float>(minColorPtIndex.at<int>(i),it-1);
        }
        Mat temp;
        for (int  i = 0; i <minColorPtIndex.rows ; i ++) {
            temp.push_back(kmeansPoints.row(minColorPtIndex.at<int>(i)));
        }
        temp.copyTo(kmeansPoints);
        temp.release();
        for (int i = 0 ; i < minColorPtIndex.rows ; i ++) {
            int r, c;
            ind2sub(locIndex.at<float>(i,it), index_img.cols, index_img.rows, r, c);
            index_img.at<float>(r,c) +=1;
        }
    }
//    imagesc("layered",mat2gray(index_img));
    Mat layerweighted_img = index_img.mul(index_img);
    layerweighted_img = mat2gray(layerweighted_img);
    gray_img.convertTo(gray_img, CV_32FC1,1/255.0);
    Mat composed  = gray_img.mul(layerweighted_img);
    
    Mat score = calculateImageSymmetryScore(composed);
    Mat scoresum;
    reduce(score.rowRange(0, composed.cols/6), scoresum, 0, CV_REDUCE_SUM,CV_32FC1);
//    plotVectors("live", scoresum.t());
    double minVal , maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(scoresum,&minVal,&maxVal,&minLoc,&maxLoc);
    int initialHC = maxLoc.x;

    int bestx = 0,bestlayer = 0;
    Mat bestIndex_img = index_img >=1;
    minMaxLoc(index_img,&minVal,&maxVal,&minLoc,&maxLoc);
    for (int i = 1 ; i<=maxVal; i++) {
        Mat indexlayer_img = index_img >=i;
        medianBlur(indexlayer_img, indexlayer_img, 5);
        indexlayer_img = removeSmallBlobs(indexlayer_img);
        indexlayer_img = fillHoleInBinary(indexlayer_img);
        indexlayer_img = fillConvexHulls(indexlayer_img);
        Mat score = calculateImageSymmetryScore(indexlayer_img);
        Mat scoresum;
        reduce(score.rowRange(0, indexlayer_img.cols/6), scoresum, 0, CV_REDUCE_SUM,CV_32FC1);
        minMaxLoc(scoresum,&minVal,&maxVal,&minLoc,&maxLoc);
        if (abs(maxLoc.x - initialHC) < abs(bestx - initialHC)) {
            
            if (sum(indexlayer_img)[0]/255 < indexlayer_img.size().area()/5*2 &&
                sum(indexlayer_img)[0]/255 > indexlayer_img.size().area()/6) {
                bestx = maxLoc.x;
                bestlayer = i;
                bestIndex_img = indexlayer_img.clone();
                
            }
            
        }
    }
    
    Point massCenter = findMassCenter_BinaryBiggestBlob(bestIndex_img);
    
    
    return Point(initialHC,massCenter.y);
}



Point findEyeCenterByGradient(Mat image) {
    const int kWeightBlurSize = 5;
    const double kGradientThreshold = 50.0;
    const bool kEnablePostProcess = true;
    const float kPostProcessThreshold = 0.97;
    
    std::vector<cv::Mat> rgbChannels(3);
    cv::split(image, rgbChannels);
    cv::Mat frame_gray = rgbChannels[2];

    GaussianBlur( frame_gray, frame_gray, cv::Size( 0, 0 ), 10);
    cv::Mat eyeROIUnscaled = equalizeImage(frame_gray);
    cv::Mat eyeROI;
    scaleToFastSize(eyeROIUnscaled, eyeROI);
    //-- Find the gradient
    cv::Mat gradientX = computeMatXGradient(eyeROI);
    cv::Mat gradientY = computeMatXGradient(eyeROI.t()).t();
    //-- Normalize and threshold the gradient
    // compute all the magnitudes
    cv::Mat mags = matrixMagnitude(gradientX, gradientY);
    //compute the threshold
    double gradientThresh = computeDynamicThreshold(mags, kGradientThreshold);
    //double gradientThresh = kGradientThreshold;
    //double gradientThresh = 0;
    //normalize
    for (int y = 0; y < eyeROI.rows; ++y) {
        double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
        const double *Mr = mags.ptr<double>(y);
        for (int x = 0; x < eyeROI.cols; ++x) {
            double gX = Xr[x], gY = Yr[x];
            double magnitude = Mr[x];
            if (magnitude > gradientThresh) {
                Xr[x] = gX/magnitude;
                Yr[x] = gY/magnitude;
            } else {
                Xr[x] = 0.0;
                Yr[x] = 0.0;
            }
        }
    }
//    imshow("gradient",gradientY);
    //-- Create a blurred and inverted image for weighting
    cv::Mat weight;
    GaussianBlur( eyeROI, weight, cv::Size( kWeightBlurSize, kWeightBlurSize ), 0, 0 );
    for (int y = 0; y < weight.rows; ++y) {
        unsigned char *row = weight.ptr<unsigned char>(y);
        for (int x = 0; x < weight.cols; ++x) {
            row[x] = (255 - row[x]);
        }
    }
//    imshow("weight",weight);
    //-- Run the algorithm!
    cv::Mat outSum = cv::Mat::zeros(eyeROI.rows,eyeROI.cols,CV_64F);
    // for each possible center
//    printf("Eye Size: %ix%i\n",outSum.cols,outSum.rows);
    for (int y = 0; y < weight.rows; ++y) {
        const unsigned char *Wr = weight.ptr<unsigned char>(y);
        const double *Xr = gradientX.ptr<double>(y), *Yr = gradientY.ptr<double>(y);
        for (int x = 0; x < weight.cols; ++x) {
            double gX = Xr[x], gY = Yr[x];
            if (gX == 0.0 && gY == 0.0) {
                continue;
            }
            testPossibleCentersFormula(x, y, Wr[x], gX, gY, outSum);
        }
    }
//    imshow("out",outSum);
    // scale all the values down, basically averaging them
    double numGradients = (weight.rows*weight.cols);
    cv::Mat out;
    outSum.convertTo(out, CV_32F,1.0/numGradients);
    
    //-- Find the maximum point
    cv::Point maxP;
    double maxVal;
    cv::minMaxLoc(out, NULL,&maxVal,NULL,&maxP);
    //-- Flood fill the edges
    if(kEnablePostProcess) {
        cv::Mat floodClone;
        //double floodThresh = computeDynamicThreshold(out, 1.5);
        double floodThresh = maxVal * kPostProcessThreshold;
        cv::threshold(out, floodClone, floodThresh, 0.0f, cv::THRESH_TOZERO);
        cv::Mat mask = floodKillEdges(floodClone);
        
        //    imshow("out",out);
        // redo max
        cv::minMaxLoc(out, NULL,&maxVal,NULL,&maxP,mask);
    }
    cout<<maxP<<endl;
    return maxP;
}




