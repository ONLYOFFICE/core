import { defineConfig } from 'vite'
import { resolve } from 'path'

export default defineConfig({
	build: {
		lib: {
			formats: ['iife', 'es'],
			entry: resolve(__dirname, 'src/index.js'),
			name: 'KeyStorageLibrary',
			fileName: (format) => `key-storage.${format}.js`
		},
		rollupOptions: {
			output: {
				generatedCode: "es5"
			}
		},
		minify: false
	},
})