import { createRouter, createWebHashHistory } from 'vue-router'
import HomeView from './views/HomeView.vue'
import ScanView from './views/ScanView.vue'

const routes = [
  { path: '/', redirect: '/home' },
  { path: '/home', component: HomeView },
  { path: '/scan', component: ScanView },
]

export const router = createRouter({
  history: createWebHashHistory(),
  routes,
})
