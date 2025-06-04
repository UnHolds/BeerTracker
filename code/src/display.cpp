#include "display.h"

Display::Display(int width, int heigth, const char* version) : display(width, heigth, &Wire, -1) {
    this->width = width;
    this->height = height;
    this->version = version;
}

void Display::begin() {
    if(!this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    this->display.clearDisplay();
}

void Display::print_center_x(String text, int y_pos, int text_size) {
    this->display.setTextSize(text_size);
    this->display.setTextColor(WHITE);
    this->display.setCursor((this->width / 2) - (6 * text_size * text.length() / 2), y_pos);
    this->display.println(text);
}

void Display::splash_screen() {

    this->print_center_x("Beer-Track", 15, 2);
    this->print_center_x("by @UnHold", 35, 1);
    std::string buf("Version: ");
    buf.append(this->version);
    this->print_center_x(buf.c_str(), 50, 1);
    this->display.display();
    delay(2000);
    this->display.clearDisplay();
    this->display.display();
}

void Display::update(InputType input) {
    this->display.clearDisplay();

    this->print_center_x("Hello", 5, 2);

    this->display.display();
}

void Display::clear() {
    this->display.clearDisplay();
    this->display.display();
}
