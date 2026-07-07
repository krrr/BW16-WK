#include <Arduino.h>
#include <WiFi.h>
#include <main.h>
#include "rtc_api.h"
#include <wifi_conf.h>
#include <ArduinoJson.h>
#include "api_all.h"
#include "utils.h"
#include "wifi_drv.h"
#include "portable.h"  // freertos

const char COMPILE_DATE[] = __DATE__;
const char COMPILE_TIME[] = __TIME__;


void handleChangeChannelApi(HttpClient& client, const String& req, const String& body) {
    if (!req.startsWith("POST")) {
        wifiClientSendJsonFail(client, "Only POST method is allowed");
        return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, body);
    if (error) {
        wifiClientSendJsonFail(client, "Invalid JSON body");
        return;
    }

    if (!doc.containsKey("channel")) {
        wifiClientSendJsonFail(client, "Missing channel parameter");
        return;
    }

    int channel = doc["channel"].as<int>();
    bool isValid = (channel >= 1 && channel <= 14) ||
                   (channel == 36 || channel == 40 || channel == 44 || channel == 48 ||
                    channel == 52 || channel == 56 || channel == 60 || channel == 64 ||
                    channel == 100 || channel == 104 || channel == 108 || channel == 112 ||
                    channel == 116 || channel == 120 || channel == 124 || channel == 128 ||
                    channel == 132 || channel == 136 || channel == 140 || channel == 144 ||
                    channel == 149 || channel == 153 || channel == 157 || channel == 161 ||
                    channel == 165);
    if (!isValid) {
        wifiClientSendJsonFail(client, "Invalid channel");
        return;
    }

    if (channel != ap_channel) {
        if (wifi_ap_switch_chl_and_inform(channel) != RTW_SUCCESS) {
            wifiClientSendJsonFail(client, "failed to switch channel");
            return;
        }
        wext_set_channel(WLAN0_NAME, channel);  // 必须，上面的调用不够
        ap_channel = channel;
        delay(100);
    }

    JsonDocument resp;
    resp["success"] = true;
    resp["ap_channel"] = ap_channel;
    wifiClientSendJson(client, resp);
}


void handleSetTimeApi(HttpClient& client, const String& req) {
    String t_str = urlDecode(extractQueryParam(req, "t"));
    long t = t_str.toInt();

    JsonDocument doc;
    if (t > 0) {
        rtc_init();  // setup里init一次后面再write总是返回-1
        rtc_write(t);
        doc["success"] = true;
        doc["rtc_time"] = rtc_read();
    } else {
        doc["success"] = false;
        doc["message"] = "invalid timestamp";
    }
    wifiClientSendJson(client, doc);
}

void handleStatusApi(HttpClient& client) {
    JsonDocument doc;
    doc["uptime"] = millis() / 1000;
    doc["compile_date"] = COMPILE_DATE;
    doc["compile_time"] = COMPILE_TIME;
    doc["rtc_time"] = rtc_read();
    doc["ap_channel"] = ap_channel;
    doc["free_heap"] = xPortGetFreeHeapSize();
    wifiClientSendJson(client, doc);
}