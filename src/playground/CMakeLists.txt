ADD_EXECUTABLE(playground playground.h playground.cpp)

TARGET_LINK_LIBRARIES(playground ${OpenCV_LIBS} ${Boost_LIBRARIES} ft jzplib)