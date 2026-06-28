#include <Arduino.h>
#include <WiFi.h>
#include <main.h>
#include <wifi_conf.h>
#include <wifi_structures.h>
#include <ArduinoJson.h>
#include "utils.h"
#include "wifi_drv.h"

// extern "C" {
//     int wifi_set_mgnt_rxfilter(uint8_t enable);
//     int wifi_set_promisc_filter_reason(uint8_t enable);
// }

#define SCAN_TIMEOUT_MS 12000

static void printBssid(const uint8_t* bssid) {
    for (int i=0;i<6;i++){if(bssid[i]<0x10)Serial.print("0");Serial.print(bssid[i],HEX);if(i<5)Serial.print(":");}
}

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

static rtw_result_t apScanResultHandler(rtw_scan_handler_result_t* scan_result) {
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

void handleApScanApi(WiFiClient& client) {
    g_scan_count = 0;
    g_scan_done = false;

    unsigned long start = millis();
    // 必须使用 _mcc 版本：普通 wifi_scan_networks 在STA+AP并发模式下 STA 链路未完整初始化，扫描结果为空
    // wifi_scan_networks_mcc() 是 SDK 专为并发模式设计的逐信道扫描版本，
    // 它逐个信道调用 wext_set_scan + 信道间插入 100ms 间隔让 AP 发送信标，
    // 保证扫描期间 AP 服务不中断且结果正确返回。
    if (wifi_scan_networks_mcc(apScanResultHandler, NULL) != RTW_SUCCESS) {
        JsonDocument doc;
        doc["success"] = false;
        doc["message"] = "scan failed to start";
        wifiClientSendJson(client, doc);
        return;
    }
    while (!g_scan_done && millis() - start < SCAN_TIMEOUT_MS) {
        delay(30);
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
    int packets_out; // Device -> Router (Uplink / Tx)
    int packets_in;  // Router -> Device (Downlink / Rx)
    int handshakes;  // Handshake packets count
} DiscoveredDevice;

static DiscoveredDevice g_devices[MAX_DISCOVERED_DEVICES];
static volatile int g_device_count = 0;
static uint8_t g_target_bssid[6];
static int g_target_channel;

// AmebaD (RTL8720DN) 的 newlib-nano sscanf 不支持 %02x 格式，
// 调用会静默返回但不写入任何值，导致 g_target_bssid 始终为全零。
// 改用纯手动十六进制解析。
static bool parseBssid(const String& str, uint8_t* bssid) {
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
        bssid[i] = (uint8_t)v;
        if (i < 5 && *s++ != ':') return false;
    }
    return true;
}

static void addDiscoveredDevice(const uint8_t* mac, bool is_uplink, bool is_handshake = false) {
    if (mac[0] == 0xFF || (mac[0] & 0x01)) return;
    if (memcmp(mac, g_target_bssid, 6) == 0) return;
    for (int i = 0; i < g_device_count; i++) {
        if (memcmp(g_devices[i].mac, mac, 6) == 0) {
            if (is_uplink) {
                g_devices[i].packets_out++;
            } else {
                g_devices[i].packets_in++;
            }
            if (is_handshake) {
                g_devices[i].handshakes++;
            }
            return;
        }
    }
    if (g_device_count >= MAX_DISCOVERED_DEVICES) return;
    memcpy(g_devices[g_device_count].mac, mac, 6);
    if (is_uplink) {
        g_devices[g_device_count].packets_out = 1;
        g_devices[g_device_count].packets_in = 0;
    } else {
        g_devices[g_device_count].packets_out = 0;
        g_devices[g_device_count].packets_in = 1;
    }
    g_devices[g_device_count].handshakes = is_handshake ? 1 : 0;
    g_device_count++;
}

#define SNIFF_DEBUG

static void deviceSniffCallback(unsigned char* buf, unsigned int len, void* user) {
    (void)user;
    if (!buf || len < 22) return;

    #ifdef SNIFF_DEBUG
    // ── DEBUG: 打印原始包前若干字节，探测固件前置头偏移 ──
    static unsigned int debug_count = 0;
    static unsigned long debug_last_log = 0;
    debug_count++;
    if (debug_count <= 20 || millis() - debug_last_log > 10000) {
        debug_last_log = millis();
        Serial.print("[SNIFF_DEBUG] #"); Serial.print(debug_count);
        Serial.print(" len="); Serial.print(len);
        Serial.print(" hex=");
        unsigned int dump_len = (len > 48) ? 48 : len;
        for (unsigned int i = 0; i < dump_len; i++) {
            if (buf[i] < 0x10) Serial.print("0");
            Serial.print(buf[i], HEX);
            if ((i & 1) && i < dump_len - 1) Serial.print(" ");
        }
        Serial.println();

        // 尝试多种偏移解析帧控制头
        const int tryOffsets[] = {0, 4, 8, 12, 16, 20, 24, 32, 36, 40};
        for (size_t t = 0; t < sizeof(tryOffsets) / sizeof(tryOffsets[0]); t++) {
            int off = tryOffsets[t];
            if (len < (unsigned)(off + 24)) continue;
            const uint8_t* base = buf + off;
            uint16_t fc = (uint16_t)base[0] | ((uint16_t)base[1] << 8);
            uint8_t ftype = (fc >> 2) & 0x3;
            uint8_t fsubtype = (fc >> 4) & 0xF;
            // 只打印看起来有效的帧控制(非全0, 非全1, type合理)
            if (fc != 0 && fc != 0xFFFF && ftype <= 3) {
                Serial.print("[SNIFF_DEBUG]   offset="); Serial.print(off);
                Serial.print(" fc=0x"); if (fc < 0x1000) Serial.print("0");
                if (fc < 0x100) Serial.print("0");
                if (fc < 0x10) Serial.print("0");
                Serial.print(fc, HEX);
                Serial.print(" type="); Serial.print(ftype);
                Serial.print(" subtype="); Serial.println(fsubtype);
            }
        }
    }
    #endif

    uint16_t fc = buf[0] | (buf[1] << 8);
    unsigned int type = (fc >> 2) & 0x03;
    bool toDS = (fc & (1 << 8)) != 0;
    bool fromDS = (fc & (1 << 9)) != 0;

    const uint8_t* addr1 = buf + 4;
    const uint8_t* addr2 = buf + 10;
    const uint8_t* addr3 = buf + 16;

    // Derive BSSID from frame control flags (TA & RA are always addr2 & addr1)
    const uint8_t* bssid;
    if (!toDS && fromDS) {
        bssid = addr2;
    } else if (toDS && !fromDS) {
        bssid = addr1;
    } else if (toDS && fromDS) {
        return;
    } else {
        bssid = addr3;
    }

    // 过滤掉不属于目标AP的所有无线数据包
    if (memcmp(bssid, g_target_bssid, 6) != 0) {
        #ifdef SNIFF_DEBUG
        Serial.print("[SNIFF] BSSID mismatch: got=");
        printBssid(bssid);
        Serial.print(" target=");
        printBssid(g_target_bssid);
        Serial.print(" fc=0x"); Serial.print(fc, HEX);
        Serial.print(" toDS="); Serial.print(toDS);
        Serial.print(" fromDS="); Serial.println(fromDS);
        #endif
        return;
    }

    const uint8_t* ta = addr2;
    const uint8_t* ra = addr1;

    bool is_eapol = false;
    if (type == 2 && !(fc & 0x4000)) { // Data type and not protected
        unsigned int subtype = (fc >> 4) & 0x0F;
        unsigned int mac_hdr_len = 0;
        if (subtype == 8) {
            mac_hdr_len = 26; // QoS Data
        } else if (subtype == 0) {
            mac_hdr_len = 24; // Data
        }
        if (mac_hdr_len > 0 && len >= mac_hdr_len + 8) {
            const uint8_t* llc = buf + mac_hdr_len;
            if (llc[0] == 0xAA && llc[1] == 0xAA && llc[2] == 0x03 &&
                llc[3] == 0x00 && llc[4] == 0x00 && llc[5] == 0x00 &&
                llc[6] == 0x88 && llc[7] == 0x8E) {
                is_eapol = true;
                Serial.print("[SNIFF] EAPOL Handshake Packet detected: len=");
                Serial.print(len);
                Serial.print(" TA="); printBssid(ta);
                Serial.print(" RA="); printBssid(ra);
                Serial.println();
            }
        }
    }

    bool ra_broadcast = (ra[0] == 0xFF && ra[1] == 0xFF && ra[2] == 0xFF &&
                         ra[3] == 0xFF && ra[4] == 0xFF && ra[5] == 0xFF);
    bool ra_multicast = (ra[0] & 0x01);

    // AP bridging broadcast from wired side → skip entirely
    if ((ra_broadcast || ra_multicast) && memcmp(ta, g_target_bssid, 6) == 0) {
        return;
    }

    if (toDS && !fromDS) {
        // STA→AP: TA=station, RA=BSSID
        if (memcmp(ta, g_target_bssid, 6) != 0) {
            #ifdef SNIFF_DEBUG
            Serial.print("[SNIFF] ADD TA (sta→ap)=");
            printBssid(ta);
            Serial.println();
            #endif
            addDiscoveredDevice(ta, true, is_eapol);
        }
    } else if (!toDS && fromDS) {
        // AP→STA: TA=BSSID, RA=station
        if (!ra_broadcast && memcmp(ra, g_target_bssid, 6) != 0) {
            #ifdef SNIFF_DEBUG
            Serial.print("[SNIFF] ADD RA (ap→sta)=");
            printBssid(ra);
            Serial.println();
            #endif
            addDiscoveredDevice(ra, false, is_eapol);
        }
    } else if (!toDS && !fromDS) {
        // Direct/management: TA=source, RA=destination
        if (memcmp(ta, g_target_bssid, 6) != 0) {
            #ifdef SNIFF_DEBUG
            Serial.print("[SNIFF] ADD TA (direct)=");
            printBssid(ta);
            Serial.println();
            #endif
            addDiscoveredDevice(ta, true, is_eapol);
        }
        if (!ra_broadcast && !ra_multicast && memcmp(ra, g_target_bssid, 6) != 0) {
            #ifdef SNIFF_DEBUG
            Serial.print("[SNIFF] ADD RA (direct)=");
            printBssid(ra);
            Serial.println();
            #endif
            addDiscoveredDevice(ra, false, is_eapol);
        }
    }
}

void handleDeviceScanApi(WiFiClient& client, const String& req) {
    String bssidStr = extractQueryParam(req, "bssid");
    String chStr = extractQueryParam(req, "channel");

    if (bssidStr.length() == 0 || chStr.length() == 0) {
        wifiClientSendJsonFail(client, "missing bssid or channel");
        return;
    }
    bssidStr = urlDecode(bssidStr);
    Serial.print("[SNIFF] rawBssidParam="); Serial.println(bssidStr);
    Serial.print("[SNIFF] decodedBssid="); Serial.println(bssidStr);
    if (!parseBssid(bssidStr, g_target_bssid)) {
        wifiClientSendJsonFail(client, "invalid bssid");
        return;
    }
    g_target_channel = chStr.toInt();

    Serial.print("[SNIFF] rawBssidStr="); Serial.println(bssidStr);
    Serial.print("[SNIFF] targetBytes=");
    printBssid(g_target_bssid);
    Serial.print(" channel="); Serial.println(g_target_channel);

    g_device_count = 0;
    // WiFi.disablePowerSave();

    // 必须切换到目标信道监听。通知客户端一起切换
    if (g_target_channel != ap_channel) {
        if (wifi_ap_switch_chl_and_inform(g_target_channel) != RTW_SUCCESS) {
            wifiClientSendJsonFail(client, "failed to switch channel");
            return;
        }
        wext_set_channel(WLAN0_NAME, g_target_channel);  // 必须，上面的调用不够
        ap_channel = g_target_channel;
        delay(100);
    }

    // 关闭过滤，确保收到管理帧
    // wifi_set_mgnt_rxfilter(0);
    // wifi_set_promisc_filter_reason(0);

    // 启用混杂模式
    if (wifi_set_promisc(RTW_PROMISC_ENABLE_2, deviceSniffCallback, 1) != RTW_SUCCESS) {
        wifiClientSendJsonFail(client, "failed to set promisc mode");
        return;
    }

    // 发送 SSE 头部
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/event-stream; charset=utf-8");
    client.println("Cache-Control: no-cache");
    client.println("Connection: keep-alive");
    client.println("Access-Control-Allow-Origin: *");
    client.println();
    client.flush();

    unsigned long start = millis();
    unsigned long last_send = 0;
    while (millis() - start < DEVICE_SCAN_TIMEOUT_MS) {
        // 如果客户端主动断开，提前退出
        if (!client.connected()) {
            Serial.println("[SNIFF] SSE client disconnected, aborting scan");
            break;
        }

        // 每秒推送一次当前扫描到的所有设备
        if (millis() - last_send >= 1000) {
            last_send = millis();

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
                d["packets_out"] = g_devices[i].packets_out;
                d["packets_in"] = g_devices[i].packets_in;
                d["handshakes"] = g_devices[i].handshakes;
            }

            String json;
            serializeJson(doc, json);

            client.print("data: ");
            client.print(json);
            client.print("\n\n");
        }
        delay(50);
    }

    // 发送 done 事件标识扫描完成
    if (client.connected()) {
        client.print("event: done\ndata: {}\n\n");
        client.flush();
    }

    wifi_set_promisc(RTW_PROMISC_DISABLE, NULL, 1);

    Serial.print("[SNIFF] Scan done, device_count="); Serial.println(g_device_count);
    for (int i = 0; i < g_device_count; i++) {
        Serial.print("[SNIFF]   device ");
        printBssid(g_devices[i].mac);
        Serial.print(" tx_packets="); Serial.print(g_devices[i].packets_out);
        Serial.print(" rx_packets="); Serial.print(g_devices[i].packets_in);
        Serial.print(" handshakes="); Serial.println(g_devices[i].handshakes);
    }
}
