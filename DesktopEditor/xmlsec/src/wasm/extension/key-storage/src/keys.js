import {CryptoBase, initClass, isEqualArrays} from "./utils";
import {c_oAscExportKeyFormat, c_oAscKeyStorageType} from "./defines";
import {BinaryWriter, writeBool, writeBuffer, writeLong, writeObject, writeString} from "./serialize/writer";
import {getCrypto} from "./crypto";
import {BinaryReader, readBool, readBuffer, readLong, readObject, readString} from "./serialize/reader";

function CryptoKeyBase() {
	CryptoBase.call(this);
	this.date = null;
	this.isValid = false;
	this.uid = null;
	this.version = 1;
	this.params = null;
	this.type = c_oAscKeyStorageType.NoType;
}
initClass(CryptoKeyBase, CryptoBase);
CryptoKeyBase.import = function(reader, version, symmetricKey) {};
CryptoKeyBase.prototype.init = function() {
	const crypto = getCrypto();
	this.setDate(new Date());
	this.setIsValid(true);
	this.setUID(crypto.randomUUID());
}
CryptoKeyBase.prototype.export = function(writer) {

};
CryptoKeyBase.prototype.encrypt = function() {

};
CryptoKeyBase.prototype.decrypt = function() {

};
CryptoKeyBase.prototype.setDate = function(date) {
	this.date = new Date(date);
};
CryptoKeyBase.prototype.setIsValid = function(isValid) {
	this.isValid = isValid;
};
CryptoKeyBase.prototype.setUID = function(uid) {
	this.uid = uid;
};
CryptoKeyBase.prototype.setVersion = function(version) {
	this.version = version;
};
CryptoKeyBase.prototype.setType = function(type) {
	this.type = type;
};
CryptoKeyBase.prototype.setParams = function(params) {
	this.params = params;
};


function WebKeyPair() {
	CryptoKeyBase.call(this);
	this.privateKey = null;
	this.publicKey = null;
	this.version = 1;
}
initClass(WebKeyPair, CryptoKeyBase);
WebKeyPair.import = function(reader) {
	const keyPair = new this();
	keyPair.setVersion(readLong(reader));
	switch (keyPair.version) {
		case 1: {
			keyPair.setDate(readString(reader));
			keyPair.setUID(readString(reader));
			keyPair.setIsValid(readBool(reader));
			keyPair.setPublicKey(readObject(reader));
			keyPair.setPrivateKey(readObject(reader));
			keyPair.setParams(readObject(reader));
			break;
		}
		default: {
			return null;
		}
	}
};

WebKeyPair.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeString(writer, this.date.toISOString());
			writeString(writer, this.uid);
			writeBool(writer, this.isValid);
			writeObject(writer, this.publicKey);
			writeObject(writer, this.privateKey);
			writeObject(writer, this.params);
			break;
		}
		default: {
			break;
		}
	}
};
WebKeyPair.prototype.setParams = function(params) {
	this.params = params;
	if (this.publicKey) {
		this.publicKey.setParams(params);
	}
	if (this.privateKey) {
		this.privateKey.setParams(params);
	}
};
WebKeyPair.prototype.setPublicKey = function(publicKey) {
	this.publicKey = publicKey;
};
WebKeyPair.prototype.setPrivateKey = function(privateKey) {
	this.privateKey = privateKey;
};
WebKeyPair.fromWebCrypto = function(publicKeyBuffer, publicCryptoKey, privateKeyBuffer, privateCryptoKey, importParams) {
	const keyPair = new this();
	keyPair.init();
	const publicKey = new (this.getPublicKeyConstructor())();
	publicKey.setBinaryKey(publicKeyBuffer);
	publicKey.setCryptoKey(publicCryptoKey);
	const privateKey = new (this.getPrivateKeyConstructor())();
	privateKey.setBinaryKey(privateKeyBuffer);
	privateKey.setCryptoKey(privateCryptoKey);
	keyPair.setPublicKey(publicKey);
	keyPair.setPrivateKey(privateKey);
	keyPair.setParams(importParams);
	return keyPair;
};
WebKeyPair.getPublicKeyConstructor = function() {
	return WebPublicKey;
};
WebKeyPair.getPrivateKeyConstructor = function() {
	return WebPrivateKey;
};
WebKeyPair.prototype.initKey = function (aesKey) {
	const crypto = getCrypto();
	return Promise.all([crypto.initKey(this.publicKey), crypto.initKey(this.privateKey, aesKey)]);
};
WebKeyPair.prototype.isHavePublicKey = function(publicKey) {
	return this.publicKey.isEqual(publicKey);
};
WebKeyPair.prototype.getExportPublicKey =function() {
	const writer = new BinaryWriter();
	writeObject(writer, this.publicKey);
	return writer.GetData();
};
WebKeyPair.prototype.changeMasterPassword = function(oldAesKey, newAesKey) {
	return this.privateKey.changeMasterPassword(oldAesKey, newAesKey);
};
export function WebSignKeyPair() {
	WebKeyPair.call(this);
}
initClass(WebSignKeyPair, WebKeyPair, c_oAscKeyStorageType.WebSignKeyPair);
WebSignKeyPair.import = WebKeyPair.import;
WebSignKeyPair.fromWebCrypto = WebKeyPair.fromWebCrypto;
WebSignKeyPair.getPublicKeyConstructor = function() {
	return WebPublicSignKey;
};
WebSignKeyPair.getPrivateKeyConstructor = function() {
	return WebPrivateSignKey;
};
WebEncryptKeyPair.prototype.verify = function (data) {
	return this.publicKey.verify(data);
};
WebEncryptKeyPair.prototype.sign = function (data) {
	return this.privateKey.sign(data);
};

export function WebEncryptKeyPair() {
	WebKeyPair.call(this);
	this.privateKey = null;
	this.publicKey = null;
}
initClass(WebEncryptKeyPair, WebKeyPair, c_oAscKeyStorageType.WebEncryptKeyPair);
WebEncryptKeyPair.import = WebKeyPair.import;
WebEncryptKeyPair.fromWebCrypto = WebKeyPair.fromWebCrypto;
WebEncryptKeyPair.getPublicKeyConstructor = function() {
	return WebPublicEncryptKey;
};
WebEncryptKeyPair.getPrivateKeyConstructor = function() {
	return WebPrivateEncryptKey;
};
WebEncryptKeyPair.prototype.encrypt = function (data) {
	return this.publicKey.encrypt(data);
};
WebEncryptKeyPair.prototype.decrypt = function (data) {
	return this.privateKey.decrypt(data);
};

function AsymmetricKey() {
	CryptoBase.call(this);
	this.binaryKey = null;
	this.cryptoKey = null;
	this.version = 1;
	this.params = null;
}
initClass(AsymmetricKey, CryptoBase);
AsymmetricKey.prototype.setBinaryKey = function(binaryKey) {
	this.binaryKey = new Uint8Array(binaryKey);
};
AsymmetricKey.prototype.setCryptoKey = function(cryptoKey) {
	this.cryptoKey = cryptoKey;
};
AsymmetricKey.prototype.setVersion = function(version) {
	this.version = version;
};
AsymmetricKey.prototype.changeMasterPassword = function(oldAesKey, newAesKey) {
	return Promise.resolve();
};
AsymmetricKey.prototype.getCryptoKey = function() {
	return this.cryptoKey;
};
AsymmetricKey.prototype.setParams = function(params) {
	this.params = params;
}
AsymmetricKey.prototype.getImportCryptoParams = function() {
	return this.params.getImportCryptoParams();
};
AsymmetricKey.prototype.getBinaryKey = function() {
	return this.binaryKey;
};
AsymmetricKey.prototype.getCryptoKey = function() {
	return this.cryptoKey;
};
AsymmetricKey.prototype.getEncryptParams = function() {
	return this.params.getEncryptParams();
}

function WebPrivateKey() {
	AsymmetricKey.call(this);
}
initClass(WebPrivateKey, AsymmetricKey);

WebPrivateKey.import = function(reader) {
	const key = new WebPrivateKey();
	key.setVersion(readLong(reader));
	switch (key.version) {
		case 1: {
			key.setBinaryKey(readBuffer(reader));
			break;
		}
		default:
			return null;
	}
	return key;
};
WebPrivateKey.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeBuffer(writer, this.binaryKey);
			break;
		}
		default: {
			break;
		}
	}
};
WebPrivateKey.prototype.decrypt = function(data) {
	const crypto = getCrypto();
	return crypto.decrypt(this, EncryptData.import(data));
};
WebPrivateKey.prototype.sign = function(data) {
	const crypto = getCrypto();
	return crypto.sign(this, data);
};
WebPrivateKey.prototype.getImportFormat = function () {
	return c_oAscExportKeyFormat.pkcs8;
}
WebPrivateKey.prototype.changeMasterPassword = function(oldAesKey, newAesKey) {
	const oThis = this;
	return oldAesKey.decrypt(this.binaryKey).then(function(decryptedKey) {
		return newAesKey.encrypt(decryptedKey);
	}).then(function(encryptedKey) {
		oThis.binaryKey = encryptedKey;
	});
};

export function WebPrivateSignKey() {
	WebPrivateKey.call(this);
}
initClass(WebPrivateSignKey, WebPrivateKey, c_oAscKeyStorageType.WebPrivateSignKey);
WebPrivateSignKey.import = WebPrivateKey.import;
WebPrivateSignKey.prototype.getCryptoUsages = function () {
	return ["sign"];
};

export function WebPrivateEncryptKey() {
	WebPrivateKey.call(this);
}
initClass(WebPrivateEncryptKey, WebPrivateKey, c_oAscKeyStorageType.WebPrivateEncryptKey);
WebPrivateEncryptKey.import = WebPrivateKey.import;
WebPrivateEncryptKey.prototype.getCryptoUsages = function () {
	return ["decrypt"];
};

function WebPublicKey() {
	AsymmetricKey.call(this);
}
initClass(WebPublicKey, AsymmetricKey);

WebPublicKey.import = function(reader) {
	const key = new WebPublicKey();
	key.setVersion(readLong(reader));
	switch (key.version) {
		case 1: {
			key.setBinaryKey(readBuffer(reader));
			break;
		}
		default:
			return null;
	}
	return key;
};
WebPublicKey.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeBuffer(writer, this.binaryKey);
			break;
		}
		default: {
			break;
		}
	}
};
WebPublicKey.prototype.encrypt = function(data) {
	const crypto = getCrypto();
	return crypto.encrypt(this, data);
};
WebPublicKey.prototype.verify = function(data) {
	const crypto = getCrypto();
	return crypto.verify(this, data);
};
WebPublicKey.prototype.getImportFormat = function () {
	return c_oAscExportKeyFormat.spki;
}
WebPublicKey.prototype.isEqual = function (publicKey) {
	return isEqualArrays(this.binaryKey, publicKey.binaryKey);
};

export function WebPublicSignKey() {
	WebPublicKey.call(this);
}
initClass(WebPublicSignKey, WebPublicKey, c_oAscKeyStorageType.WebPublicSignKey);
WebPublicSignKey.import = WebPublicKey.import;
WebPublicSignKey.prototype.getCryptoUsages = function () {
	return ["verify"];
};

export function WebPublicEncryptKey() {
	WebPublicKey.call(this);
}
initClass(WebPublicEncryptKey, WebPublicKey, c_oAscKeyStorageType.WebPublicEncryptKey);
WebPublicEncryptKey.import = WebPublicKey.import;
WebPublicEncryptKey.prototype.getCryptoUsages = function () {
	return ["encrypt"];
};

export function WebSymmetricKey() {
	CryptoKeyBase.call(this);
	this.version = 1;
	this.cryptoKey = null;
	this.binaryKey = null;
}
initClass(WebSymmetricKey, CryptoKeyBase, c_oAscKeyStorageType.WebSymmetricKey);

WebSymmetricKey.import = function(reader) {
	const symmetricKey = new WebSymmetricKey();
	symmetricKey.setVersion(readLong(reader));
	switch (symmetricKey.version) {
		case 1: {
			symmetricKey.setDate(readString(reader));
			symmetricKey.setUID(readString(reader));
			symmetricKey.setIsValid(readBool(reader));
			symmetricKey.setBinaryKey(readBuffer(reader));
			break;
		}
		default:
			return null;
	}
	return symmetricKey;
};
WebSymmetricKey.fromWebCrypto = function(symmetricKeyBuffer, symmetricCryptoKey, importParams) {
	const key = new WebSymmetricKey();
	key.init();
	key.setBinaryKey(symmetricKeyBuffer);
	key.setParams(importParams);
	return key;
};
WebSymmetricKey.fromCryptoKey = function(cryptoKey, pbkdfParams) {
	const key = new WebSymmetricKey();
	key.init();
	key.setCryptoKey(cryptoKey);
	key.setParams(pbkdfParams);
	return key;
};
WebSymmetricKey.prototype.setBinaryKey = function(buffer) {
	this.binaryKey = new Uint8Array(buffer);
};
WebSymmetricKey.prototype.getBinaryKey = function() {
	return this.binaryKey;
};
WebSymmetricKey.prototype.setCryptoKey = function(cryptoKey) {
	this.cryptoKey = cryptoKey;
};
WebSymmetricKey.prototype.getCryptoKey = function() {
	return this.cryptoKey;
};
WebSymmetricKey.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeString(writer, this.date.toISOString());
			writeString(writer, this.uid);
			writeBool(writer, this.isValid);
			writeBuffer(writer, this.binaryKey);
			break;
		}
		default: {
			break;
		}
	}
};
WebSymmetricKey.prototype.encrypt = function(data) {
	const crypto = getCrypto();
	return crypto.encrypt(this, data);
};
WebSymmetricKey.prototype.decrypt = function(data) {
	const crypto = getCrypto();
	return crypto.decrypt(this, EncryptData.import(data));
};
WebSymmetricKey.prototype.initKey = function(aesKey) {
	const crypto = getCrypto();
	return crypto.initKey(this, aesKey);
};
WebSymmetricKey.prototype.getImportFormat = function() {
	return c_oAscExportKeyFormat.raw;
};
WebSymmetricKey.prototype.getImportCryptoParams = function() {

};
WebSymmetricKey.prototype.getCryptoUsages = function() {
	return ["encrypt", "decrypt"];
};
WebSymmetricKey.prototype.isHavePublicKey = function(publicKey) {
	return false;
};
WebSymmetricKey.prototype.getExportPublicKey = function() {
	return null;
};
WebSymmetricKey.prototype.changeMasterPassword = function(oldAesKey, newAesKey) {
	const oThis= this;
	return oldAesKey.decrypt(this.binaryKey).then(function(decryptedKey) {
		return newAesKey.encrypt(decryptedKey);
	}).then(function(encryptedKey) {
		oThis.binaryKey = encryptedKey;
	});
};
WebSymmetricKey.prototype.getEncryptParams = function() {
	return this.params.getEncryptParams();
};

export function EncryptData(encryptData, params) {
	CryptoBase.call(this);
	this.version = 1;
	this.encryptData = encryptData ? new Uint8Array(encryptData) : null;
	this.params = params || null;
}
initClass(EncryptData, CryptoBase, c_oAscKeyStorageType.EncryptData);
EncryptData.import = function(binaryData) {
	const data = new EncryptData();
	const reader = new BinaryReader(binaryData);
	data.setVersion(readLong(reader));
	switch (data.version) {
		case 1: {
			data.setEncryptParams(readObject(reader));
			data.setEncryptData(readBuffer(reader));
			break;
		}
		default: {
			return null;
		}
	}
	return data;
};
EncryptData.prototype.export = function () {
	const writer = new BinaryWriter();
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeObject(writer, this.params);
			writeBuffer(writer, this.encryptData);
			break;
		}
		default: {
			break;
		}
	}
	return writer.GetData();
};
EncryptData.prototype.setEncryptData = function (encryptData) {
	this.encryptData = encryptData;
};
EncryptData.prototype.setEncryptParams = function (params) {
	this.params = params;
};
EncryptData.prototype.getEncryptParams = function () {
	return this.params;
};
EncryptData.prototype.getEncryptData = function () {
	return this.encryptData;
};
EncryptData.prototype.setVersion = function(version) {
	this.version = version;
};

