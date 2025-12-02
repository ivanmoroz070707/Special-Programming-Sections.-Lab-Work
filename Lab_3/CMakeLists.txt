cmake_minimum_required(VERSION 3.10)
cmake_minimum_required(VERSION 3.10)
project(opencv_camera)

find_package(OpenCV REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(opencv_camera
    src/main.cpp
    src/CameraProvider.cpp
    src/KeyProcessor.cpp
    src/FrameProcessor.cpp
    src/Display.cpp
)

target_link_libraries(opencv_camera ${OpenCV_LIBS})
