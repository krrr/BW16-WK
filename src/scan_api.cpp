#include <Arduino.h>
#include <WiFi.h>
#include <wifi_conf.h>
#include <wifi_structures.h>

// wifi_scan_networks() 在 STA+AP 并发模式下对 STA 接口 (wlan0) 发起的全信道扫描
// 可能因驱动层未正确初始化 STA 链路而返回空结果。
// wifi_scan_networks_mcc() 是 SDK 专为并发模式设计的逐信道扫描版本，
// 它逐个信道调用 wext_set_scan + 信道间插入 100ms 间隔让 AP 发送信标，
// 保证扫描期间 AP 服务不中断且结果正确返回。
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

static String escapeJson(const String& s) {
    String out;
    out.reserve(s.length() + 4);
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '"' || c == '\\') { out += '\\'; out += c; }
        else if (c == '\n') { out += "\\n"; }
        else if (c == '\r') { out += "\\r"; }
        else if (c == '\t') { out += "\\t"; }
        else if ((unsigned char)c < 0x20) { out += ' '; }
        else { out += c; }
    }
    return out;
}

static rtw_result_t scanResultHandler(rtw_scan_handler_result_t* scan_result) {
    if (!scan_result->scan_complete) {
        if (g_scan_count >= 64) return RTW_SUCCESS;
        rtw_scan_result_t* r = &scan_result->ap_details;
        r->SSID.val[r->SSID.len] = 0;
        ScanAP& ap = g_scan_aps[g_scan_count];
        ap.ssid = String((const char*)r->SSID.val);
        ap.rssi = r->signal_strength;
        ap.channel = r->channel;
        // SDK has bug, always 5G: r->band == RTW_802_11_BAND_5GHZ
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

static void sendJson(WiFiClient& client, const String& json) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json; charset=utf-8");
    client.print("Content-Length: ");
    client.println(json.length());
    client.println("Connection: close");
    client.println();
    client.print(json);
}

void handleScanApi(WiFiClient& client) {
    g_scan_count = 0;
    g_scan_done = false;

    unsigned long start = millis();
    // 必须使用 _mcc 版本：普通 wifi_scan_networks 在并发模式下 STA 链路未完整初始化，扫描结果为空
    if (wifi_scan_networks_mcc(scanResultHandler, NULL) != RTW_SUCCESS) {
        sendJson(client, "{\"success\":false,\"message\":\"scan failed to start\"}");
        return;
    }
    while (!g_scan_done && millis() - start < SCAN_TIMEOUT_MS) {
        delay(10);
    }

    String json = "{\"success\":true,\"count\":";
    json += g_scan_count;
    json += ",\"networks\":[";
    for (int i = 0; i < g_scan_count; i++) {
        if (i > 0) json += ",";
        json += "{";
        json += "\"ssid\":\"" + escapeJson(g_scan_aps[i].ssid) + "\",";
        json += "\"bssid\":\"" + g_scan_aps[i].bssid_str + "\",";
        json += "\"rssi\":" + String(g_scan_aps[i].rssi) + ",";
        json += "\"channel\":" + String(g_scan_aps[i].channel) + ",";
        json += "\"band\":\"" + g_scan_aps[i].band + "\",";
        json += "\"security\":\"" + g_scan_aps[i].security + "\"";
        json += "}";
    }
    json += "]}";
    sendJson(client, json);
}
