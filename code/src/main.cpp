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


void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_SCREEN, SCK_SCREEN);
  display.begin();
  display.splash_screen();
  input.begin();
}

void loop() {

  delay(1000);
  input.print_counter();
}
