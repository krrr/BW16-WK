#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "http/HttpClient.h"

void handleApScanApi(HttpClient& client);
void handleDeviceScanApi(HttpClient& client, const String& req);
void handleDeauthApi(HttpClient& client, const String& req);
void handleChangeChannelApi(HttpClient& client, const String& req, const String& body);
void handleStatusApi(HttpClient& client);
void handleSetTimeApi(HttpClient& client, const String& req);