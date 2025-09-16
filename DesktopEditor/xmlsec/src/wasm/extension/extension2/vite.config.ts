import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
    build: {
      rollupOptions: {
          input: {
              popup: "./src/popup/index.html",
              background: "./src/background/background.ts",
              content: "./src/content/content.ts",
          },
          output: {
              entryFileNames: "js/[name].js",
          },
      },
      minify: false,
        cssMinify: false,
        sourcemap: 'inline'
    }
})
