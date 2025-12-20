#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int deviceID, int apiID) {
    cap.open(deviceID, apiID);
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) cap.release();
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    if (!cap.read(frame)) {
        return cv::Mat();
    }
    return frame;
}

