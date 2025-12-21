cmake_minimum_required(VERSION 3.10)
project(opencv_camera)

# Знаходимо OpenCV
find_package(OpenCV REQUIRED)

# Обов'язково знаходимо бібліотеку потоків (Threads)
find_package(Threads REQUIRED)

add_executable(opencv_camera
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CameraProvider.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FrameProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Display.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/FaceDetector.cpp  # <--- Додано новий файл
)

target_include_directories(opencv_camera PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Лінкуємо OpenCV ТА бібліотеку потоків
target_link_libraries(opencv_camera PRIVATE 
    ${OpenCV_LIBS}
    Threads::Threads
)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
