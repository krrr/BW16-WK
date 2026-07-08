<template>
  <section>
    <article>
      <header>
        <h1>设置</h1>
      </header>

      <div>
        <h4 style="margin-bottom: 0.5rem">OTA 固件更新</h4>
        <p style="font-size: 0.9rem; margin-bottom: 0.5rem">
          选择官方编译生成的固件 <code>OTA_All.bin</code> 进行无线上传并更新系统。
        </p>
        <div style="display: flex; gap: 0.5rem; align-items: center; flex-wrap: wrap;">
          <input type="file" ref="otaFileInput" accept=".bin" style="margin-bottom: 0; flex: 1" :disabled="otaUploading" />
          <button @click="startOta" :disabled="otaUploading" :aria-busy="otaUploading" class="" style="width: auto; margin-bottom: 0">
            {{ otaUploading ? '上传中...' : '开始更新' }}
          </button>
        </div>
        <div v-if="otaProgress !== null" style="margin-top: 0.5rem">
          <progress :value="otaProgress" max="100"></progress>
          <small style="font-size: 0.85rem">{{ otaStatusText }}</small>
        </div>
      </div>
      
      <footer>
        <small>在本页调整软件的各项设置</small>
      </footer>
    </article>
  </section>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { message } from '../utils/message'

const otaFileInput = ref<HTMLInputElement | null>(null)
const otaUploading = ref(false)
const otaProgress = ref<number | null>(null)
const otaStatusText = ref('')

const startOta = () => {
  if (!otaFileInput.value || !otaFileInput.value.files || otaFileInput.value.files.length === 0) {
    message.error("请先选择固件文件 (OTA_All.bin)")
    return
  }
  const file = otaFileInput.value.files[0]
  otaUploading.value = true
  otaProgress.value = 0
  otaStatusText.value = '正在准备上传...'

  const xhr = new XMLHttpRequest()
  xhr.open('POST', '/api/ota', true)
  xhr.setRequestHeader('Content-Type', 'application/octet-stream')

  xhr.upload.onprogress = (e) => {
    if (e.lengthComputable) {
      otaProgress.value = Math.round((e.loaded / e.total) * 100)
      otaStatusText.value = `已上传 ${otaProgress.value}%`
    }
  }

  xhr.onload = () => {
    otaUploading.value = false
    if (xhr.status === 200) {
      try {
        const response = JSON.parse(xhr.responseText)
        if (response.success) {
          message.success("固件升级成功，设备将在 3 秒内重启！")
          otaStatusText.value = '更新成功，设备正在重启...'
        } else {
          message.error(response.message || "固件更新失败")
          otaStatusText.value = `更新失败: ${response.message}`
        }
      } catch (err) {
        message.error("解析服务器响应失败")
        otaStatusText.value = '更新失败：解析服务器响应失败'
      }
    } else {
      message.error(`HTTP 错误 ${xhr.status}`)
      otaStatusText.value = `上传错误 (HTTP ${xhr.status})`
    }
  }

  xhr.onerror = () => {
    otaUploading.value = false
    message.error("网络连接错误，升级失败")
    otaStatusText.value = '网络错误'
  }

  xhr.send(file)
}
</script>
