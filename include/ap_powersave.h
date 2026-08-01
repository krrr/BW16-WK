#pragma once
#include <Arduino.h>

/**
 * @file ap_powersave.h
 * @brief SoftAP 占空比省电 + 按小时时段调度
 *
 * 实现思路（避免 wifi_off 的驱动级重建）：
 * - 暂停：wext_suspend_softap(WLAN1_NAME) 挂起 softAP（先断开已关联客户端）
 * - 恢复：rltk_resume_softap(WLAN1_NAME)
 * - 省电：初始化时调用一次 wifi_enable_powersave()（IPS+LPS），
 *   softAP 挂起后驱动会自动关 RF，空闲任务进入 tickless 浅睡眠（KM4/KM0）。
 */

enum class ApPowerSaveState : uint8_t {
    DISABLED = 0,   // 功能关闭，AP 常开
    AP_ON,          // AP 开启窗口（含客户端保持 HOLD）
    DUTY_SLEEP,     // 占空比休眠：softAP 挂起，等待下个周期起点
    SCHEDULE_OFF,   // 非允许时段：softAP 挂起，等待下一个允许小时
};

// setup() 中调用：初始化 powersave（wifi_enable_powersave）
void apPowerSaveInit();

// loop() 中调用：驱动状态机
void apPowerSaveTick();

// 异步任务（如 beacon 抓取）占用射频期间禁止休眠
void apPowerSaveHold();
void apPowerSaveRelease();

// 当前关联客户端数，查询失败返回 -1
int apPowerSaveClientCount();

ApPowerSaveState apPowerSaveGetState();
bool apPowerSaveIsSuspended();   // softAP 当前是否处于挂起
const char* apPowerSaveStateName();
uint32_t apPowerSaveNextOnInSec(); // 距离下次 AP 开启的秒数（0 = 已开启/未启用）
