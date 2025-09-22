import browser from "webextension-polyfill";
import getCrypto from "../common/crypto.ts";
function ab2str(buf) {
    return String.fromCharCode.apply(null, new Uint8Array(buf));
}
class Key {

    async toJSON() {
        return {};
    }
    async fromJSON(_json: {}) {
    }
}

class PublicKey extends Key {
    type = "public";
    override async toJSON() {
        const res = {
          fo
        };
    }
    override async fromJSON(_json: {}) {

    }

}
class PrivateKey extends Key {
    type = "private";
    key: CryptoKey;
    override async toJSON(aesKey) {
        const crypto = getCrypto();
        const iv = crypto.getRandomValues(12);
        const salt = crypto.generateSalt();
        const wrapKey = await crypto.wrapKey("pkcs8", this.key, aesKey, {name: "AES-GCM", iv});
        const strWrapKey = ab2str(wrapKey);
        const base64WrapKey = btoa(strWrapKey);
        return {
          format: "pkcs8",
          salt: salt,
            key: base64WrapKey,
            keyWrappingAlgo: {

            }
        };
    }
    override async fromJSON(_json: {}, aesKey) {

    }

}


class Keys {

}
class SignKeys extends Keys {

}

class CipherKeys extends Keys {

}

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
    async getMasterSalt() {
        const item = await browser.storage.local.get("masterPasswordSalt");
        return item.masterPasswordSalt ? item.masterPasswordSalt : null;
    };
    async getMasterPasswordAesKey() {
        if (!this.aesMasterKey) {
            const masterPassword = await this.getMasterPassword();
            const masterPasswordSalt = await this.getMasterSalt();
            const crypto = getCrypto();
            this.aesMasterKey = await crypto.getAesCryptoKeyFromMasterPassword(masterPassword, masterPasswordSalt);
        }
        return this.aesMasterKey;
    };
    async compareWithCurrentMasterPassword(masterPassword) {
        const currentMasterPassword = await this.getMasterPassword();
        return masterPassword === currentMasterPassword;
    }
    async changeMasterPassword(oldMasterPassword, newMasterPassword) {
        const isOldMasterPasswordEqualsWithCurrent = await this.compareWithCurrentMasterPassword(oldMasterPassword);
        if (isOldMasterPasswordEqualsWithCurrent) {
            const keys = await this.getJsonKeys();
            const salt =
            await browser.storage.local.set({masterPassword: newMasterPassword, masterPasswordSalt: newSalt, keys});
            return true;
        }
        return false;
    }
    async exportKeys({encrypt=false, password}) {
        const passwordInfo = {
            encrypt: encrypt,
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