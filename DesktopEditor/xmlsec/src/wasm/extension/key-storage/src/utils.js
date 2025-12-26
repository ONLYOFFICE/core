import {c_oAscKeyStorageType} from "./defines";
import {getCrypto} from "./crypto";

export function CryptoBase() {
}
CryptoBase.prototype.getObjectType = function () {
	return this.objectType;
};

export function initClass(fClass, fBase, type) {
	fClass.prototype = Object.create(fBase.prototype);
	fClass.prototype.constructor = fClass;
	fClass.prototype.objectType = type || c_oAscKeyStorageType.NoType;
}

export function getRandomValues(length) {
	const crypto = getCrypto();
	return crypto.getRandomValues(length);
}