#pragma once

#include <Arduino.h>
#include <esp_now.h>

// This struct describes what inputs are used for the coldend32
typedef struct
{
    unsigned setPotMist : 1;
    unsigned setPotSpit : 1;
    unsigned setInMist : 1;
    unsigned setInFast : 1;
    unsigned setInAir : 1;
    unsigned returnACK : 1;       // Return ACK to client
    unsigned returnData : 1;      // Send some data back. This would be the current coldend state
    uint16_t updateInterval_MS;   // Interval in ms the client sends data to the coldend
} COLDEND_COMMAND;

typedef struct
{
    byte softwareVersion;
    uint16_t pot_mist;
    uint16_t pot_spit;
    unsigned in_mist : 1;
    unsigned in_fast : 1;
    unsigned in_air : 1;
    COLDEND_COMMAND command;
} DATA_TO_COLDEND;

typedef struct
{
    unsigned coolantValve : 1;
    unsigned airValve : 1;
    unsigned spitMode : 1;
    float mist_val;
    float spit_val;
} DATA_COLDEND_TO_CLIENT;

class PROTOCOL {
  public:
    void setup();
    void loop();
    static char *getMacStrFromAddress(uint8_t *address);
    static uint16_t getIntegerFromAddress(const uint8_t *address);

  private:
    static void onDataSent(const uint8_t *address, esp_now_send_status_t status);
    static void onDataRecv(const uint8_t *address, const uint8_t *incomingData, int len);
    static esp_err_t sendMessageToClient(uint8_t *address, DATA_COLDEND_TO_CLIENT *data);

    static void protocolTaskHandler(void *pvParameters);
    TaskHandle_t protocolTask;

    static void dumpDataToControl();
    // static esp_now_peer_info_t peerInfo;
    static bool addPeerIfNotExists(uint8_t *address);

    uint32_t lastTimeoutCheck = 5000;
};
