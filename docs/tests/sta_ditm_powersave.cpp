/*
 * sta_powersave.cpp
 *
 * STA 模式省电休眠测试 (LPS - Leisure Power Save + DTIM + PMU Tickless)
 *
 * 流程：
 *   1. 使用 WiFi.begin() 初始化 Wi-Fi STA 模式，连接指定 AP (支持 WPA2/WPA3 自动协商) 并获取 DHCP IP
 *   2. wifi_enable_powersave() 启用省电模式（IPS + LPS）
 *   3. wifi_set_lps_dtim() 设置 LPS 模式下的 DTIM 周期（例如 3/5），并通过 wifi_get_lps_dtim() 验证
 *   4. 配置 KM4 Clock Gating 浅睡眠（SLEEP_CG），不进 Deep Sleep
 *   5. 无限循环中释放 PMU_OS wakelock 并休眠，
 *      配合 FreeRTOS tickless 让主 CPU 在空闲时进入浅睡眠
 *   6. 醒来后发送 UDP 广播报文，验证网络接口及发包功能
 */

#include "sta_powersave.h"
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "wifi_conf.h"

// 目标 AP 的 SSID 和密码
#define STA_SSID "Re2.4"
#define STA_PASS "hajimaru1001"

// 目标 DTIM 设置（默认为 3，即每 3 个 DTIM 周期唤醒一次，节省更多 RF 功耗）
#define TARGET_DTIM 3

#ifdef __cplusplus
extern "C" {
#include "ameba_soc.h"
}
#endif

// ============================================================
//  staPowerSaveTask  (FreeRTOS 任务)
// ============================================================
static void staPowerSaveTask(void *pvParameters) {
    (void)pvParameters;

    Serial.println("[STA_PS] STA DTIM PowerSave Task started");

    // --- 阶段一：初始化 Wi-Fi 驱动、连接 AP 并获取 DHCP IP ---
    Serial.print("[STA_PS] Connecting to AP: ");
    Serial.println(STA_SSID);

    int status = WiFi.begin((char*)STA_SSID, (char*)STA_PASS);
    Serial.print("[STA_PS] WiFi.begin initial status: ");
    Serial.println(status);

    uint32_t start_time = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (millis() - start_time > 10000) {
            Serial.println("\n[STA_PS] Retrying WiFi.begin...");
            WiFi.begin((char*)STA_SSID, (char*)STA_PASS);
            start_time = millis();
        }
    }

    Serial.println("\n[STA_PS] Connected to AP successfully!");
    Serial.print("[STA_PS] Local IP: ");
    Serial.println(WiFi.localIP());

    // --- 阶段二：配置 Wi-Fi 省电模式 (LPS/IPS) 与 DTIM 周期 ---
    if (wifi_enable_powersave() != RTW_SUCCESS) {
        Serial.println("[STA_PS] wifi_enable_powersave() failed!");
    } else {
        Serial.println("[STA_PS] Wi-Fi powersave (IPS+LPS) enabled successfully.");
    }

    // 设置 LPS DTIM 唤醒周期倍率
    if (wifi_set_lps_dtim(TARGET_DTIM) == RTW_SUCCESS) {
        unsigned char current_dtim = 0;
        wifi_get_lps_dtim(&current_dtim);
        DiagPrintf("[STA_PS] LPS DTIM set to %d (current active DTIM: %d)\r\n", TARGET_DTIM, current_dtim);
    } else {
        Serial.println("[STA_PS] wifi_set_lps_dtim() failed!");
    }

    // --- 阶段三：配置 PMU 浅休眠 (KM4 SLEEP_CG) ---
    pmu_acquire_wakelock(PMU_OS);
    pmu_set_sleep_type(SLEEP_CG);
    Serial.println("[STA_PS] PMU Sleep type set to SLEEP_CG (Clock Gating)");

    // --- UDP 广播套接字：每次醒来发一包，让 WiFi 实际发送一次 ---
    WiFiUDP Udp;
    Udp.begin(8888);

    // --- 阶段四：循环释放锁休眠 ---
    const uint32_t SLEEP_INTERVAL_MS = 2000;
    uint32_t loopCount = 0;

    Serial.println("[STA_PS] Entering STA DTIM sleep loop...");
    while (true) {
        // 设置 FreeRTOS tickless 最大可休眠时长并释放 PMU_OS Wakelock
        pmu_set_max_sleep_time(SLEEP_INTERVAL_MS);
        pmu_release_wakelock(PMU_OS);

        // 进入休眠：FreeRTOS vTaskDelay 挂起当前任务，触发 FreeRTOS Tickless Idle
        vTaskDelay(pdMS_TO_TICKS(SLEEP_INTERVAL_MS));

        // 唤醒后重新获取 Wakelock
        pmu_acquire_wakelock(PMU_OS);

        // 醒来后发一包 UDP 广播，触发 WiFi 实际发包
        Udp.beginPacket(IPAddress(255, 255, 255, 255), 8888);
        Udp.write("wake_dtim");
        int sent = Udp.endPacket();

        if (loopCount <= 5 || loopCount % 10 == 0) {
            DiagPrintf("[STA_PS] Loop #%d wake packet sent: %d\r\n", (int)loopCount, sent);
        }

        loopCount++;
    }
}

// ============================================================
//  对外入口：创建 FreeRTOS 测试任务
// ============================================================
void startStaPowerSaveTask() {
    Serial.println("[STA_PS] Creating FreeRTOS task ...");
    xTaskCreate(staPowerSaveTask, "sta_powersave", 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
    Serial.println("[STA_PS] Task created");
}
