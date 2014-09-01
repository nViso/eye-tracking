SET(ft_sources
    ft.hpp              ft.cpp
    ft_data.hpp            ft_data.cpp
    shape_model.hpp  shape_model.cpp
    patch_model.hpp   patch_model.cpp
    face_detector.hpp  face_detector.cpp
    face_tracker.hpp    face_tracker.cpp
    
)
ADD_LIBRARY(asmlib ${ft_sources})

TARGET_LINK_LIBRARIES(asmlib ${OpenCV_LIBS} ${Boost_LIBRARIES})

SUBDIRS(exe)
