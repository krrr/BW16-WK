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
