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
  rssi: number
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

Alpine.data('scan', () => ({
  scanning: false,
  scanError: '',
  scanResults: [] as NetworkInfo[],
  scanResultCount: null as number | null,

  deviceScanning: null as string | null,
  deviceErrors: {} as Record<string, string>,
  deviceResults: {} as Record<string, DeviceInfo[]>,

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
      } else {
        this.scanError = data.message || '扫描失败'
      }
    } catch (e) {
      this.scanError = '请求失败: ' + (e as Error).message
    } finally {
      this.scanning = false
    }
  },

  async startDeviceScan(bssid: string, channel: number) {
    this.deviceScanning = bssid
    this.deviceErrors[bssid] = ''
    if (!this.deviceResults[bssid]) this.deviceResults[bssid] = []
    try {
      const r = await fetch(`/api/scan-devices?bssid=${encodeURIComponent(bssid)}&channel=${channel}`)
      const data: DeviceScanResponse = await r.json()
      if (data.success) {
        const now = new Date().toLocaleString('zh-CN')
        for (const dev of data.devices) {
          const existing = this.deviceResults[bssid].find(d => d.mac === dev.mac)
          if (existing) {
            Object.assign(existing, dev, { lastSeen: now })
          } else {
            dev.lastSeen = now
            this.deviceResults[bssid].push(dev)
          }
        }
      } else {
        this.deviceErrors[bssid] = data.message || '扫描失败'
      }
    } catch (e) {
      this.deviceErrors[bssid] = '请求失败: ' + (e as Error).message
    } finally {
      this.deviceScanning = null
    }
  },

  deviceCount(bssid: string): number {
    return (this.deviceResults[bssid] || []).length
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
