SET(asmptsource
ASMPupilTracker.h ASMPupilTracker.cpp
)

ADD_EXECUTABLE(pupil_tracker pupil_tracker.cpp ${asmptsource})
ADD_EXECUTABLE(headpose_tracking HeadPoseEstimation.cpp ${asmptsource})
ADD_EXECUTABLE(camcorder Camcorder.cpp ${asmptsource})
ADD_EXECUTABLE(curvedrawer PolyFitCurveDrawer.cpp ${asmptsource})
ADD_EXECUTABLE(CurveGazingRecorder CurveGazingRecorder.cpp ${asmptsource})



TARGET_LINK_LIBRARIES(pupil_tracker ${OpenCV_LIBS} ft jzplib) 
TARGET_LINK_LIBRARIES(headpose_tracking ${OpenCV_LIBS} ft jzplib) 
TARGET_LINK_LIBRARIES(camcorder ${OpenCV_LIBS} ${Boost_LIBRARIES} ft jzplib)
TARGET_LINK_LIBRARIES(curvedrawer ${OpenCV_LIBS} ${Boost_LIBRARIES} ft jzplib)
TARGET_LINK_LIBRARIES(CurveGazingRecorder ${OpenCV_LIBS} ${Boost_LIBRARIES} ft jzplib)