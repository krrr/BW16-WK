<template>
  <section>
    <article>
      <header>
        <h1>WiFi 扫描</h1>
      </header>
      <div style="display:flex;gap:1rem;align-items:center;flex-wrap:wrap;">
        <button @click="startScan" :aria-busy="scanning" :disabled="scanning" class="contrast" style="min-width:8rem;">
          <span>{{ scanning ? '扫描中...' : '开始扫描' }}</span>
        </button>
        <label>
          <input type="checkbox" v-model="persistEnabled" @change="savePersisted" role="switch"> 持久保存
        </label>
      </div>

      <p v-if="scanError" role="alert" class="error">{{ scanError }}</p>

      <div v-if="scanResults.length > 0" style="overflow-x:auto;">
        <table>
          <thead>
            <tr>
              <th>SSID</th>
              <th>BSSID</th>
              <th>信号</th>
              <th>信道</th>
              <th>频段</th>
              <th>加密</th>
              <th>最后出现</th>
              <th>操作</th>
            </tr>
          </thead>
          <template v-for="ap in scanResults" :key="ap.bssid">
            <tbody>
              <tr>
                <td>{{ ap.ssid || '(隐藏)' }}</td>
                <td>
                  <span style="font-family:monospace;font-size:0.85em;cursor:pointer" @click="openMacLookup(ap.bssid)">
                    {{ ap.bssid }}
                  </span>
                  <span v-if="isVirtualMac(ap.bssid)" class="badge badge-red" style="font-size: 0.7em;">虚拟</span>
                </td>
                <td :style="{ color: rssiColor(ap.rssi), fontWeight: 'bold' }">{{ ap.rssi }} dBm</td>
                <td>{{ ap.channel }}</td>
                <td>{{ ap.band }}</td>
                <td>{{ ap.security }}</td>
                <td style="font-size:0.85em;">{{ ap.lastSeen }}</td>
                <td class="btn-col">
                  <button
                    @click="deviceScanning === ap.bssid ? stopDeviceScan(ap.bssid) : startDeviceScan(ap.bssid, ap.channel)"
                    :aria-busy="deviceScanning === ap.bssid"
                    :class="{ 'pointer-ev-auto': deviceScanning === ap.bssid }"
                    :disabled="deviceScanning !== null && deviceScanning !== ap.bssid"
                    class="outline contrast btn-sm"
                  >
                    <span>{{ deviceScanning === ap.bssid ? '停止监听' : '扫描设备' }}</span>
                  </button>
                  <Dropdown summary-class="outline secondary btn-sm" align="right">
                    <li>
                      <a @click.prevent="clearDevices(ap.bssid)">
                        清除结果
                      </a>
                    </li>
                  </Dropdown>
                </td>
              </tr>
              <!-- Device scan results for this AP -->
              <tr v-if="deviceResults[ap.bssid]?.length > 0 || ap.advanced_info">
                <td colspan="8" style="padding:0;background:var(--card-background-color, #f8f9fa);">
                  <div style="padding:0.5rem 1rem;">
                    <div
                      style="display:flex;justify-content:space-between;align-items:center;cursor:pointer;user-select:none;"
                      @click="toggleExpanded(ap.bssid)"
                    >
                      <small><strong>»»» 发现的设备 (<span>{{ deviceResults[ap.bssid]?.length || 0 }}</span>)</strong></small>
                      
                      <!-- Middle Area for AP Advanced Properties -->
                      <div v-if="ap.advanced_info" style="font-size:0.8em;color:var(--primary);display:flex;gap:0.8rem;align-items:center;margin:0 1rem;" @click.stop>
                        <span v-if="ap.advanced_info.uptime !== undefined" title="无线已运行时间" style="display:inline-flex;align-items:center;gap:2px;">
                          <img src="../assets/hourglass.svg" width="20" height="20" />
                          运行：{{ formatUptime(ap.advanced_info.uptime) }}
                        </span>
                        <span title="PMF（保护管理帧）" style="display:inline-flex;align-items:center;gap:2px;">
                          <img src="../assets/protection.svg" width="20" height="20" /> PMF：
                          <span v-if="ap.advanced_info.pmfRequired">强制</span>
                          <span v-else-if="ap.advanced_info.pmfCapable" >可选</span>
                          <span v-else style="color:var(--muted-color)">未启用</span>
                        </span>
                      </div>
                      <span style="font-size:0.85em;color:var(--muted-color);display:inline-flex;align-items:center;gap:0.25rem;">
                        <span>{{ isExpanded(ap.bssid) ? '折叠' : '展开' }}</span>
                        <img src="../assets/chevron-up.svg" width="20" height="20"
                          style="transition: transform 0.2s;"
                          :style="isExpanded(ap.bssid) ? 'transform: rotate(0deg);' : 'transform: rotate(180deg);'"
                        />
                      </span>
                    </div>
                    <div v-show="isExpanded(ap.bssid) && deviceResults[ap.bssid]?.length > 0">
                      <table style="margin-top:0.4em;font-size:0.85em;">
                        <thead>
                          <tr>
                            <th>MAC</th>
                            <th>数据包</th>
                            <th>握手包</th>
                            <th>最后出现</th>
                            <th>攻击</th>
                          </tr>
                        </thead>
                        <tbody>
                          <tr v-for="dev in deviceResults[ap.bssid]" :key="dev.mac">
                            <td>
                              <span style="font-family:monospace;cursor:pointer" @click="openMacLookup(dev.mac)">
                                {{ dev.mac }}
                              </span>
                              <span v-if="isVirtualMac(dev.mac)" class="badge badge-red">虚拟</span>
                            </td>
                            <td>{{ '↑' + (dev.packets_out || 0) + ' / ↓' + (dev.packets_in || 0) }}</td>
                            <td>{{ dev.handshakes || 0 }}</td>
                            <td>{{ dev.lastSeen }}</td>
                            <td>
                              <button
                                @click="deauthDevice(ap.bssid, dev.mac, ap.channel)"
                                :disabled="!!deauthing[ap.bssid + '-' + dev.mac]"
                                :aria-busy="deauthing[ap.bssid + '-' + dev.mac]"
                                class="outline contrast btn-sm"
                                style="color:#e74c3c;border-color:#e74c3c;"
                              >
                                <span>{{ deauthResult[ap.bssid + '-' + dev.mac] || 'Deauth' }}</span>
                              </button>
                            </td>
                          </tr>
                        </tbody>
                      </table>
                    </div>
                  </div>
                </td>
              </tr>
              <!-- Error row -->
              <tr v-if="deviceErrors[ap.bssid]">
                <td colspan="8" style="padding:0.25rem 1rem;color:var(--del-color,#c0392b);font-size:0.85em;">
                  <span>{{ deviceErrors[ap.bssid] }}</span>
                </td>
              </tr>
            </tbody>
          </template>
        </table>
      </div>

      <footer>
        <small v-if="scanResultCount !== null">发现 {{ scanResultCount }} 个网络</small>
        <small v-else>扫描大致耗时5-10秒</small>
      </footer>
    </article>
  </section>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'
import Dropdown from '../components/Dropdown.vue'

interface ApAdvancedInfo {
  uptime?: number
  pmfCapable?: boolean
  pmfRequired?: boolean
}

interface NetworkInfo {
  ssid: string
  bssid: string
  rssi: number
  channel: number
  band: string
  security: string
  lastSeen?: string
  advanced_info?: ApAdvancedInfo
}

interface ScanResponse {
  success: boolean
  message?: string
  networks: NetworkInfo[]
}

interface DeviceInfo {
  mac: string
  packets_out: number
  packets_in: number
  handshakes: number
  lastSeen?: string
}

interface DeviceScanResponse {
  success: boolean
  message?: string
  bssid: string
  channel: number
  count: number
  devices: DeviceInfo[]
  ap_beacon_parsed?: boolean
  ap_uptime?: number
  pmf_capable?: boolean
  pmf_required?: boolean
}

interface DeauthResponse {
  success: boolean
  message?: string
  rounds: number
  packets: number
}

const STORAGE_KEY = 'wifi_scan_data'
const PERSIST_KEY = 'wifi_scan_persist'

const persistEnabled = ref(localStorage.getItem(PERSIST_KEY) === 'true')
const scanning = ref(false)
const scanError = ref('')
const scanResults = ref<NetworkInfo[]>([])
const scanResultCount = ref<number | null>(null)

const deviceScanning = ref<string | null>(null)
const deviceErrors = ref<Record<string, string>>({})
const deviceResults = ref<Record<string, DeviceInfo[]>>({})
const deviceEventSource = ref<EventSource | null>(null)

const deauthing = ref<Record<string, boolean>>({})
const deauthResult = ref<Record<string, string>>({})

const expandedBssids = ref<Record<string, boolean>>({})

const isExpanded = (bssid: string) => {
  return expandedBssids.value[bssid] !== false
}

const toggleExpanded = (bssid: string) => {
  expandedBssids.value[bssid] = !isExpanded(bssid)
}

const loadPersisted = () => {
  const saved = localStorage.getItem(STORAGE_KEY)
  if (!saved) return
  try {
    const data = JSON.parse(saved)
    if (data.scanResults) scanResults.value = data.scanResults
    if (data.deviceResults) deviceResults.value = data.deviceResults
    if (scanResults.value.length > 0) scanResultCount.value = scanResults.value.length
  } catch { /* ignore corrupt data */ }
}

const savePersisted = () => {
  localStorage.setItem(PERSIST_KEY, persistEnabled.value ? 'true' : 'false')
  if (persistEnabled.value) {
    localStorage.setItem(STORAGE_KEY, JSON.stringify({
      scanResults: scanResults.value,
      deviceResults: deviceResults.value,
    }))
  } else {
    localStorage.removeItem(STORAGE_KEY)
  }
}

const startScan = async () => {
  scanning.value = true
  scanError.value = ''
  scanResultCount.value = null
  try {
    const r = await fetch('/api/scan')
    const data: ScanResponse = await r.json()
    if (data.success) {
      const now = new Date().toLocaleString('zh-CN')
      for (const ap of data.networks) {
        const existing = scanResults.value.find(a => a.bssid === ap.bssid)
        if (existing) {
          Object.assign(existing, ap, { lastSeen: now })
        } else {
          ap.lastSeen = now
          scanResults.value.push(ap)
        }
      }
      scanResultCount.value = scanResults.value.length
      savePersisted()
    } else {
      scanError.value = data.message || '扫描失败'
    }
  } catch (e) {
    scanError.value = '请求失败: ' + (e as Error).message
  } finally {
    scanning.value = false
  }
}

const stopDeviceScan = (bssid: string) => {
  if (deviceEventSource.value) {
    deviceEventSource.value.close()
    deviceEventSource.value = null
  }
  if (deviceScanning.value === bssid) {
    deviceScanning.value = null
  }
}

const deauthDevice = async (bssid: string, mac: string, channel: number) => {
  const key = bssid + '-' + mac
  if (deauthing.value[key]) return
  if (!confirm(`确定要对 ${mac} 发起 Deauth 攻击？`)) return

  deauthing.value[key] = true
  deauthResult.value[key] = ''
  try {
    const url = `/api/deauth?bssid=${encodeURIComponent(bssid)}&mac=${encodeURIComponent(mac)}&channel=${channel}`
    const r = await fetch(url)
    const data: DeauthResponse = await r.json()
    if (data.success) {
      deauthResult.value[key] = '✓'
    } else {
      deauthResult.value[key] = '✗'
    }
  } catch {
    deauthResult.value[key] = '✗'
  }
  setTimeout(() => {
    deauthing.value[key] = false
    deauthResult.value[key] = ''
  }, 3000)
}

const clearDevices = (bssid: string) => {
  if (deviceResults.value[bssid]) {
    deviceResults.value[bssid] = []
  }
  if (deviceErrors.value[bssid]) {
    delete deviceErrors.value[bssid]
  }
  savePersisted()
}

const startDeviceScan = async (bssid: string, channel: number) => {
  if (deviceEventSource.value) {
    stopDeviceScan(deviceScanning.value || '')
  }

  deviceScanning.value = bssid
  deviceErrors.value[bssid] = ''
  if (!deviceResults.value[bssid]) {
    deviceResults.value[bssid] = []
  }

  // 记录本次扫描前的历史数据作为 Baseline 基准
  // 每当收到SSE更新时将当前会话扫描到的包数与之前的历史基数相加得到总包数，从而实现多次扫描不丢失历史数据且不重复计算
  const baselines: Record<string, { out: number, in: number, handshakes: number }> = {}
  for (const dev of deviceResults.value[bssid]) {
    baselines[dev.mac] = {
      out: dev.packets_out || 0,
      in: dev.packets_in || 0,
      handshakes: dev.handshakes || 0
    }
  }

  try {
    const url = `/api/scan-devices?bssid=${encodeURIComponent(bssid)}&channel=${channel}`
    const es = new EventSource(url)
    deviceEventSource.value = es

    es.onmessage = (event) => {
      try {
        const data: DeviceScanResponse = JSON.parse(event.data)
        if (data.success) {
          const now = new Date().toLocaleString('zh-CN')
          for (const dev of data.devices) {
            const existing = deviceResults.value[bssid].find(d => d.mac === dev.mac)
            const base = baselines[dev.mac] || { out: 0, in: 0, handshakes: 0 }
            const newPacketsOut = base.out + (dev.packets_out || 0)
            const newPacketsIn = base.in + (dev.packets_in || 0)
            const newHandshakes = base.handshakes + (dev.handshakes || 0)

            if (existing) {
              // 仅在数据包发生改变时更新数量与最后出现时间
              if (existing.packets_out !== newPacketsOut ||
                  existing.packets_in !== newPacketsIn ||
                  existing.handshakes !== newHandshakes) {
                existing.packets_out = newPacketsOut
                existing.packets_in = newPacketsIn
                existing.handshakes = newHandshakes
                existing.lastSeen = now
              }
            } else {
              // 新设备插入
              deviceResults.value[bssid].push({
                mac: dev.mac,
                packets_out: newPacketsOut,
                packets_in: newPacketsIn,
                handshakes: newHandshakes,
                lastSeen: now
              })
            }
          }

          if (data.ap_beacon_parsed) {
            const ap = scanResults.value.find(a => a.bssid === bssid)
            if (ap) {
              ap.advanced_info = {
                uptime: data.ap_uptime,
                pmfCapable: data.pmf_capable,
                pmfRequired: data.pmf_required
              }
            }
          }

          savePersisted()
        } else {
          deviceErrors.value[bssid] = data.message || '扫描失败'
          stopDeviceScan(bssid)
        }
      } catch (err) {
        deviceErrors.value[bssid] = '解析数据错误'
        stopDeviceScan(bssid)
      }
    }

    es.addEventListener('done', () => {
      stopDeviceScan(bssid)
    })

    es.onerror = () => {
      stopDeviceScan(bssid)
    }
  } catch (e) {
    deviceErrors.value[bssid] = '创建连接失败: ' + (e as Error).message
    deviceScanning.value = null
  }
}

const formatUptime = (seconds?: number) => {
  if (seconds === undefined) return ''
  const days = Math.floor(seconds / 86400)
  const hours = Math.floor((seconds % 86400) / 3600)
  const minutes = Math.floor((seconds % 3600) / 60)
  
  const parts = []
  if (days > 0) parts.push(`${days}d`)
  if (hours > 0) parts.push(`${hours}h`)
  if (parts.length == 0 || minutes > 0) parts.push(`${minutes}m`)
  return parts.join('')
}

const openMacLookup = (bssid: string) => {
  window.open(`https://maclookup.app/search/result?mac=${bssid}`, 'maclookup', 'width=800,height=600')
}

const rssiColor = (rssi: number): string => {
  if (rssi >= -55) return '#27ae60'
  if (rssi >= -70) return '#f39c12'
  return '#e74c3c'
}

const isVirtualMac = (mac: string): boolean => {
  if (!mac) return false
  const cleanMac = mac.replace(/[^0-9a-fA-F]/g, '')
  if (cleanMac.length < 2) return false
  const firstByte = parseInt(cleanMac.substring(0, 2), 16)
  return !isNaN(firstByte) && (firstByte & 2) !== 0
}

onMounted(() => {
  if (persistEnabled.value) {
    loadPersisted()
  }
})

onUnmounted(() => {
  if (deviceEventSource.value) {
    deviceEventSource.value.close()
  }
})
</script>

<style lang="scss">
  .btn-col {
    white-space: nowrap;
    button:last-of-type {
      border-top-right-radius: 0;
      border-bottom-right-radius: 0;
    }
    details summary[role=button] {
      border-top-left-radius: 0;
      border-bottom-left-radius: 0;
      border-left: 0;
      padding-left: 0;
    }
  }
</style>