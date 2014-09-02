SET(portal_source Portal.h Portal.cpp)

ADD_EXECUTABLE(Portal ${portal_source})

TARGET_LINK_LIBRARIES(Portal ${OpenCV_LIBS} ${OpenCV_LIBS} asmlib jzplib)