import Alpine from 'alpinejs'
import '@picocss/pico'

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
    this.scanResults = []
    this.scanResultCount = null
    try {
      const r = await fetch('/api/scan')
      const data = await r.json()
      if (data.success) {
        this.scanResults = data.networks
        this.scanResultCount = data.count
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
