#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0);

    if (!cap.isOpened()) {
        std::cerr << "Не вдалося відкрити камеру!" << std::endl;
        return -1;
    }

    cv::Mat frame;

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Порожній кадр!" << std::endl;
            break;
        }

        cv::circle(frame, cv::Point(100, 100), 50, cv::Scalar(0, 0, 255), 3);
        cv::putText(frame, "Hello OpenCV!", cv::Point(150, 50),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2);

        cv::imshow("Webcam Demo", frame);

        if (cv::waitKey(10) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
