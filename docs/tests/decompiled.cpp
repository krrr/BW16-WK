#include <string.h>

extern void *rltk_wlan_info[];
extern unsigned char rtw_powersave_in_STA_AP_mode_enabled;

/* SDK 内部 API 声明 */
extern int _memcmp(const void *s1, const void *s2, size_t n);
extern void _rtl_printf(const char *fmt, ...);
extern void rtw_msleep_os(int ms);
extern void cancel_timer_ex(void *timer);
extern void mod_timer(void *timer, int ms);
extern int  rltk_suspend_softap(char *ifname);
extern void rtw_suspend_cmd_thread(void *padapter);
extern void rtw_resume_cmd_thread(void *padapter);
extern int  rltk_wlan_reinit_drv_sw(char *ifname, int mode);

/**
 * @brief 模式切换【预处理】：Suspend 阶段
 */
int rltk_set_mode_prehandle(int curr_mode, int next_mode, char *ifname)
{
    unsigned int idx = (_memcmp(ifname, "wlan0", 5) != 0) ? 28 : 0;
    void *pnetdev = *(void **)((unsigned char *)rltk_wlan_info + idx);
    
    if (!pnetdev) return -1;
    void *padapter = *(void **)(*(void **)((unsigned char *)pnetdev + 16));
    if (!padapter) return -1;

    // 当从 AP 模式 (2) 切换到 STA (1) 或 Concurrent (4) 时
    if (curr_mode == 2 && (next_mode == 1 || next_mode == 4)) {
        // Step 1. 取消当前软 AP 的相关定时器
        cancel_timer_ex((unsigned char *)padapter + 1180); // offset 0x49c

        // Step 2. 挂起 SoftAP 广播与接收
        if (rltk_suspend_softap(ifname) < 0) {
            _rtl_printf("[%s] suspend softap failed\n", __FUNCTION__);
            return -1;
        }

        // Step 3. 延时 50ms 确保剩余报文收发完成
        rtw_msleep_os(50);

        // Step 4. 挂起 Wi-Fi 命令线程，为硬件重置做准备
        rtw_suspend_cmd_thread(padapter);
        return 0;
    }

    return 0;
}

/**
 * @brief 模式切换【后处理】：Resume 与驱动拉起阶段
 */
int rltk_set_mode_posthandle(int curr_mode, int next_mode, char *ifname)
{
    unsigned int idx = (_memcmp(ifname, "wlan0", 5) != 0) ? 28 : 0;
    void *pnetdev = *(void **)((unsigned char *)rltk_wlan_info + idx);

    if (!pnetdev) return -1;
    void *padapter = *(void **)(*(void **)((unsigned char *)pnetdev + 16));
    if (!padapter) return -1;

    // 模式切换完成后的恢复逻辑
    if (curr_mode == 2 && (next_mode == 1 || next_mode == 4)) {
        // Step 1. 重新初始化软件驱动层状态以适应新模式
        int ret = rltk_wlan_reinit_drv_sw(ifname, next_mode);

        // Step 2. 恢复 Wi-Fi 命令线程
        rtw_resume_cmd_thread(padapter);

        // Step 3. 若开启了省电共存模式，重新启动 2 秒控制定时器
        if (rtw_powersave_in_STA_AP_mode_enabled != 0) {
            mod_timer((unsigned char *)padapter + 1180, 2000); // 2000ms
        }
        return ret;
    }

    return 0;
}






/* 相关外部变量与结构体前置声明 */
extern void *rltk_wlan_info[];
extern unsigned char rtw_powersave_in_STA_AP_mode_enabled;

/* HWReg 宏定义/枚举 (8 对应 HW_VAR_SUSPEND_SOFTAP / HW_VAR_SET_SOFTAP_SUSPEND) */
#define HW_VAR_SUSPEND_SOFTAP 8

/* 外部 API */
extern int _memcmp(const void *s1, const void *s2, size_t n);
extern void rtw_hal_set_hwreg(void *padapter, unsigned int variable, unsigned char *val);
extern void indicate_wx_custom_event(void *padapter, char *event_str);

/**
 * @brief 暂停 SoftAP
 * @param ifname 接口名称 (如 "wlan0" 或 "wlan1")
 * @return int 0 表示成功，-1 表示失败
 */
int rltk_suspend_softap(char *ifname)
{
    unsigned char val = 0;
    unsigned int idx;
    void *pnetdev;
    void *priv;
    void *padapter;
    unsigned int driver_flags;

    /* 1. 根据网卡名称计算在 rltk_wlan_info 中的偏移 */
    if (_memcmp(ifname, "wlan0", 5) != 0) {
        idx = 28; // 0x1c: wlan1 接口信息偏移
    } else {
        idx = 0;  // 0x00: wlan0 接口信息偏移
    }

    /* 2. 获取 net_device 及对应的 padapter 指针 */
    pnetdev = *(void **)((unsigned char *)rltk_wlan_info + idx);
    priv = *(void **)((unsigned char *)pnetdev + 16);    // pnetdev->priv (offset 0x10)
    padapter = *(void **)priv;                           // pnetdev_priv->padapter (offset 0x00)

    /* 3. 检查适配器驱动状态标志位 (bit 4, 0x10) */
    driver_flags = *(unsigned int *)((unsigned char *)padapter + 16);
    if ((driver_flags & 0x10) == 0) {
        return -1; // -1 (_FAIL)
    }

    /* 4. 调用 HAL 层关闭 SoftAP 硬件配置 (传递 val = 0) */
    val = 0;
    rtw_hal_set_hwreg(padapter, HW_VAR_SUSPEND_SOFTAP, &val);

    /* 5. 设置 padapter 内的 suspend_softap 状态标志为 1 */
    *(unsigned char *)((unsigned char *)padapter + 8981) = 1; // offset 0x2315

    /* 6. 若开启了 STA+AP 模式下的省电模式，重置 Beacon 更新标志 */
    if (rtw_powersave_in_STA_AP_mode_enabled != 0) {
        *(unsigned int *)((unsigned char *)padapter + 1572) = 0; // offset 0x624
    }

    /* 7. 上报 "Softap Stop" 自定义事件通知 */
    indicate_wx_custom_event(padapter, "Softap Stop");

    return 0; // 成功
}


/**
 * @brief 恢复 SoftAP 广播与硬件接收
 * @param ifname 接口名称 (如 "wlan0")
 * @return int 0 表示成功，-1 表示失败
 */
int rltk_resume_softap(char *ifname)
{
    unsigned char val = 1; // 1 表示使能/恢复 SoftAP
    unsigned int idx;
    void *pnetdev;
    void *priv;
    void *padapter;
    unsigned int driver_flags;
    /* 1. 解析网卡接口偏移 */
    if (_memcmp(ifname, "wlan0", 5) != 0) {
        idx = 28; // wlan1
    } else {
        idx = 0;  // wlan0
    }
    /* 2. 获取 padapter 结构体指针 */
    pnetdev = *(void **)((unsigned char *)rltk_wlan_info + idx);
    priv = *(void **)((unsigned char *)pnetdev + 16);
    padapter = *(void **)priv;
    /* 3. 校验驱动状态 */
    driver_flags = *(unsigned int *)((unsigned char *)padapter + 16);
    if ((driver_flags & 0x10) == 0) {
        return -1; // 驱动未就绪，返回 -1
    }
    /* 4. 通知 HAL 层重新开启 SoftAP 硬件 Beacon 广播 (传递 val = 1) */
    val = 1;
    rtw_hal_set_hwreg(padapter, HW_VAR_SUSPEND_SOFTAP, &val);
    /* 5. 清零 suspend_softap 挂起标志 */
    *(unsigned char *)((unsigned char *)padapter + 8981) = 0; // offset 0x2315 = 0
    /* 6. 若开启了 STA+AP 省电模式，置位 beacon_updated = 3 强制刷新并重建 Beacon 帧 */
    if (rtw_powersave_in_STA_AP_mode_enabled != 0) {
        *(unsigned int *)((unsigned char *)padapter + 1572) = 3; // offset 0x624 = 3
    }
    /* 7. 上报 "Softap Start" 自定义事件通知系统 */
    indicate_wx_custom_event(padapter, "Softap Start");
    return 0; // 成功
}