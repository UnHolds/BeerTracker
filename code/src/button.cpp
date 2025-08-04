#include "button.h"

Button::Button(int pin, bool only_single_press) {
    this->pin = pin;
    this->only_single_press = only_single_press;
}

void Button::begin() {
    pinMode(this->pin, INPUT_PULLDOWN);
    attachInterruptArg(this->pin, Button::isr_static_wrapper, this, CHANGE);
}

ButtonPress Button::get_button_press() {

    if(this->button_press == ButtonPress::PRESS && millis() - this->last_button_press < this->double_press_threshold && this->only_single_press == false){
        //return non because press can still be double press
        return ButtonPress::NONE;
    }
    ButtonPress cur_press = this->button_press;
    this->button_press = ButtonPress::NONE; //reset
    return cur_press;
}

void IRAM_ATTR Button::isr_static_wrapper(void* arg) {
    Button* instance = static_cast<Button*>(arg);
    if (instance) {
        instance->isr();
    }
}

void IRAM_ATTR Button::isr(){

    long current_millis = millis();

    if(current_millis - this->last_change < this->debounce_interval){
        return;
    }

    if(digitalRead(this->pin) == LOW){
        long press_duration = current_millis - this->last_change;

        if(press_duration > this->long_press_threshold && this->only_single_press == false) {
            //long press
            this->button_press = ButtonPress::LONG_PRESS;
        }else if (current_millis - this->last_button_press < this->double_press_threshold && this->only_single_press == false){
            //double press
            this->button_press = ButtonPress::DOUBLE_PRESS;
        }else{
            //single press
            this->button_press = ButtonPress::PRESS;
            this->last_button_press = current_millis; // i think this is only needed for single press
        }

    }
    this->last_change = current_millis;

}
