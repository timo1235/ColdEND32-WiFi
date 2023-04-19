#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include <config.h>
#include <vars.h>
#include <protocol.h>
#include <func.h>
#include <display.h>

extern DATA_TO_COLDEND dataToControl;
extern DATA_TO_CLIENT dataToClient;
extern PROTOCOL protocol;