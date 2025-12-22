import {CryptoBase, initClass} from "./utils";
import {c_oAscExportKeyFormat, c_oAscKeyStorageType} from "./defines";
import {writeBool, writeBuffer, writeLong, writeObject, writeString} from "./serialize/writer";
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
initClass(CryptoBase, CryptoKeyBase);
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
WebKeyPair.fromCryptoBuffer = function(publicKeyBuffer, privateKeyBuffer, importParams) {
	const keyPair = new this();
	keyPair.init();
	const publicKey = new WebPublicKey();
	publicKey.setBinaryKey(publicKeyBuffer);
	const privateKey = new WebPrivateKey();
	privateKey.setBinaryKey(privateKeyBuffer);
	keyPair.setPublicKey(publicKey);
	keyPair.setPrivateKey(privateKey);
	keyPair.setParams(importParams);
	return keyPair;
};
WebKeyPair.prototype.initKey = function (masterPassword) {
	const crypto = getCrypto();
	return Promise.all([crypto.initKey(this.publicKey), crypto.initKey(this.privateKey, masterPassword)]);
};

export function WebSignKeyPair() {
	WebKeyPair.call(this);
}
initClass(WebSignKeyPair, WebKeyPair, c_oAscKeyStorageType.WebSignKeyPair);
WebSignKeyPair.import = WebKeyPair.import;
WebSignKeyPair.fromCryptoBuffer = WebKeyPair.fromCryptoBuffer;
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
WebEncryptKeyPair.fromCryptoBuffer = WebKeyPair.fromCryptoBuffer;
WebEncryptKeyPair.prototype.encrypt = function (data) {
	return this.publicKey.encrypt(data);
};
WebEncryptKeyPair.prototype.decrypt = function (data) {
	return this.privateKey.decrypt(data);
};

function AsymmetricKey() {
	this.binaryKey = null;
	this.cryptoKey = null;
	this.version = 1;
	this.params = null;
	this.type = c_oAscKeyStorageType.NoType;
}
AsymmetricKey.prototype.setBinaryKey = function(binaryKey) {
	this.binaryKey = binaryKey;
};
AsymmetricKey.prototype.setCryptoKey = function(cryptoKey) {
	this.cryptoKey = cryptoKey;
};
AsymmetricKey.prototype.setVersion = function(version) {
	this.version = version;
};
AsymmetricKey.prototype.changeMasterPassword = function(oldMasterPassword, newMasterPassword) {};
AsymmetricKey.prototype.getCryptoKey = function() {
	return this.cryptoKey;
};
AsymmetricKey.prototype.setParams = function(params) {
	this.params = params;
}
AsymmetricKey.prototype.getImportCryptoParams = function() {
	return this.params.getImportCryptoParams();
};

export function WebPrivateKey() {
	AsymmetricKey.call(this);
	this.salt = null;
}
initClass(WebPrivateKey, AsymmetricKey);

WebPrivateKey.import = function(reader) {
	const key = new WebPrivateKey();
	key.setVersion(readLong(reader));
	switch (key.version) {
		case 1: {
			key.setBinaryKey(readBuffer(reader));
			key.setSalt(readBuffer(reader));
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
			writeBuffer(writer, this.salt);
			break;
		}
		default: {
			break;
		}
	}
};
WebPrivateKey.prototype.setSalt = function(salt) {
	this.salt = salt;
};
WebPrivateKey.prototype.changeMasterPassword = function(oldMasterPassword, newMasterPassword) {
	const oldPasswordKey = WebSymmetricKey.getFromPassword(oldMasterPassword, this.salt);
}
WebPrivateKey.prototype.decrypt = function(data) {
	const crypto = getCrypto();
	return crypto.decrypt(this, data);
};
WebPrivateKey.prototype.sign = function(data) {
	const crypto = getCrypto();
	return crypto.sign(this, data);
};
WebPrivateKey.prototype.getImportFormat = function () {
	return c_oAscExportKeyFormat.pkcs8;
}

function WebPrivateSignKey() {
	WebPrivateKey.call(this);
}
initClass(WebPrivateSignKey, WebPrivateKey, c_oAscKeyStorageType.WebPrivateSignKey);
WebPrivateSignKey.prototype.getCryptoUsages = function () {
	return ["sign"];
};

function WebPrivateEncryptKey() {
	WebPrivateKey.call(this);
}
initClass(WebPrivateEncryptKey, WebPrivateKey, c_oAscKeyStorageType.WebPrivateEncryptKey);
WebPrivateEncryptKey.prototype.getCryptoUsages = function () {
	return ["decrypt"];
};

export function WebPublicKey() {
	AsymmetricKey.call(this);
	this.binaryKey = null;
	this.cryptoKey = null;
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

function WebPublicSignKey() {
	WebPublicKey.call(this);
}
initClass(WebPublicSignKey, WebPublicKey, c_oAscKeyStorageType.WebPublicSignKey);
WebPublicSignKey.prototype.getCryptoUsages = function () {
	return ["verify"];
};

function WebPublicEncryptKey() {
	WebPublicKey.call(this);
}
initClass(WebPublicEncryptKey, WebPublicKey, c_oAscKeyStorageType.WebPublicEncryptKey);
WebPublicEncryptKey.prototype.getCryptoUsages = function () {
	return ["encrypt"];
};

export function WebSymmetricKey() {
	CryptoKeyBase.call(this);
	this.version = 1;
	this.cryptoKey = null;
	this.binaryKey = null;
}
initClass(WebSymmetricKey, AsymmetricKey, c_oAscKeyStorageType.WebSymmetricKey);

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
WebSymmetricKey.fromCryptoBuffer = function(symmetricKeyBuffer, importParams) {
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
	this.binaryKey = buffer;
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
	crypto.encrypt(this, data);
};
WebSymmetricKey.prototype.decrypt = function(data) {
	const crypto = getCrypto();
	crypto.decrypt(this, EncryptData.import(data));
};

export function EncryptData(encryptData, params) {
	CryptoBase.call(this);
	this.version = 1;
	this.encryptData = encryptData || null;
	this.params = params || null;
}
initClass(EncryptData, CryptoBase, c_oAscKeyStorageType.EncryptData);
EncryptData.import = function(binaryData) {
	const data = new EncryptData();
	const reader = new BinaryReader(binaryData);
	data.setVersion(readLong(reader));
	switch (this.version) {
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
EncryptData.prototype.export = function (writer) {
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

