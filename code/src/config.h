#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define NUM_USER 7
#define MAX_USERNAME_LEN 16

static String RST_USERNAMES[NUM_USER] = {"UnHold", "Luggi", "Tabaluga", "Someone", "Simi", "Bene", "Martin"};

#define secToMs(secs) secs * 1000
#define secToUs(secs) secs * 1000 * 1000

#define STANDBY_TIMEOUT secToMs(20) // 20s - time until the deceive goes to sleep after no input
#define SLEEP_TIME 10 * 60 //10min // wakeup is defined as current_unix_timestamp mod SLEEP_TIME == 0

#define USER_SEND_OFFSET secToMs(1) //each user send 1 sec after the other
#define UPDATE_AWAKE_TIME (secToMs(16) + USER_SEND_OFFSET * NUM_USER)

#define LOCK_SCREEN_ENABLE false

#define VERSION "1.2"

#endif
