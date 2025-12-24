import {getCrypto} from "./crypto";
import {BinaryReader, readObject} from "./serialize/reader";
import {BinaryWriter, writeObject} from "./serialize/writer";

export function KeyStorage() {
	this.keys = [];
	this.version = 1;
}

KeyStorage.prototype.export = function () {
	const writer = new BinaryWriter();
	writer.WriteLong(this.version);
	switch (this.version) {
		case 1: {
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
	const reader = new BinaryReader(binaryData, binaryData.length);
	this.version = reader.GetLong();
	switch (this.version) {
		case 1: {
			const length = reader.GetLong();
			const keys = [];
			for (let i = 0; i < length; i++) {
				const key = readObject(reader);
				if (key) {
					keys.push(key);
				}
			}
			const initKeys = keys.map(function (key) {
				return key.initKey(masterPassword);
			});
			const oThis = this;
			return Promise.all(initKeys).then(function() {
				return oThis.addKeys(keys);
			});
		}
		default: {
			return Promise.resolve();
		}
	}
};
KeyStorage.prototype.changeMasterPassword = function (oldMasterPassword, newMasterPassword) {
	const oThis = this;
	const keys = oThis.keys.map(function(key) {
		return key.changeMasterPassword(oldMasterPassword, newMasterPassword);
	});
	return Promise.all(keys);
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
KeyStorage.prototype.generateKey = function (keyParams) {
	const crypto = getCrypto();
	return crypto.generateKey(keyParams);
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