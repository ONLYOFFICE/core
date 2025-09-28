import browser from "webextension-polyfill";
import type {JSONKeyPair} from "../common/keys/key-types.ts";
import {KeyPair} from "../common/keys/keys.ts";

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
    async getJsonKeys() {
        const jsonKeyPromises = [];
        const masterPassword = await this.getMasterPassword();
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
    }
    async setStorageKeys(jsonKeys: JSONKeyPair[]) {
        await browser.storage.local.set({keys: jsonKeys});
    }
    async getMasterPassword() {
        const item = await browser.storage.local.get("masterPassword");
        return item.masterPassword ? item.masterPassword : null;
    };
    async compareWithCurrentMasterPassword(masterPassword: string) {
        const currentMasterPassword = await this.getMasterPassword();
        return masterPassword === currentMasterPassword;
    }
    async changeMasterPassword(oldMasterPassword: string, newMasterPassword: string) {
        const isOldMasterPasswordEqualsWithCurrent = await this.compareWithCurrentMasterPassword(oldMasterPassword);
        if (isOldMasterPasswordEqualsWithCurrent) {
            const keys = await this.getJsonKeys();
            await browser.storage.local.set({masterPassword: newMasterPassword, keys});
            return true;
        }
        return false;
    }
    async exportKeys({encrypt=false, password: string}) {
        const passwordInfo = {
            encrypt: encrypt,
            data: null
        };
        if (encrypt) {

        } else {
            passwordInfo.data = await this.getJsonKeys();
        }
        const blob = new Blob([JSON.stringify(passwordInfo)], { type: "application/json"});
        const url = URL.createObjectURL(blob);

        const link = document.createElement("a");
        link.href = url;
        link.download = `onlyoffice_keychain_${(new Date()).toISOString()}.json`;
        link.click();

        URL.revokeObjectURL(url);
    }
    async importKeys(jsonKeys: JSONKeyPair[]) {
        const keyObjects = await this.loadKeys(jsonKeys);
        await this.addNewKeys(keyObjects);
    }
}