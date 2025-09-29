import browser from "webextension-polyfill";
import type {JSONExportKeyFormat, JSONKeyPair} from "./keys/key-types.ts";
import {KeyPair} from "./keys/keys.ts";
import {selectUserJSON} from "./utils.ts";

export default class KeyStorage {
    keys: KeyPair[] = [];
    async loadKeysFromStorage() {
        const item = await browser.storage.local.get("keys");
        if (item && Array.isArray(item.keys)) {
            this.keys = await this.loadKeys(item.keys);
        }
    }

    async loadKeys(jsonKeys: JSONKeyPair[]) {
        const keyPromises = [];
        const masterPassword = await this.getMasterPassword();
        if (typeof masterPassword !== "string") {
            return [];
        }
        for (const key of jsonKeys) {
            keyPromises.push(KeyPair.fromJSON(key, masterPassword));
        }
        return Promise.all(keyPromises);
    };
    async addNewKeys(keys: KeyPair[]) {
        this.keys.push.apply(this.keys, keys);
        await this.writeKeys();
    }
    async getJsonKeys(encryptPassword?: string) {
        const jsonKeyPromises = [];
        const masterPassword = typeof encryptPassword === "string" ? encryptPassword : await this.getMasterPassword();
        if (typeof masterPassword !== "string") {
            return [];
        }
        for (const key of this.keys) {
            jsonKeyPromises.push(key.toJSON(masterPassword));
        }
        return Promise.all(jsonKeyPromises);
    };
    async writeKeys() {
        const jsonKeys = await this.getJsonKeys();
        await this.setStorageKeys(jsonKeys);
    };
    async setStorageKeys(jsonKeys: JSONKeyPair[]) {
        await browser.storage.local.set({keys: jsonKeys});
    };
    async getMasterPassword() {
        const item = await browser.storage.local.get("masterPassword");
        return item.masterPassword ? item.masterPassword : null;
    };
    async changeMasterPassword(newMasterPassword: string) {
        const keys = await this.getJsonKeys(newMasterPassword);
        return browser.storage.local.set({masterPassword: newMasterPassword, keys});
    };
    async exportKeys() {
        const passwordInfo : JSONExportKeyFormat = {
            encrypt: false,
            data: await this.getJsonKeys()
        };
        const blob = new Blob([JSON.stringify(passwordInfo)], { type: "application/json"});
        const url = URL.createObjectURL(blob);

        const link = document.createElement("a");
        link.href = url;
        link.download = `onlyoffice_keychain_${(new Date()).toISOString()}.json`;
        link.click();

        URL.revokeObjectURL(url);
    };
    importKeys(callback: () => void) {
        selectUserJSON(async (file: File) => {
            try {
                const text = await file.text();
                const json: JSONExportKeyFormat = JSON.parse(text);
                if (!json.encrypt) {
                    const keyObjects = await this.loadKeys(json.data);
                    await this.addNewKeys(keyObjects);
                    callback();
                }
            } catch (e) {

            }
        });
    };
    getKeyPairByGuid(guid: string) {
        for (let i = 0; i < this.keys.length; i++) {
            const key = this.keys[i];
            if (guid === key.guid) {
                return key;
            }
        }
    };
    getValidKeys() {
        const keys = [];
        for (let i = 0; i < this.keys.length; i++) {
            const key = this.keys[i];
            if (key.isValid) {
                keys.push(key);
            }
        }
        return keys;
    }
    deprecateKey(key: KeyPair) {
        key.setIsValid(false);
        return this.writeKeys();
    };
}