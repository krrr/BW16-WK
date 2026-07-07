#include <main.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "wifi_constants.h"
#include "wifi_conf.h"
#include "webpage.h"
#include "favicon.h"
#include "utils.h"
#include "api_all.h"
#include "http/HttpServer.h"



char ap_ssid[] = "BW16-WK";
char ap_pass[] = "1234567890";
int ap_channel = 1;

HttpServer server(80);

// === Route Handlers ===

static void handleRoot(HttpClient& client) {
    client.sendHtml(webpage_gz, webpage_gz_len, true);
}

static void handleFavicon(HttpClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: image/x-icon");
    client.print("Content-Length: ");
    client.println(favicon_ico_len);
    client.println("Connection: close");
    client.println();
    client.write((const uint8_t*)favicon_ico, favicon_ico_len);
}

static void handleNotFound(HttpClient& client) {
    client.sendNotFound();
}



// === Routing ===

static constexpr uint32_t djb2_hash(const char* s, uint32_t h = 5381) {
    return *s ? djb2_hash(s + 1, ((h << 5) + h) + (unsigned char)*s) : h;
}

static constexpr uint32_t HASH_ROOT               = djb2_hash("/");
static constexpr uint32_t HASH_INDEX_HTML         = djb2_hash("/index.html");
static constexpr uint32_t HASH_FAVICON            = djb2_hash("/favicon.ico");
static constexpr uint32_t HASH_API_SCAN           = djb2_hash("/api/scan");
static constexpr uint32_t HASH_API_SCAN_DEVICES   = djb2_hash("/api/scan-devices");
static constexpr uint32_t HASH_API_STATUS         = djb2_hash("/api/status");
static constexpr uint32_t HASH_API_SET_TIME       = djb2_hash("/api/set-time");
static constexpr uint32_t HASH_API_DEAUTH         = djb2_hash("/api/deauth");
static constexpr uint32_t HASH_API_CHANGE_CHANNEL = djb2_hash("/api/change-channel");

static void dispatchRequest(HttpClient& client) {
    String path = client.path();

    switch (djb2_hash(path.c_str())) {
        case HASH_ROOT:
        case HASH_INDEX_HTML:
            handleRoot(client);
            break;
        case HASH_FAVICON:
            handleFavicon(client);
            break;
        case HASH_API_SCAN:
            handleApScanApi(client);
            break;
        case HASH_API_SCAN_DEVICES:
            handleDeviceScanApi(client);
            break;
        case HASH_API_STATUS:
            handleStatusApi(client);
            break;
        case HASH_API_SET_TIME:
            handleSetTimeApi(client);
            break;
        case HASH_API_DEAUTH:
            handleDeauthApi(client);
            break;
        case HASH_API_CHANGE_CHANNEL:
            handleChangeChannelApi(client);
            break;
        default:
            handleNotFound(client);
            break;
    }
}

void startAP(const char* ssid, const char* password, int channel) {
    // 设置AP热点
    // 本单片机有两个虚拟接口，WLAN0和WLAN1，分别用于STA和AP。但其实共享同一个 PHY，频道切换是联动的
    WiFi.enableConcurrent();
    IPAddress ip(192, 168, 4, 1);
    IPAddress gw(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gw, gw, subnet);

    char chan_char[4];
    itoa(channel, chan_char, 10);
    WiFi.apbegin((char*)ssid, (char*)password, chan_char, FALSE);
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\nBW16 WiFi Killer init ...");

    startAP(ap_ssid, ap_pass, ap_channel);

    Serial.print("SSID: ");
    Serial.println(ap_ssid);
    Serial.print("IP:   ");
    Serial.println(WiFi.localIP(1));

    server.begin();
    Serial.println("HTTP ready on port 80");
}

void loop() {
    HttpClient client = server.available();
    if (!client) return;

    if (!client.parseRequest()) {
        client.stop();
        return;
    }

    dispatchRequest(client);

    client.stop();
}
