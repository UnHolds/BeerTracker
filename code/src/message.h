#ifndef MESSAGE_H
#define MESSAGE_H


#include <WiFi.h>
#include <esp_wifi.h>
#include <Arduino.h>
#include <esp_now.h>
#include "FS.h"
#include <LittleFS.h>
#include "config.h"
#include <ESP32Time.h>

#define FILENAME "/data.bin"

//#define FORMAT_LITTLEFS_IF_FAILED true

typedef struct UserData{
    char name[MAX_USERNAME_LEN];
    uint8_t beer;
    uint8_t water;
    uint8_t shots;
} UserData;

typedef struct MessageData {
    unsigned long time;
    uint8_t user_id;
    UserData users[NUM_USER];
} MessageData;

class Message {
    public:
        void begin(ESP32Time* rtc);
        void add_peer(uint8_t mac[]);
        void send();
        UserData* getUser(int idx);
        static Message* _instance;
        void handleOnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
        void handleOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
        MessageData send_message;

    private:

        void updateSendData(bool all);
        esp_now_peer_info_t peerInfo;
        MessageData recv_message;
        void storeData();
        void loadData();
        ESP32Time* rtc;

};



void globalOnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void globalOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);


#endif
