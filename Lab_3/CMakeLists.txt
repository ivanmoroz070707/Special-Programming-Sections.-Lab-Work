cmake_minimum_required(VERSION 3.10)
project(opencv_camera)

find_package(OpenCV REQUIRED)

add_executable(opencv_camera
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CameraProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FrameProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Display.cpp
)

target_include_directories(opencv_camera PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(opencv_camera PRIVATE ${OpenCV_LIBS})
