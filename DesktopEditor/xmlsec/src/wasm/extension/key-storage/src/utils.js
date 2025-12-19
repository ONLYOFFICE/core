import {c_oAscKeyStorageType} from "./defines";

export function CryptoBase() {
	this.objectType = c_oAscKeyStorageType.NoType;
}
CryptoBase.prototype.getObjectType = function () {
	return this.objectType;
};

export function initClass(fClass, fBase, type) {
	fClass.prototype = Object.create(fBase.prototype);
	fClass.prototype.constructor = fClass;
	fClass.prototype.objectType = type || c_oAscKeyStorageType.NoType;
}