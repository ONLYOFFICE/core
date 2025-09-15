type SignAlgorithm = {};

class CCryptoBase {
        sign(_algorithm: SignAlgorithm, key, data) {
            return new Promise(function (resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        digest(_algorithm, _data) {
            return new Promise(function (resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        verify(algorithm, key, signature, data) {
            return new Promise(function (resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        decrypt(algorithm, key, data) {
            return new Promise(function (resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
        encrypt(algorithm, key, data) {
            return new Promise(function (resolve, reject) {
                reject("Override the method in the subclass");
            });
        };
    }

    class CWebCrypto extends CCryptoBase {
        subtle = window.crypto.subtle;
        constructor() {
            super();
        }
        sign(algorithm, key, data) {
            return this.subtle.sign(algorithm, key, data);
        };
        digest(algorithm, data) {
            return this.subtle.digest(algorithm, data);
        };
        verify(algorithm, key, signature, data) {
            return this.subtle.verify(algorithm, key, signature, data);
        };
        decrypt(algorithm, key, data) {
            return this.subtle.decrypt(algorithm, key, data);
        };
        encrypt(algorithm, key, data) {
            return this.subtle.encrypt(algorithm, key, data);
        };
    }

export default CWebCrypto;
