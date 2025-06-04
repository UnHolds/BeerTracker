#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

enum class ButtonPress {
    PRESS,
    DOUBLE_PRESS,
    LONG_PRESS,
    NONE
};

class Button {
    public:
        Button(int pin);
        void begin();
        ButtonPress get_button_press();

    private:
        ButtonPress button_press = ButtonPress::NONE;
        int pin;
        long last_change = millis();
        long last_button_press = 0;
        long debounce_interval = 70;
        long long_press_threshold = 800; // 0.8s
        long double_press_threshold = 500; // 0.5s
        static void IRAM_ATTR isr_static_wrapper(void* arg);

        void IRAM_ATTR isr();
};


#endif
