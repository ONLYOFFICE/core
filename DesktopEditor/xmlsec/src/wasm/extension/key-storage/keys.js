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

const c_oAscCipherKeyVersion = {
	KeyBaseNoVersion: 0,
	WebKeyPairV1: 1,
	WebSymmetricKeyV1: 2
};


function CryptoKeyBase() {
	this.date = null;
	this.isValid = false;
	this.guid = null;
	this.version = c_oAscCipherKeyVersion.KeyBaseNoVersion;
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
	this.guid = version;
};


function WebKeyPair() {
	CryptoKeyBase.call(this);
	this.privateKey = null;
	this.publicKey = null;
	this.version = c_oAscCipherKeyVersion.WebKeyPairV1;
}
WebKeyPair.prototype = Object.create(CryptoKeyBase.prototype);
WebKeyPair.prototype.constructor = WebKeyPair;

function WebPrivateKey() {

}

function WebPublicKey() {
	
}

function WebSymmetricKey() {
	CryptoKeyBase.call(this);
	this.version = c_oAscCipherKeyVersion.WebSymmetricKeyV1;
	this.cryptoKey = null;
	this.binaryKey = null;
}

WebSymmetricKey.prototype = Object.create(CryptoKeyBase.prototype);
WebSymmetricKey.prototype.constructor = WebSymmetricKey;
WebSymmetricKey.prototype.setBinaryKey = function() {

};
WebSymmetricKey.prototype.getCryptoKey = function() {
	if (!this.cryptoKey && this.binaryKey) {

	}
	return this.cryptoKey;
};

WebSymmetricKey.import = function(reader, version) {
	switch (version) {
		case c_oAscCipherKeyVersion.WebSymmetricKeyV1: {
			const symmetricKey = new WebSymmetricKey();
			symmetricKey.setVersion(readByte(reader));
			symmetricKey.setDate(readString(reader));
			symmetricKey.setGUID(readString(reader));
			symmetricKey.setIsValid(readBool(reader));
			symmetricKey.setBinaryKey();
			return symmetricKey;
		}
		default:
			return null;
	}
};
WebSymmetricKey.prototype.export = function(writer) {
	writeByte(writer, this.version);
	writeString(writer, this.date.toISOString());
	writeString(writer, this.guid);
	writeBool(this.isValid);
	writeBuffer(this.binaryKey);
};



