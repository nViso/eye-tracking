PROJECT(JZP_EYE_TRACKING)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 


cmake_minimum_required(VERSION 2.8)
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system filesystem date_time thread REQUIRED)

INCLUDE_DIRECTORIES(
${OPENCV_INCLUDE_DIR}
${Boost_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
)


SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../lib CACHE PATH
     "Output directory for the OpenCV_hotshots library" )
SET( EXECUTABLE_OUTPUT_PATH 
     ${PROJECT_BINARY_DIR}/../bin CACHE PATH
     "Output directory for the OpenCV_hotshots executables" )


SUBDIRS(asm_face)
SUBDIRS(jzp_lib)
SUBDIRS(pupil_tracker)
