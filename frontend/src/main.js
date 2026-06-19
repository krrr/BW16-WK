import Alpine from 'alpinejs'
import '@picocss/pico'

Alpine.data('app', () => ({
  message: 'Hello World!',
  count: 0,
  uptime: 0,
  _timer: null,
  init() {
    this._timer = setInterval(() => { this.uptime++ }, 1000)
  },
  destroy() {
    clearInterval(this._timer)
  },
  reset() {
    this.count = 0
  },
}))

Alpine.start()
