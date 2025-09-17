import { defineManifest } from '@crxjs/vite-plugin';
import pkg from "./package.json";

export default defineManifest({
    manifest_version: 3,
    name: pkg.name,
    version: pkg.version,
    icons: {
        16: "public/icons/icon16.png",
        32: "public/icons/icon32.png",
        48: "public/icons/icon48.png",
        64: "public/icons/icon64.png",
        128: "public/icons/icon128.png",
    },
    action: {
        default_icon: {
            16: "public/icons/icon16.png",
            32: "public/icons/icon32.png",
            48: "public/icons/icon48.png",
            64: "public/icons/icon64.png",
            128: "public/icons/icon128.png",
        },
        default_popup: 'src/popup/index.html',
    },
    content_scripts: [{
        js: ['src/content/content.ts'],
        matches: ['<all_urls>'],
        run_at: "document_end"
    }],
    background: {
        service_worker: "src/background/background.ts"
    },
    permissions: ["storage"],
});