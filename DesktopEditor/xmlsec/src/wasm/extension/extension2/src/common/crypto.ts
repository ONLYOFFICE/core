import type {
    CryptoData,
    DigestAlgorithm,
    EncryptAlgorithm,
    GenerateKeyAlgorithm, GenerateKeyUsages,
    SignAlgorithm
} from "./crypto-types.ts";
const pbkdf2Parameters = {
  iterations: 150000,
  hash: "SHA-256",
};
const aesGCMParameters = {
    keyLength: 256
};
class CCryptoBase {
        sign(_algorithm: SignAlgorithm, _key: CryptoKey, _data: CryptoData): Promise<ArrayBuffer> {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        digest(_algorithm: DigestAlgorithm, _data: CryptoData): Promise<ArrayBuffer> {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        verify(_algorithm: SignAlgorithm, _key: CryptoKey, _signature: CryptoData, _data: CryptoData): Promise<boolean> {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        decrypt(_algorithm: EncryptAlgorithm, _key: CryptoKey, _data: CryptoData): Promise<ArrayBuffer> {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        encrypt(_algorithm: EncryptAlgorithm, _key: CryptoKey, _data: CryptoData): Promise<ArrayBuffer> {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        generateKey(_algorithm: GenerateKeyAlgorithm, _extractable: boolean, _keyUsages: GenerateKeyUsages): Promise<CryptoKey | CryptoKeyPair> {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        async generateCipherKeys() {
            return await this.generateKey({ name: "X25519" }, true, ["deriveKey"]);
        }
        async encryptData(publicKey: CryptoKey, privateKey: CryptoKey, data: CryptoData) {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        }
        async decryptData(publicKey: CryptoKey, privateKey: CryptoKey, data: CryptoData) {
            return new Promise(function (_resolve, reject) {
                reject("Override the method in the subclass");
            });
        }
    }
    class CWebCrypto extends CCryptoBase {
        subtle = window.crypto.subtle;
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
        getAesCryptoKeyFromMasterPassword(masterPassword: string, salt) {
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
                    salt: salt,
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
    }

    const initCrypto = () => {
    return new CWebCrypto();
}

    const generateSalt = () => {

    }

export default initCrypto;
