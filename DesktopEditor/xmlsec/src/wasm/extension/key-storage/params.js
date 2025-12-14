import {
	aesTypes,
	algorithmTypes,

} from "./key-types.ts";

function AlgorithmParams(name) {
	this.name = name;
}
AlgorithmParams.prototype.export = function() {};
AlgorithmParams.prototype.import = function() {};
function RsaImportParams(name, hash) {
	AlgorithmParams.call(this, name);
	this.hash = hash;
}
RsaImportParams.prototype = Object.create(AlgorithmParams.prototype);
RsaImportParams.prototype.constructor = RsaImportParams;
RsaImportParams.prototype.export = function() {

};
RsaImportParams.prototype.import = function() {

};

function RSAKeyGenParams(name, hash, modulusLength, publicExponent) {
	RsaImportParams.call(this, name, hash);
	this.modulusLength = typeof modulusLength === "number" ? modulusLength : 2048;
	this.publicExponent = publicExponent || new Uint8Array([0x01, 0x00, 0x01]);
}
RSAKeyGenParams.prototype = Object.create(RsaImportParams.prototype);
RSAKeyGenParams.prototype.constructor = RSAKeyGenParams;
RSAKeyGenParams.prototype.getImportParams = function() {
	return new RsaImportParams(this.name, this.hash);
};

function Ed25519ImportParams() {
	AlgorithmParams.call(this, algorithmTypes.ED25519);
}
Ed25519ImportParams.prototype = Object.create(AlgorithmParams.prototype);
Ed25519ImportParams.prototype.constructor = Ed25519ImportParams;
Ed25519ImportParams.prototype.export = function() {

};
Ed25519ImportParams.prototype.import = function() {

};
function Ed25519KeyGenParams() {
	Ed25519ImportParams.call(this);
}
Ed25519KeyGenParams.prototype = Object.create(AlgorithmParams.prototype);
Ed25519KeyGenParams.prototype.constructor = Ed25519KeyGenParams;
Ed25519KeyGenParams.prototype.getImportParams = function() {
	return new Ed25519ImportParams();
};

function AesGcmParams(iv) {
	AlgorithmParams.call(this, algorithmTypes.AES_GCM);
	this.iv = iv;
}
AesGcmParams.prototype = Object.create(AlgorithmParams.prototype);
AesGcmParams.prototype.constructor = AesGcmParams;


function AesKeyGenParams(name, length) {
	AlgorithmParams.call(this, name);
	this.length = length;
}
AesKeyGenParams.prototype = Object.create(AlgorithmParams.prototype);
AesKeyGenParams.prototype.constructor = AesKeyGenParams;
AesKeyGenParams.prototype.getImportParams = function() {
	return new AlgorithmParams(this.name);
};

function AesGcmGenParams() {
	AesKeyGenParams.call(this, aesTypes.AES_GCM, 256);
}
AesGcmGenParams.prototype = Object.create(AlgorithmParams.prototype);
AesGcmGenParams.prototype.constructor = AesGcmGenParams;


