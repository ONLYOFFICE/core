import {getCrypto} from "./crypto";
import {BinaryReader, readObject} from "./serialize/reader";
import {BinaryWriter, writeObject} from "./serialize/writer";

export function StorageManager() {

}

StorageManager.prototype.getBinaryKeys = function () {
	return new Promise(function (resolve) {
		resolve([]);
	});
};

StorageManager.prototype.setStorageKeys = function (jsonKeys) {
	return new Promise(function (resolve) {
		resolve([]);
	});
};

StorageManager.prototype.getMasterPassword = function () {
	return new Promise(function (resolve) {
		resolve(null);
	});
};

StorageManager.prototype.setStorageKeys = function () {
	return new Promise(function (resolve) {
		resolve(null);
	});
};

export function KeyStorage(storageManager) {
	this.keys = [];
	this.storageManager = storageManager;
}

KeyStorage.prototype.loadKeysFromStorage = function () {
	const oThis = this;
	return this.storageManager.getBinaryKeys().then(function (binaryData) {
		return oThis.import(binaryData);
	}).then(function (loadedKeys) {
		oThis.keys = loadedKeys;
	});
};

KeyStorage.prototype.addNewKeys = function (keys) {
	this.keys.push.apply(this.keys, keys);
	return this.writeKeys();
};

KeyStorage.prototype.export = function () {
	const oThis = this;
	const writer = new BinaryWriter();
	writer.WriteLong(this.keys.length);
	for (let i = 0; i < this.keys.length; i++) {
		const key = this.keys[i];
		writeObject(writer, key);
	}
	return writer.GetData();
};
KeyStorage.prototype.import = function (binaryData) {
	const oThis = this;
	const reader = new BinaryReader(binaryData, binaryData.length);
	const length = reader.GetLong();
	for (let i = 0; i < length; i++) {
		const key = readObject(reader);
		if (key) {
			this.keys.push(key);
		}
	}
};

KeyStorage.prototype.writeKeys = function () {
	const oThis = this;
	return this.export().then(function (exportedKeys) {
		oThis.setStorageKeys(exportedKeys);
	});
};

KeyStorage.prototype.setStorageKeys = function (exportedKeys) {
	return this.storageManager.setStorageKeys(exportedKeys);
};

KeyStorage.prototype.getMasterPassword = function () {
	return this.storageManager.getMasterPassword();
};

KeyStorage.prototype.changeMasterPassword = function (newMasterPassword) {
	const oThis = this;
	this.storageManager.getMasterPassword().then(function(oldMasterPassword) {
		const keys = oThis.keys.map(function(key) {
			return key.changeMasterPassword(oldMasterPassword, newMasterPassword);
		});
		return Promise.all(keys);
	});
};

KeyStorage.prototype.exportKeys = function () {
	return this.export().then(function (data) {
		const passwordInfo = {
			encrypt: false,
			data   : data
		};
		const blob = new Blob([JSON.stringify(passwordInfo)], {type: "application/json"});
		const url = URL.createObjectURL(blob);

		const link = document.createElement("a");
		link.href = url;
		link.download = `onlyoffice_keychain_${(new Date()).toISOString()}.json`;
		link.click();

		URL.revokeObjectURL(url);
	});
};

KeyStorage.prototype.importKeys = function (callback) {
	const oThis = this;
	selectUserJSON(function (file) {
		try {
			file.text().then(function (text) {
				const json = JSON.parse(text);
				if (!json.encrypt) {
					return oThis.loadKeys(json.data);
				} else {
					throw "Need support encrypt JSON"
				}
			}).then(function (keyObjects) {
				return oThis.addNewKeys(keyObjects);
			}).then(function () {
				callback();
			});
		} catch (e) {
		}
	});
};

KeyStorage.prototype.getKeyPairByGuid = function (guid) {
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

KeyStorage.prototype.deprecateKey = function (key) {
	key.setIsValid(false);
	return this.writeKeys();
};
KeyStorage.prototype.generateKey = function (keyParams) {
	const crypto = getCrypto();
	return crypto.generateKey(keyParams);
};