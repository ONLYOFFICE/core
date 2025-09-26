import {
    type AesKeyGenLength,
    type AesType, aesTypes,
    type AlgorithmType,
    algorithmTypes,
    type DigestType,
    digestTypes,
    isRsaAlgorithm, type JSONAesGcmParams, type RSAJSONType,
    type RsaType
} from "./key-types.ts";
import {ab2base64, base642ui} from "../utils.ts";

export const getKeyParamsFromJson = (keyParamsJson) => {
    const name = keyParamsJson.name;
    if (isRsaAlgorithm(name)) {
        return new RSAImportParams(name, keyParamsJson.hash);
    }
    throw new Error("Unknown param type");
};


export class AlgorithmParams<TName extends AlgorithmType = AlgorithmType> {
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
    getImportParams() {
        return new AlgorithmParams(this.name);
    }
}

export class RSAImportParams extends AlgorithmParams<RsaType> {
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

export class RSAKeyGenParams extends RSAImportParams {
    modulusLength;
    publicExponent;
    constructor(name: RsaType, hash: DigestType = digestTypes.SHA256, modulusLength = 2048, publicExponent = new Uint8Array([0x01, 0x00, 0x01])) {
        super(name, hash);
        this.modulusLength = modulusLength;
        this.publicExponent = publicExponent;
    }
    override getImportParams() {
        return new RSAImportParams(this.name, this.hash);
    }
}
export class Ed25519ImportParams extends AlgorithmParams {
    constructor() {
        super(algorithmTypes.ED25519);
    }
}

export class AesImportParams extends AlgorithmParams<AesType> {
    constructor(name: AesType) {
        super(name);
    }
}

export class AesGcmParams {
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

export class AesKeyGenParams extends AesImportParams {
    length: AesKeyGenLength;
    constructor(name: AesType, length: AesKeyGenLength) {
        super(name);
        this.length = length;
    }
    override getImportParams() {
        return new AesImportParams(this.name);
    }
}
export class AesGcmGenParams extends AesKeyGenParams {
    constructor() {
        super(aesTypes.AES_GCM, 256);
    }
}
export class Ed25519KeyGenParams extends Ed25519ImportParams {
    override getImportParams() {
        return new Ed25519ImportParams();
    }
}

