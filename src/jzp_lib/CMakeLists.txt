SET(jzplib_source
ASEF.h
    LKTracker.h	GraphUtils.h	jzplib_bwOps.h		jzplib_geom.h
skinColorDetection.h		jzplib_detections.h	jzplib_img.h
jzplib_all.h		jzplib_draw.h		tld_utils.h
jzplib_math.h


LKTracker.cpp		jzplib_draw.cpp		tld_utils.cpp
jzplib_bwOps.cpp	jzplib_geom.cpp   ASEF.cpp
jzplib_detections.cpp	jzplib_img.cpp skinColorDetection.cpp
GraphUtils.cpp jzplib_math.cpp
)
ADD_LIBRARY(jzplib ${jzplib_source})

TARGET_LINK_LIBRARIES(jzplib ${OpenCV_LIBS})

