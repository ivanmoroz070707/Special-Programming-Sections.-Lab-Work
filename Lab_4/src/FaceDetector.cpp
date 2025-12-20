#include "FaceDetector.hpp"

FaceDetector::FaceDetector(const std::string& protoPath, const std::string& modelPath) {
    // Завантажуємо нейронку один раз при створенні об'єкта
    net = cv::dnn::readNetFromCaffe(protoPath, modelPath);
    
    // Запускаємо фоновий потік
    workerThread = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    shouldExit = true;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void FaceDetector::processFrameAsync(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(dataMutex);
    frame.copyTo(frameToProcess);
    frameReady = true;
}

std::vector<cv::Rect> FaceDetector::getLatestDetections() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return lastResults;
}

void FaceDetector::workerLoop() {
    while (!shouldExit) {
        cv::Mat frame;
        
        // Очікуємо на новий кадр під м'ютексом
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            if (!frameReady) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            frame = frameToProcess.clone();
            frameReady = false;
        }

        // Рівень 1: Підготовка блоба та інференс
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), 
                                            cv::Scalar(104.0, 177.0, 123.0));
        net.setInput(blob);
        cv::Mat detections = net.forward();

        // Обробка результатів
        std::vector<cv::Rect> foundFaces;
        float* data = (float*)detections.data;
        for (int i = 0; i < detections.size[2]; i++) {
            float confidence = data[i * 7 + 2];
            if (confidence > 0.5) {
                int x1 = static_cast<int>(data[i * 7 + 3] * frame.cols);
                int y1 = static_cast<int>(data[i * 7 + 4] * frame.rows);
                int x2 = static_cast<int>(data[i * 7 + 5] * frame.cols);
                int y2 = static_cast<int>(data[i * 7 + 6] * frame.rows);
                foundFaces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
            }
        }

        // Зберігаємо результат
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            lastResults = foundFaces;
        }
    }
}
