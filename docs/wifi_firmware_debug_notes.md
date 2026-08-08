# BW16 (RTL8721D) WiFi 驱动/固件排查笔记

> 来源：SoftAP 省电（`src/ap_powersave.cpp`）挂起恢复后 beacon 不恢复、客户端
> 无法关联问题的逆向研究。所有结论均来自 lib_wlan.a 反汇编 + 实机验证。
> 配套素材：`docs/lib_wlan_dump.txt`（完整反汇编）、`docs/reverse_research_20260808.7z`。

## 1. 调试工具与素材

| 素材 | 路径 |
|---|---|
| 驱动闭源库 | `lib/ameba-arduino-d-feature-platformio-support/Arduino_package/hardware/variants/common_libs/lib_wlan.a` |
| 反汇编工具 | `C:\Users\krrr\.platformio\packages\toolchain-gccarmnoneeabi\bin\arm-none-eabi-objdump.exe` |
| 全量反汇编导出 | `docs/lib_wlan_dump.txt` |
| SDK 公开源码 | `lib/ameba-arduino-d-feature-platformio-support/Arduino_package`（驱动核心仍为闭源） |
| 完整 SoC SDK（对照） | `lib/ameba-rtos-d-main`（驱动核心同样闭源，API 层被 Realtek 打码） |

导出反汇编：

```powershell
$od = "C:\Users\krrr\.platformio\packages\toolchain-gccarmnoneeabi\bin\arm-none-eabi-objdump.exe"
& $od -t "…\lib_wlan.a"          # 符号表（含每个 .o 的符号 + 地址 + 大小）
& $od -d "…\lib_wlan.a"          # 全量反汇编
& $od -d "…\lib_wlan.a" --start-address=0x1800 --stop-address=0x1c00   # 按地址区间反汇编
```

- 库内各函数以 `bl 0 <符号名>` 占位，链接期才解析；符号表能直接给出地址/大小。
- 驱动 HAL 分发器（`rtw_hal_set_hwreg` 等）与部分 `ROM_*` 函数在 KM0 固件 ROM 里，
  lib 中只有 stub 或函数指针表项。

## 2. SoftAP 挂起/恢复 API（lib_wlan.a 导出，地址为 image2.net.ram.text 偏移）

| 函数 | 地址 | 行为 |
|---|---|---|
| `rltk_suspend_softap` | 0x1818 | ① `rtw_hal_set_hwreg(adapter, 8, &0)` ② `adapter[0x2315] = 1` ③ 条件写 `adapter[0x624] = 0` ④ 上报 custom event 0x628 |
| `rltk_suspend_softap_beacon` | 0x1888 | 同上但 flag 写 2，只停 beacon |
| `rltk_resume_softap` | 0x1a7c | ① `rtw_hal_set_hwreg(adapter, 8, &1)` ② `adapter[0x2315] = 0` ③ 条件写 `adapter[0x624] = 3` ④ 上报 custom event 0x674 |
| `rltk_remove_softap_in_concurrent_mode` | 0x18e0 | 完整拆除：`_rtw_pwr_wakeup(2000)` → `stop_ap_mode` → `set_opmode` → msleep(50) → flag=1 |
| `rltk_reattach_softap_in_concurrent_mode` | 0x1af0 | `rltk_wlan_init(1)` + `rltk_wlan_start(1)` —— **接口已打开时会无限递归 `_netdev_open` 栈溢出，勿用** |
| `ROM_WIFI_RESUME_TxBeacon` | .text | 4 条指令 stub：清 0x40080522 bit6（beacon 主开关） |
| `ROM_WIFI_STOP_TXBeacon` | .text | 置 0x40080522 bit6 |
| `promisc_resume_tx_beacn` | .text | 混杂模式退出恢复 beacon：`adapter[0x624]=3` + `Set_MSR(3)` + `update_beacon`。**操作的是 wlan0(STA) 口**，不适合 wlan1-AP 场景（会把 STA 口 MSR 改成 AP 角色） |

关键结论：

- **`rltk_resume_softap` 只做"取消挂起"**：重开 beacon 硬件开关、清挂起标志、上报事件。
  它**不会**把 beacon 帧重新下发给固件，也**不会**重建 WPA2/PSK 上下文。
- custom event 0x628/0x674 在 Arduino SDK 的 `wext_wlan_indicate` 里没有对应处理分支（default: break），纯通知性质。
- Realtek 从未在公开 SDK 提供过完整的 resume API（上游 ameba-rtos-d 仓库连 `wifi_suspend_softap` 符号都被打码成 `n`）。

## 3. 驱动 adapter 结构关键偏移（RTL8721D）

| 偏移 | 含义 | 备注 |
|---|---|---|
| `+0x10` | 状态标志字 | bit4 是"AP 模式"位；`rltk_suspend/resume_softap` 入口检查它，没置位直接返回 -1 |
| `+0x2315` | softap 挂起标志字节 | **1=挂起**：`validate_recv_frame` 丢弃所有 RX 帧（probe/auth 都会收不到）；2=silent-table 模式（允许 assoc）；0=正常。`rltk_wlan_running()` 见它非 0 也返回 0 |
| `+0x624` | AP 状态字 | suspend=0 / resume=3；`setopmode_hdl`：AP=3、STA=`(old&~3)\|2`；`rtl8721d_var_set_correct_tsf` 用 bit0 与 `&3==2` 决定 TSF 校正时 STOP/RESUME TxBeacon |
| `+0x860` | AP BSS active 标志 | **`update_beacon()` 入口检查，为 0 直接 return**；`start_bss_network` 置 1，`start_ap_mode`/`stop_ap_mode` 清 0 |
| `+0x2154` | 通用 set_hwreg 分发函数指针 | `rtw_hal_set_hwreg` 读 `[adapter+0x2154]` 调用（var==7 特判走 `FillH2CCmd8721D` H2C） |
| `+0x22f9` | ioctl 门控标志 | 见下 |
| `+0x1be0+0x18` | pwrctrl_priv 基址 | `[+0x20]` 与 0x80000000 哨兵比较，见下 |

### rltk_wlan_control 的 ioctl 门控（重要）

`rltk_wlan_control`（所有 wext ioctl 的入口）在 `adapter[0x22f9] != 0` 时：
只放行 `0x89f0~0x89ff` 与 `0x8b00~0x8bff` 区间的命令，其余返回 -1；
若 pwrctrl `[+0x20] == 0x80000000`（哨兵），再收窄为 0x8b06(SIWMODE)/0x8b1a(SIWESSID)
等掩码 0x00144001 命中的子集（bit0/12/14/20）。

→ 排查"ioctl 静默失败"时，先怀疑这个门控。

## 4. Beacon 挂载链路（本次问题核心）

```
wext_set_mode(WLAN1, RTW_MODE_MASTER)
  └→ rtw_ioctl(0x8b06) → rtw_setopmode_cmd → setopmode_hdl
       └→ adapter[0x624] = 3
       └→ rtw_hal_set_hwreg(adapter, 2, 3) → rtl8721d_var_set_opmode
            ├→ ROM_WIFI_STOP_TXBeacon（置 0x40080522 bit6）
            ├→ 写寄存器 0x550/0x551/0x102/0x558/0x5b2
            ├→ UpdateInterruptMask8721d
            ├→ ROM_WIFI_SetOpmodeAP（固件切 AP 角色，握手/beacon 的固件状态机靠它）
            └→ ROM_WIFI_RESUME_TxBeacon（清 0x40080522 bit6，真正开播 beacon）
```

配套：

```
wext_set_ap_ssid → rtw_wx_set_ap_essid
  └→ rtw_generate_bcn_ie（重建 beacon IE）
  └→ rtw_check_beacon_data
  └→ rtw_hal_set_hwreg(adapter, 80, …)（beacon 内容推给固件）
  └→ psk_init（重建 WPA2 PSK 上下文）
```

- **首次上电**由 `wifi_start_ap` 的 `wext_set_mode(WLAN1, MASTER)` 触发挂载。
- **恢复后直接再调 MASTER 无效**：驱动已处于 MASTER 模式，认为无变化不重挂。
  必须先 `wext_set_mode(WLAN1, RTW_MODE_INFRA)` 切走（走 stop_ap_mode 清理），
  再切回 MASTER 才会真正重新挂载。
- `wext_set_ap_ssid` 无"ssid 相同跳过"逻辑，每次都走完整 regenerate 流程，可安全重复调用。
- "set pairwise key to hw" / "set group key to hw" 驱动日志 = 客户端 4 次握手完成的标志。

## 5. IPS（Inactive Power Save）与电源

- `ips_enter`：pwrctrl 状态机 + `rtw_hal_set_hwreg(60, …)` 等，RF 下电。
- `ips_leave`：**`rtw_reset_drv_sw` + `rtw_hal_init`**（等同重新初始化 HAL）+ 恢复信道
  （`set_channel_bwmode`）+ 重装组密钥（`rtw_set_key` keyidx 0~3）。
  **但不会重发 beacon** —— 固件侧 beacon 缓冲若被 IPS 下电清掉，无人补。
- 顺序坑：`wifi_resume_powersave()`（rtw_pm_set IPS=3/LPS=9）只是切回模式参数，
  不会做完整 IPS leave；紧接着调 `rltk_resume_softap` 的 H2C 会卡在唤醒流程。
  **必须先 `wifi_disable_powersave()`（走 `_rtw_pwr_wakeup` 真正拉起 RF）再 resume。**

## 6. 诊断日志特征

| 场景 | 日志 |
|---|---|
| 正常关联 | `+OnAuth` → `+OnAssocReq` → `rtw_ap_update_sta_ra_info` → **`set pairwise key to hw`** → **`set group key to hw`** |
| 恢复后 AP 半死 | `+OnAuth`/`+OnAssocReq` 能收到、ra_info 正常，但**没有 key 两行**，随后 `ap recv deauth reason code(4)`（客户端收不到 beacon/握手超时放弃） |

特征总结：**RX 正常（能收 probe/auth/assoc）+ TX 周期任务死（无 beacon）+ 握手不启动** →
基本可断定固件侧 AP 状态机/beacon 缓冲失效，需要重挂（见 §7），而不是驱动配置问题。

### beacon 挂载成功标志

```
wlan1: 1 DL RSVD page success! DLBcnCount:01, poll:00000001
```

- 出处：`rtl8721d_download_rsvd_page`（rtl8721d_cmd.o）：驱动把 **DL（下行）保留页**——
  beacon、PS-Poll、Null 帧——构造好后经 H2C 下载进固件 SRAM 指定页，固件回报成功。
- `DLBcnCount:01` = 固件为 beacon 分配的保留页数（存在 pwrctrl_priv+180）；
  `poll:00000001` = PS-Poll 保留页号/索引。
- **开机挂载 AP 和恢复后重挂 beacon（§7 的模式翻转）都会打这条**——恢复日志里看到它
  即代表重挂流程真正执行到了固件侧。若出现 `failed!` 变体才是异常（beacon 页下载失败 → 无 beacon）。
- 打印前还会检查 `adapter[0x21d0]/[0x21d4]`（IPS 状态标志），IPS 切换期间会跳过打印。

## 7. 最终有效恢复方案（ap_powersave.cpp 现行）

```c
wifi_disable_powersave();            // 必须：完整 IPS leave，RF 真正拉起
rltk_resume_softap(WLAN1_NAME);      // 必须：清挂起标志(0x2315)，否则 RX 全丢
wext_set_mode(WLAN1_NAME, RTW_MODE_INFRA);   // ★ 核心：先切走
wext_set_mode(WLAN1_NAME, RTW_MODE_MASTER);  // ★ 核心：重新挂载 beacon + 固件 AP 角色
// 以下可选（驱动侧配置挂起后仍在，但 INFRA 清理可能清掉 ssid，建议保留）：
wext_set_auth_param(WLAN1_NAME, RTW_AUTH_80211_AUTH_ALG, RTW_AUTH_ALG_OPEN_SYSTEM);
wext_set_key_ext(WLAN1_NAME, RTW_ENCODE_ALG_CCMP, NULL, 0, 0, 0, 0, NULL, 0);
wext_set_passphrase(WLAN1_NAME, pass, strlen(pass));
wext_set_ap_ssid(WLAN1_NAME, ssid, strlen(ssid));
wext_set_channel(WLAN1_NAME, ap_channel);
```

- 已实机确认：模式翻转是有效关键；`ROM_WIFI_RESUME_TxBeacon()` 兜底可删（翻转内部已覆盖）。
- `WiFi.apbegin` 恢复时"假死 20s"是 apActivate 的 ssid 轮询超时（应用层），不是驱动卡死。

## 8. SDK 已知的坑

1. **Symbol 打码**：上游 SDK 把 `suspend_softap`、`iw_ioctl` 等名字替换成 `n`/`ln`
   （例如 ameba-rtos-d 的 wifi_conf.c 里 `void n(void)` 就是 `wifi_suspend_softap`），
   本地 fork 的部分文件也有残留，引用 API 前先查符号表。
2. **`rltk_reattach_softap_in_concurrent_mode` 会栈溢出崩溃**（接口已打开时 `rltk_wlan_start(1)`
   无限递归 `_netdev_open`/`_netdev_if2_open`），任何情况下都不要用。
3. **`wext_wlan_indicate` 不处理 suspend/resume custom event**，别指望上层回调。
4. **`promisc_resume_tx_beacn` 固定操作 wlan0 口**，在 AP-on-wlan1 并发模式下调用会
   把 STA 口 MSR 设成 AP 角色，破坏 STA 口状态。
5. `wifi_set_mode` 的 AP→AP / AP→STA 转换内部是 `wext_set_mode(WLAN0, RTW_MODE_INFRA)`，
   说明"AP 口切 INFRA 再切回"是 SDK 自己认可的拆除方式（本次方案的理论依据）。
