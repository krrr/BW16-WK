#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

void wifiClientSendJson(WiFiClient& client, JsonDocument& doc);
