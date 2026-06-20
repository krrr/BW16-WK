import Alpine from 'alpinejs'
import '@picocss/pico'
import './styles.scss'

Alpine.data('app', () => ({
  // Home
  message: 'Hello World!',
  count: 0,
  uptime: 0,
  _timer: null,

  // Nav
  currentTab: 'home',

  // Scan
  scanning: false,
  scanError: '',
  scanResults: [],
  scanResultCount: null,

  init() {
    this._timer = setInterval(() => { this.uptime++ }, 1000)
  },
  destroy() {
    clearInterval(this._timer)
  },
  reset() {
    this.count = 0
  },

  async startScan() {
    this.scanning = true
    this.scanError = ''
    this.scanResultCount = null
    try {
      const r = await fetch('/api/scan')
      const data = await r.json()
      if (data.success) {
        const now = new Date().toLocaleString('zh-CN')
        const seen = new Set()
        for (const ap of data.networks) {
          seen.add(ap.bssid)
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
      this.scanError = '请求失败: ' + e.message
    } finally {
      this.scanning = false
    }
  },

  rssiColor(rssi) {
    if (rssi >= -50) return '#27ae60'
    if (rssi >= -70) return '#f39c12'
    return '#e74c3c'
  }
}))

Alpine.start()
