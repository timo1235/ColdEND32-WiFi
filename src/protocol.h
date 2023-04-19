#pragma once

#include <Arduino.h>
#include <esp_now.h>

//   #define POT_MIST 33                                 // Mist potentiometer pin
//   #define POT_SPIT 32                                 // Spit potentiometer pin
//   #define IN_MIST 36                                  // Mist switch
//   #define IN_FAST 34                                  // Fast mode switch
//   #define IN_AIR 39                                   // Air only switch

// This struct represents the typical message, a client sends to the main ESP32
// Max size of this struct is 250 bytes
// uint16_t : 2Bytes
// unsigned : 1 Bit each
typedef struct
{
    byte softwareVersion;
    uint16_t pot_mist;
    uint16_t pot_spit;
    unsigned in_mist : 1;
    unsigned in_fast : 1;
    unsigned in_air : 1;
} DATA_TO_COLDEND;

// This struct represents the typical message, the main ESP32 sends to a client
// Max size of this struct is 250
// uint16_t : 2Bytes
// unsigned : 1 Bit each
typedef struct
{
    byte softwareVersion;
    int temperatures[5];
    unsigned autosquareRunning : 1;
    unsigned spindelState : 1;
    unsigned alarmState : 1;
    unsigned peerIgnored : 1; // This peer is ignored by the mainboard and should stop sending data
} DATA_TO_CLIENT;

class PROTOCOL
{
public:
    void setup();
    void loop();
    static char *getMacStrFromAddress(uint8_t *address);
    static uint16_t getIntegerFromAddress(const uint8_t *address);
private:
    static void onDataSent(const uint8_t *address, esp_now_send_status_t status);
    static void onDataRecv(const uint8_t *address, const uint8_t *incomingData, int len);
    static esp_err_t sendMessageToClient(uint8_t *address, DATA_TO_CLIENT *data);
    // Saves that kind of functions are currently send to the esp by external devices
    // For example Handwheel 1 sends joystick data and Handwheel 2 sends feedrate data

    static void protocolTaskHandler(void *pvParameters);
    TaskHandle_t protocolTask;
    
    static void dumpDataToControl();
    // static esp_now_peer_info_t peerInfo;
    static bool addPeerIfNotExists(uint8_t *address);

    uint32_t lastTimeoutCheck = 5000;
    
};
