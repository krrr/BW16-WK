#pragma once
#include <Arduino.h>

bool parseMac(const String& str, uint8_t* mac);
bool isMacValidUnicast(const uint8_t* mac);
String formatMac(const uint8_t* mac);
