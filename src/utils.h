#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

void wifiClientSendJson(WiFiClient& client, JsonDocument& doc);
void wifiClientSendJsonFail(WiFiClient& client, const String& message);

String urlDecode(const String& s);

String extractQueryParam(const String& req, const String& param);
