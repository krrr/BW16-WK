import { ref } from 'vue'

export interface MessageItem {
  id: number
  type: 'success' | 'error' | 'warning'
  text: string
  duration: number
}

const items = ref<MessageItem[]>([])
let nextId = 1

export const message = {
  items,
  show(text: string, type: 'success' | 'error' | 'warning' = 'success', duration = 3000) {
    const id = nextId++
    const item: MessageItem = { id, type, text, duration }
    items.value.push(item)

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
    items.value = items.value.filter((item) => item.id !== id)
  }
}
