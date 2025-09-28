import {
    type AesKeyGenLength,
    type AesType,
    aesTypes,
    type AlgorithmType,
    algorithmTypes,
    type DigestType,
    digestTypes, isAesJson, isEd25519Json,
    isRSAJson,
    type JSONAesGcmParams,
    type JSONKeyParams,
    type RsaJSONType,
    type RsaType
} from "./key-types.ts";
import {ab2base64, base642ui} from "../utils.ts";

export const getKeyParamsFromJson = (keyParamsJson: JSONKeyParams) => {
    if (isRSAJson(keyParamsJson)) {
        return new RsaImportParams(keyParamsJson.name, keyParamsJson.hash);
    }
    if (isEd25519Json(keyParamsJson)) {
        return new Ed25519ImportParams();
    }
    if (isAesJson(keyParamsJson)) {
        return new AesImportParams(keyParamsJson.name);
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

export class RsaImportParams extends AlgorithmParams<RsaType> {
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
    override fromJSON(json: RsaJSONType) {
        this.name = json.name;
        this.hash = json.hash;
    }
}

export class RSAKeyGenParams extends RsaImportParams {
    modulusLength;
    publicExponent;
    constructor(name: RsaType, hash: DigestType = digestTypes.SHA256, modulusLength = 2048, publicExponent = new Uint8Array([0x01, 0x00, 0x01])) {
        super(name, hash);
        this.modulusLength = modulusLength;
        this.publicExponent = publicExponent;
    }
    override getImportParams() {
        return new RsaImportParams(this.name, this.hash);
    }
}
export class Ed25519ImportParams extends AlgorithmParams<typeof algorithmTypes.ED25519> {
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
    constructor(iv: ArrayBuffer = new Uint8Array(12)) {
        this.iv = iv;
    }
    toJSON() {
        return {
            name: this.name,
            iv: ab2base64(this.iv)
        }
    };
    fromJSON(json: JSONAesGcmParams) {
        this.iv = base642ui(json.iv);
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

