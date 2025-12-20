#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"
#include "FaceDetector.hpp"
#include <vector>

struct DrawState {
    bool drawing = false;
    cv::Point start;
    cv::Point end;
};

class FrameProcessor {
public:
    FrameProcessor();
    void setMode(KeyProcessor::Mode m);
    void setOverlay(const cv::Mat &img);
    cv::Mat process(const cv::Mat &frame);
    void onMouse(int event, int x, int y, int flags);
    void moveCross(int dx, int dy);
    void clearDrawings();
    void setZoom(double z);
private:
    KeyProcessor::Mode mode;
    cv::Mat overlay;
    DrawState drawState;
    std::vector<cv::Rect> rectangles;
    cv::Point crossCenter;
    double zoomFactor;

// --- Нове поле для Лабораторної №4 ---
    FaceDetector detector;
};

