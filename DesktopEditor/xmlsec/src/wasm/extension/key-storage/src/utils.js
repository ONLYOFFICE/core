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

export function isEqualArrays(array1, array2) {
	if (!array1 && array2 || array1 && !array2) {
		return false;
	}
	if (array1.length !== array2.length) {
		return false;
	}
	for (let i = 0; i < array1.length; i += 1) {
		if (array1[i] !== array2[i]) {
			return false;
		}
	}
	return true;
}