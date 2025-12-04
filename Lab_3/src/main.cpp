#include <opencv2/opencv.hpp>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"

int main() {
    CameraProvider cam(0);
    if (!cam.isOpened()) {
        std::cerr << "Camera not opened\n";
        return 1;
    }

    KeyProcessor keyProcessor;
    FrameProcessor frameProcessor;
    Display display("Camera");

    // Якщо є overlay.png у assets, можна підвантажити:
    // cv::Mat overlay = cv::imread("assets/overlay.png", cv::IMREAD_UNCHANGED);
    // if (!overlay.empty()) frameProcessor.setOverlay(overlay);

    while (true) {
        cv::Mat frame = cam.getFrame();
        if (frame.empty()) break;

        // Чекаємо клавішу ненадовго, але одразу отримуємо код
        int k = cv::waitKey(10); // 10 ms

        // Обробка клавіші (приблизно: змінить режим або zoom)
        if (k >= 0) {
            if (!keyProcessor.processKey(k)) break; // повертає false, якщо потрібно вийти
        }

        // Передаємо поточний zoom в FrameProcessor
        frameProcessor.setZoom(static_cast<double>(keyProcessor.getZoom()));

        // Передаємо режим в FrameProcessor
        frameProcessor.setMode(keyProcessor.getMode());

        // Обробляємо кадр та показуємо
        cv::Mat out = frameProcessor.process(frame);
        display.show(out);
    }

    return 0;
}
