# BW16 Stealth Deauther Project

## 技术栈

### MCU
- **Board**: Ai-Thinker BW16 (RTL8720DN)
- **Platform**: Realtek AmebaD, Arduino framework
- **WiFi AP API**: `WiFi.apbegin(ssid, pass, channel, hidden)`
- **HTTP Server**: Raw `WiFiServer` on port 80, manual HTTP parsing
- **Build**: PlatformIO (`pio run`)

### Frontend (分离开发, `frontend/`)
- **Package**: pnpm
- **Build**: Vite + `vite-plugin-singlefile` (全部 CSS/JS 内联到单 HTML)
- **CSS**: Pico CSS v2
- **JS**: Vue 3

## 开发

### Webpage → Firmware Pipeline
1. `pnpm --dir frontend build` → `frontend/dist/index.html`
2. `node scripts/compress-to-c.mjs` (gz压缩) → `include/webpage.h`
3. `pio run` → 编译固件

### Build One-liner
```powershell
.\build-all.ps1              # 完整构建
.\build-all.ps1 -Upload      # 构建+烧录
.\build-all.ps1 -Monitor     # 构建+串口监视
.\build-all.ps1 -Ota         # 构建+OTA上传 (至192.168.4.1)
```

### 文档位置
单片机SDK参考源码: `lib/ameba-arduino-d-feature-platformio-support/Arduino_package`
单片机手册：`docs/extracted`

## 编码规范
- 不要随意删除代码里的注释