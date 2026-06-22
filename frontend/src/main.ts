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

interface StatusResponse {
  uptime: number
  ap_channel: number
  compile_date: string
  compile_time: string
  rtc_time?: number
}

Alpine.data('app', () => ({
  uptime: -1,
  apChannel: -1,
  compileDate: '',
  rtcTime: null as Date | null,
  timeDiff: null as number | null,

  currentTab: 'home' as 'home' | 'scan',

  scanning: false,
  scanError: '',
  scanResults: [] as NetworkInfo[],
  scanResultCount: null as number | null,

  deviceScanning: null as string | null,
  deviceErrors: {} as Record<string, string>,
  deviceResults: {} as Record<string, DeviceInfo[]>,

  async fetchStatus() {
    try {
      const r = await fetch('/api/status')
      const data: StatusResponse = await r.json()
      this.uptime = data.uptime
      this.apChannel = data.ap_channel
      this.compileDate = this.formattedCompileTime(data.compile_date, data.compile_time)
      if (data.rtc_time !== undefined) {
        // MCU 返回的 rtc_time 是 Unix 秒数，JS new Date() 接收毫秒，所以 * 1000 做单位换算。
        this.rtcTime = new Date(data.rtc_time * 1000)
        this.timeDiff = Math.abs(Math.floor(Date.now() / 1000) - data.rtc_time)
      }
    } catch (e) { console.error('fetchStatus error:', e) }
  },

  async syncTime() {
    try {
      const now = Math.floor(Date.now() / 1000)
      const r = await fetch(`/api/set-time?t=${now}`)
      const data = await r.json()
      if (data.success) {
        this.rtcTime = new Date(data.rtc_time * 1000)
        this.timeDiff = 0
      }
    } catch (e) { console.error('syncTime error:', e) }
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

  get formattedUptime(): string {
    if (this.uptime < 0) return ''
    const h = Math.floor(this.uptime / 3600)
    const m = Math.floor((this.uptime % 3600) / 60)
    if (h > 0) return `${h}小时${m}分钟`
    return `${m}分钟`
  },

  formattedCompileTime(compileDate: string, compileTime: string): string {
    if (!compileDate || !compileTime) return ''
    const d = new Date(`${compileDate} ${compileTime}`)
    if (isNaN(d.getTime())) return `${compileDate} ${compileTime}`
    return d.toLocaleString('zh-CN')
  }
}))

Alpine.start()
