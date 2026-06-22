import { defineConfig } from 'vite'
import { viteSingleFile } from 'vite-plugin-singlefile'
import injectHTML from 'vite-plugin-html-inject'

export default defineConfig({
  plugins: [injectHTML(), viteSingleFile()],
  server: {
    proxy: {
      '/api/': {
        target: 'http://192.168.4.1',
        changeOrigin: true,
      },
    },
  },
  build: {
    cssCodeSplit: false,
    assetsInlineLimit: 100000000,
    rollupOptions: {
      output: {
        manualChunks: undefined,
        inlineDynamicImports: true,
      },
    },
  },
})
