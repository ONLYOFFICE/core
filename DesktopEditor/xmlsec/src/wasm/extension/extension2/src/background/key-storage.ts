import browser from "webextension-polyfill";

const jsonKeysFormat = {
    SIGN: "SIGN",
    CIPHER: "CIPHER"
} as const;

export default class KeyStorage {
    keys: Keys[];
    aesMasterKey?: Key;
    async loadKeysFromStorage() {
        const item = await browser.storage.local.get("keys");
        if (item && Array.isArray(item.keys)) {
            this.keys = await this.loadKeys(item.keys);
        }
    }

    async loadKeys(jsonKeys) {
        const keyPromises = [];
        const keyObjects = [];
        for (const key of jsonKeys) {
            const keysObject = key.format === jsonKeysFormat.CIPHER ? new CipherKeys() : new SignKeys();
            keyObjects.push(keysObject);
            keyPromises.push(keysObject.fromJSON(key));
        }
        await Promise.all(keyPromises);
        return keyObjects;
    };
    async addNewKeys(keys) {
        this.keys.push.apply(this.keys, keys);
        await this.writeKeys();
    }
    async getJsonKeys() {
        const jsonKeyPromises = [];
        for (const key of this.keys) {
            jsonKeyPromises.push(key.toJSON());
        }
        return await Promise.all(jsonKeyPromises);
    };

    async writeKeys() {
        const jsonKeys = await this.getJsonKeys();
        await this.setStorageKeys(jsonKeys);
    }
    async setStorageKeys(jsonKeys) {
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
    async exportKeys({encrypt=false, password}) {
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
    async importKeys(jsonKeys) {
        const keyObjects = await this.loadKeys(jsonKeys);
        await this.addNewKeys(keyObjects);
    }
}