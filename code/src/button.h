#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>


class Button {
    public:
        Button(int pin);
        void begin();
        int counter = 0;

    private:
        int pin;
        long last_change = millis();
        long last_button_press = 0;
        long debounce_interval = 70;
        long long_press_threshold = 1500; // 1.5s
        long double_press_threshold = 1500; // 1.5s
        static void IRAM_ATTR isr_static_wrapper(void* arg);

        void IRAM_ATTR isr();
};


#endif
