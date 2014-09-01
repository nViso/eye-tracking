SET(asmptsource
ASMPupilTracker.h ASMPupilTracker.cpp
)

ADD_EXECUTABLE(PupilTracker PupilTracker.cpp ${asmptsource})
ADD_EXECUTABLE(HeadPoseEstimation HeadPoseEstimation.cpp )
ADD_EXECUTABLE(CurveDrawer CurveDrawer.cpp )
ADD_EXECUTABLE(CurveGazingRecorder CurveGazingRecorder.cpp ${asmptsource})


TARGET_LINK_LIBRARIES(PupilTracker ${OpenCV_LIBS} asmlib jzplib)
TARGET_LINK_LIBRARIES(HeadPoseEstimation ${OpenCV_LIBS} asmlib jzplib)
TARGET_LINK_LIBRARIES(CurveDrawer ${OpenCV_LIBS} ${Boost_LIBRARIES} asmlib jzplib)
TARGET_LINK_LIBRARIES(CurveGazingRecorder ${OpenCV_LIBS} ${Boost_LIBRARIES} asmlib jzplib)