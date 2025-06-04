#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "input.h"

class Display {
    public:
        Display(int width, int heigth, const char* version);
        void begin();
        void splash_screen();
        void update(InputType input);
        void clear();

    private:
        int width;
        int height;
        const char* version;
        Adafruit_SSD1306 display;

        void print_center_x(String text, int y_pos, int text_size);
};


#endif
