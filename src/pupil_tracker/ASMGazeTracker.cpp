//
//  ASMPupilTracker.cpp
//  JZP_EYE_TRACKING
//
//  Created by Zhiping Jiang on 14-8-31.
//
//

#include "ASMGazeTracker.h"

ASM_Gaze_Tracker::ASM_Gaze_Tracker(const fs::path & trackermodel, const fs::path & cameraProfile) {
    tracker = load_ft<face_tracker>(trackermodel.string());
    tracker.detector.baseDir = trackermodel.parent_path().string() + fs::path("/").make_preferred().native();
    
    if (cameraProfile.empty() == false) {
        readCameraProfile(cameraProfile, cameraMatrix, distCoeffs);
        findBestFrontalFaceShapeIn3D();
    }
}

vector<float> ASM_Gaze_Tracker::toDataSlot() {
    vector<float> slot;
    
    if (isTrackingSuccess == false) {
        for (int i = 0; i < 80; i++) {
            slot.push_back(0.0f);
        }
        return slot;
    }
    
    slot.push_back(leftEyePoint.x);
    slot.push_back(leftEyePoint.y);
    slot.push_back(rightEyePoint.x);
    slot.push_back(rightEyePoint.y);
    for (int i = 0; i < tracker.points.size(); i++) {
        slot.push_back(tracker.points[i].x);
        slot.push_back(tracker.points[i].y);
    }
    for (int i = 0; i < reprojectedFacialPointsInImage.size(); i++) {
        slot.push_back(reprojectedFacialPointsInImage[i].x);
        slot.push_back(reprojectedFacialPointsInImage[i].y);
    }
    
    Mat temp = cameraMatrix.t();
    temp = temp.reshape(1,1);
    for (int i = 0 ; i < 9 ; i ++) {
        slot.push_back((float)temp.at<double>(i));
    }
    
    for (int i = 0 ; i < 3 ; i ++) {
        slot.push_back((float)tvec.at<double>(i));
    }
    poseRMatrix = poseRMatrix.t();
    poseRMatrix = poseRMatrix.reshape(1,1);
    for (int i = 0 ; i < 9 ; i ++) {
        slot.push_back((float)poseRMatrix.at<double>(i));
    }
    
    slot.push_back(leftCenterInFace.x);
    slot.push_back(leftCenterInFace.y);
    slot.push_back(leftCenterInFace.z);
    slot.push_back(rightCenterInFace.x);
    slot.push_back(rightCenterInFace.y);
    slot.push_back(rightCenterInFace.z);
    
    for (int i = 0 ; i < facialPointsIn3D.size() ; i ++) {
        slot.push_back(facialPointsIn3D[i].x);
        slot.push_back(facialPointsIn3D[i].y);
        slot.push_back(facialPointsIn3D[i].z);
    }
    
    
    return slot;
}

bool ASM_Gaze_Tracker::featureTracking(const cv::Mat & im) {
    if(tracker.track(im) == 0) {// failed
        isTrackingSuccess = false;
        return false;
    }
    glabellaPoint = tracker.points[0]*0.5 + tracker.points[1]*0.5;
    im.copyTo(this->im);
    isTrackingSuccess = true;
    return true;
    
}

void ASM_Gaze_Tracker::reDetectFace() {
    tracker.reset();
}

bool ASM_Gaze_Tracker::calculatePupilCenter(){
    Mat  leftEyeImg,rightEyeImg,cropped;

    if (isTrackingSuccess == false) {
        return false;
    }
    
    canthusPts = vector<Point2f>(tracker.points.begin(),tracker.points.begin()+4);
    nosePts = vector<Point2f>(tracker.points.begin()+4,tracker.points.begin()+6);
    
    if (canthusPts[3].x < canthusPts[1].x && canthusPts[1].x < canthusPts[0].x && canthusPts[0].x < canthusPts[2].x) {
    } else {
        return false;
    }
    
    eyePairTileAngle = calculateEyePairTileAngle(canthusPts);
    glabellaPoint= caculateEyePairCenter(canthusPts);
    
    rotationMatrix = getRotationMatrix2D(glabellaPoint, eyePairTileAngle, 1.0);
    Mat Mback = getRotationMatrix2D(glabellaPoint, -eyePairTileAngle, 1.0);
    
    vector<Point2f> rotatedCanthusPts = rotatePointsByRotationMatrix(canthusPts, rotationMatrix);
    vector<Point2f> rotatedNosePts = rotatePointsByRotationMatrix(nosePts, rotationMatrix);
    
    float eyePairRectWidth =abs(rotatedCanthusPts[2].x - rotatedCanthusPts[3].x)+1;
    Size2f eyePairRectSize(eyePairRectWidth,eyePairRectWidth/7);
    Rect cropRect(Point2f(glabellaPoint.x-eyePairRectWidth/2,glabellaPoint.y -eyePairRectWidth/14.0f),eyePairRectSize);
    
    warpAffine(im, rotated_img, rotationMatrix, im.size(),CV_INTER_LINEAR);
    getRectSubPix(rotated_img, eyePairRectSize, glabellaPoint, cropped);
    
    Rect rightEyeRect = Rect(0,0,rotatedCanthusPts[1].x-rotatedCanthusPts[3].x,eyePairRectSize.height);
    Rect leftEyeRect = Rect(rotatedCanthusPts[0].x-rotatedCanthusPts[3].x,0,rotatedCanthusPts[2].x-rotatedCanthusPts[0].x,eyePairRectSize.height);
    
    if (leftEyeRect.area() < 50 || rightEyeRect.area()< 50) {
        return false;
    }
    cropped(leftEyeRect).copyTo(leftEyeImageRectified);
    cropped(rightEyeRect).copyTo(rightEyeImageRectified);
    Point2f leftEyeCenter, rightEyeCenter;
    
//    eyeCenterLocalizationImpl(leftEyeImageRectified, leftEyeCenter);
//    eyeCenterLocalizationImpl(leftEyeImageRectified, rightEyeCenter);
//    cout<<"debug"<<endl;
    
    boost::thread leftEyeThread(eyeCenterLocalizationImpl, leftEyeImageRectified, boost::ref(leftEyeCenter), 0.35,4,3,0.26);
    boost::thread  rightEyeThread(eyeCenterLocalizationImpl, leftEyeImageRectified, boost::ref(rightEyeCenter), 0.34,4,3,0.26);
    leftEyeThread.join();
    rightEyeThread.join();
    
    leftEyeCenter += Point2f(leftEyeRect.tl().x,leftEyeRect.tl().y);
    leftEyeCenter += Point2f(cropRect.tl().x, cropRect.tl().y);
    rightEyeCenter += Point2f(rightEyeRect.tl().x,rightEyeRect.tl().y);
    rightEyeCenter += Point2f(cropRect.tl().x,cropRect.tl().y);
    
    leftEyePoint= rotatePointByRotationMatrix(leftEyeCenter, Mback);
    rightEyePoint= rotatePointByRotationMatrix(rightEyeCenter, Mback);
    isTrackingSuccess = true;
    return true;
}

bool ASM_Gaze_Tracker::estimateFacePose() {
    if (isTrackingSuccess == false) {
        return false;
    }
    vector<Point2f> imagePoints = tracker.points;
    float maxErrorDistance = norm(imagePoints[2]-imagePoints[3])/20.0f;
    solvePnPRansac(facialPointsIn3D, imagePoints, cameraMatrix, distCoeffs, rvec, tvec, false, 20,maxErrorDistance,7);
//    solvePnP(facialPointsIn3D, imagePoints, cameraMatrix, distCoeffs, rvec, tvec, false, CV_EPNP);
    this->projectPoints(facialPointsIn3D, reprojectedFacialPointsInImage);
    // change the rvec to rotation matrix, and then reshape the matrix to a row vector.
    // please note that, the opencv reshapes the matrix by the row-first order. However,
    // Matlab's reshape is column-first, so, here the matrix is transposed before opencv's reshape.
    Rodrigues(rvec, poseRMatrix);
    return true;
}

void ASM_Gaze_Tracker::estimatePupilCenterFaceCoordinates() {
    Point3d facePlanNormInW(0.0f,0.0,100.0);
    Mat facePlanNormInC = poseRMatrix*(Mat(facePlanNormInW).reshape(1));
    
    vector<Point2f> pupilsInPixel;
    pupilsInPixel.push_back(leftEyePoint);
    pupilsInPixel.push_back(rightEyePoint);
    vector<Point3f> pupilsInFace;
    vector<Point3f> pupilsInHomo;
    convertPointsToHomogeneous(pupilsInPixel, pupilsInHomo);
    for (int i = 0 ; i < pupilsInHomo.size() ; i ++) {
        Point3f pHomo = pupilsInHomo[i];
        Mat pInFocalPlane;
        Mat(pHomo).convertTo(pInFocalPlane, CV_64F);
        pInFocalPlane = (cameraMatrix.inv())*pInFocalPlane;
        pInFocalPlane = pInFocalPlane / norm(pInFocalPlane);
        Point3d pHeight(0,0,1.0f);
        Mat pHeightPlanePoint = poseRMatrix*Mat(pHeight).reshape(1) + Mat(tvec).reshape(1);
        Mat p3DInCamera = pInFocalPlane*facePlanNormInC.dot(pHeightPlanePoint)/facePlanNormInC.dot(pInFocalPlane);
        Mat p3DtoW = p3DInCamera -  Mat(tvec).reshape(1);
        Mat p3DInW = poseRMatrix.t()*p3DtoW;
        pupilsInFace.push_back(Point3f(p3DInW));
    }

    leftCenterInFace = pupilsInFace[0];
    rightCenterInFace = pupilsInFace[1];
//    vector<Point2f> pupilCenterReprojected;
//    projectPoints(pupilsInFace, pupilCenterReprojected);
//    cout<<"left error"<<norm(leftEyePoint - pupilCenterReprojected[0])<<" right error"<<norm(rightEyePoint - pupilCenterReprojected[1])<<endl;
//    cout<<"leftCenterInFace"<<leftCenterInFace<<endl;
//    cout<<"rightCenterInFace"<<rightCenterInFace<<endl;
}

void ASM_Gaze_Tracker::projectPoints(const vector<Point3f> & sourcePoints, vector<Point2f> & destPoints) {
    cv::projectPoints(sourcePoints, rvec, tvec, cameraMatrix, distCoeffs, destPoints,projectionJacobian);
}

float ASM_Gaze_Tracker::distanceToCamera() {
    return norm(tvec);
}

// No matter how to optimize the metric, the result is still not better than the manually annotated first image.
// This is because, the model is not always aligned with true head 3D movement. The ASM model is just a partial 2D fitting for the 3D points.
void ASM_Gaze_Tracker::findBestFrontalFaceShapeIn3D()  {
    fs::path faceModelFile = fs::path(tracker.detector.baseDir) / "facial2dmodel.txt";
    if (fs::exists(faceModelFile)) {
        vector< vector<float> > datas = parseTextTableFile(faceModelFile.string()," ");
        vector<Point2f> points;
        for (int i = 0 ; i < datas.size() ; i ++) {
            Point2f p(datas[i][0],datas[i][1]);
            points.push_back(p);
        }
        facialPointsIn2D = points;
        cout<<"from txt:"<<points<<endl;
    }
    
    vector<vector<Point2f> > pointsSeries = tracker.smodel.matY2pts();
    vector<Point2f> bestFace;
    if (pointsSeries[0].size()<10) {
        bestFace = pointsSeries[0];
    } else {
        
        float scale = 1.0f;//calc_scale(tracker.smodel.V.col(0), 200);
        float tranx = 0.0f;//n * 150.0 / tracker.smodel.V.col(2).dot(Mat::ones(2 * n, 1, CV_32F));
        float trany = 0.0f;//n * 150.0 / tracker.smodel.V.col(3).dot(Mat::ones(2 * n, 1, CV_32F));
        
        float largestRatio = 1000.0f;
        float largestArea = 0.0f;
        for (float i = -1.0f ; i <1.0f ; i +=0.02)
            for (float j = -1.0f ; j <1.0f ; j +=0.02) {
//                cout<<i<<" "<<j<<endl;
                Mat p = Mat::zeros(tracker.smodel.V.cols,1,CV_32F);
                p.at<float>(0) = scale;
                p.at<float>(2) = tranx;
                p.at<float>(3) = trany;
                p.at<float>(4) = scale * i * 3.0 * sqrt(tracker.smodel.e.at<float>(4));
                p.at<float>(5) = scale * j * 3.0 * sqrt(tracker.smodel.e.at<float>(5));
                
                p.copyTo(tracker.smodel.p);
                vector<Point2f> cp = tracker.smodel.calc_shape();
                Point2f glabellaPoint  = cp[0] * 0.5f + cp[1] * 0.5f ;
                Point2f noseCenter     = cp[4] * 0.5f + cp[5] * 0.5f ;
                Point2f philtrumPoint  = cp[6];

                float noisePhiltrumRatio = norm(glabellaPoint - noseCenter)/norm(noseCenter - philtrumPoint);
                float area = contourArea(cp);
//                cout<<"ratio"<<noisePhiltrumRatio<<endl;
                float verticality = tileRadianBtwn2Pts(cp[2],cp[6]) + tileRadianBtwn2Pts(cp[6], cp[3]) + tileRadianBtwn2Pts(cp[0],cp[6]) + tileRadianBtwn2Pts(cp[6], cp[1]) + tileRadianBtwn2Pts(cp[0], cp[4]) + tileRadianBtwn2Pts(cp[5], cp[1]) + tileRadianBtwn2Pts(cp[2],cp[4]) + tileRadianBtwn2Pts(cp[5], cp[3]);
                verticality = abs(rad2deg(verticality));
//                cout<< verticality<<endl;
                if (verticality < 10 && area > largestArea) {
                    cout<<i<<" "<<j<<endl;
                    cout<<"best"<<verticality<<" "<<area<<endl;
                    bestFace = cp;
                    largestArea = area;
                }
                
            }
    }

    
    vector<Point2f> points;
    if (fs::exists(faceModelFile))
         points = facialPointsIn2D;
    else points= bestFace;

	Point2f center = points[0] * 0.5f + points[1] * 0.5f;
	float normVaue = tracker.annotations.getDistanceBetweenOuterCanthuses()
			/ norm(points[3] - points[2]);
	for (int j = 0; j < points.size(); j++) {
		points[j] = center - points[j];
		points[j].y = -points[j].y;
		points[j] *= normVaue;
	}
	vector<Point3f> faceFeatures;
	vector<Point2f> faceFeatures2;
	for (int i = 0; i < points.size(); i++) {
		faceFeatures.push_back(Point3f(points[i].x, points[i].y, 0));
		faceFeatures2.push_back(Point2f(points[i].x, points[i].y));
	}
    faceFeatures[4].z = tracker.annotations.getNoseHeight();//noseHeight;
    faceFeatures[5].z = tracker.annotations.getNoseHeight();//noseHeight;
    faceFeatures[6].z = tracker.annotations.getPhiltrumHeight();//philtrumHeight;

	facialPointsIn3D = faceFeatures;
	facialPointsIn2D = faceFeatures2;
    cout<<"final:"<<facialPointsIn3D<<endl;
}

void ASM_Gaze_Tracker::eyeCenterLocalizationImpl(const Mat& image, Point2f & eyeCoord, float coordinateWeight, int kmeansIterations, int kmeansRepeats, float blurSizeRatio) {
    
    Mat img, gray_img, bluredGrayImg, temp;
    Mat colorpoints, kmeansPoints;
    
    img = equalizeImage(image);
    
    int width = img.cols;
    int ksize =(int) (blurSizeRatio * width);
    if (ksize %2 ==0) {
        ksize ++;
    }
    
    //        GaussianBlur(img, img, Size(ksize,ksize),ksize);
    gray_img = img.clone();
    medianBlur(img, img, ksize);
    //        imshow("blured",img);
    cvtColor(img, bluredGrayImg, CV_BGR2GRAY);
    
    bluredGrayImg = imcomplement(bluredGrayImg);
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
    //        imagesc("layed", index_img);
    double minVal , maxVal;
    Point minLoc, maxLoc;
    
    int bestx = 0,bestscore = 9999999 ,bestlayer = 0;
    Mat bestIndex_img = index_img >=1;
    minMaxLoc(index_img,&minVal,&maxVal,&minLoc,&maxLoc);
    int levels = maxVal;
    for (int i = 1 ; i<=levels; i++) {
        Mat indexlayer_img = index_img >=i;
        medianBlur(indexlayer_img, indexlayer_img, 5);
        erode(indexlayer_img, indexlayer_img, ksize);
        erode(indexlayer_img, indexlayer_img, ksize);
        indexlayer_img = removeSmallBlobs(indexlayer_img);
        
        indexlayer_img = fillHoleInBinary(indexlayer_img);
        indexlayer_img = fillConvexHulls(indexlayer_img);
        
        Mat score = calculateImageSymmetryScore(indexlayer_img);
        Mat scoresum;
        reduce(score.rowRange(0, width/6), scoresum, 0, CV_REDUCE_SUM,CV_32FC1);
        
        int currentScore = sum(scoresum).val[0];
        //            cout<<"score"+boost::lexical_cast<string>(i)+":"<<currentScore<<endl;
        //            plotLiveData("score"+boost::lexical_cast<string>(i), currentScore);
        //            plotVectors("scoresum"+boost::lexical_cast<string>(i), scoresum.t());
        if (bestscore > currentScore && currentScore > 0) {
            bestscore = currentScore;
            minMaxLoc(scoresum,&minVal,&maxVal,&minLoc,&maxLoc);
            bestx = maxLoc.x;
            bestlayer = i;
            bestIndex_img = indexlayer_img.clone();
        }
    }
    
    //        imshow("best",bestIndex_img);
    Point2f massCenter = findMassCenter_BinaryBiggestBlob(bestIndex_img);
    
    Mat maskOut(bestIndex_img.size(),CV_8UC1,Scalar::all(255));
    gray_img.copyTo(maskOut, bestIndex_img);
    //    imshow("maskout",maskOut);
    Point2f isoCenter;
    Mat centermap = isoPhote(gray_img, true, width/7, width/4, Size(ksize,ksize), ksize*0.25,bestIndex_img);
    
    if (massCenter.x >0 && massCenter.y >0) {
        circle(centermap, Point2f(bestx,massCenter.y), ksize/4, Scalar::all(50));
    }
    
    GaussianBlur(centermap, centermap, Size(ksize,ksize), ksize/2);
    
    //        imagesc("hybrid",centermap);
    
    minMaxLoc(centermap,&minVal,&maxVal,&minLoc,&maxLoc);
    eyeCoord =  Point2f(bestx,maxLoc.y);

}
