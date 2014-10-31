SET(portal_source Portal.cpp)

ADD_EXECUTABLE(Portal ${portal_source})

TARGET_LINK_LIBRARIES(Portal ${OpenCV_LIBS} ${OpenCV_LIBS} asmlib jzplib)

 add_custom_command(TARGET Portal POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../resources ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../resources ${EXECUTABLE_OUTPUT_PATH}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../resources ${EXECUTABLE_OUTPUT_PATH}/Release

)