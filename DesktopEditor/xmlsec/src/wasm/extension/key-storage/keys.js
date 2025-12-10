import {getGUID} from "../../background/utils.ts";
import {ab2base64, base642ui, str2ui} from "../utils.ts";
import {
	exportKeyFormats,
	keyTypes,
	pairKeyTypes
} from "./key-types.ts";
import getCrypto from "../crypto.ts";
import {AesGcmParams, getKeyParamsFromJson} from "./params.ts";


function Key(key, params, exportFormat) {
	this.key = key;
	this.params = params;
	this.exportFormat = exportFormat;
}
Key.fromJSON = function(json, _masterPassword, _keyUsage) {
	const params = getKeyParamsFromJson(json.params);
	const key = base642ui(json.key);
	return Promise.resolve(new this.constructor(key, params, exportKeyFormats.raw));
};
Key.prototype.toJSON = function(_masterPassword, _keyUsage) {
	const key = ab2base64(this.key);
	return Promise.resolve({
		params: this.params.toJSON(),
		key: key
	});
};
function SymmetricKey(key, params, keyUsage) {
	Key.call(this, key, params, exportKeyFormats.raw);
	this.type = keyTypes.symmetric;
	this.keyUsages = keyUsage || new KeyUsages(true);
}
SymmetricKey.prototype = Object.create(Key.prototype);
SymmetricKey.prototype.constructor = SymmetricKey;

function PublicKey(key, params) {
	Key.call(this, key, params, exportKeyFormats.spki);
	this.type = pairKeyTypes.publicKey;
}

PublicKey.fromJSON = function(json) {
	const params = getKeyParamsFromJson(json.params);
	const key = base642ui(json.key);
	return Promise.resolve(new PublicKey(key, params));
}
PublicKey.prototype.toJSON = function() {
	const params = this.params.toJSON();
	const base64Key = ab2base64(this.key);
	return Promise.resolve({
		format: exportKeyFormats.spki,
		key: base64Key,
		params
	});
}

function PrivateKey(key: ArrayBuffer, params: KeyParams, salt: ArrayBuffer) {
	Key.call(this);
	this.type = pairKeyTypes.private;
	this.salt = salt;
}

export class PrivateKey extends Key {
	type = pairKeyTypes.private;
	salt;
	constructor(key: ArrayBuffer, params: KeyParams, salt: ArrayBuffer) {
		super(key, params, exportKeyFormats.pkcs8);
		this.salt = salt;
	}
	static override async fromJSON(json: JSONPrivateKey, masterPassword: string, keyUsage: KeyUsages) {
		const salt = base642ui(json.salt);
		const params = getKeyParamsFromJson(json.params);
		const crypto = getCrypto();
		const strWrapKey = json.key;
		const wrapKey = base642ui(strWrapKey);
		const wrapParams = new AesGcmParams();
		wrapParams.fromJSON(json.wrapParams);
		const key = await crypto.unwrapKey(exportKeyFormats.pkcs8, wrapKey, str2ui(masterPassword), salt, wrapParams, params, keyUsage);

		return new PrivateKey(key, params, salt);
	}
	override async toJSON(masterPassword: string, keyUsage: KeyUsages) {
		const crypto = getCrypto();
		const iv = crypto.getRandomValues(12);
		const aesParams = new AesGcmParams(iv);
		const wrapKey = await crypto.wrapKey(this.exportFormat, this, str2ui(masterPassword), this.salt, aesParams, keyUsage);
		const base64WrapKey = ab2base64(wrapKey);
		const params = this.params.toJSON();
		const wrapParams = aesParams.toJSON();
		return {
			format: this.exportFormat,
			key: base64WrapKey,
			salt: ab2base64(this.salt),
			params,
			wrapParams
		};
	}
}

export class KeyPair {
	privateKey;
	publicKey;
	date;
	type = keyTypes.pair;
	keyUsage;
	guid;
	isValid;
	static async fromJSON(json: JSONKeyPair, masterPassword: string) {
		const keyUsage = KeyUsages.fromJSON(json.keyUsage);
		const publicKey = await PublicKey.fromJSON(json.publicKey);
		const privateKey = await PrivateKey.fromJSON(json.privateKey, masterPassword, keyUsage);
		const date = new Date(json.date);
		const guid = json.guid;
		const isValid = json.isValid;
		return new KeyPair(publicKey, privateKey, keyUsage, date, guid, isValid);
	}
	constructor(publicKey: PublicKey, privateKey: PrivateKey, keyUsage = new KeyUsages(true), date = new Date(), guid: string = getGUID(), isValid: boolean = true) {
		this.privateKey = privateKey;
		this.publicKey = publicKey;
		this.date = date;
		this.keyUsage = keyUsage;
		this.guid = guid;
		this.isValid = isValid;
	}
	async toJSON(masterPassword: string) {
		return {
			publicKey: await this.publicKey.toJSON(),
			privateKey: await this.privateKey.toJSON(masterPassword, this.keyUsage),
			date: this.date.toISOString(),
			keyUsage: this.keyUsage.toJSON(),
			guid: this.guid,
			isValid: this.isValid
		}
	}
	setIsValid(isValid: boolean) {
		this.isValid = isValid;
	};
}
export class KeyUsages {
	isEncrypt;
	isSign;
	constructor(isEncrypt?: boolean, isSign?: boolean) {
	this.isEncrypt = !!isEncrypt;
	this.isSign = !!isSign;
}
static fromJSON(json: ReturnType<KeyUsages["toJSON"]>) {
	return new KeyUsages(json.isEncrypt, json.isSign);
}
toJSON() {
	return {
		isEncrypt: this.isEncrypt,
		isSign: this.isSign
	}
}
}


function writeString(memory) {

}
function writeBool() {

}
function writeLong() {

}
function writeByte() {

}
function writeDouble() {

}
function writeBuffer() {

}
function writeBuffer() {

}

const c_oAscCipherKeyType = {
	NoType: 0,
	WebSymmetricKey: 1,
	WebSignKeyPair: 2,
	WebEncryptKeyPair: 3,
	WebPublicKey: 4,
	WebPrivateKey: 5,
};


function CryptoKeyBase() {
	this.date = null;
	this.isValid = false;
	this.guid = null;
	this.version = 1;
	this.type = c_oAscCipherKeyType.NoType;
}
CryptoKeyBase.import = function(reader, version, symmetricKey) {

};
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
CryptoKeyBase.prototype.setGUID = function(guid) {
	this.guid = guid;
};
CryptoKeyBase.prototype.setVersion = function(version) {
	this.version = version;
};
CryptoKeyBase.prototype.setType = function(type) {
	this.type = type;
};


function WebKeyPair() {
	CryptoKeyBase.call(this);
	this.privateKey = null;
	this.publicKey = null;
	this.version = 1;
}
WebKeyPair.prototype = Object.create(CryptoKeyBase.prototype);
WebKeyPair.prototype.constructor = WebKeyPair;
WebKeyPair.prototype.setPublicKey = function(publicKey) {
	this.publicKey = publicKey;
};
WebKeyPair.prototype.setPrivateKey = function(privateKey) {
	this.privateKey = privateKey;
};

WebKeyPair.import = function(reader) {
	const keyPair = new this();
	keyPair.setVersion(readLong(reader));
	switch (keyPair.version) {
		case 1: {
			keyPair.setDate(readString(reader));
			keyPair.setGUID(readString(reader));
			keyPair.setIsValid(readBool(reader));
			keyPair.setPublicKey(readObject(reader));
			keyPair.setPrivateKey(readObject(reader));
			break;
		}
		default: {
			return null;
		}
	}
};
WebKeyPair.prototype.export = function() {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeString(writer, this.date.toISOString());
			writeString(writer, this.guid);
			writeBool(writer, this.isValid);
			writeObject(writer, this.publicKey);
			writeObject(writer, this.privateKey);
			break;
		}
		default: {
			break;
		}
	}
};


function WebSignKeyPair() {
	WebKeyPair.call(this);
	this.type = c_oAscCipherKeyType.WebSignKeyPair;
}
WebSignKeyPair.prototype = Object.create(WebKeyPair.prototype);
WebSignKeyPair.prototype.constructor = WebSignKeyPair;
WebSignKeyPair.import = WebKeyPair.import;

function WebEncryptSignPair() {
	WebKeyPair.call(this);
	this.privateKey = null;
	this.publicKey = null;
	this.type = c_oAscCipherKeyType.WebEncryptKeyPair;
}
WebEncryptSignPair.prototype = Object.create(WebKeyPair.prototype);
WebEncryptSignPair.prototype.constructor = WebEncryptSignPair;
WebEncryptSignPair.import = WebKeyPair.import;

function AsymmetricKey() {
	this.binaryKey = null;
	this.cryptoKey = null;
	this.version = 1;
	this.type = c_oAscCipherKeyType.NoType;
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

function WebPrivateKey() {
	AsymmetricKey.call(this);
	this.type = c_oAscCipherKeyType.WebPrivateKey;
	this.salt = null;
}
WebPrivateKey.prototype = Object.create(AsymmetricKey.prototype);
WebPrivateKey.prototype.constructor = WebPrivateKey;


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



function WebPublicKey() {
	AsymmetricKey.call(this);
	this.binaryKey = null;
	this.cryptoKey = null;
}
WebPublicKey.prototype = Object.create(AsymmetricKey.prototype);
WebPublicKey.prototype.constructor = WebPublicKey;
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

function WebSymmetricKey() {
	CryptoKeyBase.call(this);
	this.version = 1;
	this.type = c_oAscCipherKeyType.WebSymmetricKey;
	this.cryptoKey = null;
	this.binaryKey = null;
}
WebSymmetricKey.prototype = Object.create(CryptoKeyBase.prototype);
WebSymmetricKey.prototype.constructor = WebSymmetricKey;
WebSymmetricKey.prototype.setBinaryKey = function(buffer) {
	this.binaryKey = buffer;
};
WebSymmetricKey.prototype.getCryptoKey = function() {
	if (!this.cryptoKey && this.binaryKey) {

	}
	return this.cryptoKey;
};

WebSymmetricKey.import = function(reader) {
	const symmetricKey = new WebSymmetricKey();
	symmetricKey.setVersion(readLong(reader));
	switch (symmetricKey.version) {
		case 1: {
			symmetricKey.setDate(readString(reader));
			symmetricKey.setGUID(readString(reader));
			symmetricKey.setIsValid(readBool(reader));
			symmetricKey.setBinaryKey(readBuffer(reader));
			break;
		}
		default:
			return null;
	}
	return symmetricKey;
};
WebSymmetricKey.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeString(writer, this.date.toISOString());
			writeString(writer, this.guid);
			writeBool(writer, this.isValid);
			writeBuffer(writer, this.binaryKey);
			break;
		}
		default: {
			break;
		}
	}
};



