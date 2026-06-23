import Alpine from 'alpinejs'

interface NetworkInfo {
  ssid: string
  bssid: string
  rssi: number
  channel: number
  band: string
  security: string
  lastSeen?: string
}

interface ScanResponse {
  success: boolean
  message?: string
  networks: NetworkInfo[]
}

interface DeviceInfo {
  mac: string
  packets: number
  lastSeen?: string
}

interface DeviceScanResponse {
  success: boolean
  message?: string
  bssid: string
  channel: number
  count: number
  devices: DeviceInfo[]
}

const STORAGE_KEY = 'wifi_scan_data'
const PERSIST_KEY = 'wifi_scan_persist'

Alpine.data('scan', () => ({
  persistEnabled: localStorage.getItem(PERSIST_KEY) === 'true',
  scanning: false,
  scanError: '',
  scanResults: [] as NetworkInfo[],
  scanResultCount: null as number | null,

  deviceScanning: null as string | null,
  deviceErrors: {} as Record<string, string>,
  deviceResults: {} as Record<string, DeviceInfo[]>,
  deviceEventSource: null as EventSource | null,

  init() {
    if (this.persistEnabled) {
      this.loadPersisted()
    }
  },

  loadPersisted() {
    const saved = localStorage.getItem(STORAGE_KEY)
    if (!saved) return
    try {
      const data = JSON.parse(saved)
      if (data.scanResults) this.scanResults = data.scanResults
      if (data.deviceResults) this.deviceResults = data.deviceResults
      if (this.scanResults.length > 0) this.scanResultCount = this.scanResults.length
    } catch { /* ignore corrupt data */ }
  },

  savePersisted() {
    localStorage.setItem(PERSIST_KEY, this.persistEnabled ? 'true' : 'false')
    if (this.persistEnabled) {
      localStorage.setItem(STORAGE_KEY, JSON.stringify({
        scanResults: this.scanResults,
        deviceResults: this.deviceResults,
      }))
    } else {
      localStorage.removeItem(STORAGE_KEY)
    }
  },

  async startScan() {
    this.scanning = true
    this.scanError = ''
    this.scanResultCount = null
    try {
      const r = await fetch('/api/scan')
      const data: ScanResponse = await r.json()
      if (data.success) {
        const now = new Date().toLocaleString('zh-CN')
        for (const ap of data.networks) {
          const existing = this.scanResults.find(a => a.bssid === ap.bssid)
          if (existing) {
            Object.assign(existing, ap, { lastSeen: now })
          } else {
            ap.lastSeen = now
            this.scanResults.push(ap)
          }
        }
        this.scanResultCount = this.scanResults.length
        this.savePersisted()
      } else {
        this.scanError = data.message || '扫描失败'
      }
    } catch (e) {
      this.scanError = '请求失败: ' + (e as Error).message
    } finally {
      this.scanning = false
    }
  },

  stopDeviceScan(bssid: string) {
    if (this.deviceEventSource) {
      this.deviceEventSource.close()
      this.deviceEventSource = null
    }
    if (this.deviceScanning === bssid) {
      this.deviceScanning = null
    }
  },

  async startDeviceScan(bssid: string, channel: number) {
    if (this.deviceEventSource) {
      this.stopDeviceScan(this.deviceScanning || '')
    }

    this.deviceScanning = bssid
    this.deviceErrors[bssid] = ''
    if (!this.deviceResults[bssid]) {
      this.deviceResults[bssid] = []
    }

    // 记录本次扫描前的历史数据作为 Baseline 基准
    // 每当收到SSE更新时将当前会话扫描到的包数与之前的历史基数相加得到总包数，从而实现多次扫描不丢失历史数据且不重复计算
    const baselines: Record<string, number> = {}
    for (const dev of this.deviceResults[bssid]) {
      baselines[dev.mac] = dev.packets || 0
    }

    try {
      const url = `/api/scan-devices?bssid=${encodeURIComponent(bssid)}&channel=${channel}`
      const es = new EventSource(url)
      this.deviceEventSource = es

      es.onmessage = (event) => {
        try {
          const data: DeviceScanResponse = JSON.parse(event.data)
          if (data.success) {
            const now = new Date().toLocaleString('zh-CN')
            for (const dev of data.devices) {
              const existing = this.deviceResults[bssid].find(d => d.mac === dev.mac)
              const basePackets = baselines[dev.mac] || 0
              const newPackets = basePackets + dev.packets

              if (existing) {
                // 仅在数据包发生改变时更新数量与最后出现时间
                if (existing.packets !== newPackets) {
                  existing.packets = newPackets
                  existing.lastSeen = now
                }
              } else {
                // 新设备插入
                this.deviceResults[bssid].push({
                  mac: dev.mac,
                  packets: newPackets,
                  lastSeen: now
                })
              }
            }
            this.savePersisted()
          } else {
            this.deviceErrors[bssid] = data.message || '扫描失败'
            this.stopDeviceScan(bssid)
          }
        } catch (err) {
          this.deviceErrors[bssid] = '解析数据错误'
          this.stopDeviceScan(bssid)
        }
      }

      es.addEventListener('done', () => {
        this.stopDeviceScan(bssid)
      })

      es.onerror = () => {
        this.stopDeviceScan(bssid)
      }
    } catch (e) {
      this.deviceErrors[bssid] = '创建连接失败: ' + (e as Error).message
      this.deviceScanning = null
    }
  },

  openMacLookup(bssid: string) {
    window.open(`https://maclookup.app/search/result?mac=${bssid}`, 'maclookup', 'width=800,height=600')
  },

  rssiColor(rssi: number): string {
    if (rssi >= -55) return '#27ae60'
    if (rssi >= -70) return '#f39c12'
    return '#e74c3c'
  },
}))
