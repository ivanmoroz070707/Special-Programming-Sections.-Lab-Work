#ifndef FACEDETECTOR_HPP
#define FACEDETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class FaceDetector {
public:
    // Конструктор приймає шляхи до файлів моделі
    FaceDetector(const std::string& protoPath, const std::string& modelPath);
    
    // Деструктор зупиняє потік
    ~FaceDetector();

    // Передаємо новий кадр для обробки (не блокує основний потік)
    void processFrameAsync(const cv::Mat& frame);

    // Отримуємо останній знайдений список облич
    std::vector<cv::Rect> getLatestDetections();

private:
    // Основна функція фонового потоку
    void workerLoop();

    cv::dnn::Net net;
    std::thread workerThread;
    std::mutex dataMutex;
    std::atomic<bool> shouldExit{false};

    // Спільні дані
    cv::Mat frameToProcess;
    std::vector<cv::Rect> lastResults;
    bool frameReady = false;
};

#endif
