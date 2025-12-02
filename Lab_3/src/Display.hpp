#pragma once
#include <opencv2/opencv.hpp>

class Display {
public:
    Display(const std::string &winName = "Camera") : windowName(winName) {
        cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    }
    void show(const cv::Mat &frame) {
        cv::imshow(windowName, frame);
    }
private:
    std::string windowName;
};
