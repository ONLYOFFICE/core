import {KeyStorage} from "key-storage";
import {downloadBinary, selectBinary} from "./utils.ts";

export function StorageManager() {
    this.keyStorage = new KeyStorage();
}
StorageManager.prototype.getBinaryKeys = function () {
    return Promise.resolve(null);
};
StorageManager.prototype.loadKeysFromStorage = function() {
    const oThis = this;
    return Promise.all([this.getMasterPassword(), this.getBinaryKeys()]).then(function ([masterPassword, binaryData]) {
        return oThis.keyStorage.import(binaryData, masterPassword);
    });
}
StorageManager.prototype.changeMasterPassword = function(newMasterPassword) {
    const oThis = this;
    return this.getMasterPassword().then(function (oldMasterPassword) {
        return oThis.keyStorage.changeMasterPassword(oldMasterPassword, newMasterPassword);
    });
};
StorageManager.prototype.getMasterPassword = function() {
    return Promise.resolve(null);
};
StorageManager.prototype.writeKeys = function() {
    const oThis = this;
    return this.keyStorage.export().then(function (exportedKeys) {
        return oThis.setStorageKeys(exportedKeys);
    });
}
StorageManager.prototype.setStorageKeys = function (exportedKeys) {
    return Promise.resolve();
};
StorageManager.prototype.addNewKeys = function (keys) {
    this.keyStorage.addKeys(keys);
    return this.writeKeys();
};
StorageManager.prototype.deprecateKey = function (key) {
    key.setIsValid(false);
    return this.writeKeys();
};

StorageManager.prototype.exportKeys = function () {
    return this.keyStorage.export().then(downloadBinary);
};

StorageManager.prototype.importKeys = function (callback) {
    const oThis = this;
    return this.getMasterPassword().then(function (masterPassword) {
        selectBinary(function (file) {
            try {
                file.arrayBuffer().then(function (binaryData) {
                    return oThis.keyStorage.import(binaryData, masterPassword);
                }).then(function (keyObjects) {
                    return oThis.addNewKeys(keyObjects);
                }).then(function () {
                    callback();
                });
            } catch (e) {
            }
        });
    })
};

StorageManager.prototype.generateKeys = function (params) {
    return this.keyStorage.generateKey(params);
};
StorageManager.prototype.getValidKeys = function () {
    return this.keyStorage.getValidKeys();
};
StorageManager.prototype.getKeyByGuid = function (guid) {
    return this.keyStorage.getKeyByGuid(guid);
};
