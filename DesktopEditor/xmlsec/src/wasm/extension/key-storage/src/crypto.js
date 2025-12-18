import {WebEncryptKeyPair, WebSignKeyPair, WebSymmetricKey} from "./keys";
import {AesGcmGenParams} from "./params";
import {c_oAscDigestType, c_oAscExportKeyFormat} from "./defines";
import {writeBuffer, writeLong} from "./serialize/writer";
import {readBuffer, readLong} from "./serialize/reader";
import {initClass} from "./utils";

const PBKDFSaltLength = 16;
function PBKDF2Params() {
	this.version = 1;
	this.iterations = 600000;
	this.hash = c_oAscDigestType.SHA256;
	this.salt = null;
}
PBKDF2Params.import = function(reader) {
	const params = new PBKDF2Params();
	params.setVerison(readLong(reader));
	switch (params.version) {
		case 1: {
			this.setIterations(readLong(reader));
			this.setHash(readLong(reader));
			this.setSalt(readBuffer(reader));
			break;
		}
		default: {
			return null;
		}
	}
	return params;
}
PBKDF2Params.prototype.export = function(writer) {
	writeLong(this.version);
	switch (this.version) {
		case 1: {
			writeLong(writer, this.iterations);
			writeLong(writer, this.hash);
			writeBuffer(writer, this.salt);
			break;
		}
		default: {
			break;
		}
	}
}
PBKDF2Params.prototype.getCryptoParams = function() {
	return {
		name: 'PBKDF2',
		salt: this.salt,
		iterations: this.iterations,
		hash: getCryptoHash(this.hash)
	};
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
initClass(CWebCrypto, CCryptoBase);
CWebCrypto.prototype.getRandomValues = function(length) {
	const ui = new Uint8Array(length);
	return this.crypto.getRandomValues(ui);
}
CWebCrypto.prototype.getAesCryptoKey = function(masterPassword, salt) {
	const oThis = this;
	const pbkdfParams = new PBKDF2Params();
	pbkdfParams.setSalt(salt);
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
			new AesGcmGenParams(),
			false,
			['wrapKey', 'unwrapKey', 'encrypt', 'decrypt']
		);
	}).then(function(aesKey) {
		return WebSymmetricKey.fromCryptoKey(aesKey, pbkdfParams);

	});
};

CWebCrypto.prototype.wrapKey = function (format, key, masterPassword, salt, aesParams) {
	const oThis = this;
	const cryptoKey = key.getCryptoKey();
	return Promise.all([this.getAesCryptoKey(masterPassword, salt),  cryptoKey]).then(function(cryptoKeys) {
		return oThis.subtle.wrapKey(format, cryptoKeys[1], cryptoKeys[0], aesParams);
	});
	
}
CWebCrypto.prototype.unwrapKey = function(format, key, masterPassword, salt, aesParams, keyParams) {
	const oThis = this;
	return this.getAesCryptoKey(masterPassword, salt).then(function(cryptoAesKey) {
		return oThis.subtle.unwrapKey(format, key, cryptoAesKey, aesParams, keyParams, true, /*this.getKeyUsages(keyUsages)*/["sign"]);
	}).then(function(cryptoKey) {
		return oThis.subtle.exportKey(format, cryptoKey);
	});
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
CWebCrypto.prototype.generateKey = function(params, password, salt) {
	const oThis = this;
	const cryptoParams = params.getCryptoParams();
	const cryptoUsages = params.getCryptoUsages();
	return this.getAesCryptoKey(password, salt).then(function(aesKey) {
		return this.subtle.generateKey(cryptoParams, true, cryptoUsages).then(function(cryptoKey) {
			if (cryptoKey.privateKey && cryptoKey.publicKey) {
				const publicKey = oThis.subtle.exportKey(c_oAscExportKeyFormat.spki, cryptoKey.publicKey);
				const privateKey = aesKey.export(c_oAscExportKeyFormat.pkcs8, cryptoKey.privateKey).then(function(data) {
					return aesKey.encrypt(data);
				});
				return Promise.all([publicKey,privateKey]);
			}
			return oThis.subtle.exportKey(c_oAscExportKeyFormat.raw, cryptoKey);
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
	});
};

CWebCrypto.prototype.randomUUID = function() {
	return this.crypto.randomUUID();
}

export function getCrypto() {
	return new CWebCrypto();
}
