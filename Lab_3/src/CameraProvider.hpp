#pragma once
#include <opencv2/opencv.hpp>

class CameraProvider {
private:
    cv::VideoCapture cap;

public:
    CameraProvider(int deviceID = 0, int apiID = cv::CAP_ANY);
    ~CameraProvider();

    bool isOpened() const;
    cv::Mat getFrame();
};
