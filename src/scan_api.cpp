#include <Arduino.h>
#include <WiFi.h>
#include <wifi_conf.h>
#include <wifi_structures.h>
#include <ArduinoJson.h>
#include "utils.h"

#define SCAN_TIMEOUT_MS 12000

typedef struct {
    String ssid;
    String bssid_str;
    short  rssi;
    uint   channel;
    String band;
    String security;
} ScanAP;

static ScanAP g_scan_aps[64];
static volatile int g_scan_count = 0;
static volatile bool g_scan_done = false;

static rtw_result_t scanResultHandler(rtw_scan_handler_result_t* scan_result) {
    if (!scan_result->scan_complete) {
        if (g_scan_count >= 64) return RTW_SUCCESS;
        rtw_scan_result_t* r = &scan_result->ap_details;
        r->SSID.val[r->SSID.len] = 0;
        ScanAP& ap = g_scan_aps[g_scan_count];
        ap.ssid = String((const char*)r->SSID.val);
        ap.rssi = r->signal_strength;
        ap.channel = r->channel;
        ap.band = (r->channel >= 36) ? "5GHz" : "2.4GHz";
        switch (r->security) {
            case RTW_SECURITY_OPEN:                 ap.security = "OPEN"; break;
            case RTW_SECURITY_WEP_PSK:              ap.security = "WEP"; break;
            case RTW_SECURITY_WEP_SHARED:           ap.security = "WEP-SHARED"; break;
            case RTW_SECURITY_WPA_TKIP_PSK:         ap.security = "WPA-TKIP"; break;
            case RTW_SECURITY_WPA_AES_PSK:          ap.security = "WPA-AES"; break;
            case RTW_SECURITY_WPA_MIXED_PSK:        ap.security = "WPA-MIXED"; break;
            case RTW_SECURITY_WPA2_AES_PSK:         ap.security = "WPA2-AES"; break;
            case RTW_SECURITY_WPA2_TKIP_PSK:        ap.security = "WPA2-TKIP"; break;
            case RTW_SECURITY_WPA2_MIXED_PSK:       ap.security = "WPA2-MIXED"; break;
            case RTW_SECURITY_WPA_WPA2_TKIP_PSK:    ap.security = "WPA/WPA2-TKIP"; break;
            case RTW_SECURITY_WPA_WPA2_AES_PSK:     ap.security = "WPA/WPA2-AES"; break;
            case RTW_SECURITY_WPA_WPA2_MIXED_PSK:   ap.security = "WPA/WPA2"; break;
            case RTW_SECURITY_WPA2_AES_CMAC:        ap.security = "WPA2-AES-CMAC"; break;
            case RTW_SECURITY_WPA_TKIP_ENTERPRISE:  ap.security = "WPA-TKIP-ENT"; break;
            case RTW_SECURITY_WPA_AES_ENTERPRISE:   ap.security = "WPA-AES-ENT"; break;
            case RTW_SECURITY_WPA_MIXED_ENTERPRISE: ap.security = "WPA-MIXED-ENT"; break;
            case RTW_SECURITY_WPA2_TKIP_ENTERPRISE: ap.security = "WPA2-TKIP-ENT"; break;
            case RTW_SECURITY_WPA2_AES_ENTERPRISE:  ap.security = "WPA2-AES-ENT"; break;
            case RTW_SECURITY_WPA2_MIXED_ENTERPRISE:ap.security = "WPA2-MIXED-ENT"; break;
            case RTW_SECURITY_WPA_WPA2_TKIP_ENTERPRISE:  ap.security = "WPA/WPA2-TKIP-ENT"; break;
            case RTW_SECURITY_WPA_WPA2_AES_ENTERPRISE:   ap.security = "WPA/WPA2-AES-ENT"; break;
            case RTW_SECURITY_WPA_WPA2_MIXED_ENTERPRISE: ap.security = "WPA/WPA2-ENT"; break;
            case RTW_SECURITY_WPS_OPEN:             ap.security = "WPS-OPEN"; break;
            case RTW_SECURITY_WPS_SECURE:           ap.security = "WPS-SECURE"; break;
            case RTW_SECURITY_WPA3_AES_PSK:         ap.security = "WPA3-SAE"; break;
            case RTW_SECURITY_WPA2_WPA3_MIXED:      ap.security = "WPA2/WPA3"; break;
            case RTW_SECURITY_WPA3_ENTERPRISE:      ap.security = "WPA3-ENT"; break;
            case RTW_SECURITY_WPA3_OWE:             ap.security = "WPA3-OWE"; break;
            default:                                ap.security = "UNKNOWN"; break;
        }
        char bssid_str[] = "XX:XX:XX:XX:XX:XX";
        snprintf(bssid_str, sizeof(bssid_str), "%02X:%02X:%02X:%02X:%02X:%02X",
            r->BSSID.octet[0], r->BSSID.octet[1], r->BSSID.octet[2],
            r->BSSID.octet[3], r->BSSID.octet[4], r->BSSID.octet[5]);
        ap.bssid_str = bssid_str;
        g_scan_count++;
    } else {
        g_scan_done = true;
    }
    return RTW_SUCCESS;
}

void handleScanApi(WiFiClient& client) {
    g_scan_count = 0;
    g_scan_done = false;

    unsigned long start = millis();
    // 必须使用 _mcc 版本：普通 wifi_scan_networks 在STA+AP并发模式下 STA 链路未完整初始化，扫描结果为空
    // wifi_scan_networks_mcc() 是 SDK 专为并发模式设计的逐信道扫描版本，
    // 它逐个信道调用 wext_set_scan + 信道间插入 100ms 间隔让 AP 发送信标，
    // 保证扫描期间 AP 服务不中断且结果正确返回。
    if (wifi_scan_networks_mcc(scanResultHandler, NULL) != RTW_SUCCESS) {
        JsonDocument doc;
        doc["success"] = false;
        doc["message"] = "scan failed to start";
        wifiClientSendJson(client, doc);
        return;
    }
    while (!g_scan_done && millis() - start < SCAN_TIMEOUT_MS) {
        delay(10);
    }

    JsonDocument doc;
    doc["success"] = true;
    doc["count"] = g_scan_count;
    JsonArray networks = doc["networks"].to<JsonArray>();
    for (int i = 0; i < g_scan_count; i++) {
        JsonObject net = networks.add<JsonObject>();
        net["ssid"] = g_scan_aps[i].ssid;
        net["bssid"] = g_scan_aps[i].bssid_str;
        net["rssi"] = g_scan_aps[i].rssi;
        net["channel"] = g_scan_aps[i].channel;
        net["band"] = g_scan_aps[i].band;
        net["security"] = g_scan_aps[i].security;
    }
    wifiClientSendJson(client, doc);
}
