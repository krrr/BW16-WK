#pragma once
#include <Arduino.h>
#include <WiFi.h>

void handleApScanApi(WiFiClient& client);
void handleDeviceScanApi(WiFiClient& client, const String& req);
void handleDeauthApi(WiFiClient& client, const String& req);
void handleChangeChannelApi(WiFiClient& client, const String& req, const String& body);
void handleStatusApi(WiFiClient& client);
void handleSetTimeApi(WiFiClient& client, const String& req);