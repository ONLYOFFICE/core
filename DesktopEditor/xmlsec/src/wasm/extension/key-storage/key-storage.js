import browser from "webextension-polyfill";
import {KeyPair} from "./keys/keys.ts";
import {selectUserJSON} from "./utils.ts";

(function(global, factory) {
	if (typeof define === 'function' && define.amd) {
		define([], factory);
	} else if (typeof module === 'object' && module.exports) {
		module.exports = factory();
	} else {
		global["AscCommon"] = global["AscCommon"] || {};
		global["AscCommon"].KeyStorageLibrary = factory();
	}
}(typeof window !== 'undefined' ? window : global, function() {
	function StorageManager() {

	}
	StorageManager.prototype.getStorageKeys = function() {
		return new Promise(function(resolve) {
			resolve([]);
		});
	};
	StorageManager.prototype.setStorageKeys = function(jsonKeys) {
		return new Promise(function(resolve) {
			resolve([]);
		});
	};
	StorageManager.prototype.getMasterPassword = function() {
		return new Promise(function(resolve) {
			resolve(null);
		});
	};
	StorageManager.prototype.setStorageKeys = function() {
		return new Promise(function(resolve) {
			resolve(null);
		});
	};
	StorageManager.prototype.setMasterPasswordWithKeys = function() {
		return new Promise(function(resolve) {
			resolve(null);
		});
	};

	function KeyStorage(storageManager) {
		this.keys = [];
		this.storageManager = storageManager;
	}

	KeyStorage.prototype.loadKeysFromStorage = function() {
		const oThis = this;
		return this.storageManager.getStorageKeys().then(function(keys) {
			return oThis.loadKeys(keys);
		}).then(function(loadedKeys) {
			oThis.keys = loadedKeys;
		});
	};

	KeyStorage.prototype.loadKeys = function(exportedKeys) {
		return this.getMasterPassword().then(function(masterPassword) {
			if (masterPassword === null) {
				return [];
			}

			const promises = exportedKeys.map(function(key) {
				return KeyPair.fromJSON(key, masterPassword);
			});

			return Promise.all(promises);
		});
	};

	KeyStorage.prototype.addNewKeys = function(keys) {
		this.keys.push.apply(this.keys, keys);
		return this.writeKeys();
	};

	KeyStorage.prototype.export = function() {
		const oThis = this;
		return this.getMasterPassword().then(function(masterPassword) {
			if (typeof masterPassword !== "string") {
				return [];
			}
			const exportedKeyPromise = oThis.keys.map(function(key) {
				return key.export(masterPassword);
			});

			return Promise.all(exportedKeyPromise);
		});
	};

	KeyStorage.prototype.writeKeys = function() {
		const oThis = this;
		return this.export().then(function(exportedKeys) {
			oThis.setStorageKeys(exportedKeys);
		});
	};

	KeyStorage.prototype.setStorageKeys = function(exportedKeys) {
		return this.storageManager.setStorageKeys(exportedKeys);
	};

	KeyStorage.prototype.getMasterPassword = function() {
		return this.storageManager.getMasterPassword();
	};

	KeyStorage.prototype.changeMasterPassword = function(newMasterPassword) {
		const oThis = this;
		return this.export(newMasterPassword).then(function(keys) {
			return oThis.storageManager.setMasterPasswordWithKeys(newMasterPassword, keys);
		});
	};

	KeyStorage.prototype.exportKeys = function() {
		return this.export().then(function(data) {
			const passwordInfo = {
				encrypt: false,
				data: data
			};
			const blob = new Blob([JSON.stringify(passwordInfo)], { type: "application/json"});
			const url = URL.createObjectURL(blob);

			const link = document.createElement("a");
			link.href = url;
			link.download = `onlyoffice_keychain_${(new Date()).toISOString()}.json`;
			link.click();

			URL.revokeObjectURL(url);
		});
	};

	KeyStorage.prototype.importKeys = function(callback) {
		const oThis = this;
		selectUserJSON(function (file) {
			try {
				file.text().then(function(text) {
					const json = JSON.parse(text);
					if (!json.encrypt) {
						return oThis.loadKeys(json.data);
					} else {
						throw "Need support encrypt JSON"
					}
				}).then(function(keyObjects) {
					return oThis.addNewKeys(keyObjects);
				}).then(function() {
					callback();
				});
			} catch (e) {
			}
		});
	};

	KeyStorage.prototype.getKeyPairByGuid = function(guid) {
		for (let i = 0; i < this.keys.length; i++) {
			const key = this.keys[i];
			if (guid === key.guid) {
				return key;
			}
		}
	};

	KeyStorage.prototype.getValidKeys = function() {
		const keys = [];
		for (let i = 0; i < this.keys.length; i++) {
			const key = this.keys[i];
			if (key.isValid) {
				keys.push(key);
			}
		}
		return keys;
	};

	KeyStorage.prototype.deprecateKey = function(key) {
		key.setIsValid(false);
		return this.writeKeys();
	};

	return {
		KeyStorage: KeyStorage,
		StorageManager: StorageManager
	};
}));