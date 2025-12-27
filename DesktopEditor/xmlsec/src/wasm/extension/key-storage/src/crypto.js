import {EncryptData, WebEncryptKeyPair, WebSignKeyPair, WebSymmetricKey} from "./keys";
import {AesGcmKeyGenParams, PBKDF2Params} from "./params";
import {c_oAscExportKeyFormat} from "./defines";
import {BinaryWriter} from "./serialize/writer";
import {initClass} from "./utils";

function CCryptoBase() {

}
CCryptoBase.prototype.sign = function(key, data) {};
CCryptoBase.prototype.digest = function(algorithm, data) {};
CCryptoBase.prototype.verify = function(key, signature, data) {};
CCryptoBase.prototype.decrypt = function(key, data) {};
CCryptoBase.prototype.encrypt = function(key, data) {};
CCryptoBase.prototype.generateKey = function(params) {};
CCryptoBase.prototype.getRandomValues = function(length) {};
CCryptoBase.prototype.randomUUID = function() {};
CCryptoBase.prototype.initKey = function(key, masterPassword) {return Promise.resolve()};
CCryptoBase.prototype.getAesKey = function(key, pbkdfParams) {return Promise.resolve()};

function CWebCrypto() {
	CCryptoBase.call(this);
	this.crypto = self.crypto;
	this.subtle = this.crypto.subtle;
}
initClass(CWebCrypto, CCryptoBase);
CWebCrypto.prototype.getRandomValues = function(length) {
	const ui = new Uint8Array(length);
	return this.crypto.getRandomValues(ui);
}
CWebCrypto.prototype.getAesKey = function(masterPassword, pbkdfParams) {
	const oThis = this;
	const aesKeyGenParams = new AesGcmKeyGenParams();
	return this.subtle.importKey(
		'raw',
		masterPassword,
		{ name: 'PBKDF2' },
		false,
		['deriveKey']
	).then(function(pwKey) {
		return oThis.subtle.deriveKey(
			pbkdfParams.getCryptoParams(),
			pwKey,
			aesKeyGenParams.getKeyGenCryptoParams(),
			false,
			['encrypt', 'decrypt']
		);
	}).then(function(aesKey) {
		return WebSymmetricKey.fromCryptoKey(aesKey, aesKeyGenParams.getImportParams());
	});
};
CWebCrypto.prototype.sign = function(key, data) {
	const oThis = this;
	const cryptoKey = key.getCryptoKey();
	const params = key.getCryptoParams();
	return oThis.subtle.sign(cryptoKey, cryptoKey, data);
}
CWebCrypto.prototype.digest = function(algorithm, data) {
	return this.subtle.digest(algorithm, data);
}
CWebCrypto.prototype.verify = function(key, signature, data) {
	const oThis = this;
	const cryptoKey = key.getCryptoKey();
	return oThis.subtle.verify(key.params, cryptoKey, signature, data);
}
CWebCrypto.prototype.decrypt = function(key, data) {
	const oThis = this;
	const cryptoKey = key.getCryptoKey();
	const encryptParams = data.getEncryptParams();
	const algorithm = encryptParams.getCryptoParams();
	return oThis.subtle.decrypt(algorithm, cryptoKey, data.getEncryptData()).then(function(data) {
		return new Uint8Array(data);
	});
}
CWebCrypto.prototype.encrypt = function(key, data) {
		const cryptoKey = key.getCryptoKey();
		const encryptParams = key.getEncryptParams();
		const algorithm = encryptParams.getCryptoParams();
		return this.subtle.encrypt(algorithm, cryptoKey, data).then(function (encryptedData) {
			const encryptData = new EncryptData(encryptedData, encryptParams);
			return encryptData.export();
		});
}
CWebCrypto.prototype.exportKey = function(key) {
	const cryptoKey = key.getCryptoKey();
	const format = key.getCryptoFormat();
	return this.subtle.exportKey(format, cryptoKey);
}
CWebCrypto.prototype.generateKey = function(params, aesKey) {
	const oThis = this;
	const cryptoParams = params.getKeyGenCryptoParams();
	const cryptoUsages = params.getCryptoUsages();
	let saveCryptoKey;
	return this.subtle.generateKey(cryptoParams, true, cryptoUsages).then(function(cryptoKey) {
		saveCryptoKey = cryptoKey;
		if (cryptoKey.privateKey && cryptoKey.publicKey) {
			const publicKey = oThis.subtle.exportKey(c_oAscExportKeyFormat.spki, cryptoKey.publicKey);
			const privateKey = oThis.subtle.exportKey(c_oAscExportKeyFormat.pkcs8, cryptoKey.privateKey).then(function(data) {
				return aesKey.encrypt(data);
			});
			return Promise.all([publicKey, privateKey]);
		}
		return oThis.subtle.exportKey(c_oAscExportKeyFormat.raw, cryptoKey).then(function (data) {
			return aesKey.encrypt(data);
		});
	}).then(function(exportedKeys) {
		const importParams = params.getImportParams();
		if (Array.isArray(exportedKeys)) {
			const publicKeyBuffer = exportedKeys[0];
			const privateKeyBuffer = exportedKeys[1];
			if (params.isSign()) {
				return WebSignKeyPair.fromWebCrypto(publicKeyBuffer, saveCryptoKey.publicKey, privateKeyBuffer, saveCryptoKey.privateKey, importParams);
			}
			return WebEncryptKeyPair.fromWebCrypto(publicKeyBuffer, saveCryptoKey.publicKey, privateKeyBuffer, saveCryptoKey.privateKey, importParams);
		} else {
			return WebSymmetricKey.fromWebCrypto(exportedKeys, saveCryptoKey, importParams);
		}
	});
};

CWebCrypto.prototype.randomUUID = function() {
	return this.crypto.randomUUID();
}
CWebCrypto.prototype.initKey = function (key, aesKey) {
	const binaryKey = key.getBinaryKey();
	const oThis = this;
	let binaryKeyPromise;
	if (aesKey) {
		binaryKeyPromise = aesKey.decrypt(binaryKey);
	} else {
		binaryKeyPromise = Promise.resolve(binaryKey);
	}
	return binaryKeyPromise.then(function(binaryCryptoData) {
		return oThis.subtle.importKey(key.getImportFormat(), binaryCryptoData, key.getImportCryptoParams(), true, key.getCryptoUsages());
	}).then(function (cryptoKey) {
		key.setCryptoKey(cryptoKey);
	});
};

export function getCrypto() {
	return new CWebCrypto();
}
