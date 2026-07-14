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
#include "sys_api.h"

const char COMPILE_DATE[] = __DATE__;
const char COMPILE_TIME[] = __TIME__;


void handleChangeChannelApi(HttpClient& client) {
    if (client.method() != "POST") {
        client.sendJsonFail("Only POST method is allowed");
        return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, client.body());
    if (error) {
        client.sendJsonFail("Invalid JSON body");
        return;
    }

    if (!doc.containsKey("channel")) {
        client.sendJsonFail("Missing channel parameter");
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
        client.sendJsonFail("Invalid channel");
        return;
    }

    if (channel != ap_channel) {
        if (wifi_ap_switch_chl_and_inform(channel) != RTW_SUCCESS) {
            client.sendJsonFail("failed to switch channel");
            return;
        }
        wext_set_channel(WLAN0_NAME, channel);  // 必须，上面的调用不够
        ap_channel = channel;
        delay(100);
    }

    JsonDocument resp;
    resp["success"] = true;
    resp["ap_channel"] = ap_channel;
    client.sendJson(resp);
}


void handleSetTimeApi(HttpClient& client) {
    String t_str = client.queryParam("t");
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
    client.sendJson(doc);
}

void handleStatusApi(HttpClient& client) {
    JsonDocument doc;
    doc["uptime"] = millis() / 1000;
    doc["compile_date"] = COMPILE_DATE;
    doc["compile_time"] = COMPILE_TIME;
    doc["rtc_time"] = rtc_read();
    doc["ap_channel"] = ap_channel;
    doc["free_heap"] = xPortGetFreeHeapSize();
    client.sendJson(doc);
}

void handleRebootApi(HttpClient& client) {
    if (client.method() != "POST") {
        client.sendJsonFail("Only POST method is allowed");
        return;
    }

    JsonDocument doc;
    doc["success"] = true;
    doc["message"] = "Rebooting...";
    client.sendJson(doc);
    client.stop();

    delay(500);
    sys_reset();
}