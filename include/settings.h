#pragma once
#include <Arduino.h>

#define SETTINGS_MAGIC 0x574B3031  // "WK01"
#define FLASH_MEMORY_APP_BASE 0x00200000  // 4MB flash

#pragma pack(push, 4)
struct BeaconTimeRecord {
    uint8_t bssid[6];        // AP MAC address
    uint8_t channel;         // AP Wi-Fi channel
    uint8_t reserved;        // Alignment padding
    uint64_t saved_tsf;      // 64-bit Beacon TSF (microseconds)
    int64_t time_offset_ms;  // RealTime_ms - (TSF / 1000)
};

struct AppSettings {
    uint32_t magic;         // Magic number (0x574B3031)
    uint32_t struct_size;   // Struct size for forward/backward compatibility
    char ap_ssid[33];       // SoftAP SSID (max 32 chars + null terminator)
    char ap_pass[33];       // SoftAP Password (max 32 chars + null terminator)
    uint8_t ap_mac[6];      // Custom SoftAP MAC (6 bytes binary, all 0s = default)
    uint8_t enable_beacon_time_sync; // 0 = disabled (default), 1 = enabled
    uint8_t beacon_record_count;     // Number of valid beacon records (0..4)
    uint8_t reserved[2];             // Alignment padding to 4-byte boundary
    BeaconTimeRecord beacon_records[4];

    // === AP 占空比省电 & 时段调度 ===
    uint8_t ap_powersave_enable; // 0 = disabled (default), 1 = enabled
    uint8_t schedule_enable;     // 0 = disabled, 1 = enabled
    uint16_t duty_period_sec;    // 占空比周期（秒），默认 120
    uint16_t duty_on_sec;        // 每周期 AP 开启时长（秒），默认 10
    uint16_t client_hold_sec;    // 客户端全部断开后的保持宽限（秒），默认 15
    uint32_t schedule_hours_mask;// bit0..23 = 0..23 点是否允许开启 AP
    uint8_t reserved2[4];        // Alignment padding
    uint32_t checksum;      // CRC-32 checksum at the end
};
#pragma pack(pop)

static_assert(offsetof(AppSettings, checksum) + sizeof(uint32_t) == sizeof(AppSettings), "AppSettings checksum must be at the very end with no trailing padding!");
static_assert(sizeof(AppSettings) <= 8192, "AppSettings size exceeds 8KB limit!");

extern AppSettings g_appSettings;

uint32_t calculate_crc32(const uint8_t* data, size_t length);
void loadSettings();
bool saveSettings();
