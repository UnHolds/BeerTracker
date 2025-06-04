#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

class Display {
    public:
        Display(int width, int heigth);
        void begin();
        void splash_screen();

    private:
        int width;
        int height;
        Adafruit_SSD1306 display;

        void print_center_x(String text, int y_pos, int text_size);
};


#endif
