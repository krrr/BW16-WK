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
  free_heap?: number
}

interface MessageItem {
  id: number
  type: 'success' | 'error' | 'warning'
  text: string
  duration: number
}

// Register Alpine store for messages
Alpine.store('message', {
  items: [] as MessageItem[],
  nextId: 1,
})

export const message = {
  show(text: string, type: 'success' | 'error' | 'warning' = 'success', duration = 3000) {
    let msg_store = (Alpine.store('message') as any)
    const id = msg_store.nextId++
    const item: MessageItem = { id, type, text, duration }
    msg_store.items.push(item)

    if (duration > 0) {
      setTimeout(() => {
        this.close(id)
      }, duration)
    }
    return id
  },

  success(text: string, duration = 3000) {
    return this.show(text, 'success', duration)
  },

  error(text: string, duration = 4000) {
    return this.show(text, 'error', duration)
  },

  warning(text: string, duration = 3500) {
    return this.show(text, 'warning', duration)
  },

  close(id: number) {
    let msg_store = (Alpine.store('message') as any)
    msg_store.items = msg_store.items.filter((item: MessageItem) => item.id !== id)
  }
}

Alpine.data('app', () => ({
  uptime: -1,
  apChannel: -1,
  selectedChannel: -1,
  switchingChannel: false,
  compileDate: '',
  rtcTime: null as Date | null,
  timeDiff: null as number | null,
  freeHeap: '',

  currentTab: 'home' as 'home' | 'scan',

  async fetchStatus() {
    try {
      const r = await fetch('/api/status')
      const data: StatusResponse = await r.json()
      this.uptime = data.uptime
      this.apChannel = data.ap_channel
      if (!this.switchingChannel) {
        this.selectedChannel = data.ap_channel
      }
      this.compileDate = this.formattedCompileTime(data.compile_date, data.compile_time)
      if (data.rtc_time !== undefined) {
        // MCU 返回的 rtc_time 是 Unix 秒数，JS new Date() 接收毫秒，所以 * 1000 做单位换算。
        this.rtcTime = new Date(data.rtc_time * 1000)
        this.timeDiff = Math.abs(Math.floor(Date.now() / 1000) - data.rtc_time)
      }
      if (data.free_heap !== undefined) {
        this.freeHeap = (data.free_heap / 1024).toFixed(1)
        console.log(this.freeHeap);
        
      }
    } catch (e) { console.error('fetchStatus error:', e) }
  },

  async changeChannel() {
    if (this.selectedChannel === this.apChannel || this.selectedChannel < 1 || this.selectedChannel > 13) return
    this.switchingChannel = true
    try {
      const r = await fetch('/api/change-channel', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ channel: this.selectedChannel })
      })
      const data = await r.json()
      if (data.success) {
        this.apChannel = data.ap_channel
        this.selectedChannel = data.ap_channel
        message.success(`信道已成功切换至 ${data.ap_channel}`)
      } else {
        message.error(data.message || '切换信道失败')
        this.selectedChannel = this.apChannel
      }
    } catch (e) {
      console.error('changeChannel error:', e)
      message.error('网络错误，切换信道失败')
      this.selectedChannel = this.apChannel
    } finally {
      this.switchingChannel = false
    }
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
  },
}))

Alpine.start()
