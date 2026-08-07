---
name: iot-power-reader
description: 调用 IOT Power USB 电流计，测量设备的电压、电流和功耗。适用于功耗评估与定时采样测试。
---

# IOT Power 电流计脚本 (iot_power_reader.py) 指南

## 概述
`scripts/iot_power_reader_skill/iot_power_reader.py` 用于通过 `iot_parser.dll` 自动识别并读取 IOT Power USB 电流计的电压 (V)、电流 (mA/A) 和功耗 (W)。
可自动识别 WinUSB 直连设备。

---

## 常用命令行调用指令

### 定时采样测试（推荐 Agent 自动化评估）
执行 5 秒采样，完成后自动退出并关闭串口，不产生死循环：
```powershell
python scripts/iot_power_reader_skill/iot_power_reader.py --duration 5
```

### 测量并保存日志到 CSV 文件
```powershell
python scripts/iot_power_reader_skill/iot_power_reader.py --duration 10 -o power_data.csv
```

### 指定特定的设备或串口
- **指定 USB 直连设备**：
  ```powershell
  python scripts/iot_power_reader_skill/iot_power_reader.py -d CC-1A60BA0B0 --duration 5
  ```

### 自定义采样间隔（默认 1 秒）
```powershell
python scripts/iot_power_reader_skill/iot_power_reader.py --duration 5 -i 0.5
```

---

## 周期性功耗与波峰持续时间评估 (Pulse Peak Analyzer)

适用于评估芯片休眠/唤醒脉冲、Wi-Fi 广播/Beacon 发包波峰、BLE 广播等高频脉冲的**持续时间、唤醒周期、占空比与峰值电流**。

### 测量 10 秒并自动统计波峰持续时间与周期
```powershell
# --analyze-peaks: 启用脉冲波峰分析器
# --threshold 50: 高门限阈值 (50 mA)，高于此电流判定为波峰开始
# --low-threshold 40: (可选) 低门限迟滞阈值 (40 mA)，低于此电流判定波峰结束
python scripts/iot_power_reader_skill/iot_power_reader.py --duration 10 --analyze-peaks --threshold 50
```

### 自动生成报告示例：
```
==================================================
         CURRENT PEAK ANALYSIS REPORT         
==================================================
Total Samples Recorded : 2048
Detected Peak Count    : 14
Avg Peak Duration      : 205.42 ms (Min: 198.10ms, Max: 212.50ms)
Duration Std Dev       : 4.15 ms
Avg Pulse Period       : 701.80 ms
Avg Peak Current       : 124.50 mA
Avg Idle Current       : 14.80 mA
Duty Cycle             : 29.27 %
==================================================
```

---


## Python 代码内部调用 (Direct API)

```python
import time
from scripts.iot_power_reader.iot_power_reader import IoTPowerReader, DEFAULT_DLL_PATH

# 实例化并连接
reader = IoTPowerReader(DEFAULT_DLL_PATH)
devs = reader.get_scanned_usb_devices()

if devs and reader.open_usb(devs[0]):
    data = reader.read_once()
    print(f"电压: {data['voltage']}V, 电流: {data['current']}{data['unit']}")
    reader.close()
```

---

## Agent 调用注意事项
- **设备独占性**：测试前确保没有打开 IOT Power 官方 PC 端软件，避免串口/USB 接口被占用。
