import type {CryptoData, DigestAlgorithm, EncryptAlgorithm, SignAlgorithm} from "./crypto-types.ts";
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
    }

export default CWebCrypto;
