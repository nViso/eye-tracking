//
//  EyeTrackingFast_XCODE
//
//  Created by Zhiping Jiang on 14-6-28.
//  Copyright (c) 2014年 Zhiping Jiang. All rights reserved.
//


//#include "asef_utils.hpp"
#include "jzplib_all.h"


using namespace cv;
using namespace std;


VideoCapture capture(0);
ASEF_Algorithm algorithm;
CascadeClassifier frontalface_cascade, mouth_cascade, nose_cascade;



Scalar markColor(255, 255, 255);

int initializing() {
    
    String haar_frontalface_cascade_name = "haarcascade_frontalface_alt2.xml";
    String haar_nose_cascade_name = "haarcascade_mcs_nose.xml";
    String haar_mouth_cascade_name = "haarcascade_mcs_mouth.xml";
    
    if (!frontalface_cascade.load(haar_frontalface_cascade_name)) {
        printf("--(!)Error loading face detection\n");
        return -1;
    };
    
    if (!mouth_cascade.load(haar_mouth_cascade_name)) {
        printf("--(!)Error loading mouth detection\n");
        return -1;
    };
    
    if (!nose_cascade.load(haar_nose_cascade_name)) {
        printf("--(!)Error loading nose detection\n");
        return -1;
    };
    
    algorithm.initialize();
    
    if (algorithm.isInitialized == false) {
        cout<<"--(!)Error initalizing ASEF\n";
        return -1;
    }
    
    if (capture.isOpened() == false) {
        fprintf(stderr, "ERROR: capture is NULL \n");
        return -1;
    }
    return 0;
}



int eyeLocalizationStatic()
{
    
    Rect faceRect;
    Mat original_img,color_img;
    
    
    color_img = captureImage(capture);
    while (true) {
        
        
        color_img = captureImage(capture);
        Rect faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
        Mat  faceMask = Mat::zeros(color_img.size(), CV_8UC1);
        faceMask(faceRect) = 255;
        Mat skinColor = cvSkinYUV(color_img,false);
        Mat skinMask;
        cvtColor(skinColor, skinMask, CV_RGB2GRAY);
        threshold(skinMask, skinMask, 10, 255, CV_THRESH_BINARY);
        //        dilate(skinMask, skinMask, Mat(8,8,CV_8UC1));
        //        erode(skinMask, skinMask, Mat(10,10,CV_8UC1));
        //        bitwise_and(faceMask, skinMask, faceMask);
        //            faceMask = fillHoleInBinary(faceMask);
        calculateColorHistogram(skinColor,skinMask);
        imshow("faceMask",skinMask);
        algorithm.findEyes(color_img.clone(),faceRect);
        
        Point leftEyeCenter =  findEyeCenterByColorDistribution(color_img(algorithm.leftEyeRect));
        //        findEyeCenterByColorBlob(color_img(algorithm.leftEyeRect).clone());
        //        Point rightEyeCenter = findEyeCenter(color_img(eyeRectsVector[1]));
        leftEyeCenter += algorithm.leftEyeRect.tl();
        //        rightEyeCenter +=eyeRectsVector[1].tl();
        
        
        
        circle(color_img,leftEyeCenter,3,Scalar(0,255,0));
        //        circle(color_img,rightEyeCenter,3,Scalar(0,255,0));
        circle(color_img,algorithm.leftEyePoint,3,Scalar(255,0,0));
        circle(color_img,algorithm.rightEyePoint,3,Scalar(255,0,0));
        imshow("color", color_img);
        if (waitKey(30) > 0)
            break;
    }
    return 0;
}

int eyeLocalizationDynamic()
{
    
    Rect faceRect;
    Mat original_img,color_img,gray_img,preImg,curImg;
    vector<Point2f> prevFeatures, nextFeatures;
    vector<uchar> trackingStatus;
    vector<float> errors;
    bool needfeatureReInitialize = true;
    int frameCount = 0 ;
    Mat faceColorHist, backProj;
    while (true) {
        frameCount ++;
        
        color_img = captureImage(capture);
        cvtColor(color_img, gray_img, CV_RGB2GRAY);
        equalizeHist(gray_img, gray_img);
        
        if (needfeatureReInitialize) {
            Rect faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
            Mat  faceMask = Mat::zeros(color_img.size(), CV_8UC1);
            faceMask(faceRect) = 255;
            
            Mat skinMask = cvSkinYUV(color_img,true);
            dilate(skinMask, skinMask, Mat(6,6,CV_8UC1));
            erode(skinMask, skinMask, Mat(6,6,CV_8UC1));
            bitwise_and(faceMask, skinMask, faceMask);
            //            faceMask = fillHoleInBinary(faceMask);
            imshow("faceMask",faceMask);
            
            faceColorHist = calculateColorHistogram(color_img, faceMask);
            
            
            if (faceRect.area()>0) {
                
                goodFeaturesToTrack(gray_img, prevFeatures, 100 , 0.01, 10,faceMask);
                for (int i = 0 ; i < prevFeatures.size() ; i ++) {
                    circle(color_img, prevFeatures[i], 3, Scalar(255,0,0));
                }
                needfeatureReInitialize = false;
                preImg = gray_img.clone();
            }
        } else{
            
            calcOpticalFlowPyrLK(preImg, gray_img, prevFeatures, nextFeatures
                                 , trackingStatus, errors);
            preImg = gray_img.clone();
            prevFeatures = nextFeatures;
            nextFeatures.clear();
            
            
            backProj = calculateColorBackProjection(color_img, faceColorHist);
            imshow("backProj",backProj);
            
            for (int i = 0 ; i < prevFeatures.size() ; i ++) {
                circle(color_img, prevFeatures[i], 3, Scalar(255,0,0));
            }
            
            
        }
        
        imshow("color", color_img);
        if (waitKey(30) > 0)
            break;
    }
    return 0;
}

int HeadTrackingCamshift()
{
    
    Rect faceRect;
    Mat original_img,color_img,gray_img,preImg,curImg;
    vector<Point2f> prevFeatures, nextFeatures;
    vector<uchar> trackingStatus;
    vector<float> errors;
    bool needfeatureReInitialize = true;
    int frameCount = 0 ;
    Mat faceColorHist, backProj;
    RotatedRect faceRotRect;
    while (true) {
        frameCount ++;
        
        color_img = captureImage(capture);
        cvtColor(color_img, gray_img, CV_RGB2GRAY);
        equalizeHist(gray_img, gray_img);
        
        if (needfeatureReInitialize) {
            Rect faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
            Mat  faceMask = Mat::zeros(color_img.size(), CV_8UC1);
            faceMask(faceRect) = 255;
            
            Mat skinMask = cvSkinYUV(color_img,true);
            dilate(skinMask, skinMask, Mat(6,6,CV_8UC1));
            erode(skinMask, skinMask, Mat(6,6,CV_8UC1));
            bitwise_and(faceMask, skinMask, faceMask);
            //            faceMask = fillHoleInBinary(faceMask);
            imshow("faceMask",faceMask);
            
            faceColorHist = calculateColorHistogram(color_img, faceMask);
            drawColorHistGram(faceColorHist);
            needfeatureReInitialize = false;
            
            
            //            if (faceRect.area()>0) {
            //
            //                goodFeaturesToTrack(gray_img, prevFeatures, 100 , 0.01, 10,faceMask);
            //                for (int i = 0 ; i < prevFeatures.size() ; i ++) {
            //                    circle(color_img, prevFeatures[i], 3, Scalar(255,0,0));
            //                }
            //
            //                preImg = gray_img.clone();
            
            //            }
        } else{
            Rect  trackingWindow =  (faceRotRect.boundingRect());
            backProj = calculateColorBackProjection(color_img, faceColorHist);
            faceRotRect = CamShift(backProj, trackingWindow, TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1));
            Point2f rect_points[4]; faceRotRect.points( rect_points );
            
            for( int j = 0; j < 4; j++ )
                line( color_img, rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 1, 8 );
            
            ellipse( color_img, faceRotRect, Scalar(0,0,255), 3, CV_AA );
            
            //            calcOpticalFlowPyrLK(preImg, gray_img, prevFeatures, nextFeatures
            //                                 , trackingStatus, errors);
            //            preImg = gray_img.clone();
            //            prevFeatures = nextFeatures;
            //            nextFeatures.clear();
            //
            //
            //            backProj = calculateColorBackProjection(color_img, faceColorHist);
            //            imshow("backProj",backProj);
            //
            //            for (int i = 0 ; i < prevFeatures.size() ; i ++) {
            //                circle(color_img, prevFeatures[i], 3, Scalar(255,0,0));
            //            }
            
            
        }
        
        imshow("color", color_img);
        if (waitKey(30) > 0)
            break;
    }
    return 0;
}




void trackingEyeDirectly() {
    Mat color_img, gray_img, priv_img, next_img;
    Rect faceRect;
    vector<Point2f> privFeatures, nextFeatures;
    vector<uchar> trackingStatus;
    vector<float> errors;
    bool hasEyeFound = false;
    LKTracker tracker;
    color_img = captureImage(capture);
    
    while (true) {
        color_img = captureImage(capture);
        cvtColor(color_img, gray_img, CV_RGB2GRAY);
        equalizeHist(gray_img, gray_img);
        
        if (hasEyeFound == false) {
            priv_img = gray_img.clone();
            faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
            if (faceRect.area() >0) {
                algorithm.findEyes(color_img,faceRect);
                if (algorithm.eyeFound ) {
                    //                    equalizeHist(gray_img(algorithm.leftEyeRect), gray_img(algorithm.leftEyeRect));
                    //                    equalizeHist(gray_img(algorithm.rightEyeRect), gray_img(algorithm.rightEyeRect));
                    imshow("equal_for_eyes",gray_img);
                    Rect canthus_ll_rect(algorithm.leftEyeRect.x+algorithm.leftEyeRect.width/4*3,algorithm.leftEyeRect.y,algorithm.leftEyeRect.width/4, algorithm.leftEyeRect.height);
                    Rect canthus_lr_rect(algorithm.leftEyeRect.x,algorithm.leftEyeRect.y,algorithm.leftEyeRect.width/4, algorithm.leftEyeRect.height);
                    Rect canthus_rr_rect(algorithm.rightEyeRect.x,algorithm.rightEyeRect.y,algorithm.rightEyeRect.width/4, algorithm.rightEyeRect.height);
                    Rect canthus_rl_rect(algorithm.rightEyeRect.x+algorithm.rightEyeRect.width/4*3,algorithm.rightEyeRect.y,algorithm.rightEyeRect.width/4, algorithm.rightEyeRect.height);
                    Mat  faceMask = Mat::zeros(color_img.size(), CV_8UC1);
                    faceMask(canthus_ll_rect) = 255;
                    faceMask(canthus_lr_rect) = 255;
                    faceMask(canthus_rr_rect) = 255;
                    faceMask(canthus_rl_rect) = 255;
                    Mat masked ;
                    color_img.copyTo(masked, faceMask);
                    goodFeaturesToTrack(gray_img, privFeatures, 4 , 0.05, 40,faceMask,6,false);
                    
                    for (int i = 0 ; i < privFeatures.size() ; i ++) {
                        circle(masked, privFeatures[i], 3, Scalar(255,0,0));
                        //                        cout<<errors[0]<<" "<<errors[1]<<","<<(int)trackingStatus[0]<<" "<<(int)trackingStatus[1]<<endl;
                        
                    }
                    imshow("eyes_mask",masked);
                    
                    
                    hasEyeFound = true;
                    cout <<"eye found"<<endl;
                }
            }
            
        } else {
            //            equalizeHist(gray_img(algorithm.leftEyeRect), gray_img(algorithm.leftEyeRect));
            //            equalizeHist(gray_img(algorithm.rightEyeRect), gray_img(algorithm.rightEyeRect));
            calcOpticalFlowPyrLK(priv_img, gray_img, privFeatures, nextFeatures
                                 , trackingStatus, errors);
            //            tracker.trackf2f(priv_img, gray_img, privFeatures, nextFeatures);
            priv_img = gray_img.clone();
            privFeatures = nextFeatures;
            nextFeatures.clear();
            
            for (int i = 0 ; i < privFeatures.size() ; i ++) {
                circle(color_img, privFeatures[i], 3, Scalar(255,0,0));
                //                cout<<errors[0]<<" "<<errors[1]<<","<<(int)trackingStatus[0]<<" "<<(int)trackingStatus[1]<<endl;
                
            }
            
            
        }
        imshow("track eye",color_img);
        if (waitKey(30) > 0)
            break;
        
    }
    
}

int EyeContourExtraction()
{
    
    Rect faceRect;
    Mat original_img,color_img;
    vector<Point2f> features;
    
    color_img = captureImage(capture);
    while (true) {
        
        
        
        color_img = captureImage(capture);
        Rect faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
        algorithm.findEyes(color_img.clone(),faceRect);
        
        Mat eye_img = color_img( algorithm.leftEyeRect);
        imshow("eye color", eye_img);
        Mat eye_skin_mask = cvSkinOtsu(eye_img);
        dilate(eye_skin_mask,eye_skin_mask,Mat(3,3,CV_8UC1));
        eye_skin_mask = Scalar::all(255) - eye_skin_mask;
        eye_skin_mask = removeSmallBlobs(eye_skin_mask);
        //        eye_skin_mask = fillHoleInBinary(eye_skin_mask);
        //        dilate(eye_skin_mask,eye_skin_mask,Mat(5,5,CV_8UC1));
        //        Mat eye_skin_mask2 = cvSkinOtsu(eye_img);
        eye_skin_mask = Scalar::all(255) - eye_skin_mask;
        //        bitwise_xor(eye_skin_mask, eye_skin_mask2, eye_skin_mask);
        imshow("eye skin" ,eye_skin_mask);
        Mat masked ;
        eye_img.copyTo(masked, eye_skin_mask);
        imshow("eye masked" ,masked);
        Mat eye_gray ;
        cvtColor(eye_img, eye_gray, CV_RGB2GRAY);
        goodFeaturesToTrack(eye_gray, features, 2 , 0.01, 20,eye_skin_mask);
        for (int i = 0 ; i < features.size() ; i ++) {
            circle(eye_img, features[i], 3, Scalar(255,0,0));
        }
        imshow("eye color", eye_img);
        
        if (waitKey(30) > 0)
            break;
    }
    return 0;
}


int eyeLocalizationDynamic2()
{
    
    Rect faceRect,noseRect;
    Mat original_img,color_img,gray_img,facePrivImg, eyesPrivImg,curImg,faceEdge_img, edge_img,skinMask;
    vector<Point2f> faceBaseFeatures, facePrivFeatures, faceNextFeatures,eyesBaseFeatures, measuredFeatures, eyesPrivFeatures,eyesNextFeatures,nosePrivFeatures;
    vector<uchar> faceTrackingStatus, eyesTrackingStatus;
    vector<float> faceTrackingErrors, eyesTrackingErrors;
    bool needfeatureInitialization = true;
    Mat faceColorHist, backProj;
    LKTracker tracker;
    KalmanFilter KF(2, 2,2);
    setIdentity(KF.transitionMatrix,Scalar::all(1));
    setIdentity(KF.controlMatrix,Scalar::all(1));
    setIdentity(KF.measurementMatrix);
    setIdentity(KF.processNoiseCov, Scalar::all(1e-3));
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-2));
    setIdentity(KF.errorCovPost, Scalar::all(.1));
    Mat estimate;
    while (true) {
        
        color_img = captureImage(capture);
        cvtColor(color_img, gray_img, CV_RGB2GRAY);
        equalizeHist(gray_img, gray_img);
        
        
        if (needfeatureInitialization) {
            faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
            algorithm.findEyes(color_img, faceRect);
            Mat  faceMask = Mat::zeros(color_img.size(), CV_8UC1);
            faceMask(faceRect) = 255;
            skinMask = cvSkinYUV(color_img,true);
            dilate(skinMask, skinMask, Mat(6,6,CV_8UC1));
            erode(skinMask, skinMask, Mat(6,6,CV_8UC1));
            bitwise_and(faceMask, skinMask, faceMask);
            imshow("faceMask",faceMask);
            
            
            if (algorithm.eyeFound ) {
                
                eyesPrivFeatures = ASEF_Algorithm::getInnerCanthus(gray_img, algorithm.leftEyeRect, algorithm.rightEyeRect);
                
                
                
                eyesPrivImg = gray_img.clone();
                estimate = Mat(eyesPrivFeatures[0]).clone().reshape(1,2);
                KF.statePre =estimate;
                
            }
            if (faceRect.area()> 0 && algorithm.eyeFound) {
                needfeatureInitialization = false;
                imshow("points",color_img);
                
            }
        } else{
            
            faceRect = haarPatternDetection(frontalface_cascade, color_img, 200);
            algorithm.findEyes(color_img, faceRect);
            
            
            //            tracker.trackf2f(eyesPrivImg, gray_img, eyesPrivFeatures, eyesNextFeatures);
            
            calcOpticalFlowPyrLK(eyesPrivImg, gray_img, eyesPrivFeatures, eyesNextFeatures
                                 , eyesTrackingStatus, eyesTrackingErrors);
            cout<<eyesTrackingErrors[0]<<endl;
            eyesPrivImg =gray_img.clone();
            
            
            
            if (norm(Mat(eyesNextFeatures[0]).clone().reshape(1,2)-estimate)>10) {
                eyesNextFeatures[0] = Point2f(estimate.at<float>(0),estimate.at<float>(1));
            }
            else {
                estimate = KF.predict(Mat(eyesNextFeatures[0]).clone().reshape(1,2)-Mat(eyesPrivFeatures[0]).clone().reshape(1,2));
            }
            
            eyesPrivFeatures = eyesNextFeatures;
            eyesNextFeatures.clear();
            
            if (algorithm.eyeFound) {
                measuredFeatures = ASEF_Algorithm::getInnerCanthus(gray_img, algorithm.leftEyeRect, algorithm.rightEyeRect);
                estimate = KF.correct(Mat(measuredFeatures[0]).reshape(1,2));
                
                for (int i = 0 ; i < measuredFeatures.size()-1 ; i ++) {
                    circle(color_img, measuredFeatures[i], 3, Scalar(255,0,0));
                }
                
                if (norm(estimate-Mat(measuredFeatures[0]).clone().reshape(1,2))<3) {
                    eyesPrivFeatures[0] =measuredFeatures[0];
                    setIdentity(KF.measurementNoiseCov, Scalar::all(1e2));
                    //Point2f(estimate.at<float>(0),estimate.at<float>(1));
                    
                } else {
                    setIdentity(KF.measurementNoiseCov, Scalar::all(1e0));
                }
                
                
            }
            
            circle(color_img, Point(estimate.at<float>(0),estimate.at<float>(1)), 3, Scalar(255,0,255));
            circle(color_img, Point(estimate.at<float>(2),estimate.at<float>(3)), 3, Scalar(0,0,255));
            
            for (int i = 0 ; i < eyesPrivFeatures.size() ; i ++) {
                circle(color_img, eyesPrivFeatures[i], 3, Scalar(0,255,0));
            }
            
            
            drawBox(color_img, algorithm.leftEyeRect);
            drawBox(color_img, algorithm.rightEyeRect);
        }
        //        imshow("edge gray",faceEdge_img);
        imshow("color", color_img);
        if (waitKey(30) > 0)
            break;
    }
    return 0;
}


int surfTracker() {
    Rect faceRect,noseRect;
    Mat original_img,color_img,gray_img,facePrivImg, eyesPrivImg,curImg;
    vector<Point2f> faceBaseFeatures, facePrivFeatures, faceNextFeatures,eyesBaseFeatures, eyesPrivFeatures,eyesNextFeatures,nosePrivFeatures;
    vector<uchar> faceTrackingStatus, eyesTrackingStatus;
    vector<float> faceTrackingErrors, eyesTrackingErrors;
    bool needfeatureInitialization = true;
    Mat faceColorHist, backProj;
    
    
    SiftFeatureDetector detector(0);
    vector<KeyPoint> templateKPs, detectedKPs;
    SiftDescriptorExtractor extractor;
    Mat template_img, templateDescriptor, detectedDescriptor;
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    
    while (true) {
        
        color_img = captureImage(capture);
        cvtColor(color_img, gray_img, CV_RGB2GRAY);
        equalizeHist(gray_img, gray_img);
        
        if (needfeatureInitialization) {
            faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
            algorithm.findEyes(color_img, faceRect);
            
            if (algorithm.eyeFound ) {
                
                template_img =imresize(gray_img(algorithm.leftEyeRect).clone(), 2.0f);
                detector.detect(template_img, templateKPs);
                extractor.compute(template_img, templateKPs, templateDescriptor);
                
            }
            
            if (faceRect.area()> 0 && algorithm.eyeFound) {
                needfeatureInitialization = false;
                
            }
        } else{
            faceRect = haarPatternDetection(frontalface_cascade, color_img, 150);
            algorithm.findEyes(color_img, faceRect);
            
            detector.detect(gray_img(algorithm.leftEyeRect), detectedKPs);
            extractor.compute(gray_img, detectedKPs, detectedDescriptor);
            matcher.match( templateDescriptor, detectedDescriptor, matches );
            
            
            double max_dist = 0; double min_dist = 100;
            
            //-- Quick calculation of max and min distances between keypoints
            for( int i = 0; i < templateDescriptor.rows; i++ )
            { double dist = matches[i].distance;
                if( dist < min_dist ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }
            
            printf("-- Max dist : %f \n", max_dist );
            printf("-- Min dist : %f \n", min_dist );
            
            //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist,
            //-- or a small arbitary value ( 0.02 ) in the event that min_dist is very
            //-- small)
            //-- PS.- radiusMatch can also be used here.
            std::vector< DMatch > good_matches;
            
            for( int i = 0; i < templateDescriptor.rows; i++ )
            {
                //                good_matches.push_back( matches[i]);
                if( matches[i].distance <= max(1*min_dist, 0.02) )
                { good_matches.push_back( matches[i]); }
            }
            
            //-- Draw only "good" matches
            Mat img_matches;
            drawMatches( template_img, templateKPs, gray_img(algorithm.leftEyeRect), detectedKPs,
                        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
                        vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
            
            
            imshow( "Good Matches", img_matches );
        }
        
        if (waitKey(30) > 0)
            break;
    }
    
    
    return 0;
    
}


int eyeLipsLocalization()
{
    
    Rect faceRect;
    Mat original_img,color_img,gray_img,bw_img,preImg,curImg;
    vector<Point2f> prevFeatures, nextFeatures;
    vector<uchar> trackingStatus;
    vector<float> errors;
    bool needfeatureReInitialize = true;
    int frameCount = 0 ;
    Mat faceColorHist, backProj;
    RotatedRect faceRotRect;
    Point leftEyeCenter, rightEyeCenter;
    
    while (true) {
        frameCount ++;
        
        color_img = captureImage(capture);
        cvtColor(color_img, gray_img, CV_RGB2GRAY);
        equalizeHist(gray_img, gray_img);
        
        if (needfeatureReInitialize) {
            Rect faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
            Mat  faceMask = Mat::zeros(color_img.size(), CV_8UC1);
            faceMask(faceRect) = 255;
            
            
            
            Mat skinMask = cvSkinYUV(color_img,true);
            dilate(skinMask, skinMask, Mat(6,6,CV_8UC1));
            erode(skinMask, skinMask, Mat(6,6,CV_8UC1));
            bitwise_and(faceMask, skinMask, faceMask);
            imshow("faceMask",faceMask);
            
            faceColorHist = calculateColorHistogram(color_img, faceMask,2);
            //            drawColorHistGram(faceColorHist);
            needfeatureReInitialize = false;
            
        } else{
            Rect faceRect = haarPatternDetection(frontalface_cascade,color_img,200);
            algorithm.findEyes(color_img.clone(),faceRect);
            Mat leftEye_img = color_img(algorithm.leftEyeRect).clone();
            Mat rightEye_img =color_img(algorithm.rightEyeRect).clone();
            
            if (leftEye_img.rows * leftEye_img.cols >0) {
                
                leftEyeCenter = findEyeCenterByMassCenter(leftEye_img);
                //                rightEyeCenter =findEyeCenterByMassCenter(rightEye_img);
            }
            
            
            
            leftEyeCenter +=  algorithm.leftEyeRect.tl();
            rightEyeCenter+= algorithm.rightEyeRect.tl();
            circle(color_img,leftEyeCenter,3,Scalar(0,255,0));
            circle(color_img,rightEyeCenter,3,Scalar(0,255,0));
            circle(color_img,algorithm.leftEyePoint,3,Scalar(255,0,0));
            circle(color_img,algorithm.rightEyePoint,3,Scalar(255,0,0));
        }
        
        imshow("color", color_img);
        if (waitKey(30) > 0)
            break;
    }
    return 0;
}


