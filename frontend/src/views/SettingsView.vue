<template>
  <section>
    <article>
      <header>
        <img src="../assets/setting.svg" />
        <h1>Settings</h1>
      </header>

      <div>
        <h4 style="margin-bottom: 0.5rem">OTA Firmware Update</h4>
        <p style="font-size: 0.9rem; margin-bottom: 0.5rem">
          Select the compiled firmware <code>OTA_All.bin</code> to upload wirelessly and update the system.
        </p>
        <div style="display: flex; gap: 0.5rem; align-items: center; flex-wrap: wrap;">
          <input type="file" ref="otaFileInput" accept=".bin" style="margin-bottom: 0; flex: 1" :disabled="otaUploading" />
          <button @click="startOta" :disabled="otaUploading" :aria-busy="otaUploading" class="" style="width: auto; margin-bottom: 0">
            {{ otaUploading ? 'Uploading...' : 'Start Update' }}
          </button>
        </div>
        <div v-if="otaProgress !== null" style="margin-top: 0.5rem">
          <progress :value="otaProgress" max="100"></progress>
          <small style="font-size: 0.85rem">{{ otaStatusText }}</small>
        </div>
      </div>
      
      <hr style="margin: 1.5rem 0" />

      <div>
        <h4 style="margin-bottom: 0.5rem">System Control</h4>
        <p style="font-size: 0.9rem; margin-bottom: 0.5rem">
          Manually trigger a hardware reboot for the microcontroller.
        </p>
        <button @click="rebootDevice" :disabled="rebooting" :aria-busy="rebooting" class="outline secondary" style="width: auto; margin-bottom: 0">
          {{ rebooting ? 'Rebooting...' : 'Reboot Device' }}
        </button>
      </div>

      <footer>
        <small>Configure software settings on this page</small>
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
const rebooting = ref(false)

const rebootDevice = async () => {
  if (!confirm('Are you sure you want to reboot the device?')) {
    return
  }
  rebooting.value = true
  try {
    const res = await fetch('/api/reboot', { method: 'POST' })
    const data = await res.json()
    if (data.success) {
      message.success('Device is rebooting...')
    } else {
      message.error(data.message || 'Failed to reboot device')
      rebooting.value = false
    }
  } catch (err) {
    message.error('Failed to send reboot request')
    rebooting.value = false
  }
}

const startOta = () => {
  if (!otaFileInput.value || !otaFileInput.value.files || otaFileInput.value.files.length === 0) {
    message.error("Please select a firmware file (OTA_All.bin) first")
    return
  }
  const file = otaFileInput.value.files[0]
  otaUploading.value = true
  otaProgress.value = 0
  otaStatusText.value = 'Preparing upload...'

  const xhr = new XMLHttpRequest()
  xhr.open('POST', '/api/ota', true)
  xhr.setRequestHeader('Content-Type', 'application/octet-stream')

  xhr.upload.onprogress = (e) => {
    if (e.lengthComputable) {
      otaProgress.value = Math.round((e.loaded / e.total) * 100)
      otaStatusText.value = `Uploaded ${otaProgress.value}%`
    }
  }

  xhr.onload = () => {
    otaUploading.value = false
    if (xhr.status === 200) {
      try {
        const response = JSON.parse(xhr.responseText)
        if (response.success) {
          message.success("Firmware updated successfully, device will reboot in 3 seconds!")
          otaStatusText.value = 'Update successful, device is rebooting...'
        } else {
          message.error(response.message || "Firmware update failed")
          otaStatusText.value = `Update failed: ${response.message}`
        }
      } catch (err) {
        message.error("Failed to parse server response")
        otaStatusText.value = 'Update failed: Failed to parse server response'
      }
    } else {
      message.error(`HTTP error ${xhr.status}`)
      otaStatusText.value = `Upload error (HTTP ${xhr.status})`
    }
  }

  xhr.onerror = () => {
    otaUploading.value = false
    message.error("Network connection error, update failed")
    otaStatusText.value = 'Network error'
  }

  xhr.send(file)
}
</script>
