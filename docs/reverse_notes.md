# WiFi 驱动逆向研究笔记

> 用途：下一次会话能照着这里快速继续分析 RTL8721D/BW16 的 `lib_wlan` 闭源驱动。
> 最终目标：未关联STA情况下间歇性发raw帧达到连了AP时类似LPS省电的效果：浅度睡眠几乎完全关闭射频，醒来发包恢复射频极快（30ms）不需要像IPS那样每次睡醒完整初始化射频（100+ms）。

## 1. 背景

BW16 = Ai-Thinker RTL8721D（Realtek AmebaD）。主核 KM4 + 独立 WiFi 子系统。
WiFi 核心驱动 **闭源**，以预编译库随 Arduino/PlatformIO SDK 发布。
驱动在 `Arduino_package/hardware/system/component/common/drivers/wlan/` 下只有少量 `.c`，
真正的实现（`rtw_pwrctrl.c`、`rtl8721d_hal_init.c`、`rtw_mlme.c` 等）都是二进制。

因此研究手段 = **对预编译库做反汇编**。产物已放在仓库 `reverse/`。

## 2. 环境与工具

- ARM objdump（PlatformIO 自带）：
  `C:\Users\krrr\.platformio\packages\toolchain-gccarmnoneeabi\bin\arm-none-eabi-objdump.exe`
- 用法：
  ```powershell
  $od="C:\Users\krrr\.platformio\packages\toolchain-gccarmnoneeabi\bin\arm-none-eabi-objdump.exe"
  & $od -t "reverse\libwlan\rtw_pwrctrl.o"      # 符号表
  & $od -d "reverse\libwlan\rtw_pwrctrl.o"      # 反汇编
  ```
- 已整理的反汇编 `.txt` 放在 `reverse/*_dump.txt`（命名与既有 `hal_intf_dump.txt` 一致）。
- 使用电流计可以测量单片机实时电流 `scripts\iot_power_reader_skill\SKILL.md`

### 2.1 原始固件镜像（km0_image2_all.bin）逆向经验

Ghidra 脚本统一放在 `reverse/ghidra_scripts/`：
- `MakeFuncs2.java` — 给无 xref 目标地址手动建函数（改 `addrs` 数组后跑）
- `DecompileAll.java` — 全量反编译落盘 `decompiled.txt`（可用 `KM0_DECOMP_OUT` 改路径）
- `ScanStrings.java` / `ListFuncs.java` / `FindRefs.java` / `Inspect.java` / `Decompile.java`

这轮从零逆向裸固件，沉淀了 5 条可直接复用的硬经验：

1. **裸二进制 + Thumb 反汇编**：原始 `.bin` 无 ELF 头，objdump 无法自动判定 Thumb。
   用 `arm-none-eabi-objcopy -O elf32-littlearm -B armv5te -I binary` 转 ELF，
   再 `objdump -d -M force-thumb` 才能正确反汇编（movw/movt 出现即证 Thumb-2）。
2. **Ghidra 导入裸二进制**：语言**必须**用 `ARM:LE:32:v8-m`（`Cortex_M23` 报"Unsupported
   language"）；`-loader BinaryLoader -loader-baseAddr 0x0C000000` 把基址设在镜像
   加载地址上，Ghidra 才能直接解析 movw/movt 产生的绝对地址（如 0x00084990）。
3. **绝对地址换算**：镜像头（32B）给出加载地址 `0x0C000020` → 载荷基址 `0x0C000000`；
   `函数绝对地址 = payload偏移 + 0x0C000000`。找目标函数先按 32 位对齐搜
   `payload_off + 0x0C000000` 的字模式。
4. **Ghidra 建函数遇 "Function body must contain the entrypoint"**：裸二进制无
   xref（尤其被函数指针表间接调用的例程），分析器不会自动建函数。解法：
   先 `disassemble(addr)` 手工反汇编，再 `fm.createFunction(null, addr, 显式AddressSet,
   USER_DEFINED)` 传入 body（不能传 null body）。写完记得用 JDK21 javac 单独编译
   排错（headless 的报错信息会误导为"class not found"）。
5. **decompiled.txt 才是可靠输出**：手动建函数的单目标反编译常返回空 C，
   但 `DecompileAll.java` 全量落盘（324 函数）结果完整可信——反编译结果一律以
   全量 dump 为准，不要以单目标脚本的 stdout 为准。

## 3. `reverse/` 目录结构

```
reverse/
  rtl8721d_hal_init.o            rtl8721d_hal_init_dump.txt   (hal 的核心: rtl8721d_hal_init/deinit, SetHwReg8721D, GetHwReg8721D)
  rtl8721d_rf_onoff.o             (ROM code: rtw_rf_cmd 射频寄存器操作 / rtw_wait_tx_sm_ok / rtw_get_tsf)
  rtl8721d_phycfg.o               (phy 配置)
  hal_com.o  hal_intf.o  hal_intf_dump.txt   (通用 HAL 跳板: rtw_hal_init/deinit/set_hwreg/get_hwreg)
  rtw_pwrctrl.o                   (★ 电源管理核心: ips_enter/leave, LPS_Enter/Leave, rtw_ps_processor, PS_RDY_CHECK, _rtw_pwr_wakeup)
  pmclp/  rtl8721d_freertos_pmu.o  rtl8721dlp_pmc*.o  pmu_dump.txt  pmc*.txt  (KM4 PMU 睡眠执行路径)
  rom_rtl8721d_hal_init.o         (ROM 侧 hal_init)
  rtw_intfs.o  rtw_psk.o  phydm_hwconfig.o ...
  libwlan/  rtw_pwrctrl.o  rtw_ap.o  rs8721d_suspend.o
            alldump.txt  (★ 整个 lib_wlan 的完整反汇编，约 14.6 万行)
            nm_all.txt   (★ 整个 lib_wlan 的 nm 符号表)
```

### 关键索引文件（最有价值）

- **`reverse/libwlan/nm_all.txt`** —— 全库符号表，先靠它找函数名。
- **`reverse/libwlan/alldump.txt`** —— 全库反汇编。用它定位函数行号后 `Read`。
  行号会因后续编辑变化，建议用 `rg` 重新定位：
  ```bash
  rg -n "<rtl8721d_hal_init>|<ips_leave>|<LPS_Leave>" reverse/libwlan/alldump.txt
  ```

## 4. 已解锁的结构知识（逆向下钻的"钥匙"）

### 4.1 核心驱动对象与偏移（关键！）

驱动以 `padapter`（netdev_priv->adapter）为根，所有子结构用**绝对偏移**访问：

- `padapter + 0x1be0` = `pwrctrl_priv`
- `pwrctrl_priv + 0x18` (= `padapter+0x1bf8`) = `ps_priv`（电源状态机核心，绝大多数 PS 字节字段都在这）
- `ps_priv` 内字节偏移（以 `ps_priv` 为基）：
  - `+0x08` = 当前 `ps_mode`
  - `+0x0a` = `lps_enable`
  - `+0x18` = `pwr_state_code`(旧 lps_state 320)/`lps_level`... 见 4.2
  - `+0x24`..`+0x50` = 计数/状态字节（`ip/ipsw_disconn` 计数区）
  - `+0x7c` = 当前 `pwr_state`（2 = PS_STATE_S0/IPIPS? 关键状态值）
  - `+0x7d` = `pwr_state_processing`
- 直接挂在 `padapter` 上的标志（`movw r3,#0x22e4` 一类）：
  - `+0x21d0` `+0x21d4` `+0x21d8` `+0x21dc`（off/on 计数、hal_init 完成位）
  - `+0x2284` = HAL 函数指针区起始偏移（`rtw_hal_*` 那些通过函数指针分发）
  - `+0x2355` 等 |= wifi 状态

> 技巧：反正汇编里大量 `movw r3,#小立即数; ldr? [r4,r3]`，立即数即结构体偏移。
> 用 `rg '"movw.*22e[0-9a-f]'` 或直接看 `ldrb.w r3,[rN,#0x..]`。

### 4.2 HW_VAR 枚举（rtw_hal_set/get_hwreg 的分发号）

`SetHwReg8721D`(alldump:83806) / `GetHwReg8721D`(alldump:84183) 用 `tbh/tbb` 跳转表分发。

已确认的关键号：
| 号 | 含义 | 去向 |
|----|------|------|
| 43 (0x2b) | `HW_VAR_SET_PS_MODE` | → `rtl8721d_set_FwPwrMode_cmd`（H2C 命令，固件握手） |
| 45 (0x2d) | `HW_VAR_WIFI_PS_STATE` | PS 状态上报 |
| 46 (0x2e) | `HW_VAR_RF_STATE`(读) | 读 RF 是否 ON（`LPS_RF_ON_check` 用它轮询） |
| 56 (0x38) | antenna select | `rtw_hal_set_hwreg`+def_var |
| 60/64(0x3c/0x40) | `HW_VAR_RF_OFF` | RF 关闭 |
| 88 (0x58) | `HW_VAR_UCPS_CLOSE_RF` | `rtl8721d_ucps_closeRF` |

> 若解跳转表：`tbh [pc, r1, lsl #1]` 的基址 = 指令地址+4，后随 16bit 偏移表。
> objdump 不会解 TBH，需要手动按半字索引。
> 注意 SetHwReg8721D 与 GetHwReg8721D 的跳转表不同、且都是 `(r1 - base)` 后再查表（Get 开头 `subs r3, r1, #6`）。

### 4.3 关键函数 → alldump.txt 定位（历史行号，用 rg 重定位）

- `ips_enter` ≈ 123479   `ips_leave` ≈ 123540
- `LPS_Enter` ≈ 123875   `LPS_Leave` ≈ 123923   `LPS_RF_ON_check` ≈ 123834
- `rtw_ps_processor` ≈ 123692   `PS_RDY_CHECK` ≈ 123735   `rtw_set_ps_mode` ≈ 123781
- `_rtw_pwr_wakeup` ≈（rtw_pwrctrl.o 内）
- `rtl8721d_hal_init` ≈ 31961 + 全程
- `rtl8721d_hal_deinit` ≈ 31696
- `rtl8721d_set_FwPwrMode_cmd` ≈ 80352
- `rtl8721d_FirmwareDownload` ≈ 82003  `rtl8721d_FirmwareDisable` ≈ 82036
- `lps_ctrl_wk_hdl` ≈ 94889  `rtw_ps_cmd` ≈ 95184  `rtw_lps_ctrl_wk_cmd` ≈ 94944
- `SetHwReg8721D` ≈ 83806  `GetHwReg8721D` ≈ 84183
- `rltk_wlan_rf_on` ≈ 74066  `rltk_wlan_rf_off` ≈ 74106
- KM4 睡眠：`pmc_dump.txt` 中 `SOCPS_SleepCG`、`SOCPS_KM4Wake`、`SOCPS_WakeFromPG`

### 4.4 关键调用链（骨架）

```
[WLAN 电源机]
  rtw_ps_processor / rtw_ps_cmd（命令线程）
    ├─ PS_RDY_CHECK：判断能否进 PS
    ├─ ips_enter/ips_leave      → rtw_hal_set_hwreg(RF_OFF/43) + rtw_hal_deinit/init
    └─ LPS_Enter/LPS_Leave      → rtw_hal_set_hwreg(43) → rtl8721d_set_FwPwrMode_cmd → H2C
         lps_ctrl_wk_hdl        → LPS_Leave + rtw_hal_set_hwreg(45)

[HAL 跳板 rtw_hal_* (hal_intf.o)]
  rtw_hal_init ./ rtw_hal_deinit   → 函数指针 @0x20d8 / 0x20dc → rtl8721d_hal_init/deinit
  rtw_hal_set_hwreg/get_hwreg      → 函数指针 @0x2154 / 0x2158 → SetHwReg8721D / GetHwReg8721D

[固件侧]  WiFi 子固件独立运行；H2C 由 FillH2CCmd8721D / ipc_send_message 通信
```

### 4.5 固件通信

- `rtl8721d_FirmwareDownload/Disable` 用 `ipc_send_message` + 轮询 `0x40080080`。
- H2C 命令内容见 `rtl8721d_set_FwPwrMode_cmd`（构造栈上字节 + `rtw_hal_fill_h2c_cmd`）。
- 锅中 `rtw_rf_cmd`（rtl8721d_rf_onoff.o）直接写 `0x4008xxxx` 寄存器，是 ROM 侧 RF 开关。

## 5. 常用工作流

1. `nm_all.txt` 找函数名 → 得到它属于哪个 `.o`（每文件开头有 `xxx.o: file format`）。
2. `alldump.txt` 里 `rg -n "<函数名>"` 定位行号 → `Read` 该区域。
3. 需要 `.o` 单独反汇编时用 objdump（见 §2），方便聚焦单个目标。
4. 画调用链：`rg -n "bl.*<xxx>"` 反向找调用者；`nm_all.txt` 里 `U` 表示外部引用，`T` 本库定义。

## 6. 待办 / 未完成

- SetHwReg8721D 的完整 43/60/64/88 case 边界（TBH 表暂只做过局部求证；43=SET_PS_MODE 已确认通向 `FUN_0c01304c`→`FUN_0c00c40c`）。
- lxbus（SDIO 类似总线）suspend/resume 与主机侧 PMU 的交互（`lxbus_suspend.o` 未深入）。
- 若拿到 `platformio` 实际链接产物（.elf），可用 `-Mcortex-m3` Ghidra 做符号化高级分析。

## 7. LPS vs IPS 快慢根因总结（已确认）

**LPS 快 = WiFi 固件活着，唤醒只发一条 H2C 让固件恢复 RF；IPS 慢 = 固件被整体
掉电，唤醒必须完整 `rtl8721d_hal_init()`（MAC/BB/RF 重配 + 重校准 + 固件重启）。**

### 7.1 IPS 的"慢"全部在 ips_leave → rtw_hal_init()

`ips_leave`（alldump:123540）：

```
rtw_reset_drv_sw()            ; 软件层重置
rtw_hal_init()                ; ★ 完整 HAL 重初始化
set_channel_bwmode(); rtw_set_key(); netif_post_sleep_processing()
```

`rtw_hal_init → rtl8721d_hal_init`（alldump:31961）耗时操作：

- `PHY_MACConfig8721D` + `PHY_BBConfig8721D` + `PHY_RFConfig8721D`
- `phy_syn_calibrate_part1_8721d`（RF 锁相环/合成器校准，几十 ms）
- LLT 表、TX buffer boundary、SIFS/EDCA/RateFallback 初始化
- `rtl8721d_FirmwareDownload`（alldump:82003）：`ipc_send_message(1,0)` 让子固件从头启动，
  轮询 `0x40080080`，上限 1000 × 100µs = **100ms**

对端（睡眠侧）`ips_enter → rtw_hal_deinit → rtl8721d_hal_deinit`（alldump:31696）：
RF off + `rtl8721d_FirmwareDisable` + `HalPwrSeqCmdParsing`
（电源时序把整块射频掉电）+ `pmu_unregister_sleep_callback`。

唤醒等待：`_rtw_pwr_wakeup`（rtw_pwrctrl.o，已解）：先等
ps_priv+68(pwr_state_processing) 清 0（10ms 轮询，上限 3s），再查
pwr_state==2（IPS 态）→ 走 ips_leave 完整重初始化，最后查 hal_init 完成位
（padapter+0x2288 / +0x21ed），全部就绪才返回 1；PS 未激活时
（ps_priv+68==0 且 pwr_state!=2）直接秒回。

### 7.2 LPS 的"快"在于只有一条 H2C

```
LPS_Leave:
  rtw_hal_set_hwreg(padapter, HW_VAR_SET_PS_MODE=43, &val)
  → SetHwReg8721D（+0x450 尾调用）→ rtl8721d_set_FwPwrMode_cmd
  → H2C_8721D_SET_PWR_MODE(0x20) 发给仍在运行的固件
```

固件收到后立即把 RF 恢复收/发：无 PHY/RF 寄存器重写、无校准、无固件重启。
宿主侧只做：命令线程调度 + H2C 握手 + `LPS_RF_ON_check`（轮询 HW_VAR_RF_STATE=46，
`rtw_usleep_os(100)` 快循环）→ **20~30ms**。

### 7.3 为什么"睡眠"本身不影响射频

- SLEEP_CG（KM4 Clock Gating）+ tickless 只门控 KM4 CPU 时钟；`SOCPS_SleepCG`（pmc_dump）
  只执行 `pmu_exec_sleep/wakeup_hook_funs`；WiFi 子系统电源域独立、一直供电。
- 电流表实测修正（2026-08-04）：LPS 睡眠时 RF 模拟前端其实已**大部分断电**，与 IPS 只差
  个位数 mA；关键是**固件（KM0 数字域）活着**，RF 门控由固件在 DTIM 间自行执行
  （`rtw_rf_cmd`，rtl8721d_rf_onoff.o），PLL/校准数据保留在固件 RAM 中。

### 7.4 系统怎么选 LPS 还是 IPS

- 关联上且空闲 → `PS_RDY_CHECK`（alldump:123735）通过 → LPS（DTIM 睡眠，sta_powersave.cpp）。
- 未关联/长时间空闲 → IPS（全关射频）。

### 7.5 对项目的意义

- STA 场景快速出包用**关联态 LPS**：首包前延迟 20~30ms，够做周期性 UDP 上报。
- IPS 只适合深度休眠：每次唤醒 100ms+ 是硬件必须付出的（RF 校准 + 固件重启），软件层面
  没有捷径；可考虑缩短 `_rtw_pwr_wakeup` 的 10ms 轮询间隔，但大头在校准/固件启动本身。

## 8. 续：未连接时模拟 LPS（保持固件存活）的可行性结论

> 一句话结论：**可行，但正解不是"保存校准数据再跳过校准"，而是让 WiFi 固件不死**。
> LPS 快 = 固件活着 = 校准数据天然保留在固件 RAM/驱动结构中；IPS 慢 = 固件被 rtw_hal_deinit
> 杀掉，醒来必须固件重启 + PHY 重配 + 重校准。

### 8.1 校准数据在哪、IPS 期间丢不丢

- 校准结果分两层：
  - **驱动侧（KM4 RAM，IPS 不掉电，其实一直活着）**：ODM 结构里的 IQK/LCK 结果数组、
    thermal 值等。`phy_iq_calibrate_8721d`（alldump:19845）0xe6~0xfc 把结果写进其
    r0 参数 +0xbec~+0xbf8 的数组（即 ODM 结构内，约 pHalData+0x2fb4 一带）。
  - **WiFi 子系统侧（固件 RAM + RF/BB 寄存器，IPS 全丢）**：真正决定射频工作的值。
- 关键：`halrf_iqk_trigger`（alldump:30463）检查 bIQK_Forbidden（pHalData+0x27ac=10156）后
  直接 return；`halrf_lck_trigger` 同理（bLCK_Forbidden，pHalData+0x27a9=10153）。
  **跳过校准 = 连"把旧值写回寄存器"都不做**——驱动里没有 restore 路径。
- 所以"快照校准值→醒来写回"需要自己按 channel 复刻
  `phy_iq_calibrate_8721d` 0x116~0x176 的写回序列（BB 0xe28/0x88c/0xc04/0xc08/0x874/0xe70/0xb00 等）
  + RF 寄存器写回；但固件下载+PHY 初始化省不掉，且旧值有温度漂移风险。**不建议走这条路。**

### 8.2 正解：LPS 的本质 = 固件活着，校准天然保留

- `LPS_Leave`（alldump:123923）：`rtw_hal_set_hwreg(padapter, 43, &0)`
  → SetHwReg8721D → `rtl8721d_set_FwPwrMode_cmd`（alldump:80352）
  → H2C 0x20（SET_PWR_MODE，7 字节：mode / ps_priv+9 | lps_level<<4 / 4 /
  padapter+0x1ba0 / 12或0 / 0x40）。固件把自己 RAM 里的 RF 状态写回，无校准、无固件重启。
- **未连接时同样可玩**（对照 rf_sleep_test.cpp 的封堵手法）：
  1. `wifi_on(RTW_MODE_STA)` 不连接（或 AP+STA 且 beacon 挂起）。
  2. 永久封死自动 PS：ps_priv 关键字节清零（pwr_state=0、ps_mode=0、
     ps_priv+59/68/24/73=0）→ `rtw_hal_deinit` 永远不被调用 → **固件不死**。
  3. 睡眠：`rtw_hal_set_hwreg(padapter, 43, &val=1)`（= LPS_Enter 的 H2C，
     固件自己门控 RF）+ `pmu_release_wakelock(PMU_OS)` 让 KM4 进 SLEEP_CG。
  4. 醒来：`pmu_acquire_wakelock` + `rtw_hal_set_hwreg(padapter, 43, &val=0)`
     （= LPS_Leave 的 H2C）→ 固件恢复 RF → 直接 `wext_send_mgnt` 发包。

### 8.3 rltk_wlan_rf_off / rltk_wlan_rf_on 的坑（不对称）

- `rltk_wlan_rf_off`（alldump:74106）：要求 ps_priv+32(rf_pwrstate)==0 且
  ps_priv+124(pwr_state)==0；构造 6 字节 {0,0,0,0,0,1} 发固件级 RF off（软关，固件活着），
  然后置 rf_pwrstate=0x80000000。
- `rltk_wlan_rf_on`（alldump:74066 → 0xefc）：**要求 pwr_state==2（IPS 态）** 才执行
  `_rtw_pwr_wakeup(2000)`——这是 IPS 唤醒路径。在"固件活着"方案里 pwr_state=0，它直接返回 0 啥也不干。
- 结论：**off 可用 rltk_wlan_rf_off；on 必须用 rtw_hal_set_hwreg(43, 0)**（LPS_Leave 等价序列）。

### 8.4 固件侧 RF 电源门控序列（rtw_rf_cmd, rtl8721d_rf_onoff.o）

RF off：0x40083030|=0x800000 → rtw_wait_tx_sm_ok → 0x400806b7=0x11 →
轮询 0x400806b4 bit28==0 → 0x400807d5|=0x80 → 轮询 0x40080662 bit31==1 / bit30==0 →
0x40080006 bit1 翻转 → 0x40080100=0x3F → 0x40080101&=~2 → 0x40080553|=0x20 →
0x400807d5&=~0x80 → 0x4008307c|=0x80000000。

RF on：0x4008307c&=~0x80000000 → 0x40083030|=0x800000 → 0x40080101|=2 →
0x40080100=0xFF → 0x400807bc=0x09 → 0x400806f0=0x86 → 0x40080006|=2。

注意：0x40083030/0x4008307c 与 KM4↔固件 RF 状态机/IPC 配合；
现有 rf_sleep_test.cpp 只改 0x40080100 bit0~2，缺 IPC 握手和其余电源位，
睡眠深度与一致性不如走固件 H2C。

### 8.5 唯一不确定点（需上板实验）

KM0 固件在**未关联**时对 H2C 0x20 的处理（接受/忽略/仅部分执行）在
km0_image2_all.bin 里，无法从 lib_wlan.a 静态确认。
实验：ips_mode 封死后调 rtw_hal_set_hwreg(43,1) 测睡眠电流（对比 LPS 个位数 mA 差），
再 (43,0) 测首包延迟与 TX 成功率。
若固件拒绝：退路 rltk_wlan_rf_off + (43,0)；再退路：现方案但补全 §8.4 完整序列。

### 8.6 上板实测结论（2026-08-04，决定性）

**未关联时"模拟 LPS"不可行，三条路全部实测失败/不可靠：**

1. **H2C-LPS（rtw_hal_set_hwreg(43,1)）**：固件不实际门控 RF
   （0x100 REG_SYS_CLK_CTRL 读回 0x06FF 不变）；且 H2C-LPS + KM4 睡眠后，
   KM4 第一次访问 WiFi 子系统寄存器（读 0x1cc/0x100）即整机总线冻结
   （心跳、任务全停，无 HardFault dump）。
2. **手动切 0x100 RF 电源位（清 bit0~2）+ KM4 睡眠**：前 3 轮正常，
   之后固件后台射频任务（周期校准/TX power tracking）损坏，TX 路径报
   `rtl8721dd_mgnt_xmit(1545), desc unavailable. Drop management frame`，
   每包卡 2s 后丢弃——固件不知道 RF 被切，其 TX 状态机/描述符池被破坏。
3. **根本机制**：KM4 tickless 睡眠时，驱动注册的 PMU 睡眠回调
   `rs8721d_wlan_suspend`（alldump:71692）会自动执行
   `rs8721d_set_FwPwrMode_cmd(1)`（= H2C-LPS！）+ `ROM_WIFI_32K_Cmd` 切 32K 时钟；
   `rs8721d_wlan_resume`（alldump:71815）恢复。未关联时固件进入 LPS 后无 TBTT
   可依赖，恢复不完整 → 子系统不可访问。

**可行方案（已验证稳定 50+ 轮）**：

```
wifi_on(RTW_MODE_STA)                 // 固件下载 + PHY/RF 配置 + 校准（一次性）
wifi_set_power_mode(0, 0)             // 封死 IPS/LPS
pmu_unregister_sleep_callback(1)      // ★ 摘掉 WiFi PMU 睡眠回调（module id=1）
// 循环：
pmu_set_max_sleep_time(500); pmu_release_wakelock(PMU_OS); delay(500);
pmu_acquire_wakelock(PMU_OS);
wifi_tx_beacon_frame(...);            // 唤醒到发包 ~3ms，固件始终健康
```

代价：KM4 睡眠期间 RF 保持全开（不切电源），睡眠电流 = KM4 SLEEP_CG + WiFi 全开。
换取：唤醒 ~3ms、固件/校准长期存活、TX 可靠。
若后续要省 RF 电，唯一可行方向是**关联态 LPS**（sta_powersave.cpp）或
真 IPS（100ms+ 唤醒）；未关联 + 深 RF 睡眠在此固件上无解。

另注意：DiagPrintf 此 SDK 只支持 %d/%x 等，**不支持 %u/%lu**（会原样输出 "u"/"lu"），
打时间戳请用 %d + (int) 强转。

### 8.7 突破：软关 RF + 手动深切 0x100 + H2C 唤醒（2026-08-05 实测成功）

在 §8.6 基础上继续实验，找到**未关联也能深睡 + 毫秒级唤醒**的组合：

```
wifi_on(RTW_MODE_STA)
wifi_set_power_mode(0, 0)            // 封死 IPS/LPS
pmu_unregister_sleep_callback(1)     // 摘 PMU 回调，避免自动 H2C-LPS 冻结
// 循环（实测 75+ 轮稳定，TX=0ms）：
rltk_wlan_rf_off();                                   // ① 固件软关 RF（固件知情）
rtw_write16(0x100, read16(0x100) & ~0x00C0);          // ② 手动深切 bit6-7
release wakelock; delay(500);
acquire wakelock;
rtw_write16(0x100, read16(0x100) | 0x00C0);           // ③ 恢复电源
*(ps_priv+32) = 0;                                    // ④ 清 rf_pwrstate
rtw_hal_set_hwreg(padapter, 43, &0);                  // ⑤ H2C active 唤醒固件
wifi_tx_beacon_frame(...);                            // TX ~0ms
```

关键点：

- **顺序很重要**：先 `rltk_wlan_rf_off()`（固件级软关，payload {0,0,0,0,0,1}，
  固件据此停掉后台射频任务），再手动清 0x100 bit6-7（= rtw_rf_cmd RF-OFF 的 0x3F 值，
  真正断 RF 电源）。之前"盲切"（固件不知情）3 轮后 TX 报 desc unavailable 的根因就是
  固件后台任务往断电域写寄存器；软关后固件知情，不再破坏。
- 0x100 睡眠值 0x063F（bit6-7 清）与固件自己 RF-OFF 的 0x3F 完全一致；
  醒来恢复 0x06FF 后 H2C active 正常，固件健康、校准保留。
- 睡眠中寄存器可读（无冻结），唤醒 TX=0ms，无 desc unavailable。

失败的两条补充实验（同批完成）：

1. **自关联（wlan1 建 AP + wlan0 连自己）**：STA 收不到自己 AP 的 beacon
   （扫描/直连均 "scan stage, scan timeout"）——固件自过滤或共享 PHY 限制，
   真帧自关联在此硬件走不通。
2. **AP 侧 LPS（IPS off + LPS on + suspend_softap）**：驱动不进入 LPS，
   TX 稳定 138ms（softAP 恢复代价）——关 IPS 后 PS 机制整体停摆，AP 侧无独立 LPS 入口。

### 8.8 电流计实测：KM4 侧寄存器方案省不了电（2026-08-05，决定性）

用 IOT Power 电流计（scripts/iot_power_reader_skill）实测 §8.7 的组合：
**电流 56-64mA 平直，与未睡眠完全相同，无任何睡眠低谷**。

排查过程与结论：

1. **真正的掉电握手寄存器 KM4 不可达**：`rtw_rf_cmd`（rtl8721d_rf_onoff.o）的
   RF-OFF 序列里，0x40083030（bit23）与 0x4008307c（bit31）是固件侧掉电握手的关键位，
   但从 KM4 的 adapter I/O 窗口读这两个寄存器返回 **0xEAEAEAEA**（总线不可读）。
   它们只在固件（KM0）自己的地址空间可见，KM4 只能"请求"（H2C/API），不能"直接执行"。
2. **KM4 摸得到的寄存器不影响功耗**：0x100（REG_SYS_CLK_CTRL，0xFF↔0x3F 切换）、
   0x101、0x553、0x6 都能写、能读回，TX 也正常，但电流纹丝不动。
3. **KM4 睡眠收益被淹没**：即使挂起 WiFi cmd 线程 + 连续 3s 空闲
   （tickless 已确认开启，wakelock 释放后为 0，freertos_pre_sleep_processing 会走
   SOCPS_SleepCG），电流依然平直——WiFi 子系统（KM0 固件 + 射频 RX）全开就是
   **~56mA 的地板**，KM4 SLEEP_CG 只省自身几 mA，电流计上看不出。
4. **本质**：本固件的"深度断电"（LPS 的 DTIM 间 RF 门控 / IPS 的整块掉电）全部由
   KM0 固件内部的电源状态机执行；KM4 侧无论 H2C、软关、还是直接写寄存器，
   都无法让固件执行真正的掉电动作。

**出路只剩三条**：关联态 LPS（真实连接，sta_powersave.cpp）、
IPS（深度休眠但 100ms+ 唤醒）、或 **KM0 固件补丁**（让固件在未关联时执行真正的
掉电握手——即 §8.7 方案里缺失的最后一步，需要逆向 km0_image2_all.bin 定位并修改
H2C 0x20 / RF 掉电路径）。

---

### 8.9 KM0 固件逆向：RF 电源门控序列已定位（2026-08-05，Ghidra + objdump）

逆向对象 `km0_image2_all.bin`（Ameba_misc\Image_Releated 构建，106,496 B）：
- 镜像头：签名 "81958711" + size `0x176E0` + 加载地址 `0x0C000020`（头部 0x20 字节）。
- 有效载荷在文件偏移 0x20，加载基址 `0x0C000000` → 解决符号由 `payload_off + 0x0C000000`。
- 用 Ghidra headless 导入：`-loader BinaryLoader -loader-baseAddr 0x0C000000
  -processor "ARM:LE:32:v8-m"`（注意语言 `Cortex_M23` 无效，须用 `ARM:LE:32:v8-m`）；
  objdump：`arm-none-eabi-objdump -d -M force-thumb`（工具链在
  `.platformio/packages/toolchain-gccarmnoneeabi`）。

**三条关键例程**（工作目录 `C:\Users\krrr\AppData\Local\Temp\opencode`）：
- IPC（H2C）任务线程：payload+0x7e18 = **0x0C007E38**（里含 RF 电源状态机与睡眠握手）
- **RF-ON**：payload+0x8268 = **0x0C008288**
- **RF-OFF**：payload+0x8320 = **0x0C008340**
- 0x0C00841C：RF 状态机内联等价物（Ghidra 无法解析为独立函数 → 直接用 objdump 读）

#### RF-ON 序列（0x0C008288，共 0x37 指令）
```
0x40083030 |= 0x800000          ; bit23 上电请求
0x40080101 |= 0x02
0x40080100  = 0xFF              ; SYS_CLK 全开
0x400806b7  = 0x09
0x400806b4  = 0x86
call(GPIO 控制, RAM[0x8_484c])  ; 参数来自 RAM[0x8_5eac] 偏移 0x716/717/718, 0x685/686/687
0x40080662 |= 0x06
r0 = call(0x0000398d / ROM)     ; 复位/安全序列，结果存 RAM[0x8_4fa4+0x2c]
```

#### RF-OFF 序列（0x0C008340，共 0x4b 指令）——**真正的掉电握手**
```
0x40083030 |= 0x800000          ; bit23 掉电请求
call(RAM[0x8_47c4])
0x400806b7  = 0x11              ; 通知固件测距/校准状态
poll 0x400806b4 直到低4位==0    ; 等射频空闲（配合 0x000014c3 延时）
0x400807d5 |= 0x80              ; 掉电控制位
poll 0x40080662 bit31==0        ; 等握手完成（bit31 置位期间持续延时）
0x40080662 bit1 清零/置位/清零   ; bit1 触发
0x40080100  = 0x3F              ; SYS_CLK 收窄
0x40080101 &= ~0x02
0x40080553 |= 0x20
0x400807d5 &= ~0x80
r0 = call(0x0000398d / ROM)     ; 结果存 RAM[0x8_4fa4+0x24]
```

#### 与 §8.7/8.8 的对应关系（已确认的寄存器）
：§8.8 的"握手位 0x40083030/bit23、0x4008307c/bit31"正是上述固件序列。
KM4 侧 H2C / 直接写寄存器都无法触碰 `0x400806b4/b7/0x40080662` 的握手轮询，
这解释了为什么 §8.8 电流计方案无效——真正的握手完全在 KM0 固件内。

#### H2C dispatch 链已完整追踪（decompiled.txt 证实，更新"下一步"）

用 Ghidra（`MakeFuncs2.java` 手动 createFunction + `DecompileAll.java` 全量反编译，
324 个函数全部成功）确认了固件结构。**关键：RF-ON/RF-OFF 经函数指针表间接调用**
（init 例程 `FUN_0c006c88` 把 `&FUN_0c008288_1` 存到 RAM `_DAT_000807d4`、
`&FUN_0c008340_1` 存到 `_DAT_000807d0`），这就是为什么镜像里没有对二者的静态指针引用。

调用链（自上而下）：
```
H2C 命令到达 → FUN_0c01304c(byte *cmd,...)  [解码命令字节缓冲]
   └─ 按 DAT_40080527 / 校准状态选择 PS 状态
      → FUN_0c0040c0? 实为 FUN_0c00c40c(uint state)  ← RF 电源状态机
          state==2  → (*_DAT_000807d0)()  = FUN_0c008340  [RF-OFF]
          state==0  → (*_DAT_000807d4)()  = FUN_0c008288  [RF-ON]
   （辅助）FUN_0c00c600： 看 DAT_40080543 符号 → FUN_0c00c40c(1/2)
```
- `FUN_0c00c40c`：真正的 RF 门控状态机（0/1/2/3/4 状态）。state==2 断言走 RF-OFF，
  state==0/1/3/4 走 RF-ON 或仅发 C2H 命令。
- 伴随命令 `(*_DAT_000807bc)(0xff,fA,fB)`（对应 LAB_0c008580）发 C2H 给 KM4 主机，
  参数 0x1f/0x20/0x21/0x22/0x23/0x24/0x25 = 电源状态上报字段。
- 睡眠任务 `FUN_0c007e38`：死循环 `func_0x0c001934(RAM[0x85744],0xffffffff)`（等 H2C
  唤醒邮箱）；一旦置位就把 `0x40080130/0x120/0x138=0`、
  `0x40080134/0x124/0x13c=0xffffffff` 后 `(*_DAT_00085eac)()` 进睡眠，醒来调
  `FUN_0c007954()`（上电，内含 `0x40080120 清 1` 等）。`FUN_0c007f68` 是清唤醒源版本。
- `FUN_0c008080` 是 RF 通道/校准状态设置（`0x4008041f=0x20`，读 0x4008041d 门控）。

**结论**：SET_PWR_MODE/H2C 掉电完全由 H2C 命令 `FUN_0c01304c` 驱动；固件不会在
"未关联"时自行主动 RF-OFF。要触发掉电，必须由 **KM4 侧发对应 H2C**——这正是
`docs/tests/sta_powersave.cpp`（关联 LPS）能成功、而单纯未关联不掉电的原因。

#### 决定性补充：RF-OFF 门控点在哪（2026-08-05 续，反汇编确认）

进一步追踪（objdump + decompiled.txt 核对）确认 RF-OFF 唯一入口 `FUN_0c00c40c(2)`
**内部没有关联检查**——`param_1==2` 分支无条件执行 `(*_DAT_000807d0)()`（= `FUN_0c008340`
完整掉电握手）。真正"未关联不触发"的门控在**上游**：

1. `FUN_0c00c600`（payload+0xc5e0）：读 `DAT_40080543`（0x40080543）bit7——
   - bit7=1 → `FUN_0c00c40c(1)`（LPS 保持 RF，等 DTIM）
   - bit7=0 → `FUN_0c00c40c(2)`（RF-OFF 全关）
   即 **bit7 是"LPS 使能"标志**，只有它被清空才走 RF-OFF。
2. `FUN_0c00c3b4`（payload+0xc61c）：就是 `return DAT_40080543 >> 7`（读该 bit）。
3. 周期驱动点 `FUN_0c00ab50/b4a0/b504`：只有 `DAT_0008635f` bit31（适配器"power-save
   armed"位）置位才调用 `FUN_0c00c600`。
4. H2C 解码 `FUN_0c01304c`：`DAT_40080527` + 适配器 `+0x4b3/0x4b6` 各 bit 决定投递到
   `FUN_0c00c40c(0/1/2/4)`。

所以 KM0 侧需要的只是 `DAT_0008635f` bit31 置位 + `DAT_40080543` bit7 清零，即
**固件本身不拦未关联**；真正把 LPS H2C 在"未关联"时拦住的是 **KM4 预编译驱动**
（`lib_wlan.a`，`pm_set`/`rtl8721d_set_FwPwrMode_cmd` 里的关联检查）。

### 8.10 进一步的 H2C 电源模式实验与结论（2026-08-06）

在 test_powersave.cpp 中尝试了不同的 H2C SET_PWR_MODE (0x20) 模式以及伪造 H2C_MEDIA_STATUS_RPT (0x1B)，结果如下：

1. **mode=2 (IPS / 深度休眠)**：
   - 现象：能成功使电流降至 ~16mA。
   - 失败原因：当唤醒时发送 mode=0，虽然固件似乎接受了，但随后发送 raw frame 必然遇到 desc unavailable（MAC 队列卡死）。
   - 本质：mode=2 是深度的 IPS 睡眠，会关闭 MAC 和 RF 电源域。要从该状态恢复，必须在主机侧（KM4）执行完整的 IPS_leave 序列（如 
ltk_wlan_rf_on，耗时 100+ms 重写大量射频和基带寄存器）。因此，**mode=2 永远无法实现 30ms 极速唤醒发包**。
2. **mode=1 (LPS / 浅度睡眠) + 伪造 media_status=1**：
   - 现象：电流保持在 60mA，没有任何睡眠迹象。
   - 失败原因：即便通过欺骗 KM0 已连接，KM0 的 LPS 状态机依然拒绝关闭射频。这极有可能是因为 LPS 状态机是一个**自主的定时状态机**，它依赖有效的 BSSID、Beacon Interval 和 DTIM（这些信息通常由 H2C_JOINBSS 0x21 提供）来规划射频睡眠。在未提供完整网络参数的情况下，KM0 直接忽略 mode=1 命令。

**最终阶段结论与下一阶段路线**：
- **纯 H2C 路线在未关联状态下走不通**：mode=2 唤醒太慢，mode=1 缺乏网络参数不生效。
- **路线 A（欺骗全套关联态）**：除了伪造 media_status，还需要发送完整的 H2C_JOINBSS（0x21）等参数，让 KM0 拥有可以计算睡眠周期的假 AP 环境。
- **路线 B（绕过 KM0，纯 KM4 寄存器门控）**：查找并直接利用 KM4 侧的 
tw_hal_set_hwreg 接口（例如 HW_VAR_RF_STATE 或其它特定的 0x824/0x82C 射频合成器寄存器）来**同步地**关闭和开启射频，从而完全摆脱 KM0 复杂的自主状态机束缚。
