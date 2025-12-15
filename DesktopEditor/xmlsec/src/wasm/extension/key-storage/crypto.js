import {digestTypes, exportKeyFormats,} from "./keys/key-types.ts";
import {AesGcmGenParams} from "./keys/params.ts";


const pbkdf2Parameters = {
	iterations: 150000,
	hash: digestTypes.SHA256,
	saltLength: 16
};

function CCryptoBase() {

}
CCryptoBase.prototype.sign = function(key, data) {};
CCryptoBase.prototype.digest = function(algorithm, data) {};
CCryptoBase.prototype.verify = function(key, signature, data) {};
CCryptoBase.prototype.decrypt = function(key, data) {};
CCryptoBase.prototype.encrypt = function(key, data) {};
CCryptoBase.prototype.generateKey = function(params) {};
CCryptoBase.prototype.wrapKey = function(format, key, masterPassword, salt, aesParams, keyUsage) {};
CCryptoBase.prototype.unwrapKey = function(format, key, masterPassword, salt, aesParams, keyParams, keyUsage) {};
CCryptoBase.prototype.getRandomValues = function(length) {};
CCryptoBase.prototype.randomUUID = function() {};

function CWebCrypto() {
	CCryptoBase.call(this);
	this.crypto = self.crypto;
	this.subtle = this.crypto.subtle;
}
CWebCrypto.prototype = Object.create(CCryptoBase.prototype);
CWebCrypto.prototype.constructor = CWebCrypto;
CWebCrypto.prototype.getRandomValues = function(length) {
	const ui = new Uint8Array(length);
	return this.crypto.getRandomValues(ui);
}
CWebCrypto.prototype.getAesCryptoKey = function(masterPassword, salt) {
	const oThis = this;
	return this.subtle.importKey(
		'raw',
		masterPassword,
		{ name: 'PBKDF2' },
		false,
		['deriveKey']
	).then(function(pwKey) {
		return oThis.subtle.deriveKey(
			{
				name: 'PBKDF2',
				salt: salt,
				iterations: pbkdf2Parameters.iterations,
				hash: pbkdf2Parameters.hash
			},
			pwKey,
			new AesGcmGenParams(),
			false,
			['wrapKey', 'unwrapKey', 'encrypt', 'decrypt']
		);
	});
};

CWebCrypto.prototype.wrapKey = function (format, key, masterPassword, salt, aesParams, keyUsage) {
	const oThis = this;
	return Promise.all([this.getAesCryptoKey(masterPassword, salt),  this.getCryptoKeyFromWrapper(key, keyUsage)]).then(function(cryptoKeys) {
		return oThis.subtle.wrapKey(format, cryptoKeys[1], cryptoKeys[0], aesParams);
	});
	
}
CWebCrypto.prototype.unwrapKey = function(format, key, masterPassword, salt, aesParams, keyParams, keyUsages) {
	const oThis = this;
	return this.getAesCryptoKey(masterPassword, salt).then(function(cryptoAesKey) {
		return oThis.subtle.unwrapKey(format, key, cryptoAesKey, aesParams, keyParams, true, /*this.getKeyUsages(keyUsages)*/["sign"]);
	}).then(function(cryptoKey) {
		return oThis.subtle.exportKey(format, cryptoKey);
	});
}
CWebCrypto.prototype.getCryptoKeyFromWrapper = function(key, keyUsages) {
	return this.subtle.importKey(key.exportFormat, key.key, key.params, true, keyUsages);
}
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
	const algorithm = key.getCryptoAlgrotihm();
	return oThis.subtle.decrypt(algorithm, cryptoKey, data);
}
CWebCrypto.prototype.encrypt = function(key, data) {
		const cryptoKey = key.getCryptoKey();
		const algorithm = key.getCryptoAlgorithm();
		return this.subtle.encrypt(algorithm, cryptoKey, data);
}
CWebCrypto.prototype.exportKey = function(key) {
	const cryptoKey = key.getCryptoKey();
	const format = key.getCryptoFormat();
	return this.subtle.exportKey(format, cryptoKey);
}
CWebCrypto.prototype.generateKey = function(params) {
	const oThis = this;
	const cryptoParams = params.getCryptoParams();
	const cryptoUsages = params.getCryptoUsages();
	return this.subtle.generateKey(cryptoParams, true, cryptoUsages).then(function(cryptoKey) {
		if (cryptoKey.privateKey && cryptoKey.publicKey) {
			return Promise.all([oThis.subtle.exportKey(exportKeyFormats.spki, cryptoKey.publicKey), oThis.subtle.exportKey(exportKeyFormats.pkcs8, cryptoKey.privateKey)]);
		}
		return oThis.subtle.exportKey(exportKeyFormats.raw, cryptoKey);
	}).then(function(exportedKeys) {
		const importParams = params.getImportParams();
		if (Array.isArray(exportedKeys)) {
			const publicKeyBuffer = exportedKeys[0];
			const privateKeyBuffer = exportedKeys[1];
			if (params.isSign()) {
				return WebSignKeyPair.fromCryptoBuffer(publicKeyBuffer, privateKeyBuffer, importParams);
			}
			return WebEncryptKeyPair.fromCryptoBuffer(publicKeyBuffer, privateKeyBuffer, importParams);
		} else {
			return WebSymmetricKey.fromCryptoBuffer(exportedKeys, importParams);
		}
	});
};

CWebCrypto.prototype.randomUUID = function() {
	return this.crypto.randomUUID();
}

function getCrypto() {
	return new CWebCrypto();
}
