// @ts-ignore
import {StorageManager} from "../../../key-storage/key-storage.js";
import browser from "webextension-polyfill";
import type {JSONKeyPair} from "./keys/key-types.ts";

function ExtensionStorageManager() {
    StorageManager.call(this);
}
ExtensionStorageManager.prototype = Object.create(StorageManager);
ExtensionStorageManager.prototype.constructor = ExtensionStorageManager;
ExtensionStorageManager.prototype.getStorageKeys = function() {
    return browser.storage.local.get("keys").then(function(item) {
        if (item && Array.isArray(item.keys)) {
            return item.keys;
        }
        return [];
    });
};
ExtensionStorageManager.prototype.getMasterPassword = function() {
    return browser.storage.local.get("masterPassword").then(function(item) {
        return item.masterPassword ? item.masterPassword : null;
    });
};
ExtensionStorageManager.prototype.setStorageKeys = function(exportedKeys: JSONKeyPair[]) {
    return browser.storage.local.set({keys: exportedKeys});
}
ExtensionStorageManager.prototype.setMasterPasswordWithKeys = function(exportedKeys: JSONKeyPair[]) {
    return browser.storage.local.set({keys: exportedKeys});
}