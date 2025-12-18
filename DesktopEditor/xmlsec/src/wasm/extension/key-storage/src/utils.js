export function initClass(fClass, fBase, type) {
	fClass.prototype = Object.create(fBase.prototype);
	fClass.prototype.constructor = fClass;
	fClass.prototype.objectType = type;
}