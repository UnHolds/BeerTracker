#include "display.h"

Display::Display(int width, int heigth, const char* version) : display(width, heigth, &Wire, -1) {
    this->width = width;
    this->height = height;
    this->version = version;
}

void Display::begin(Message* message) {
    if(!this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    this->display.clearDisplay();
    this->message = message;
}

void Display::print_x_y(String text, int x_pos, int y_pos, int text_size) {
    this->display.setTextSize(text_size);
    this->display.setTextColor(WHITE);
    this->display.setCursor(x_pos, y_pos);
    this->display.println(text);
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
    this->print_x_y(this->message->send_message.users[this->message->send_message.user_id].name, 0, 0, 1);


    switch(this->menu){
        case Menu::MAIN:
            this->update_main_menu(input);
        break;
        case Menu::BEER:
            this->increment_menu(input, &this->message->send_message.users[this->message->send_message.user_id].beer);
        break;
        case Menu::SHOT:
            this->increment_menu(input, &this->message->send_message.users[this->message->send_message.user_id].shots);
        break;
        case Menu::WATER:
            this->increment_menu(input, &this->message->send_message.users[this->message->send_message.user_id].water);
        break;
        case Menu::USER:
            this->change_user(input);
        break;
    }

    this->display.display();
}

void Display::change_user(InputType input) {
    if(this->temp_user_id == -1){
        this->temp_user_id = this->message->send_message.user_id;
    }
    this->print_center_x(this->message->send_message.users[this->temp_user_id].name, 32 - 8, 2);
    this->print_x_y("<", 10, 32 - 8, 2);
    this->print_x_y(">", 110, 32 - 8, 2);

    if(input == InputType::LEFT){
        this->temp_user_id = (this->temp_user_id + NUM_USER - 1) % NUM_USER;
    }
    if(input == InputType::RIGHT){
        this->temp_user_id = (this->temp_user_id + 1) % NUM_USER;
    }
    if(input == InputType::CENTER){
        (&this->message->send_message)->user_id = this->temp_user_id;
        this->menu = Menu::MAIN;
        this->message->send();
    }
}

void Display::print_menu(const char* name) {
    this->print_center_x(name, 32 - 8, 2);
}

void Display::increment_menu(InputType input, uint8_t* value) {
    char num_s[4];
    sprintf(num_s, "%d", *value);
    this->print_center_x(num_s, 32 - 8, 2);
    this->print_x_y("-", 10, 32 - 8, 2);
    this->print_x_y("+", 110, 32 - 8, 2);

    if(input == InputType::LEFT){
        *value -= 1;
    }
    if(input == InputType::RIGHT){
        *value += 1;
    }
    if(input == InputType::CENTER){
        this->menu = Menu::MAIN;
        this->message->send();
    }

}

void Display::update_main_menu(InputType input) {

    int num_entries = 4;

    if(input == InputType::UP) {
        this->main_menu_idx = (this->main_menu_idx + num_entries - 1) % num_entries;
    }

    if(input == InputType::DOWN) {
        this->main_menu_idx = (this->main_menu_idx + 1) % num_entries;
    }

    switch(this->main_menu_idx) {
        case 0:
            this->print_menu("Beer");
            if(input == InputType::CENTER){
                this->menu = Menu::BEER;
                this-> update(InputType::NONE);
            };
        break;
        case 1:
            this->print_menu("Water");
            if(input == InputType::CENTER){
                this->menu = Menu::WATER;
                this-> update(InputType::NONE);
            };
        break;
        case 2:
            this->print_menu("Shot");
            if(input == InputType::CENTER){
                this->menu = Menu::SHOT;
                this-> update(InputType::NONE);
            };
        break;
        case 3:
            this->print_menu("User");
            if(input == InputType::CENTER){
                this->menu = Menu::USER;
                this-> change_user(InputType::NONE);
            };
        break;
    }
}

void Display::clear() {
    this->display.clearDisplay();
    this->display.display();
}
