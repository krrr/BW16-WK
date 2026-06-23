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
            existing.packets = (existing.packets || 0) + dev.packets
            existing.lastSeen = now
          } else {
            dev.lastSeen = now
            this.deviceResults[bssid].push(dev)
          }
        }
        this.savePersisted()
      } else {
        this.deviceErrors[bssid] = data.message || '扫描失败'
      }
    } catch (e) {
      this.deviceErrors[bssid] = '请求失败: ' + (e as Error).message
    } finally {
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
