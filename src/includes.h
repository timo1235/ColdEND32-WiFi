#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#include <config.h>
#include <display.h>
#include <func.h>
#include <protocol.h>
#include <vars.h>

extern DATA_TO_COLDEND dataToColdEnd;
extern DATA_COLDEND_TO_CLIENT dataToClient;
extern PROTOCOL protocol;