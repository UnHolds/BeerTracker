#include <Arduino.h>
#include <Wire.h>
#include <ESP32Time.h>

#include "display.h"
#include "input.h"
#include "message.h"


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64// OLED display height, in pixels
#define SDA_SCREEN 22
#define SCK_SCREEN 23
#define OLED_RESET -1 // Reset pin

#define secToMs(secs) secs * 1000
#define secToUs(secs) secs * 1000 * 1000

#define STANDBY_TIMEOUT secToMs(5) // 5s - time until the deceive goes to sleep after no input
#define SLEEP_TIME secToUs(200) //200s

#define VERSION "0.1"


Display display(SCREEN_WIDTH, SCREEN_HEIGHT, VERSION);
Input input(14, 27, 26, 15, 4);
ESP32Time rtc(3600);
Message message;

#ifdef DEBUG
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
#endif

void go_to_sleep() {
    esp_sleep_enable_timer_wakeup(SLEEP_TIME);
    display.clear();
    esp_deep_sleep_start();
}

void add_peers(){
    uint8_t peer1[] =  {0x20, 0x6a, 0x8a, 0xa3, 0x60, 0x49};
    uint8_t peer2[] = {0x34, 0x5F, 0x45, 0x38, 0xED, 0xA4}; // olimex
    uint8_t peer3[] = {0x24, 0x6F, 0x28, 0x24, 0x6D, 0x20}; //other esp
    //message.add_peer(peer1);
    message.add_peer(peer2);
}


long last_command = millis();



void setup() {

    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    Serial.begin(115200);
    rtc.setTime(30, 24, 15, 17, 1, 2021); //TODO update
    Wire.begin(SDA_SCREEN, SCK_SCREEN);

    if (!LittleFS.begin(true)) {
        Serial.println("Formatting LittleFs!!");
        if (!LittleFS.begin()) {
            Serial.println("FATAL: Failed to mount LittleFS even after formatting!");
            return;
        }
    }

    input.begin();
    message.begin();

    display.begin(&message);
    if (wakeup_reason != ESP_SLEEP_WAKEUP_TIMER){
        display.splash_screen();
    }

    add_peers();

    #ifdef DEBUG
    switch (wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
        case ESP_SLEEP_WAKEUP_EXT1:
            Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println("Wakeup caused by timer");
        break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            Serial.println("Wakeup caused by touchpad");
        break;
        case ESP_SLEEP_WAKEUP_ULP:
            Serial.println("Wakeup caused by ULP program");
        break;
        default:
            Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
    #endif

    last_command = millis();
}



void loop() {

    long current_millis = millis();
    InputType t = input.get_input();
    if(t != InputType::NONE) {
        last_command = current_millis;
        #ifdef DEBUG
        Serial.println(InputTypetoString(t));
        #endif
    }

    if(current_millis - last_command > STANDBY_TIMEOUT){
        go_to_sleep();
    }

    display.update(t);
    //message.send();

}
