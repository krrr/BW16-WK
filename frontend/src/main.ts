import Alpine from 'alpinejs'
import '@picocss/pico'
import './styles.scss'

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

interface StatusResponse {
  uptime: number
}

Alpine.data('app', () => ({
  uptime: 0,

  currentTab: 'home' as 'home' | 'scan',

  scanning: false,
  scanError: '',
  scanResults: [] as NetworkInfo[],
  scanResultCount: null as number | null,

  async fetchStatus() {
    try {
      const r = await fetch('/api/status')
      const data: StatusResponse = await r.json()
      this.uptime = data.uptime
    } catch (_) {}
  },

  init() {
    this.fetchStatus()
  },
  destroy() {
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
      } else {
        this.scanError = data.message || '扫描失败'
      }
    } catch (e) {
      this.scanError = '请求失败: ' + (e as Error).message
    } finally {
      this.scanning = false
    }
  },

  rssiColor(rssi: number): string {
    if (rssi >= -50) return '#27ae60'
    if (rssi >= -70) return '#f39c12'
    return '#e74c3c'
  }
}))

Alpine.start()
