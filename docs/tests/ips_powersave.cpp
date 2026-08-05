#include <Arduino.h>
#include "wifi_conf.h"
#include "wifi_cust_tx.h"

// === STA PowerSave + Light Sleep + Raw Frame Test (FreeRTOS Task) ===
#ifdef __cplusplus
extern "C" {
#include "ameba_soc.h"
}
#endif

/**
 * @brief STA 模式浅度睡眠 + 周期唤醒发送 RAW 帧测试 FreeRTOS 任务
 * 
 * 测试结果：共持续170ms左右，平均电流70ma左右。发出第一个包之前的准备时间就要150ms。
 */
static void testStaSleepRawTask(void *pvParameters) {
    (void)pvParameters;
    Serial.println("\n[TEST Task] Starting STA PowerSave + Light Sleep + Raw Frame Tx Task...");

    // 1. 初始化 STA 模式（不连接任何 AP）
    wifi_on(RTW_MODE_STA);
    Serial.println("[TEST Task] Wi-Fi initialized in STA mode (unconnected)");

    // 2. 设置 Clock Gating 浅睡眠模式，并启用 Wi-Fi 省电 (IPS/LPS)
    pmu_acquire_wakelock(PMU_OS);
    pmu_set_sleep_type(SLEEP_CG); // KM4 浅度睡眠 (Clock Gating)
    if (wifi_enable_powersave() == RTW_SUCCESS) {
        Serial.println("[TEST Task] Wi-Fi power save (IPS/LPS) enabled successfully");
    } else {
        Serial.println("[TEST Task] wifi_enable_powersave failed!");
    }

    // 预定义测试用 MAC 地址
    uint8_t src_mac[6] = {0x00, 0xE0, 0x4C, 0x87, 0x00, 0x01};
    uint8_t dst_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const uint32_t SLEEP_INTERVAL_MS = 5000; // 唤醒间隔：5 秒

    while (true) {
        // 3. 设置最大休眠时间并释放 PMU_OS Wakelock 允许 CPU 进入浅度睡眠
        Serial.print("[TEST Task] Setting max sleep time: ");
        Serial.print(SLEEP_INTERVAL_MS);
        Serial.println(" ms, releasing wakelock & entering light sleep...");
        Serial.flush();

        pmu_set_max_sleep_time(SLEEP_INTERVAL_MS);
        pmu_release_wakelock(PMU_OS);

        // 进入睡眠（FreeRTOS vTaskDelay 阻塞当前任务，使得 CPU 空闲并进入 Tickless 浅度睡眠）
        vTaskDelay(pdMS_TO_TICKS(SLEEP_INTERVAL_MS));

        // 4. 醒来后重新获取 Wakelock
        pmu_acquire_wakelock(PMU_OS);
        Serial.println("[TEST Task] Woken up from light sleep!");

        // 5. 发送 RAW 802.11 帧（此处以发送 Beacon 帧为例）
        Serial.println("[TEST Task] Transmitting raw frame (Beacon)...");
        wifi_tx_beacon_frame(src_mac, dst_mac, "STA_SLEEP_TEST");
        Serial.println("[TEST Task] Raw frame sent. Returning to sleep loop...\n");
        Serial.flush();
    }
}

void startTestStaSleepRawTask() {
    if (xTaskCreate(testStaSleepRawTask, "sta_sleep_raw", 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
        Serial.println("[TEST Task] Failed to create sta_sleep_raw task!");
    } else {
        Serial.println("[TEST Task] sta_sleep_raw task created successfully.");
    }
}