#pragma once
#include <Arduino.h>

#define SETTINGS_MAGIC 0x574B3031  // "WK01"
#define FLASH_MEMORY_APP_BASE 0x00200000  // 4MB flash

struct AppSettings {
    uint32_t magic;         // Magic number (0x574B3031)
    uint32_t struct_size;   // Struct size for forward/backward compatibility
    char ap_ssid[33];       // SoftAP SSID (max 32 chars + null terminator)
    char ap_pass[33];       // SoftAP Password (max 32 chars + null terminator)
    uint8_t ap_mac[6];      // Custom SoftAP MAC (6 bytes binary, all 0s = default)
    uint32_t checksum;      // CRC-32 checksum
};

static_assert(sizeof(AppSettings) <= 8192, "AppSettings size exceeds 8KB limit!");

extern AppSettings g_appSettings;

uint32_t calculate_crc32(const uint8_t* data, size_t length);
void loadSettings();
bool saveSettings();
