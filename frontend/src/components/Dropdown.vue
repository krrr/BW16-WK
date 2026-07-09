<template>
  <details :class="['dropdown', `align-${align}`]" ref="detailsRef" :style="customStyle">
    <summary :role="summaryRole" :class="summaryClass" style="margin: 0;">
      <slot name="trigger">
        {{ label }}
      </slot>
    </summary>
    <ul @click="closeDropdown">
      <slot></slot>
    </ul>
  </details>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted } from 'vue'

defineProps({
  label: {
    type: String,
    default: '​'
  },
  summaryClass: {
    type: String,
    default: 'outline secondary'
  },
  summaryRole: {
    type: String,
    default: 'button'
  },
  customStyle: {
    type: [String, Object, Array] as any,
    default: undefined
  },
  align: {
    type: String as () => 'left' | 'right',
    default: 'left'
  }
})

const detailsRef = ref<HTMLDetailsElement | null>(null)

const closeDropdown = () => {
  if (detailsRef.value) {
    detailsRef.value.removeAttribute('open')
  }
}

const handleClickOutside = (event: MouseEvent) => {
  if (detailsRef.value && !detailsRef.value.contains(event.target as Node)) {
    closeDropdown()
  }
}

onMounted(() => {
  document.addEventListener('click', handleClickOutside)
})

onUnmounted(() => {
  document.removeEventListener('click', handleClickOutside)
})
</script>

<style scoped>
/* Ensure dropdown list doesn't overlap or look misaligned if used in tight spaces */
details.dropdown {
  display: inline-block;
  margin: 0;
}

details.dropdown.align-right ul {
  left: auto;
  right: 0;
}
</style>
