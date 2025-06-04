#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>
#include "button.h"

enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER,
    UP_LONG,
    DOWN_LONG,
    LEFT_LONG,
    RIGTH_LONG,
    CENTER_LONG,
    UP_DOUBLE,
    DOWN_DOUBLE,
    LEFT_DOUBLE,
    RIGHT_DOUBLE,
};

class Input {
    public:
        Input(int up_pin, int down_pin, int left_pin, int right_pin, int center_pin);
        void begin();
        void print_counter();

    private:
        Button up_btn;
        Button down_btn;
        Button left_btn;
        Button right_btn;
        Button center_btn;


};


#endif
