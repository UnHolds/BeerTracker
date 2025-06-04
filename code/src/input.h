#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "button.h"
#include "driver/rtc_io.h"

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)

enum class InputType {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER,
    UP_LONG,
    DOWN_LONG,
    LEFT_LONG,
    RIGHT_LONG,
    CENTER_LONG,
    UP_DOUBLE,
    DOWN_DOUBLE,
    LEFT_DOUBLE,
    RIGHT_DOUBLE,
    CENTER_DOUBLE,
    NONE
};




class Input {
    public:
        Input(int up_pin, int down_pin, int left_pin, int right_pin, int center_pin);
        void begin();
        InputType get_input();

    private:
        Button up_btn;
        Button down_btn;
        Button left_btn;
        Button right_btn;
        Button center_btn;
        uint64_t wakeup_pin_mask;
};


#endif
