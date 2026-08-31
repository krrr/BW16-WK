<template>
  <main class="container">
    <nav class="container-fluid" style="margin-top: 1rem;">
      <ul>
        <li><strong>BW16 Stealth Deauther</strong></li>
      </ul>
      <ul class="tabs">
        <router-link to="/home">
          <li>Home</li>
        </router-link>
        <router-link to="/scan">
          <li>Scan</li>
        </router-link>
        <router-link to="/attack">
          <li>Attack</li>
        </router-link>
        <router-link to="/settings">
          <li>Settings</li>
        </router-link>
      </ul>
    </nav>

    <!-- Router View -->
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

<style scoped lang="scss">
.tabs > a {
  margin: 0 var(--pico-nav-link-spacing-horizontal);
  padding: 0;
  position: relative;
  height: 1.8rem;

  > li {
    padding: 0;
    margin: 0;
    color: #373c44
  }
  a:hover {
    text-decoration: none;
    font-weight: 600;
  }
}
.tabs > a.router-link-active {
  > li {
    color: #373c44;
    font-weight: 600;
    text-decoration: none;
  }
  > li::before {
    content: "​";
    background: var(--pico-primary);;
    width: 100%;
    display: block;
    height: 4px;
    position: absolute;
    bottom: 0;
    left: 0;
  }
}
</style>
