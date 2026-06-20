#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "utils.h"

void wifiClientSendJson(WiFiClient& client, JsonDocument& doc) {
    String json;
    serializeJson(doc, json);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json; charset=utf-8");
    client.print("Content-Length: ");
    client.println(json.length());
    client.println("Connection: close");
    client.println();
    client.print(json);
}

char hexNibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

String urlDecode(const String& s) {
    String out;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '%' && i + 2 < (int)s.length()) {
            out += (char)(hexNibble(s[i+1]) << 4 | hexNibble(s[i+2]));
            i += 2;
        } else if (s[i] == '+') {
            out += ' ';
        } else {
            out += s[i];
        }
    }
    return out;
}

String extractQueryParam(const String& req, const String& param) {
    int qm = req.indexOf('?');
    if (qm < 0) return "";
    int sp = req.indexOf(' ', qm);
    if (sp < 0) return "";
    String qs = req.substring(qm + 1, sp);
    int p = qs.indexOf(param + "=");
    if (p < 0) return "";
    int vstart = p + param.length() + 1;
    int vend = qs.indexOf('&', vstart);
    if (vend < 0) vend = qs.length();
    return qs.substring(vstart, vend);
}
