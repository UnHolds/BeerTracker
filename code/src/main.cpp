#include <Arduino.h>
#include <Wire.h>

#include "display.h"
#include "input.h"


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32// OLED display height, in pixels

#define SDA_SCREEN 22
#define SCK_SCREEN 23

#define OLED_RESET -1 // Reset pin


Display display(SCREEN_WIDTH, SCREEN_HEIGHT);


Input input(14, 5, 17, 16, 4);

String InputTypetoString(InputType type) {
    switch (type) {
        case InputType::UP:
            return "UP";
        case InputType::DOWN:
            return "DOWN";
        case InputType::LEFT:
            return "LEFT";
        case InputType::RIGHT:
            return "RIGHT";
        case InputType::CENTER:
            return "CENTER";
        case InputType::UP_LONG:
            return "UP_LONG";
        case InputType::DOWN_LONG:
            return "DOWN_LONG";
        case InputType::LEFT_LONG:
            return "LEFT_LONG";
        case InputType::RIGHT_LONG:
            return "RIGHT_LONG";
        case InputType::CENTER_LONG:
            return "CENTER_LONG";
        case InputType::UP_DOUBLE:
            return "UP_DOUBLE";
        case InputType::DOWN_DOUBLE:
            return "DOWN_DOUBLE";
        case InputType::LEFT_DOUBLE:
            return "LEFT_DOUBLE";
        case InputType::RIGHT_DOUBLE:
            return "RIGHT_DOUBLE";
        case InputType::CENTER_DOUBLE:
            return "CENTER_DOUBLE";
        case InputType::NONE:
            return "NONE";
        default:
            return "UNKNOWN_INPUT_TYPE"; // Fallback for any unhandled or future enum values
    }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_SCREEN, SCK_SCREEN);
  display.begin();
  display.splash_screen();
  input.begin();
}

void loop() {

  InputType t = input.get_input();
  if(t != InputType::NONE) {
    Serial.println(InputTypetoString(t));
  }

}
