<template>
  <section>
    <article>
      <header>
        <h1>状态</h1>
      </header>
      <div style="display:flex;gap:0.5rem;align-items:center;flex-wrap:wrap;margin-bottom:1rem;">
        <span>当前信道: </span>
        <div style="display:flex;gap:0.4rem;align-items:center;">
          <select v-model.number="selectedChannel" class="chan-sel">
            <optgroup label="2.4 GHz">
              <option v-for="ch in [1,2,3,4,5,6,7,8,9,10,11,12,13,14]" :key="ch" :value="ch">CH {{ ch }}</option>
            </optgroup>
            <optgroup label="5 GHz">
              <option v-for="ch in [36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,144,149,153,157,161,165]" :key="ch" :value="ch">CH {{ ch }}</option>
            </optgroup>
          </select>
          <button @click="changeChannel" :disabled="switchingChannel || selectedChannel === apChannel" :aria-busy="switchingChannel" class="outline contrast btn-sm" style="margin-left: 6px">
            切换
          </button>
        </div>
      </div>
      <p v-if="formattedUptime">运行时间: {{ formattedUptime }}</p>
      <p v-if="freeHeap">空闲堆内存: {{ freeHeap }} KB</p>
      <p>
        RTC 时间: <span>{{ rtcTime ? rtcTime.toLocaleString('zh-CN') : '未设置' }}</span>
        <span v-if="timeDiff !== null && timeDiff > 30" style="color:var(--del-color,#c0392b);font-size:0.9em;">
          (偏差 <span>{{ Math.round(timeDiff) }}</span> 秒)
        </span>
        <button v-if="timeDiff !== null && timeDiff > 30" @click="syncTime" class="outline contrast btn-sm" style="margin-left:0.4rem">同步时间</button>
      </p>
      <footer>
        <small v-if="compileDate">固件编译: {{ compileDate }}</small>
        <small style="float: right">by <a href="https://github.com/krrr" target="_blank">krrr</a></small>
      </footer>
    </article>
  </section>
</template>

<script setup lang="ts">
import { ref, computed, onMounted } from 'vue'
import { message } from '../utils/message'

interface StatusResponse {
  uptime: number
  ap_channel: number
  compile_date: string
  compile_time: string
  rtc_time?: number
  free_heap?: number
}

const uptime = ref(-1)
const apChannel = ref(-1)
const selectedChannel = ref(-1)
const switchingChannel = ref(false)
const compileDate = ref('')
const rtcTime = ref<Date | null>(null)
const timeDiff = ref<number | null>(null)
const freeHeap = ref('')

const fetchStatus = async () => {
  try {
    const r = await fetch('/api/status')
    const data: StatusResponse = await r.json()
    uptime.value = data.uptime
    apChannel.value = data.ap_channel
    if (!switchingChannel.value) {
      selectedChannel.value = data.ap_channel
    }
    compileDate.value = formattedCompileTime(data.compile_date, data.compile_time)
    if (data.rtc_time !== undefined) {
      // MCU 返回的 rtc_time 是 Unix 秒数，JS new Date() 接收毫秒，所以 * 1000 做单位换算。
      rtcTime.value = new Date(data.rtc_time * 1000)
      timeDiff.value = Math.abs(Math.floor(Date.now() / 1000) - data.rtc_time)
    }
    if (data.free_heap !== undefined) {
      freeHeap.value = (data.free_heap / 1024).toFixed(1)
    }
  } catch (e) {
    console.error('fetchStatus error:', e)
  }
}

const changeChannel = async () => {
  if (selectedChannel.value === apChannel.value) return
  switchingChannel.value = true
  try {
    const r = await fetch('/api/change-channel', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ channel: selectedChannel.value })
    })
    const data = await r.json()
    if (data.success) {
      apChannel.value = data.ap_channel
      selectedChannel.value = data.ap_channel
      message.success(`信道已成功切换至 ${data.ap_channel}`)
    } else {
      message.error(data.message || '切换信道失败')
      selectedChannel.value = apChannel.value
    }
  } catch (e) {
    console.error('changeChannel error:', e)
    message.error('网络错误，切换信道失败')
    selectedChannel.value = apChannel.value
  } finally {
    switchingChannel.value = false
  }
}

const syncTime = async () => {
  try {
    const now = Math.floor(Date.now() / 1000)
    const r = await fetch(`/api/set-time?t=${now}`)
    const data = await r.json()
    if (data.success) {
      rtcTime.value = new Date(data.rtc_time * 1000)
      timeDiff.value = 0
      message.success("同步成功")
    }
  } catch (e) {
    console.error('syncTime error:', e)
  }
}

const formattedUptime = computed(() => {
  if (uptime.value < 0) return ''
  const h = Math.floor(uptime.value / 3600)
  const m = Math.floor((uptime.value % 3600) / 60)
  if (h > 0) return `${h}小时${m}分钟`
  return `${m}分钟`
})

const formattedCompileTime = (cDate: string, cTime: string): string => {
  if (!cDate || !cTime) return ''
  const d = new Date(`${cDate} ${cTime}`)
  if (isNaN(d.getTime())) return `${cDate} ${cTime}`
  return d.toLocaleString('zh-CN')
}

onMounted(() => {
  fetchStatus()
})
</script>
