import { readFileSync, writeFileSync, mkdirSync } from 'fs'
import { gzipSync } from 'zlib'
import { dirname, join } from 'path'
import { fileURLToPath } from 'url'

const __dirname = dirname(fileURLToPath(import.meta.url))
const ROOT = join(__dirname, '..')

const distHtml = join(ROOT, 'frontend', 'dist', 'index.html')
const outDir = join(ROOT, 'include')
const outFile = join(outDir, 'webpage.h')

try {
  const html = readFileSync(distHtml)
  const compressed = gzipSync(html)

  const bytes = Array.from(compressed)
    .map(b => '  0x' + b.toString(16).padStart(2, '0'))
    .join(',\n')

  const header = `#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <Arduino.h>

// Gzip-compressed web page (Pico CSS + Alpine.js)
// Generated automatically - DO NOT EDIT
const uint8_t webpage_gz[] PROGMEM = {
${bytes}
};

const size_t webpage_gz_len = ${compressed.length};

#endif
`

  mkdirSync(outDir, { recursive: true })
  writeFileSync(outFile, header)

  const ratio = ((compressed.length / html.length) * 100).toFixed(1)
  console.log(`OK  ${html.length} B -> ${compressed.length} B (${ratio}%)`)
  console.log(`OK  ${outFile}`)
} catch (err) {
  if (err.code === 'ENOENT') {
    console.error('ERROR: frontend/dist/index.html not found.')
    console.error('  Run "pnpm --dir frontend build" first.')
  } else {
    console.error('ERROR:', err.message)
  }
  process.exit(1)
}
