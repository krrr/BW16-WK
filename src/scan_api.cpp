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

// ─── Device Scan (被动监听特定AP关联设备) ───

#define DEVICE_SCAN_TIMEOUT_MS 25000
#define MAX_DISCOVERED_DEVICES 32

typedef struct {
    uint8_t mac[6];
    short rssi;
    unsigned long last_seen;
    int packet_count;
} DiscoveredDevice;

static DiscoveredDevice g_devices[MAX_DISCOVERED_DEVICES];
static volatile int g_device_count = 0;
static uint8_t g_target_bssid[6];
static int g_target_channel;

static bool parseBssid(const String& str, uint8_t* bssid) {
    unsigned int v[6];
    if (sscanf(str.c_str(), "%02x:%02x:%02x:%02x:%02x:%02x",
               &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]) != 6)
        return false;
    for (int i = 0; i < 6; i++) bssid[i] = (uint8_t)v[i];
    return true;
}

static void addDiscoveredDevice(const uint8_t* mac, short rssi) {
    if (mac[0] == 0xFF || (mac[0] & 0x01)) return;
    if (memcmp(mac, g_target_bssid, 6) == 0) return;
    for (int i = 0; i < g_device_count; i++) {
        if (memcmp(g_devices[i].mac, mac, 6) == 0) {
            g_devices[i].last_seen = millis();
            g_devices[i].packet_count++;
            if (rssi > g_devices[i].rssi) g_devices[i].rssi = rssi;
            return;
        }
    }
    if (g_device_count >= MAX_DISCOVERED_DEVICES) return;
    memcpy(g_devices[g_device_count].mac, mac, 6);
    g_devices[g_device_count].rssi = rssi;
    g_devices[g_device_count].last_seen = millis();
    g_devices[g_device_count].packet_count = 1;
    g_device_count++;
}

static void deviceSniffCallback(unsigned char* buf, unsigned int len, void* user) {
    (void)user;
    if (!buf || len < 24) return;

    uint16_t fc = buf[0] | (buf[1] << 8);
    unsigned int type = (fc >> 2) & 0x03;
    bool toDS = (fc & (1 << 8)) != 0;
    bool fromDS = (fc & (1 << 9)) != 0;

    const uint8_t* addr1 = buf + 4;
    const uint8_t* addr2 = buf + 10;
    const uint8_t* addr3 = buf + 16;

    // Infer BSSID/DA/SA from addressing bits
    const uint8_t* da = addr1;
    const uint8_t* sa = addr2;
    const uint8_t* bssid = addr3;

    if (!toDS && fromDS) {
        // AP→STA: A1=DA, A2=BSSID, A3=SA
        bssid = addr2;
        sa = addr3;
    } else if (toDS && !fromDS) {
        // STA→AP: A1=BSSID, A2=SA, A3=DA
        bssid = addr1;
        da = addr3;
    } else if (toDS && fromDS) {
        return;
    }

    if (memcmp(bssid, g_target_bssid, 6) != 0) return;

    // Collect all non-AP addresses
    if (memcmp(sa, g_target_bssid, 6) != 0) addDiscoveredDevice(sa, -70);
    if (memcmp(da, g_target_bssid, 6) != 0 && memcmp(da, sa, 6) != 0)
        addDiscoveredDevice(da, -70);
}

void handleDeviceScanApi(WiFiClient& client, const String& req) {
    String bssidStr = extractQueryParam(req, "bssid");
    String chStr = extractQueryParam(req, "channel");

    if (bssidStr.length() == 0 || chStr.length() == 0) {
        JsonDocument doc;
        doc["success"] = false;
        doc["message"] = "missing bssid or channel";
        wifiClientSendJson(client, doc);
        return;
    }
    bssidStr = urlDecode(bssidStr);
    if (!parseBssid(bssidStr, g_target_bssid)) {
        JsonDocument doc;
        doc["success"] = false;
        doc["message"] = "invalid bssid";
        wifiClientSendJson(client, doc);
        return;
    }
    g_target_channel = chStr.toInt();

    g_device_count = 0;

    wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 1);
    delay(100);

    wext_set_channel(WLAN0_NAME, g_target_channel);
    delay(100);

    wifi_set_promisc(RTW_PROMISC_ENABLE_2, deviceSniffCallback, 1);

    unsigned long start = millis();
    while (millis() - start < DEVICE_SCAN_TIMEOUT_MS) {
        delay(50);
        if ((millis() - start) % 3000 < 60) {
            wext_set_channel(WLAN0_NAME, g_target_channel);
        }
    }

    wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 1);

    JsonDocument doc;
    doc["success"] = true;
    doc["bssid"] = bssidStr;
    doc["channel"] = g_target_channel;
    doc["count"] = g_device_count;
    JsonArray devices = doc["devices"].to<JsonArray>();
    for (int i = 0; i < g_device_count; i++) {
        JsonObject d = devices.add<JsonObject>();
        char mac[18];
        snprintf(mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X",
            g_devices[i].mac[0], g_devices[i].mac[1], g_devices[i].mac[2],
            g_devices[i].mac[3], g_devices[i].mac[4], g_devices[i].mac[5]);
        d["mac"] = mac;
        d["rssi"] = (int)g_devices[i].rssi;
        d["packets"] = g_devices[i].packet_count;
    }
    wifiClientSendJson(client, doc);
}
