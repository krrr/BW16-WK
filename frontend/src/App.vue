<template>
  <main class="container">
    <nav class="container-fluid" style="margin-top: 1rem;">
      <ul>
        <li><strong>BW16 WiFi Killer</strong></li>
      </ul>
      <ul class="tabs">
        <li :class="{ selected: $route.path === '/home' }">
          <router-link to="/home">首页</router-link>
        </li>
        <li :class="{ selected: $route.path === '/scan' }">
          <router-link to="/scan">扫描</router-link>
        </li>
      </ul>
    </nav>

    <!-- 路由视图 -->
    <router-view></router-view>
  </main>

  <!-- Floating Message Notifications -->
  <div class="message-container">
    <div
      v-for="msg in items"
      :key="msg.id"
      class="message-item"
      :class="'message-' + msg.type"
    >
      <span class="message-icon" v-html="icons[msg.type]"></span>
      <span class="message-content">{{ msg.text }}</span>
    </div>
  </div>
</template>

<script setup lang="ts">
import { message } from './utils/message'
import successSvg from './assets/success.svg?raw'
import errorSvg from './assets/error.svg?raw'
import warningSvg from './assets/warning.svg?raw'

const items = message.items
const icons: Record<string, string> = {
  success: successSvg,
  error: errorSvg,
  warning: warningSvg
}
</script>
