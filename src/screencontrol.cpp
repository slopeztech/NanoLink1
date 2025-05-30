// screencontrol.cpp
#include "screencontrol.h"

ScreenControl::ScreenControl(st7567sfGK* screen) {
    display = screen;
    clearAll();
}

void ScreenControl::setLine(uint8_t row, const String& text) {
    if (row >= 6) return;
    lines[row] = text;
}

void ScreenControl::clearAll() {
    for (int i = 0; i < 6; i++) {
        lines[i] = "";
    }
}

void ScreenControl::refresh() {
    display->clear(st7567sfGK::colorblack);
    for (int i = 0; i < 6; i++) {
        if (lines[i].length() > 0) {
            display->text(0, yCoordinates[i], st7567sfGK::colorwhite, lines[i].c_str());
        }
    }
}