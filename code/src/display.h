#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "input.h"
#include "message.h"
#include <ESP32Time.h>


#define BAT_PIN 34
#define PWR_PIN 39


enum class Menu {
    MAIN,
    BEER,
    WATER,
    SHOT,
    SELECT_USER,
    RESET,
    SHOW_USERS,
    SET_TIME,
};

class Display {
    public:
        Display(int width, int heigth, const char* version, ESP32Time* rtc);
        void begin(Message* message);
        void splash_screen();
        void update(InputType input);
        void clear();

    private:
        int width;
        int height;
        const char* version;
        Menu menu = Menu::MAIN;
        Adafruit_SSD1306 display;
        ESP32Time* rtc;
        int main_menu_idx = 0;
        Message* message;
        int submenu_idx = -1;

        void print_center_x(String text, int y_pos, int text_size);
        void print_x_y(String text, int x_pos, int y_pos, int text_size);
        void update_main_menu(InputType input);
        void increment_menu(InputType input, uint8_t* value);
        void print_menu(const char* name);
        void change_user(InputType input);
        void reset(InputType input);
        void print_battery();
        void print_time();
        void show_users(InputType input);
        void set_time(InputType input);
};


#endif
