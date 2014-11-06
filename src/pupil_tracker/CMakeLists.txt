SET(asmgazesource
ASMGazeTracker.h ASMGazeTracker.cpp
)

ADD_EXECUTABLE(PupilTracker PupilTracker.cpp ${asmgazesource})
ADD_EXECUTABLE(HeadPoseAdviser HeadPoseAdviser.cpp ${asmgazesource})
ADD_EXECUTABLE(CurveDrawer CurveDrawer.cpp )
ADD_EXECUTABLE(CurveGazingRecorder CurveGazingRecorder.cpp)
ADD_EXECUTABLE(CameraCalibrator CameraCalibrator.cpp)
ADD_EXECUTABLE(AnnotationFlip AnnotationFlip.cpp)
ADD_EXECUTABLE(Scan3DFaceModel Scan3DFaceModel.cpp ${asmgazesource})

TARGET_LINK_LIBRARIES(Scan3DFaceModel ${OpenCV_LIBS} asmlib jzplib)
TARGET_LINK_LIBRARIES(AnnotationFlip ${OpenCV_LIBS} asmlib jzplib)
TARGET_LINK_LIBRARIES(PupilTracker ${OpenCV_LIBS} asmlib jzplib)
TARGET_LINK_LIBRARIES(HeadPoseAdviser ${OpenCV_LIBS} asmlib jzplib)
TARGET_LINK_LIBRARIES(CurveDrawer ${OpenCV_LIBS} ${Boost_LIBRARIES} asmlib jzplib)
TARGET_LINK_LIBRARIES(CurveGazingRecorder ${OpenCV_LIBS} ${Boost_LIBRARIES} asmlib jzplib)
TARGET_LINK_LIBRARIES(CameraCalibrator ${OpenCV_LIBS} ${Boost_LIBRARIES} jzplib)