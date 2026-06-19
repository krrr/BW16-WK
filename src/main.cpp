#include <Arduino.h>
#include <WiFi.h>
#include "webpage.h"

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

static void handleNotFound(WiFiClient& client) {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Connection: close");
    client.println();
}

// === Routing ===

static constexpr uint32_t djb2_hash(const char* s, uint32_t h = 5381) {
    return *s ? djb2_hash(s + 1, ((h << 5) + h) + (unsigned char)*s) : h;
}

static constexpr uint32_t HASH_ROOT       = djb2_hash("/");
static constexpr uint32_t HASH_INDEX_HTML = djb2_hash("/index.html");

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
    WiFi.apbegin(ap_ssid, ap_pass, ap_channel, 0);

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
