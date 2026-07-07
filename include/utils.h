#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "http/HttpClient.h"


void wifiClientSendJson(HttpClient& client, JsonDocument& doc);
void wifiClientSendJsonFail(HttpClient& client, const String& message);

String urlDecode(const String& s);

String extractQueryParam(const String& req, const String& param);
