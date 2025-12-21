#include "FrameProcessor.hpp"
#include <cstdlib>

FrameProcessor::FrameProcessor()
    : mode(KeyProcessor::Mode::ORIGINAL),
      zoomFactor(1.0),
      crossCenter(0, 0),detector("resources/deploy.prototxt", "resources/res10_300x300_ssd_iter_140000.caffemodel")

{}

// --- Roll-функция для GLITCH ---
static cv::Mat roll(const cv::Mat& src, int shift_row, int shift_col)
{
    cv::Mat dst = src.clone();

    int rows = src.rows;
    int cols = src.cols;

    if (rows == 0 || cols == 0)
        return dst;

    // нормализуем сдвиги
    shift_row = ((shift_row % rows) + rows) % rows;
    shift_col = ((shift_col % cols) + cols) % cols;

    // Сдвиг по строкам
    if (shift_row != 0)
    {
        cv::Mat tmp;
        cv::vconcat(src.rowRange(rows - shift_row, rows),
                    src.rowRange(0, rows - shift_row), tmp);
        tmp.copyTo(dst);
    }

    // Сдвиг по колонкам
    if (shift_col != 0)
    {
        cv::Mat tmp;
        cv::hconcat(dst.colRange(cols - shift_col, cols),
                    dst.colRange(0, cols - shift_col), tmp);
        tmp.copyTo(dst);
    }

    return dst;
}

void FrameProcessor::setMode(KeyProcessor::Mode m) {
    mode = m;
}

void FrameProcessor::setOverlay(const cv::Mat &img) {
    overlay = img.clone();
}

void FrameProcessor::setZoom(double z) {
    zoomFactor = z;
}



// --- Основная обработка кадра ---
cv::Mat FrameProcessor::process(const cv::Mat &frame)
{
    cv::Mat src = frame;

    // ========= ZOOM ============
    if (zoomFactor != 1.0)
    {
        cv::Mat resized;
        cv::resize(frame, resized, cv::Size(), zoomFactor, zoomFactor);
        src = resized;
    }

    cv::Mat dst;

    switch (mode)
    {
        case KeyProcessor::Mode::INVERT:
            cv::bitwise_not(src, dst);
            break;

        case KeyProcessor::Mode::GAUSSIAN:
            cv::GaussianBlur(src, dst, cv::Size(7, 7), 1.5);
            break;

        case KeyProcessor::Mode::CANNY:
            cv::Canny(src, dst, 50, 150);
            cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
            break;

        case KeyProcessor::Mode::SOBEL:
        {
            cv::Mat gray, sx, sy, mag;
            cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
            cv::Sobel(gray, sx, CV_32F, 1, 0);
            cv::Sobel(gray, sy, CV_32F, 0, 1);
            cv::magnitude(sx, sy, mag);

            double minv, maxv;
            cv::minMaxLoc(mag, &minv, &maxv);
            mag.convertTo(mag, CV_8U, 255.0/(maxv + 1e-9));

            cv::cvtColor(mag, dst, cv::COLOR_GRAY2BGR);
            break;
        }

        case KeyProcessor::Mode::BINARY:
        {
            cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
            cv::threshold(dst, dst, 128, 255, cv::THRESH_BINARY);
            cv::cvtColor(dst, dst, cv::COLOR_GRAY2BGR);
            break;
        }

        // ========= GLITCH EFFECT ============
        case KeyProcessor::Mode::GLITCH:
        {
            cv::Mat gl = src.clone();

            // Разделяем на каналы
            std::vector<cv::Mat> ch;
            cv::split(gl, ch);

            int shift = 15;

            // R канал вправо
            ch[2] = roll(ch[2], 0, shift);

            // G канал влево
            ch[1] = roll(ch[1], 0, -shift);

            cv::merge(ch, gl);

            // Горизонтальные строки со сдвигом
            for (int y = 0; y < gl.rows; y += 8)
            {
                int height = std::min(8, gl.rows - y);
                cv::Mat roi = gl.rowRange(y, y + height);

                int dx = (rand() % 25) - 12;  // [-12; +12]

                cv::Mat shifted = roll(roi, 0, dx);
                shifted.copyTo(roi);
            }

            return gl;
        }

        // ========== Picture-in-picture ==========
        case KeyProcessor::Mode::PIP:
        {
            cv::Mat pip = src.clone();

            if (!pip.empty())
            {
                cv::Mat small;
                cv::resize(src, small, cv::Size(), 0.25, 0.25);

                int x = pip.cols - small.cols - 10;
                int y = pip.rows - small.rows - 10;

                if (x >= 0 && y >= 0)
                {
                    cv::Mat roi = pip(cv::Rect(x, y, small.cols, small.rows));
                    small.copyTo(roi);
                }
            }

            return pip;
        }
	//============ Face Detector =============
case KeyProcessor::Mode::FACE: {
            dst = src.clone();
            // Відправляємо кадр в детектор (не блокує UI!)
            detector.processFrameAsync(src);
           // std::cout<<"dst "<<std::flush;
            // Забираємо результати (миттєво)
            std::vector<cv::Rect> faces = detector.getLatestDetections();
            
            for (const auto& r : faces) {
                cv::rectangle(dst, r, cv::Scalar(0, 255, 0), 2);
                cv::putText(dst, "FACE", cv::Point(r.x, r.y - 10), 
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
            }
            break;
        }	
        default:
            dst = src.clone();
            break;
    }

    // ====== Overlay ======
    if (!overlay.empty())
        cv::addWeighted(dst, 1.0, overlay, 0.5, 0, dst);

    return dst;
}

// Заглушки (если рисование не нужно)
void FrameProcessor::onMouse(int, int, int, int) {}
void FrameProcessor::moveCross(int, int) {}
void FrameProcessor::clearDrawings() {}
