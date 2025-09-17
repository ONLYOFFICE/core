import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import {crx} from "@crxjs/vite-plugin";
import manifest from "./manifest.config.ts";

// https://vite.dev/config/
export default defineConfig({
  plugins: [crx({ manifest }), react()],
    build: {
        sourcemap: 'inline'
    }
})
