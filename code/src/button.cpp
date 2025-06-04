#include "button.h"

Button::Button(int pin) {
    this->pin = pin;
}

void Button::begin() {
    pinMode(this->pin, INPUT_PULLUP);
    attachInterruptArg(this->pin, Button::isr_static_wrapper, this, CHANGE);
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

    if(digitalRead(this->pin) == HIGH){
        long press_duration = current_millis - this->last_change;

        if(press_duration > this->long_press_threshold) {
            //long press
            this->counter += 10000000;
        }else if (current_millis - this->last_button_press < this->double_press_threshold){
            //double press
            this->counter += 1000;
        }else{
            //single press
            this->counter += 1;
            this->last_button_press = current_millis; // i think this is only needed for single press
        }

    }
    this->last_change = current_millis;

}
