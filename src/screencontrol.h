// screencontrol.h
#ifndef SCREENCONTROL_H
#define SCREENCONTROL_H

#include <Arduino.h>
#include <Wire.h>
#include <st7567sfGK.h>

class ScreenControl {
    private:
        st7567sfGK* display;
        String lines[6];
        const int yCoordinates[6] = {54, 44, 34, 24, 14, 4};

    public:
        ScreenControl(st7567sfGK* screen); // Constructor
        void setLine(uint8_t row, const String& text);
        void clearAll();
    void refresh();
};

#endif