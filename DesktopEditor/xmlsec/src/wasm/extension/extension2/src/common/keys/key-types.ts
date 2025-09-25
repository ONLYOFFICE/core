import {Ed25519KeyGenParams, type RSAImportParams} from "./params.ts";

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


export const isRsaAlgorithm = (name: AlgorithmType): name is RsaType => {
    return Object.values(rsaTypes).includes(name as RsaType);
}

export type RSAJSONType = ReturnType<RSAImportParams["toJSON"]>;
export type JSONAesGcmParams = ReturnType<AesGcmParams["toJSON"]>;
export type AesKeyGenLength = 128 | 192 | 256;
export type KeyParams = RSAImportParams | Ed25519KeyGenParams;
export type DigestType = typeof digestTypes[keyof typeof digestTypes];
export type AesType = typeof aesTypes[keyof typeof aesTypes];
export type RsaType = typeof rsaTypes[keyof typeof rsaTypes];
export type AlgorithmType = typeof algorithmTypes[keyof typeof algorithmTypes];
export type ExportKeyFormat = typeof exportKeyFormats[keyof typeof exportKeyFormats];