#include "message.h"

Message* Message::_instance = nullptr;
bool data_was_sent = false;

void globalOnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    if (Message::_instance != nullptr) {
        Message::_instance->handleOnDataRecv(mac, incomingData, len);
    }
}

void globalOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (Message::_instance != nullptr) {
        Message::_instance->handleOnDataSent(mac_addr, status);
    }
}

UserData* Message::getUser(int idx){
    return &this->send_message.users[idx];
}

void Message::storeData() {
    File file = LittleFS.open(FILENAME, "w"); // "w" for write, creates if not exists, truncates if exists
    if (!file) {
        #ifdef DEBUG
        Serial.println("Failed to open file for writing.");
        #endif
        return;
    }

    size_t bytesWritten = file.write((uint8_t*)&this->send_message, sizeof(MessageData));

    #ifdef DEBUG
    if (bytesWritten != sizeof(MessageData)) {
        Serial.printf("WARNING: Mismatch in bytes written. Expected %u, got %u\n", sizeof(MessageData), bytesWritten);
    } else {
        Serial.printf("Successfully wrote %u bytes to '%s'\n", bytesWritten, FILENAME);
    }
    #endif

    file.close();
}

void Message::loadData() {
    //memset(&this->send_message, 0, sizeof(MessageData));
    File file = LittleFS.open(FILENAME, "r"); // "r" for read
    if (!file) {
        #ifdef DEBUG
        Serial.println("Failed to open file for reading.");
        #endif
        return;
    }

    size_t bytesRead = file.read((uint8_t*)&this->recv_message, sizeof(MessageData));
    #ifdef DEBUG
    if (bytesRead != sizeof(MessageData)) {
        Serial.printf("WARNING: Mismatch in bytes read. Expected %u, got %u\n", sizeof(MessageData), bytesRead);
    } else {
        Serial.printf("Successfully read %u bytes from '%s'\n", bytesRead, FILENAME);
    }
    #endif

    if(bytesRead == sizeof(MessageData)){
        this->updateSendData(true);
    }
}

uint8_t Message::current_user_idx() {
    return this->send_message.user_id;
}

void Message::updateSendData(bool all) {
    #ifdef DEBUG
        Serial.print("Updating send_message data form recv_message, all: ");
        Serial.println(all);
    #endif
    this->send_message.time = std::max(this->recv_message.time, this->send_message.time);
    this->rtc->setTime(std::max(this->send_message.time, this->rtc->getEpoch()), 0);

    for(int i = 0; i < NUM_USER; i++) {
        UserData* recv_user = &this->recv_message.users[i];
        UserData* send_user = &this->send_message.users[i];
        if(recv_user->idx >= send_user->idx){ //only update if newer idx
            send_user->beer = recv_user->beer;
            send_user->water = recv_user->water;
            send_user->shots = recv_user->shots;
            send_user->idx = recv_user->idx;
            strcpy(send_user->name, recv_user->name);
        }
    }

    if(all){
        this->send_message.user_id = this->recv_message.user_id;
    }
}


void Message::begin(ESP32Time* rtc) {

    Message::_instance = this;
    this->rtc = rtc;

    #ifdef DEBUG
    Serial.print("ESP Board MAC Address: ");
    Serial.println(WiFi.macAddress());
    #endif
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        #ifdef DEBUG
        Serial.println("Error initializing ESP-NOW");
        #endif
    }else{
        #ifdef DEBUG
        Serial.println("OK initializing ESP-NOW");
        #endif
    }

    esp_now_register_recv_cb(esp_now_recv_cb_t(globalOnDataRecv));
    esp_now_register_send_cb(globalOnDataSent);

    this->peerInfo.channel = 0;
    this->peerInfo.encrypt = false;

    this->loadData();
}

void Message::handleOnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

    memcpy(&this->recv_message, incomingData, sizeof(MessageData));
    #ifdef DEBUG
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Time: ");
    Serial.println(this->recv_message.time);
    for(int i = 0; i < NUM_USER; i++) {
        UserData userdata = this->recv_message.users[i];
        Serial.print("Name: ");
        Serial.println(userdata.name);
        Serial.print("Beer: ");
        Serial.println(userdata.beer);
        Serial.print("Water: ");
        Serial.println(userdata.water);
        Serial.print("Shots: ");
        Serial.println(userdata.shots);
    }
    #endif

    this->updateSendData(false);
    #ifdef DEBUG
    Serial.println("Storing recv data");
    #endif
    this->storeData();
}

void Message::handleOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

    #ifdef DEBUG
    char macStr[18];
    Serial.print("Packet to: ");
    // Copies the sender mac address to a string
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.print(macStr);
    Serial.print(" send status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
    #endif

    data_was_sent = true;
}


void Message::add_peers(uint8_t** macs, uint8_t size) {

    this->peers = macs;
    this->num_peers = size;

    for(int i = 0; i < size; i++){
        memcpy(peerInfo.peer_addr, macs[i], 6);

        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            #ifdef DEBUG
            Serial.println("Failed to add peer");
            #endif
        }else{
            #ifdef DEBUG
            Serial.println("Added peer");
            #endif
        }
    }

}


void Message::send() {

    this->send_message.time = this->rtc->getEpoch();
    this->send_message.users[this->send_message.user_id].idx += 1;

    this->storeData();

    #ifdef DEBUG
    Serial.print("Message Size: ");
    Serial.println(sizeof(MessageData));
    #endif

    data_was_sent = true;
    for(int i = 0; i < this->num_peers; i++){

        while(data_was_sent == false){
            __asm__("nop");
        }
        data_was_sent = false;

        esp_err_t result = esp_now_send(this->peers[i], (uint8_t *) &this->send_message, sizeof(MessageData));
        #ifdef DEBUG
        if (result == ESP_OK) {
            Serial.println("Sent with success");
        }
        else {
            Serial.print("Error sending the data; Status:");
            Serial.println(esp_err_to_name(result));
        }
        #endif
    }
}
