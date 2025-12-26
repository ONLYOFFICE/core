import {getCrypto} from "./crypto";
import {BinaryReader, readObject} from "./serialize/reader";
import {BinaryWriter, writeObject} from "./serialize/writer";
import {PBKDF2Params, PBKDFSaltLength} from "./params";

export function KeyStorage() {
	this.keys = [];
	this.version = 1;
	this.pbkdfParams = null;
	this.isInit = false;
}
KeyStorage.prototype.init = function() {
	this.pbkdfParams = new PBKDF2Params(true);
	this.isInit = true;
};
KeyStorage.prototype.export = function () {
	const writer = new BinaryWriter();
	writer.WriteLong(this.version);
	switch (this.version) {
		case 1: {
			writeObject(writer, this.pbkdfParams);
			writer.WriteLong(this.keys.length);
			for (let i = 0; i < this.keys.length; i++) {
				const key = this.keys[i];
				writeObject(writer, key);
			}
			break;
		}
		default: {
			break;
		}
	}
	return writer.GetData();
};
KeyStorage.prototype.import = function (binaryData, masterPassword) {
	const oThis = this;
	const reader = new BinaryReader(binaryData, binaryData.length);
	this.version = reader.GetLong();
	switch (this.version) {
		case 1: {
			this.setPBKDFParams(readObject(reader));
			const length = reader.GetLong();
			const keys = [];
			for (let i = 0; i < length; i++) {
				const key = readObject(reader);
				if (key) {
					keys.push(key);
				}
			}
			const crypto = getCrypto();
			return crypto.getAesKey(masterPassword, this.pbkdfParams).then(function(aesKey) {
				const initKeys = keys.map(function (key) {
					return key.initKey(aesKey);
				});
				return Promise.all(initKeys).then(function() {
					return oThis.addKeys(keys);
				});
			});
		}
		default: {
			return Promise.resolve();
		}
	}
};
KeyStorage.prototype.changeMasterPassword = function (oldMasterPassword, newMasterPassword) {
	const oThis = this;
	const oldPBKDFParams = this.pbkdfParams;
	const newPBKDFParams = new PBKDF2Params(true);
	const crypto = getCrypto();
	Promise.all([crypto.getAesKey(oldMasterPassword, oldPBKDFParams), crypto.getAesKey(newMasterPassword, newPBKDFParams)]).then(function(aesKeys) {
		const keys = oThis.keys.map(function(key) {
			return key.changeMasterPassword(aesKeys[0], aesKeys[1]);
		});
		return Promise.all(keys);
	}).then(function() {
		oThis.pbkdfParams = newPBKDFParams;
	});
};
KeyStorage.prototype.getKeyByGuid = function (guid) {
	for (let i = 0; i < this.keys.length; i++) {
		const key = this.keys[i];
		if (guid === key.guid) {
			return key;
		}
	}
};
KeyStorage.prototype.getValidKeys = function () {
	const keys = [];
	for (let i = 0; i < this.keys.length; i++) {
		const key = this.keys[i];
		if (key.isValid) {
			keys.push(key);
		}
	}
	return keys;
};
KeyStorage.prototype.generateKey = function (keyParams, masterPassword) {
	const crypto = getCrypto();
	return crypto.getAesKey(masterPassword, this.pbkdfParams).then(function(aesKey) {
		return crypto.generateKey(keyParams, aesKey);
	});
};
KeyStorage.prototype.addKeys = function (keys) {
	this.keys.push.apply(this.keys, keys);
};
KeyStorage.prototype.setKeys = function (keys) {
	this.keys = keys;
};
KeyStorage.prototype.getKeyByPublicKey = function(publicKeyData) {
	const reader = new BinaryReader(publicKeyData, publicKeyData.length);
	const publicKey = readObject(reader);
	for (let i = 0; i < this.keys.length; i++) {
		if (this.keys[i].isHavePublicKey(publicKey)) {
			return this.keys[i];
		}
	}
	return null;
};
KeyStorage.prototype.setPBKDFParams = function(params) {
	this.pbkdfParams = params;
};