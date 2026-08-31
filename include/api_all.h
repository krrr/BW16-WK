#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "http/HttpClient.h"

void handleApScanApi(HttpClient& client);
void handleDeviceScanApi(HttpClient& client);
void handleTestDeauthApi(HttpClient& client);
void handleChangeChannelApi(HttpClient& client);
void handleStatusApi(HttpClient& client);
void handleSetTimeApi(HttpClient& client);
void handleSettingsApi(HttpClient& client);
void handleOtaApi(HttpClient& client);
void handleRebootApi(HttpClient& client);
void handleDeepSleepApi(HttpClient& client);
void handleAttackStatusApi(HttpClient& client);
void handleAttackPlanApi(HttpClient& client);