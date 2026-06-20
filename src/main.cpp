#include <Arduino.h>
#include <WiFi.h>
#include "webpage.h"
#include "favicon.h"

extern void handleScanApi(WiFiClient& client);
static void handleStatusApi(WiFiClient& client);

char ap_ssid[] = "BW16-WK";
char ap_pass[] = "1234567890";
char ap_channel[] = "1";

WiFiServer server(80);

// === Route Handlers ===

static void handleRoot(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=utf-8");
    client.println("Content-Encoding: gzip");
    client.print("Content-Length: ");
    client.println(webpage_gz_len);
    client.println("Connection: close");
    client.println();
    client.write((const uint8_t*)webpage_gz, webpage_gz_len);
}

static void handleFavicon(WiFiClient& client) {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: image/x-icon");
    client.print("Content-Length: ");
    client.println(favicon_ico_len);
    client.println("Connection: close");
    client.println();
    client.write((const uint8_t*)favicon_ico, favicon_ico_len);
}

static void handleNotFound(WiFiClient& client) {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Connection: close");
    client.println();
}

static void handleStatusApi(WiFiClient& client) {
    unsigned long uptime = millis() / 1000;
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print("{\"uptime\":");
    client.print(uptime);
    client.println("}");
}

// === Routing ===

static constexpr uint32_t djb2_hash(const char* s, uint32_t h = 5381) {
    return *s ? djb2_hash(s + 1, ((h << 5) + h) + (unsigned char)*s) : h;
}

static constexpr uint32_t HASH_ROOT       = djb2_hash("/");
static constexpr uint32_t HASH_INDEX_HTML = djb2_hash("/index.html");
static constexpr uint32_t HASH_FAVICON    = djb2_hash("/favicon.ico");
static constexpr uint32_t HASH_API_SCAN   = djb2_hash("/api/scan");
static constexpr uint32_t HASH_API_STATUS = djb2_hash("/api/status");

static void dispatchRequest(WiFiClient& client, const String& req) {
    int s = req.indexOf(' ') + 1;
    int e = req.indexOf(' ', s);
    if (s < 1 || e < 0) { handleNotFound(client); return; }
    String path = req.substring(s, e);

    switch (djb2_hash(path.c_str())) {
        case HASH_ROOT:
        case HASH_INDEX_HTML:
            handleRoot(client);
            break;
        case HASH_FAVICON:
            handleFavicon(client);
            break;
        case HASH_API_SCAN:
            handleScanApi(client);
            break;
        case HASH_API_STATUS:
            handleStatusApi(client);
            break;
        default:
            handleNotFound(client);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\nBW16 WiFi AP + Web Server");

    IPAddress ip(192, 168, 4, 1);
    IPAddress gw(192, 168, 4, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gw, gw, subnet);
    WiFi.enableConcurrent();
    WiFi.apbegin(ap_ssid, ap_pass, ap_channel, FALSE);

    Serial.print("SSID: ");
    Serial.println(ap_ssid);
    Serial.print("IP:   ");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("HTTP ready on port 80");
}

void loop() {
    WiFiClient client = server.available();
    if (!client) return;

    unsigned long timeout = millis() + 2000;
    while (!client.available() && millis() < timeout) delay(1);
    if (!client.available()) { client.stop(); return; }

    String req = client.readStringUntil('\r');
    client.read();

    while (client.available()) {
        String line = client.readStringUntil('\n');
        if (line.length() <= 1) break;
    }

    dispatchRequest(client, req);

    client.flush();
    client.stop();
}
