ADD_EXECUTABLE(pupil_tracker pupil_tracker.cpp)
ADD_EXECUTABLE(camcorder Camcorder.cpp)
ADD_EXECUTABLE(symmetry_test SymmetryDetectionTest.cpp)


TARGET_LINK_LIBRARIES(pupil_tracker ${OpenCV_LIBS} ft jzplib) 
TARGET_LINK_LIBRARIES(symmetry_test ${OpenCV_LIBS} ft jzplib)
TARGET_LINK_LIBRARIES(camcorder ${OpenCV_LIBS} ${Boost_LIBRARIES} ft jzplib)