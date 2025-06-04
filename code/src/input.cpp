#include "input.h"

Input::Input(int up_pin, int down_pin, int left_pin, int right_pin, int center_pin) : up_btn(up_pin), down_btn(down_pin), left_btn(left_pin), right_btn(right_pin), center_btn(center_pin) {
}

void Input::print_counter() {
    Serial.printf("Btn_up: %d\n", this->up_btn.counter);
    //Serial.printf("Btn_down: %d\n", this->down_btn.counter);
    //Serial.printf("Btn_left: %d\n", this->left_btn.counter);
    //Serial.printf("Btn_rigth: %d\n", this->right_btn.counter);
    //Serial.printf("Btn_center: %d\n", this->center_btn.counter);
}

void Input::begin() {
    this->up_btn.begin();
    this->down_btn.begin();
    this->left_btn.begin();
    this->right_btn.begin();
    this->center_btn.begin();
}
