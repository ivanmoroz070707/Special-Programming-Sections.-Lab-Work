#include "FrameProcessor.hpp"

FrameProcessor::FrameProcessor()
    : mode(KeyProcessor::Mode::ORIGINAL), zoomFactor(1.0), crossCenter(0,0) {}

void FrameProcessor::setMode(KeyProcessor::Mode m) { mode = m; }
void FrameProcessor::setOverlay(const cv::Mat &img) { overlay = img.clone(); }

cv::Mat FrameProcessor::process(const cv::Mat &frame) {
    cv::Mat dst;

    switch(mode) {
        case KeyProcessor::Mode::INVERT:
            cv::bitwise_not(frame, dst);
            break;
        case KeyProcessor::Mode::GAUSSIAN:
            cv::GaussianBlur(frame, dst, cv::Size(7,7), 1.5);
            break;
        case KeyProcessor::Mode::CANNY:
            cv::Canny(frame, dst, 50, 150);
            cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
            break;
        case KeyProcessor::Mode::SOBEL: {
            cv::Mat gray, sx, sy, mag;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, sx, CV_32F, 1, 0);
            cv::Sobel(gray, sy, CV_32F, 0, 1);
            cv::magnitude(sx, sy, mag);
            double minv, maxv;
            cv::minMaxLoc(mag, &minv, &maxv);
            mag.convertTo(mag, CV_8U, 255.0/(maxv+1e-9));
            cv::cvtColor(mag, dst, cv::COLOR_GRAY2BGR);
            break;
        }
        case KeyProcessor::Mode::BINARY: {
            cv::cvtColor(frame, dst, cv::COLOR_BGR2GRAY);
            cv::threshold(dst, dst, 128, 255, cv::THRESH_BINARY);
            cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
            break;
        }
        default:
            dst = frame.clone();
            break;
    }

    // overlay
    if (!overlay.empty()) cv::addWeighted(dst, 1.0, overlay, 0.5, 0, dst);

    return dst;
}

void FrameProcessor::onMouse(int, int, int, int) {}
void FrameProcessor::moveCross(int, int) {}
void FrameProcessor::clearDrawings() {}

