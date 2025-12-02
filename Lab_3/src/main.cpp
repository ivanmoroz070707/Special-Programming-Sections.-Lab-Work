#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include <opencv2/opencv.hpp>

int main() {
    CameraProvider camera(0);
    if (!camera.isOpened()) return -1;

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Camera");

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        int key = cv::waitKey(1);
        if (!keyProcessor.processKey(key)) break;

        frameProcessor.setMode(keyProcessor.getMode());
        cv::Mat processed = frameProcessor.process(frame);

        display.show(processed);
    }

    return 0;
}
