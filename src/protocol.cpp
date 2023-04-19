#include <includes.h>

// void receiveEvent(int count){
// Serial.println(count);
// }

uint8_t newMACAddress[] = CONTROLLER_MAC_ADDRESS;

DATA_TO_COLDEND dataToControl = {};
DATA_TO_CLIENT dataToClient = {};

void PROTOCOL::setup()
{
    WiFi.enableLongRange(true);
    WiFi.mode(WIFI_STA);

    Serial.print("My old Mac Address: ");
    Serial.println(WiFi.macAddress());

    esp_wifi_set_ps(WIFI_PS_NONE);
    esp_wifi_set_mac(WIFI_IF_STA, &newMACAddress[0]);

    Serial.print("My new Mac Address: ");
    Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW. Things wont work");
        return;
    }

    esp_now_register_recv_cb(PROTOCOL::onDataRecv);
    esp_now_register_send_cb(PROTOCOL::onDataSent);

    // Create a task for the protocol
    xTaskCreatePinnedToCore(
        PROTOCOL::protocolTaskHandler, /* Task function. */
        "Protocol task",               /* name of task. */
        10000,                         /* Stack size of task */
        this,                          /* parameter of the task */
        tskIDLE_PRIORITY,              /* priority of the task */
        &protocolTask,                 /* Task handle to keep track of created task */
        0);
}

void PROTOCOL::protocolTaskHandler(void *pvParameters)
{
    auto *protocol = (PROTOCOL *)pvParameters;
    for (;;)
    {
        if (millis() - protocol->lastTimeoutCheck > WIFI_TIMEOUT_CHECK_INTERVAL_MS)
        {
            bool anyClientConnected = false;
            protocol->lastTimeoutCheck = millis();
            // ToDo: Reset outputs if client is not active anymore
        }
    }
    vTaskDelay(1);
}

bool PROTOCOL::addPeerIfNotExists(uint8_t *address)
{
    // Return if the peer already exists
    if (esp_now_is_peer_exist(address))
    {
        return false;
    }

    Serial.print("Wifi: Adding peer with MAC: ");
    Serial.print(PROTOCOL::getMacStrFromAddress(address));

    // register peer
    esp_now_peer_info_t peerInfo = {};
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    memcpy(peerInfo.peer_addr, address, 6);

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.print("Wifi: Failed to add peer: ");
        Serial.println(PROTOCOL::getMacStrFromAddress(address));
        return false;
    }
    return true;
}

esp_err_t PROTOCOL::sendMessageToClient(uint8_t *address, DATA_TO_CLIENT *data)
{
    return esp_now_send(address, (uint8_t *)data, sizeof(DATA_TO_CLIENT));
}

// callback function that will be executed when data is received
void PROTOCOL::onDataRecv(const uint8_t *address, const uint8_t *incomingData, int len)
{
    // Add the peer to the list of known peers
    bool isNew = PROTOCOL::addPeerIfNotExists((uint8_t *)address);

    uint16_t intAddress = PROTOCOL::getIntegerFromAddress(address);
    // Search for the client in the list of known clients

    memcpy(&dataToControl, incomingData, sizeof(dataToControl));
    PROTOCOL::dumpDataToControl();
    // ToDo use send data
    // ToDo return a message?
    // PROTOCOL::sendMessageToClient((uint8_t *)address, &dataToClient);
}

void PROTOCOL::onDataSent(const uint8_t *address, esp_now_send_status_t status)
{
    if (status == ESP_NOW_SEND_SUCCESS)
    {
    }
    else
    {
    }
}

char *PROTOCOL::getMacStrFromAddress(uint8_t *address)
{
    static char macStr[18];
    // Copies the sender mac address to a string
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", address[0], address[1], address[2], address[3], address[4], address[5]);
    return macStr;
}

uint16_t PROTOCOL::getIntegerFromAddress(const uint8_t *address)
{
    uint16_t integer = 0;
    integer += address[0];
    integer += address[1];
    integer += address[2];
    integer += address[3];
    integer += address[4];
    integer += address[5];
    return integer;
}

void PROTOCOL::dumpDataToControl()
{
    Serial.println("--- Data to control ---");
    Serial.print("Pot Mist: \t\t");
    Serial.println(dataToControl.pot_mist);
    Serial.print("Pot Spit: \t\t");
    Serial.println(dataToControl.pot_spit);
    Serial.print("In Mist: \t\t");
    Serial.println(dataToControl.in_mist);
    Serial.print("In Fast: \t\t");
    Serial.println(dataToControl.in_fast);
    Serial.print("In Air: \t\t");
    Serial.println(dataToControl.in_air);
}

PROTOCOL protocol;