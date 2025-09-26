import {Key, KeyPair, PrivateKey, PublicKey, SymmetricKey} from "./keys/keys.ts";
import {
    type DigestType,
    digestTypes,
    type ExportKeyFormat,
    exportKeyFormats,
    type KeyGenParams,
    type KeyParams
} from "./keys/key-types.ts";
import {AesGcmGenParams, type AesGcmParams} from "./keys/params.ts";

const pbkdf2Parameters = {
  iterations: 150000,
  hash: digestTypes.SHA256,
    saltLength: 16
};
// type DecryptKey = PrivateKey | SymmetricKey;
// type EncryptKey = PublicKey | SymmetricKey;
abstract class CCryptoBase {
    abstract sign(key: PrivateKey, data: ArrayBuffer): Promise<ArrayBuffer>;
    abstract digest(algorithm: DigestType, data: ArrayBuffer): Promise<ArrayBuffer>;
    abstract verify(key: PublicKey, signature: ArrayBuffer, data: ArrayBuffer): Promise<boolean>;
    // abstract decrypt(key: DecryptKey, data: ArrayBuffer): Promise<ArrayBuffer>;
    // abstract encrypt(key: EncryptKey, data: ArrayBuffer): Promise<ArrayBuffer>;
    abstract generateKey(params: KeyGenParams, isEncrypt: boolean, isSign: boolean): Promise<SymmetricKey | KeyPair>;
    abstract wrapKey(format: ExportKeyFormat, key: Key, masterPassword: ArrayBuffer, salt: ArrayBuffer, aesParams: AesGcmParams): Promise<ArrayBuffer>
    abstract unwrapKey(format: ExportKeyFormat, key: ArrayBuffer, masterPassword: ArrayBuffer, salt: ArrayBuffer, aesParams: AesGcmParams, keyParams: KeyParams, keyUsages): Promise<ArrayBuffer>
    }
    class CWebCrypto extends CCryptoBase {
        crypto = window.crypto;
        subtle = this.crypto.subtle;
        constructor() {
            super();
        }
        getRandomValues(length: number) {
            return this.crypto.getRandomValues(new Uint8Array(length));
        }
        async getAesCryptoKey(masterPassword: ArrayBuffer, salt: ArrayBuffer) {
            const pwKey = await this.subtle.importKey(
                'raw',
                masterPassword,
                { name: 'PBKDF2' },
                false,
                ['deriveKey']
            );
            return await this.subtle.deriveKey(
                {
                    name: 'PBKDF2',
                    salt: salt,
                    iterations: pbkdf2Parameters.iterations,
                    hash: pbkdf2Parameters.hash
                },
                pwKey,
                new AesGcmGenParams(),
                true,
                ['encrypt', 'decrypt', 'wrapKey', 'unwrapKey']
            );
        };
        async wrapKey(format: ExportKeyFormat, key: Key, masterPassword: ArrayBuffer, salt: ArrayBuffer, aesParams: AesGcmParams) {
            const cryptoAesKey = await this.getAesCryptoKey(masterPassword, salt);
            const importKey = await this.getCryptoKeyFromWrapper(key);
            return this.subtle.wrapKey(format, importKey, cryptoAesKey, aesParams);
        }
        async unwrapKey(format: ExportKeyFormat, key: ArrayBuffer, masterPassword: ArrayBuffer, salt: ArrayBuffer, aesParams: AesGcmParams, keyParams: KeyParams, keyUsages) {
            const cryptoAesKey = await this.getAesCryptoKey(masterPassword, salt);
            const cryptoKey = await this.subtle.unwrapKey(format, key, cryptoAesKey, aesParams, keyParams, true, keyUsages);
            return this.subtle.exportKey(format, cryptoKey);
        }
        async getCryptoKeyFromWrapper(key: Key) {
            return await this.subtle.importKey(key.exportFormat, key.key, key.params, true, this.getKeyUsages());
        }
        getKeyUsages(isCrypt?: boolean, isSign?: boolean) {
            const keyUsages: KeyUsage[] = [];
            if (isCrypt) {
                keyUsages.push("encrypt", "decrypt");
            }
            if (isSign) {
                keyUsages.push("sign", "verify");
            }
            return keyUsages;
        }
        async sign(key: PrivateKey, data: ArrayBuffer): Promise<ArrayBuffer> {
            const cryptoKey = await this.getCryptoKeyFromWrapper(key);
            return this.subtle.sign(key.params, cryptoKey, data);
        }
        async digest(algorithm: DigestType, data: ArrayBuffer): Promise<ArrayBuffer> {
            return this.subtle.digest(algorithm, data);
        }
        async verify(key: PublicKey, signature: ArrayBuffer, data: ArrayBuffer): Promise<boolean> {
            const cryptoKey = await this.getCryptoKeyFromWrapper(key);
            return this.subtle.verify(key.params, cryptoKey, signature, data);
        }
        // async decrypt(key: DecryptKey, data: ArrayBuffer): Promise<ArrayBuffer> {
        //     const cryptoKey = await this.getCryptoKeyFromWrapper(key);
        //     return this.subtle.decrypt(cryptoKey);
        // }
        // async encrypt(key: EncryptKey, data: ArrayBuffer): Promise<ArrayBuffer> {
        //     throw new Error("Method not implemented.");
        // }
        async generateKey(params: KeyGenParams, keyUsage) {
            const cryptoKey = await this.subtle.generateKey(params, true, this.getKeyUsages(keyUsage.isEncrypt, keyUsave));
            const importParams = params.getImportParams();
            if (("privateKey" in cryptoKey) && ("publicKey" in cryptoKey)) {
                const publicKeyBuffer = await this.subtle.exportKey(exportKeyFormats.spki, cryptoKey.publicKey);
                const publicKey = new PublicKey(publicKeyBuffer, importParams);
                const privateKeyBuffer = await this.subtle.exportKey(exportKeyFormats.pkcs8, cryptoKey.privateKey);
                const privateKey = new PrivateKey(privateKeyBuffer, importParams, this.getRandomValues(pbkdf2Parameters.saltLength));
                return new KeyPair(publicKey, privateKey);
            }
            const keyBuffer = await this.subtle.exportKey(exportKeyFormats.raw, cryptoKey);
            return new SymmetricKey(keyBuffer, importParams);
        }
    }

    const getCrypto = () => {
    return new CWebCrypto();
}

export default getCrypto;