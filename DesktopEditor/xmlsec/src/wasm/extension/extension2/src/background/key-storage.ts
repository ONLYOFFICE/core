import browser from "webextension-polyfill";
import getCrypto from "../common/crypto.ts";
import {getGUID} from "./utils.ts";

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
function invertObject<T extends Record<string, string>>(object: T) {
    return Object.fromEntries(
        Object.entries(object).map(([k, v]) => [v, k])
    ) as { [K in keyof T as T[K]]: K };
}

class Ed25519KeyGenParams {
    name = algorithmTypes.ED25519;
}
type RSAJSONType = ReturnType<RSAImportParams["toJSON"]>;
class AlgorithmParams<TName extends AlgorithmType = AlgorithmType> {
    name: TName;
    constructor(name: TName) {
        this.name = name;
    }
    toJSON() {
        return {
            name: this.name
        };
    };
    fromJSON(json: {name: TName}) {
        this.name = json.name;
    }
}
class RSAImportParams extends AlgorithmParams<RsaType> {
    hash;
    constructor(name: RsaType, hash: DigestType = digestTypes.SHA256) {
        super(name);
        this.hash = hash;
    }
    override toJSON() {
        return {
            name: this.name,
            hash: this.hash,
        }
    }
    override fromJSON(json: RSAJSONType) {
        this.name = json.name;
        this.hash = json.hash;
    }
}
class RSAKeyGenParams extends RSAImportParams {
    modulusLength;
    publicExponent;
    constructor(name: RsaType, hash: DigestType = digestTypes.SHA256, modulusLength = 2048, publicExponent = new Uint8Array([0x01, 0x00, 0x01])) {
        super(name, hash);
        this.modulusLength = modulusLength;
        this.publicExponent = publicExponent;
    }
}
type KeyParams = RSAImportParams;
type KeyJSON<T extends AlgorithmParams> = {
    guid: string;
    params: RSAImportParams;
};
class Key<T extends AlgorithmParams = AlgorithmParams> {
    guid = getGUID();
    params: KeyParams;
    constructor(params: KeyParams) {
        this.params = params;
    }
    async toJSON(_masterPassword?: string): Promise<KeyJSON<T>> {
        return {
            guid: this.guid,
            params: this.params.toJSON()
        };
    }
    async fromJSON(json: Awaited<ReturnType<Key["toJSON"]>>, _masterPassword?: string) {
        this.guid = json.guid;
        this.params = getKeyParamsFromJson(json.params);
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
type ExportKeyFormat = typeof exportKeyFormats[keyof typeof exportKeyFormats];
const algorithmTypes = {
    AES_GCM: "AES-GCM",
    AES_CTR: "AES-CTR",
    AES_CBC: "AES-CBC",
    AES_KW: "AES-KW",
    ED25519: "Ed25519",
    RSASSA_PKCS1_v1_5: "RSASSA-PKCS1-v1_5",
    RSA_PSS: "RSA-PSS",
    RSA_OAEP: "RSA-OAEP"
} as const;
type AlgorithmType = typeof algorithmTypes[keyof typeof algorithmTypes];
const rsaTypes = {
    RSASSA_PKCS1_v1_5: algorithmTypes.RSASSA_PKCS1_v1_5,
    RSA_PSS: algorithmTypes.RSA_PSS,
    RSA_OAEP: algorithmTypes.RSA_OAEP,
} as const;

type RsaType = typeof rsaTypes[keyof typeof rsaTypes];

const aesTypes = {
    AES_GCM: algorithmTypes.AES_GCM,
    AES_CTR: algorithmTypes.AES_CTR,
    AES_CBC: algorithmTypes.AES_CBC,
    AES_KW: algorithmTypes.AES_KW,
} as const;

type AesType = typeof aesTypes[keyof typeof aesTypes];


const digestTypes = {
    SHA1: "SHA-1",
    SHA256: "SHA-256",
    SHA384: "SHA-384",
    SHA512: "SHA-512",
} as const;
const isRsaAlgorithm = (type: string): type is RsaType => {
    return Object.values(rsaTypes).includes(type as RsaType);
}
    type DigestType = typeof digestTypes[keyof typeof digestTypes];


type JSONAesGcmParams = ReturnType<AesGcmParams["toJSON"]>;

class AesImportParams extends AlgorithmParams<AesType> {
    constructor(name: AesType) {
        super(name);
    }
}
type AesKeyGenLength = 128 | 192 | 256;
class AesKeyGenParams extends AesImportParams {
    length: AesKeyGenLength;
    constructor(name, length: AesKeyGenLength) {
        super(name);
        this.length = length;
    }
}
class AesGcmParams {
    name = algorithmTypes.AES_GCM;
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
    fromJSON(json: JSONAesGcmParams) {
        this.iv = base642ui(json.iv);
        this.additionalData = typeof json.additionalData === "string" ? base642ui(json.additionalData) : undefined;
        this.tagLength = json.tagLength;
    };
}
type JSONPrivateKey<T extends AlgorithmParams> = {
    format: ExportKeyFormat;
    key: string;
    salt: string;
    params: ReturnType<T["toJSON"]>;
    guid: string;
    wrapParams: ReturnType<AesGcmParams["toJSON"]>;

}

const getKeyParamsFromJson = (keyParamsJson) => {
    const name = keyParamsJson.name;
    if (isRsaAlgorithm(name)) {
        return new RSAImportParams(name, keyParamsJson.hash);
    }
    throw new Error("Unknown param type");
};
class PrivateKey<T extends AlgorithmParams = AlgorithmParams> extends Key<T> {
    type = "private";
    salt;
    key;
    constructor(key: string, salt: string, params: T) {
        super(params);
        this.key = key;
        this.salt = salt;
    }
    override async toJSON(masterPassword: string): Promise<JSONPrivateKey<T>> {
        const crypto = getCrypto();
        const iv = crypto.getRandomValues(12);
        const aesParams = new AesGcmParams(iv);
        const wrapKey = await crypto.wrapKey(exportKeyFormats.pkcs8, this.key, masterPassword, this.salt, aesParams);
        const base64WrapKey = ab2base64(wrapKey);
        const params = this.params.toJSON();
        const wrapParams = aesParams.toJSON();
        return {
          format: exportKeyFormats.pkcs8,
            key: base64WrapKey,
            salt: btoa(this.salt),
            params,
            wrapParams,
            guid: this.guid
        };
    }
    override async fromJSON(json:  JSONPrivateKey, masterPassword: string) {
        this.salt = atob(json.salt);
        const params = getKeyParamsConstructorByName(json.params.name);
        params.fromJSON(json.params);
        this.params = params;
        const crypto = getCrypto();
        const strWrapKey = json.key;
        const wrapKey = base642ui(strWrapKey);
        const wrapParams = new AesGcmParams();
        wrapParams.fromJSON(json.wrapParams);

        const cryptoKey = crypto.unwrapKey(exportKeyFormats.pkcs8, wrapKey, masterPassword, this.salt, wrapParams, params);
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