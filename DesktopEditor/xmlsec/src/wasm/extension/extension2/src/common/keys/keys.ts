import {getGUID} from "../../background/utils.ts";
import {ab2base64, base642ui} from "../utils.ts";
import {exportKeyFormats, type KeyParams, keyTypes, pairKeyTypes} from "./key-types.ts";
import getCrypto from "../crypto.ts";
import {AesGcmParams, getKeyParamsFromJson} from "./params.ts";

export class Key<T extends KeyParams> {
    guid;
    params: KeyParams;
    key;
    constructor(key: ArrayBuffer, params: T, guid: string = getGUID()) {
        this.key = key;
        this.params = params;
        this.guid = guid;
    }
    static async fromJSON<P extends KeyParams>(json: Awaited<ReturnType<Key<P>["toJSON"]>>, _masterPassword?: string) {
        const params = getKeyParamsFromJson(json.params);
        const key = base642ui(json.key);
        return new this(key, params, json.guid);
    }
    async toJSON(_masterPassword?: string) {
        const key = ab2base64(this.key);
        return {
            guid: this.guid,
            params: this.params.toJSON(),
            key: key
        };
    }
}
export class SymmetricKey<T extends KeyParams = KeyParams> extends Key<T> {
    type = keyTypes.symmetric;
    constructor(key: ArrayBuffer, params: T, guid?: string) {
        super(key, params, guid);
    }
}
export class PublicKey<T extends KeyParams = KeyParams> extends Key<T> {
    type = pairKeyTypes.public;
    constructor(key: ArrayBuffer, params: T, guid?: string) {
        super(key, params, guid);
    }
    static override async fromJSON<P extends KeyParams>(json: Awaited<ReturnType<PublicKey<P>["toJSON"]>>) {
        const params = getKeyParamsFromJson(json.params);
        const key = base642ui(json.key);
        return new PublicKey<typeof params>(key, params, json.guid);
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

export class PrivateKey<T extends KeyParams = KeyParams> extends Key<T> {
    type = pairKeyTypes.private;
    salt;
    constructor(key: ArrayBuffer, params: T, salt: string, guid?: string) {
        super(key, params, guid);
        this.salt = salt;
    }
    static override async fromJSON<P extends KeyParams>(json: Awaited<ReturnType<PrivateKey<P>["toJSON"]>>, masterPassword: string) {
        const salt = atob(json.salt);
        const params = getKeyParamsFromJson(json.params);
        const guid = json.guid;
        const crypto = getCrypto();
        const strWrapKey = json.key;
        const wrapKey = base642ui(strWrapKey);
        const wrapParams = new AesGcmParams();
        wrapParams.fromJSON(json.wrapParams);
        const key = await crypto.unwrapKey(exportKeyFormats.pkcs8, wrapKey, masterPassword, salt, wrapParams, params);

        return new PrivateKey<typeof params>(key, params, salt, guid);
    }
    override async toJSON(masterPassword: string) {
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
}

export class KeyPair<T extends KeyParams = KeyParams> {
    privateKey;
    publicKey;
    date;
    type = keyTypes.pair;
    static async fromJSON<P extends KeyParams>(json: Awaited<ReturnType<KeyPair<P>["toJSON"]>>, masterPassword: string) {
        const publicKey = await PublicKey.fromJSON<P>(json.publicKey);
        const privateKey = await PrivateKey.fromJSON<P>(json.privateKey, masterPassword);
        const date = new Date(json.date);
        return new KeyPair(publicKey, privateKey, date);
    }
    constructor(publicKey: PublicKey<T>, privateKey: PrivateKey<T>, date = new Date()) {
        this.privateKey = privateKey;
        this.publicKey = publicKey;
        this.date = date;
    }
    async toJSON(masterPassword: string) {
        return {
            publicKey: await this.publicKey.toJSON(),
            privateKey: await this.privateKey.toJSON(masterPassword),
            date: this.date.toISOString()
        }
    }
}