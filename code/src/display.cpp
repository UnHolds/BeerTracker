#include "display.h"

Display::Display(int width, int heigth, const char* version, ESP32Time* rtc) : display(width, heigth, &Wire, -1) {
    this->width = width;
    this->height = height;
    this->version = version;
    this->rtc = rtc;
}

void Display::begin(Message* message) {
    if(!this->display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    this->display.clearDisplay();
    this->message = message;

    pinMode(BAT_PIN, INPUT);
    pinMode(PWR_PIN, INPUT);

    if((&this->message->send_message)->time > rtc->getEpoch()){
        #ifdef DEBUG
        Serial.println("updating the epoch to the time stored at esp32");
        #endif
        this->rtc->setTime((&this->message->send_message)->time, 0);
    }
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

void Display::print_battery() {
    char bat_v[6];
    float pwr = 3.3 / 4095.0 * analogRead(PWR_PIN);
    float bat = 3.3 / 4095.0 * analogRead(BAT_PIN);

    if(pwr > 2){

        this->print_x_y("PWR", this->width - 6 * 3, 0, 1);
    } else {
        this->print_icon(Icon::BAT, 128-8, 8/2);
        //snprintf (bat_v, sizeof(bat_v), "%.2fVV", bat);
        //this->print_x_y(bat_v, this->width - 6 * (sizeof(bat_v)), 0, 1);
    }
}

void Display::lock_screen(){
    this->clear();
    this->print_icon(Icon::LOCKED, 64, 20);
    this->print_center_x("To Unlock: < > < >", 42, 1);
    this->display.display();
}

void Display::print_time() {
    char time_str[32];
    // 2024-06-27 23:24:25
    sprintf(time_str, "%04d-%02d-%02d %02d:%02d:%02d", rtc->getYear(), rtc->getMonth(), rtc->getDay(), rtc->getHour(true), rtc->getMinute(), rtc->getSecond());
    this->print_center_x(time_str, 64-7, 1);
}
void Display::print_icon(Icon icon, int x_pos, int y_pos) {
    int id = static_cast<int>(icon) & 0xFF;
    int y_size = (static_cast<int>(icon) >> 8) & 0xFF;
    int x_size = (static_cast<int>(icon) >> 16) & 0xFF;
    this->display.drawBitmap(x_pos - x_size / 2, y_pos - y_size / 2, icon_array[id], x_size, y_size, WHITE);
}

void Display::update(InputType input) {
    this->display.clearDisplay();
    //print the username in the topleft
    this->print_x_y(this->message->send_message.users[this->message->send_message.user_id].name, 0, 0, 1);
    print_battery();
    print_time();


    switch(this->menu){
        case Menu::MAIN:
            this->update_main_menu(input);
        break;
        case Menu::BEER:
            this->print_center_x("Beer", 0, 1);
            this->increment_menu(input, &this->message->send_message.users[this->message->send_message.user_id].beer);
        break;
        case Menu::SHOT:
            this->print_center_x("Shot", 0, 1);
            this->increment_menu(input, &this->message->send_message.users[this->message->send_message.user_id].shots);
        break;
        case Menu::WATER:
            this->print_center_x("Water", 0, 1);
            this->increment_menu(input, &this->message->send_message.users[this->message->send_message.user_id].water);
        break;
        case Menu::SELECT_USER:
            this->print_center_x("WhoAmI", 0, 1);
            this->change_user(input);
        break;
        case Menu::RESET:
            this->reset(input);
        break;
        case Menu::SHOW_USERS:
            this->print_center_x("Users", 0, 1);
            this->show_users(input);
        break;
        case Menu::SET_TIME:
            this->print_center_x("Set Time", 0, 1);
            this->set_time(input);
        break;
    }

    this->display.display();
}

void Display::change_user(InputType input) {
    if(this->submenu_idx == -1){
        this->submenu_idx = this->message->send_message.user_id;
    }

    //safety
    this->submenu_idx = this->submenu_idx % NUM_USER;

    this->print_center_x(this->message->send_message.users[this->submenu_idx].name, 32 - 8, 2);
    this->print_x_y("<", 10, 32 - 8, 2);
    this->print_x_y(">", 110, 32 - 8, 2);

    if(input == InputType::LEFT){
        this->submenu_idx = (this->submenu_idx + NUM_USER - 1) % NUM_USER;
    }
    if(input == InputType::RIGHT){
        this->submenu_idx = (this->submenu_idx + 1) % NUM_USER;
    }
    if(input == InputType::CENTER){
        (&this->message->send_message)->user_id = this->submenu_idx;
        this->menu = Menu::MAIN;
        this->message->send();
    }
}

void Display::reset(InputType input) {
    this->print_center_x("<- Y  (RST)  N ->", 32 - 8, 1);

    if(input == InputType::LEFT){
        for(int i = 0; i < NUM_USER; i++){
            strcpy((&this->message->send_message)->users[i].name, RST_USERNAMES[i].c_str());
            (&this->message->send_message)->users[i].beer = 0;
            (&this->message->send_message)->users[i].water = 0;
            (&this->message->send_message)->users[i].shots = 0;
            (&this->message->send_message)->users[i].idx = 0;
        }
        this->menu = Menu::MAIN;
    }
    if(input == InputType::RIGHT){
        this->menu = Menu::MAIN;
    }
    rtc->setTime(1751203509); //Sun Jun 29 2025 13:25:09 GMT+0000
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

void Display::set_time(InputType input) {

    if(this->submenu_idx == -1){
        this->submenu_idx = 0;
    }

    //year, month, day, hour, min
    this->submenu_idx = this->submenu_idx % 5;

    char num_s[16];

    long epoch = this->rtc->getEpoch();
    long change = 0;

    switch(this->submenu_idx) {
        case 0: //year
            this->print_center_x("YEAR", 1, 1);
            sprintf(num_s, "%d", this->rtc->getYear());
            change = 31556926;
        break;
        case 1: //month
            this->print_center_x("MONTH", 1, 1);
            sprintf(num_s, "%d", this->rtc->getMonth());
            change = 2629743;
        break;
        case 2: //day
            this->print_center_x("DAY", 1, 1);
            sprintf(num_s, "%d", this->rtc->getDay());
            change = 86400 ;
        break;
        case 3: //hour
            this->print_center_x("HOUR", 1, 1);
            sprintf(num_s, "%d", this->rtc->getHour());
            change = 60 * 24;
        break;
        case 4: //min
            this->print_center_x("MINUTE", 1, 1);
            sprintf(num_s, "%d", this->rtc->getMinute());
            change = 60;
        break;

    }

    this->print_center_x(num_s, 32 - 8, 2);
    this->print_x_y("-", 10, 32 - 8, 2);
    this->print_x_y("+", 110, 32 - 8, 2);

    if(input == InputType::LEFT){
        this->rtc->setTime(epoch - change);
    }
    if(input == InputType::RIGHT){
        this->rtc->setTime(epoch + change);
    }
    if(input == InputType::UP){
        this->submenu_idx = (5 + this->submenu_idx - 1) % 5;
    }
    if(input == InputType::DOWN){
        this->submenu_idx = (this->submenu_idx + 1) % 5;
    }
    if(input == InputType::CENTER){
        this->menu = Menu::MAIN;
    }
}

void Display::show_users(InputType input) {
    if(this->submenu_idx == -1){
        this->submenu_idx = this->message->send_message.user_id;
    }

    //safety
    this->submenu_idx = this->submenu_idx % NUM_USER;

    this->print_x_y("<", 10, 32 - 8, 2);
    this->print_x_y(">", 110, 32 - 8, 2);

    int offset = 15;
    char buf[16];

    this->print_center_x(this->message->send_message.users[this->submenu_idx].name, offset, 1);
    offset += 5;

    snprintf (buf, sizeof(buf), "Beer: %d", this->message->send_message.users[this->submenu_idx].beer);
    this->print_center_x(buf, offset+8, 1);
    snprintf (buf, sizeof(buf), "Water: %d", this->message->send_message.users[this->submenu_idx].water);
    this->print_center_x(buf, offset+16, 1);
    snprintf (buf, sizeof(buf), "Shots: %d", this->message->send_message.users[this->submenu_idx].shots);
    this->print_center_x(buf, offset+24, 1);

    if(input == InputType::LEFT){
        this->submenu_idx = (this->submenu_idx + NUM_USER - 1) % NUM_USER;
    }
    if(input == InputType::RIGHT){
        this->submenu_idx = (this->submenu_idx + 1) % NUM_USER;
    }

    if(input == InputType::CENTER){
        this->menu = Menu::MAIN;
    }
}

void Display::update_main_menu(InputType input) {

    int num_entries = 7;

    if(input == InputType::UP) {
        this->main_menu_idx = (this->main_menu_idx + num_entries - 1) % num_entries;
    }

    if(input == InputType::DOWN) {
        this->main_menu_idx = (this->main_menu_idx + 1) % num_entries;
    }

    switch(this->main_menu_idx) {
        case 0:
            this->print_icon(Icon::BEER, 64, 32);
            this->print_center_x("Beer", 0, 1);
            if(input == InputType::CENTER){
                this->menu = Menu::BEER;
                this-> update(InputType::NONE);
            };
        break;
        case 1:
            this->print_icon(Icon::WATER, 64, 32);
            this->print_center_x("Water", 0, 1);
            if(input == InputType::CENTER){
                this->menu = Menu::WATER;
                this-> update(InputType::NONE);
            };
        break;
        case 2:
            this->print_icon(Icon::SHOT, 64, 32);
            this->print_center_x("Shot", 0, 1);
            if(input == InputType::CENTER){
                this->menu = Menu::SHOT;
                this-> update(InputType::NONE);
            };
        break;
        case 3:
            this->print_icon(Icon::USER_SEL, 64, 32);
            this->print_center_x("WhoAmI", 0, 1);
            if(input == InputType::CENTER){
                this->menu = Menu::SELECT_USER;
                this-> change_user(InputType::NONE);
            };
        break;
        case 4:
            this->print_menu("Reset");
            if(input == InputType::CENTER){
                this->menu = Menu::RESET;
                this-> reset(InputType::NONE);
            };
        break;
        case 5:
            this->print_icon(Icon::USER, 64, 32);
            this->print_center_x("Users", 0, 1);
            if(input == InputType::CENTER){
                this->menu = Menu::SHOW_USERS;
                this-> show_users(InputType::NONE);
            };
        break;
        case 6:
            this->print_icon(Icon::TIME, 64, 32);
            this->print_center_x("Set Time", 0, 1);
            if(input == InputType::CENTER){
                this->menu = Menu::SET_TIME;
                this-> set_time(InputType::NONE);
            };
        break;
    }
}

void Display::clear() {
    this->display.clearDisplay();
    this->display.display();
}
