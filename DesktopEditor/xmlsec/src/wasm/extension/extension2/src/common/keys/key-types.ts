import {
    AesGcmParams,
    AesImportParams,
    AesKeyGenParams, Ed25519ImportParams,
    Ed25519KeyGenParams,
    type RsaImportParams,
    RSAKeyGenParams
} from "./params.ts";
import {Key, KeyPair, PrivateKey, PublicKey} from "./keys.ts";
export const exportKeyFormats = {
    pkcs8: "pkcs8",
    spki: "spki",
    raw: "raw"
} as const;

export const algorithmTypes = {
    AES_GCM: "AES-GCM",
    AES_CTR: "AES-CTR",
    AES_CBC: "AES-CBC",
    AES_KW: "AES-KW",
    ED25519: "Ed25519",
    RSASSA_PKCS1_v1_5: "RSASSA-PKCS1-v1_5",
    RSA_PSS: "RSA-PSS",
    RSA_OAEP: "RSA-OAEP"
} as const;

export const rsaTypes = {
    RSASSA_PKCS1_v1_5: algorithmTypes.RSASSA_PKCS1_v1_5,
    RSA_PSS: algorithmTypes.RSA_PSS,
    RSA_OAEP: algorithmTypes.RSA_OAEP,
} as const;

export const aesTypes = {
    AES_GCM: algorithmTypes.AES_GCM,
    AES_CTR: algorithmTypes.AES_CTR,
    AES_CBC: algorithmTypes.AES_CBC,
    AES_KW: algorithmTypes.AES_KW,
} as const;

export const digestTypes = {
    SHA1: "SHA-1",
    SHA256: "SHA-256",
    SHA384: "SHA-384",
    SHA512: "SHA-512",
} as const;

export const keyTypes = {
    symmetric: "symmetric",
    pair: "pair"
} as const;

export const pairKeyTypes = {
    private: "private",
    public: "public",
} as const;

export const signAlgorithms = {
    ED25519: algorithmTypes.ED25519
}

export const cryptAlgorithms = {
    ...aesTypes,
    RSA_OAEP: algorithmTypes.RSA_OAEP
}

export const isRSAJson = (obj: JSONKeyParams): obj is RsaJSONType => {
    const name = obj.name;
    return Object.values(rsaTypes).includes(name as RsaType);
}
export const isEd25519Json = (obj: JSONKeyParams): obj is Ed25519JSONParams => {
    const name = obj.name;
    return name === algorithmTypes.ED25519;
};
export const isAesJson = (obj: JSONKeyParams): obj is AesJSONType => {
    const name = obj.name;
    return Object.values(aesTypes).includes(name as AesType);
}

export type RsaJSONType = ReturnType<RsaImportParams["toJSON"]>;
export type AesJSONType = ReturnType<AesImportParams["toJSON"]>;
export type Ed25519JSONParams = ReturnType<Ed25519ImportParams["toJSON"]>;
export type JSONAesGcmParams = ReturnType<AesGcmParams["toJSON"]>;
export type AesKeyGenLength = 128 | 192 | 256;
export type KeyParams = RsaImportParams | AesImportParams | Ed25519ImportParams;
export type JSONKeyParams = RsaJSONType | AesJSONType | Ed25519JSONParams;
export type KeyGenParams = RSAKeyGenParams | Ed25519KeyGenParams | AesKeyGenParams;
export type DigestType = typeof digestTypes[keyof typeof digestTypes];
export type AesType = typeof aesTypes[keyof typeof aesTypes];
export type RsaType = typeof rsaTypes[keyof typeof rsaTypes];
export type AlgorithmType = typeof algorithmTypes[keyof typeof algorithmTypes];
export type ExportKeyFormat = typeof exportKeyFormats[keyof typeof exportKeyFormats];
export type SignAlgorithm = typeof signAlgorithms[keyof typeof signAlgorithms];
export type CryptAlgorithm = typeof cryptAlgorithms[keyof typeof cryptAlgorithms];
export type JSONKey = Awaited<ReturnType<Key["toJSON"]>>;
export type JSONPublicKey = Awaited<ReturnType<PublicKey["toJSON"]>>;
export type JSONPrivateKey = Awaited<ReturnType<PrivateKey["toJSON"]>>;
export type JSONKeyPair = Awaited<ReturnType<KeyPair["toJSON"]>>;
export type PairKey = PrivateKey | PublicKey;
type JSONEncryptExportKeyFormat = {
    encrypt: true;
    salt: string;
    data: string;
};
type JSONDecryptExportKeyFormat = {
    encrypt: false;
    data: JSONKeyPair[];
};
export type JSONExportKeyFormat = JSONEncryptExportKeyFormat | JSONDecryptExportKeyFormat;
