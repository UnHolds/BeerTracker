#include "input.h"

Input::Input(int up_pin, int down_pin, int left_pin, int right_pin, int center_pin, bool only_single_press) : up_btn(up_pin, only_single_press), down_btn(down_pin, only_single_press), left_btn(left_pin, only_single_press), right_btn(right_pin, only_single_press), center_btn(center_pin, only_single_press) {

    this->wakeup_pin_mask = BUTTON_PIN_BITMASK(up_pin) |
                            BUTTON_PIN_BITMASK(down_pin) |
                            BUTTON_PIN_BITMASK(left_pin) |
                            BUTTON_PIN_BITMASK(right_pin) |
                            BUTTON_PIN_BITMASK(center_pin);
}

InputType Input::get_input() {
    switch(this->up_btn.get_button_press()) {
        case ButtonPress::PRESS:
            return InputType::UP;
        break;
        case ButtonPress::DOUBLE_PRESS:
            return InputType::UP_DOUBLE;
        break;
        case ButtonPress::LONG_PRESS:
            return InputType::UP_LONG;
        break;
    }

    switch(this->down_btn.get_button_press()) {
        case ButtonPress::PRESS:
            return InputType::DOWN;
        break;
        case ButtonPress::DOUBLE_PRESS:
            return InputType::DOWN_DOUBLE;
        break;
        case ButtonPress::LONG_PRESS:
            return InputType::DOWN_LONG;
        break;
    }

    switch(this->left_btn.get_button_press()) {
        case ButtonPress::PRESS:
            return InputType::LEFT;
        break;
        case ButtonPress::DOUBLE_PRESS:
            return InputType::LEFT_DOUBLE;
        break;
        case ButtonPress::LONG_PRESS:
            return InputType::LEFT_LONG;
        break;
    }

    switch(this->right_btn.get_button_press()) {
        case ButtonPress::PRESS:
            return InputType::RIGHT;
        break;
        case ButtonPress::DOUBLE_PRESS:
            return InputType::RIGHT_DOUBLE;
        break;
        case ButtonPress::LONG_PRESS:
            return InputType::RIGHT_LONG;
        break;
    }

    switch(this->center_btn.get_button_press()) {
        case ButtonPress::PRESS:
            return InputType::CENTER;
        break;
        case ButtonPress::DOUBLE_PRESS:
            return InputType::CENTER_DOUBLE;
        break;
        case ButtonPress::LONG_PRESS:
            return InputType::CENTER_LONG;
        break;
    }


    return InputType::NONE;
}

void Input::begin() {

    this->up_btn.begin();
    this->down_btn.begin();
    this->left_btn.begin();
    this->right_btn.begin();
    this->center_btn.begin();

    esp_sleep_enable_ext1_wakeup(this->wakeup_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH);
}
