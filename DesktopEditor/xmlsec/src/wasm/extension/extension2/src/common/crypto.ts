import type {
    CryptoData,
    DigestAlgorithm,
    EncryptAlgorithm,
    GenerateKeyAlgorithm, GenerateKeyUsages,
    SignAlgorithm
} from "./crypto-types.ts";
import {Key, KeyPair, type SymmetricKey} from "./keys/keys.ts";
import type {KeyParams} from "./keys/key-types.ts";
const pbkdf2Parameters = {
  iterations: 150000,
  hash: "SHA-256",
};
const aesGCMParameters = {
    keyLength: 256
};
abstract class CCryptoBase {
    abstract sign(_algorithm: SignAlgorithm, _key: CryptoKey, _data: CryptoData): Promise<ArrayBuffer>;
    abstract digest(_algorithm: DigestAlgorithm, _data: CryptoData): Promise<ArrayBuffer>;
    abstract verify(_algorithm: SignAlgorithm, _key: CryptoKey, _signature: CryptoData, _data: CryptoData): Promise<boolean>;
    abstract decrypt<T extends KeyParams>(_key: Key<T>, _data: CryptoData): Promise<ArrayBuffer>;
    abstract encrypt(_algorithm: EncryptAlgorithm, _key: CryptoKey, _data: CryptoData): Promise<ArrayBuffer>;
    abstract generateKey(_algorithm: GenerateKeyAlgorithm, _keyUsages: GenerateKeyUsages): Promise<SymmetricKey | KeyPair>

    }
    class CWebCrypto extends CCryptoBase {
        crypto = window.crypto;
        subtle = this.crypto.subtle;
        constructor() {
            super();
        }
        override sign(algorithm: SignAlgorithm, key: CryptoKey, data: CryptoData) {
            return this.subtle.sign(algorithm, key, data);
        };
        override digest(algorithm: DigestAlgorithm, data: CryptoData) {
            return this.subtle.digest(algorithm, data);
        };
        override verify(algorithm: SignAlgorithm, key: CryptoKey, signature: CryptoData, data: CryptoData) {
            return this.subtle.verify(algorithm, key, signature, data);
        };
        override decrypt(algorithm: EncryptAlgorithm, key: CryptoKey, data: CryptoData) {
            return this.subtle.decrypt(algorithm, key, data);
        };
        override encrypt(algorithm: EncryptAlgorithm, key: CryptoKey, data: CryptoData) {
            return this.subtle.encrypt(algorithm, key, data);
        };
        override generateKey(algorithm: GenerateKeyAlgorithm, extractable: boolean, keyUsages: GenerateKeyUsages) {
            return this.subtle.generateKey(algorithm, extractable, keyUsages);
        };
        getRandomValues(length: number) {
            return this.crypto.getRandomValues(new Uint8Array(length));
        }
        async getAesCryptoKeyFromMasterPassword(masterPassword: string, salt: string) {
            const encoder = new TextEncoder();
            const pwKey = await crypto.subtle.importKey(
                'raw',
                encoder.encode(masterPassword),
                { name: 'PBKDF2' },
                false,
                ['deriveKey']
            );
            const aesKey = await crypto.subtle.deriveKey(
                {
                    name: 'PBKDF2',
                    salt: encoder.encode(salt),
                    iterations: pbkdf2Parameters.iterations,
                    hash: pbkdf2Parameters.hash
                },
                pwKey,
                { name: 'AES-GCM', length: aesGCMParameters.keyLength },
                false,
                ['encrypt', 'decrypt']
            );
            return aesKey;
        }
        encryptWithMasterPassword(masterPassword: string, salt, data: CryptoData) {

        }
        decryptWithMasterPassword(masterPassword: string, salt, data) {

        }
        async generateCipherKeys() {
            return await this.generateKey({ name: "X25519" }, true, ["deriveKey"]);
        }
    }

    const getCrypto = () => {
    return new CWebCrypto();
}

export default getCrypto;