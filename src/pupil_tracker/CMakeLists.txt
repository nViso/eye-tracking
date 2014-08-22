ADD_EXECUTABLE(pupil_tracker pupil_tracker.cpp)
ADD_EXECUTABLE(headpose_tracking HeadPoseEstimation.cpp)
ADD_EXECUTABLE(camcorder Camcorder.cpp)


TARGET_LINK_LIBRARIES(pupil_tracker ${OpenCV_LIBS} ft jzplib) 
TARGET_LINK_LIBRARIES(headpose_tracking ${OpenCV_LIBS} ft jzplib) 
TARGET_LINK_LIBRARIES(camcorder ${OpenCV_LIBS} ${Boost_LIBRARIES} ft jzplib)