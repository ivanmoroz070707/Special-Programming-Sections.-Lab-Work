#include "KeyProcessor.hpp"
#include <algorithm>

KeyProcessor::KeyProcessor() : mode(Mode::ORIGINAL), zoom(1.0f) {}

bool KeyProcessor::processKey(int key) {
    if (key == 27 || key == 'q') return false; // ESC or q
    switch (key) {
        case '1': mode = Mode::ORIGINAL; break;
        case '2': mode = Mode::INVERT; break;
        case '3': mode = Mode::GAUSSIAN; break;
        case '4': mode = Mode::CANNY; break;
        case '5': mode = Mode::SOBEL; break;
        case '6': mode = Mode::BINARY; break;
        case '7': mode = Mode::GLITCH; break;
        case '8': mode = Mode::PIP; break;
        case '=': case '+': increaseZoom(); break;
        case '_': case '-': decreaseZoom(); break;
	case 'f': case 'F': mode = Mode::FACE; break;
        default: break;
    }
    return true;
}

float KeyProcessor::getZoom() const {
    return zoom;
}
