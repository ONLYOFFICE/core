import browser from "webextension-polyfill";
import getCrypto from "../common/crypto.ts";
function ab2str(buf: ArrayBuffer) {
    return String.fromCharCode.apply(null, Array.from(new Uint8Array(buf)));
}
function ab2base64(buf: ArrayBuffer) {
    const str = ab2str(buf);
    return btoa(str);
}
function base642ui(base64: string) {
    const str = atob(base64);
    return str2ui(str);
}
function str2ui(str: string) {
    const encoder = new TextEncoder();
    return encoder.encode(str);
}
class Key {
    guid: string
    constructor(params) {
        this.params = params;
    }
    async toJSON() {
        return {};
    }
    async fromJSON(_json: {}) {
    }
}

class SymmetricKey extends Key {

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
const exportKeyFormats = {
    pkcs8: "pkcs8",
    spki: "spki",
    raw: "raw"
} as const;
const algorithmTypes = {
    GCM: "AES-GCM"
} as const;
class AesGcmParams{
    name = algorithmTypes.GCM;
    iv: ArrayBuffer;
    additionalData?: ArrayBuffer;
    tagLength?: number;
    constructor(iv?: ArrayBuffer, additionalData?: ArrayBuffer, tagLength?: number) {
        this.iv = iv || new Uint8Array(12);
        this.additionalData = additionalData;
        this.tagLength = tagLength;
    }
    toJSON() {
        return {
            name: this.name,
            iv: ab2base64(this.iv),
            additionalData: this.additionalData && ab2base64(this.additionalData),
            tagLength: this.tagLength
        }
    };
    fromJSON(json: {}) {
        this.iv = base642ui(json.iv);
        this.additionalData = json.add
    };
}

type JSONPrivateKey = {
    format: typeof exportKeyFormats.pkcs8;
    key: string;
    salt: string;
    wrapParams:
};
class PrivateKey extends Key {
    type = "private";
    salt: string;
    key: string;
    override async toJSON(masterPassword: string): Promise<JSONPrivateKey> {
        const crypto = getCrypto();
        const iv = crypto.getRandomValues(12);
        const aesParams = new AesGcmParams(iv);
        const wrapKey = await crypto.wrapKey(exportKeyFormats.pkcs8, this.key, masterPassword, this.salt, aesParams);
        const base64WrapKey = ab2base64(wrapKey);
        const keyParams = this.params.toJSON();
        const wrapParams = aesParams.toJSON();
        return {
          format: exportKeyFormats.pkcs8,
            key: base64WrapKey,
            salt: btoa(this.salt),
            keyParams,
            wrapParams: aesParams.toJSON()
        };
    }
    override async fromJSON(json:  JSONPrivateKey, masterPassword) {
        this.salt = atob(json.salt);
        const keyParams = json.keyParams;
        this.params = keyParams;
        const crypto = getCrypto();
        const strWrapKey = json.key;
        const wrapKey = base642ui(strWrapKey);
        const wrapParams = new AesGcmParams();
        wrapParams.fromJSON(json.wrapParams);

        const cryptoKey = crypto.unwrapKey(exportKeyFormats.pkcs8, wrapKey, masterPassword, this.salt, wrapParams, keyParams);
        this.key = cryptoKey;
    }

}

class KeyPair {
    privateKey: PrivateKey;
    publicKey: PublicKey;
    date: Date
    async toJSON(masterPassword: string) {
        return {
            publicKey: this.publicKey.toJSON(),
            privateKey: this.privateKey.toJSON(masterPassword),
            date: this.date.toISOString()
        }
    }
    async fromJSON(masterPassword: string) {

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