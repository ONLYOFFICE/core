import {getGUID} from "../../background/utils.ts";
import {ab2base64, base642ui, str2ui} from "../utils.ts";
import {
    type ExportKeyFormat,
    exportKeyFormats, type JSONKey, type JSONKeyPair, type JSONPrivateKey, type JSONPublicKey,
    type KeyParams,
    keyTypes,
    pairKeyTypes
} from "./key-types.ts";
import getCrypto from "../crypto.ts";
import {AesGcmParams, getKeyParamsFromJson} from "./params.ts";

export class Key {
    guid;
    params: KeyParams;
    key;
    exportFormat;
    constructor(key: ArrayBuffer, params: KeyParams, guid: string = getGUID(), exportFormat: ExportKeyFormat) {
        this.key = key;
        this.params = params;
        this.guid = guid;
        this.exportFormat = exportFormat;
    }
    static async fromJSON(json: JSONKey, _masterPassword?: string, _keyUsage?: KeyUsages) {
        const params = getKeyParamsFromJson(json.params);
        const key = base642ui(json.key);
        return new this(key, params, json.guid, exportKeyFormats.raw);
    }
    async toJSON(_masterPassword?: string, _keyUsage?: KeyUsages) {
        const key = ab2base64(this.key);
        return {
            guid: this.guid,
            params: this.params.toJSON(),
            key: key
        };
    }
}
export class SymmetricKey extends Key {
    type = keyTypes.symmetric;
    keyUsages;
    constructor(key: ArrayBuffer, params: KeyParams, keyUsage = new KeyUsages(true), guid?: string) {
        super(key, params, guid, exportKeyFormats.raw);
        this.keyUsages = keyUsage;
    }
}
export class PublicKey extends Key {
    type = pairKeyTypes.public;
    constructor(key: ArrayBuffer, params: KeyParams, guid?: string) {
        super(key, params, guid, exportKeyFormats.spki);
    }
    static override async fromJSON(json: JSONPublicKey) {
        const params = getKeyParamsFromJson(json.params);
        const key = base642ui(json.key);
        return new PublicKey(key, params, json.guid);
    }
    override async toJSON() {
        const params = this.params.toJSON();
        const base64Key = ab2base64(this.key);
        return {
            format: exportKeyFormats.spki,
            key: base64Key,
            params,
            guid: this.guid
        };
    }
}

export class PrivateKey extends Key {
    type = pairKeyTypes.private;
    salt;
    constructor(key: ArrayBuffer, params: KeyParams, salt: ArrayBuffer, guid?: string) {
        super(key, params, guid, exportKeyFormats.pkcs8);
        this.salt = salt;
    }
    static override async fromJSON(json: JSONPrivateKey, masterPassword: string, keyUsage: KeyUsages) {
        const salt = base642ui(json.salt);
        const params = getKeyParamsFromJson(json.params);
        const guid = json.guid;
        const crypto = getCrypto();
        const strWrapKey = json.key;
        const wrapKey = base642ui(strWrapKey);
        const wrapParams = new AesGcmParams();
        wrapParams.fromJSON(json.wrapParams);
        const key = await crypto.unwrapKey(exportKeyFormats.pkcs8, wrapKey, str2ui(masterPassword), salt, wrapParams, params, keyUsage);

        return new PrivateKey(key, params, salt, guid);
    }
    override async toJSON(masterPassword: string, keyUsage: KeyUsages) {
        const crypto = getCrypto();
        const iv = crypto.getRandomValues(12);
        const aesParams = new AesGcmParams(iv);
        const wrapKey = await crypto.wrapKey(this.exportFormat, this, str2ui(masterPassword), this.salt, aesParams, keyUsage);
        const base64WrapKey = ab2base64(wrapKey);
        const params = this.params.toJSON();
        const wrapParams = aesParams.toJSON();
        return {
            format: this.exportFormat,
            key: base64WrapKey,
            salt: ab2base64(this.salt),
            params,
            wrapParams,
            guid: this.guid
        };
    }
}

export class KeyPair {
    privateKey;
    publicKey;
    date;
    type = keyTypes.pair;
    keyUsage;
    static async fromJSON(json: JSONKeyPair, masterPassword: string) {
        const keyUsage = KeyUsages.fromJSON(json.keyUsage);
        const publicKey = await PublicKey.fromJSON(json.publicKey);
        const privateKey = await PrivateKey.fromJSON(json.privateKey, masterPassword, keyUsage);
        const date = new Date(json.date);
        return new KeyPair(publicKey, privateKey, keyUsage, date);
    }
    constructor(publicKey: PublicKey, privateKey: PrivateKey, keyUsage = new KeyUsages(true), date = new Date()) {
        this.privateKey = privateKey;
        this.publicKey = publicKey;
        this.date = date;
        this.keyUsage = keyUsage;
    }
    async toJSON(masterPassword: string) {
        return {
            publicKey: await this.publicKey.toJSON(),
            privateKey: await this.privateKey.toJSON(masterPassword, this.keyUsage),
            date: this.date.toISOString(),
            keyUsage: this.keyUsage.toJSON()
        }
    }
}
export class KeyUsages {
    isEncrypt;
    isSign;
    constructor(isEncrypt?: boolean, isSign?: boolean) {
        this.isEncrypt = !!isEncrypt;
        this.isSign = !!isSign;
    }
    static fromJSON(json: ReturnType<KeyUsages["toJSON"]>) {
        return new KeyUsages(json.isEncrypt, json.isSign);
    }
    toJSON() {
        return {
            isEncrypt: this.isEncrypt,
            isSign: this.isSign
        }
    }
}