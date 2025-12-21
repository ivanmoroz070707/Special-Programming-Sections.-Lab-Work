#pragma once
#include <algorithm>

class KeyProcessor {
public:
    enum class Mode {
        ORIGINAL,
        INVERT,
        GAUSSIAN,
        CANNY,
        SOBEL,
        BINARY,
        GLITCH,
        PIP
    };

    KeyProcessor();

    bool processKey(int key);
    float getZoom() const ;
    Mode getMode() const { return mode; }

private:
    Mode mode;
    float zoom;
	
    void increaseZoom() { zoom = std::min(4.0f, zoom + 0.1f); }
    void decreaseZoom() { zoom = std::max(0.2f, zoom - 0.1f); }
   
};
