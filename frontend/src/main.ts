import Alpine from 'alpinejs'
import '@picocss/pico'
import './styles.scss'
import './tabs/scan'

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
    const tab = location.hash.slice(1)
    if (['home', 'scan'].includes(tab)) {
      this.currentTab = tab as any
    }
    this.$watch('currentTab', (val: string) => {
      location.hash = val
    })
  },
  destroy() {
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
