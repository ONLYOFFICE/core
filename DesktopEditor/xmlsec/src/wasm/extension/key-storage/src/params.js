import {c_oAscAlgorithmType, c_oAscCryptoDigestType, c_oAscDigestType, c_oAscKeyStorageType} from "./defines";
import {readLong, readBuffer} from "./serialize/reader";
import {writeBuffer, writeLong} from "./serialize/writer";
import {CryptoBase, initClass} from "./utils";

function AlgorithmParams() {
	CryptoBase.call(this);
	this.version = 1;
}
initClass(AlgorithmParams, CryptoBase);
AlgorithmParams.import = function(reader) {
	const params = new this();
	params.setVersion(readLong(reader));
	switch (params.version) {
		case 1: {
			break;
		}
		default: {
			return null;
		}
	}
	return params;
};
AlgorithmParams.prototype.setVersion = function (version) {
	this.version = version;
};
AlgorithmParams.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeLong(writer, this.type);
			break;
		}
		default: {
			break;
		}
	}
};

function RsaImportParams(hash) {
	AlgorithmParams.call(this);
	this.hash = hash;
}
initClass(RsaImportParams, AlgorithmParams);
RsaImportParams.import = function(reader) {
	const params = new RsaImportParams();
	params.setVersion(readLong(reader));
	switch (params.version) {
		case 1: {
			params.setHash(readLong(reader));
			break;
		}
		default: {
			return null;
		}
	}
	return params;
};
RsaImportParams.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeLong(this.hash);
			break;
		}
		default: {
			break;
		}
	}
};
RsaImportParams.prototype.setHash = function (hash) {
	this.hash = hash;
};
export function RsaOAEPImportParams(hash) {
	RsaImportParams.call(this, hash);
}
initClass(RsaOAEPImportParams, RsaImportParams, c_oAscKeyStorageType.RSAOAEPImportParams);

function RSAKeyGenParams(hash, modulusLength, publicExponent) {
	RsaImportParams.call(this, hash);
	this.modulusLength = typeof modulusLength === "number" ? modulusLength : 2048;
	this.publicExponent = publicExponent || new Uint8Array([0x01, 0x00, 0x01]);
}
initClass(RSAKeyGenParams, RsaImportParams);
RSAKeyGenParams.prototype.getImportParams = function() {
	return new RsaImportParams(this.hash);
};

export function RsaOAEPKeyGenParams(hash, modulusLength, publicExponent) {
	RSAKeyGenParams.call(this, hash, modulusLength, publicExponent);
}
initClass(RsaOAEPImportParams, RsaImportParams, c_oAscKeyStorageType.RSAOAEPKeyGenParams);

export function Ed25519ImportParams() {
	AlgorithmParams.call(this);
}
initClass(Ed25519ImportParams, AlgorithmParams, c_oAscKeyStorageType.Ed25519ImportParams);
Ed25519ImportParams.import = AlgorithmParams.import;

export function Ed25519KeyGenParams() {
	Ed25519ImportParams.call(this);
}
initClass(Ed25519KeyGenParams, Ed25519ImportParams, c_oAscKeyStorageType.Ed25519KeyGenParams);
Ed25519KeyGenParams.prototype.getImportParams = function() {
	return new Ed25519ImportParams();
};

export function AesGcmCryptoParams(iv, tagLength) {
	AlgorithmParams.call(this);
	this.iv = iv || null;
	this.tagLength = typeof tagLength === "number" ? tagLength : null;
}
initClass(AesGcmCryptoParams, AlgorithmParams, c_oAscKeyStorageType.AesGCMCryptoParams);
AesGcmCryptoParams.import = function (reader) {
	const params = new AesGcmCryptoParams();
	params.setVersion(readLong(reader));
	switch (params.version) {
		case 1: {
			params.setIV(readBuffer(reader));
			params.setTagLength(readLong(reader));
			break;
		}
		default: {
			return null;
		}
	}
	return params;
};
AesGcmCryptoParams.prototype.setIV = function (iv) {
	this.iv = iv;
};
AesGcmCryptoParams.prototype.setTagLength = function (tagLength) {
	this.tagLength = tagLength;
};
AesGcmCryptoParams.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeBuffer(writer, this.iv);
			writeLong(writer, this.tagLength);
			break;
		}
		default: {
			break;
		}
	}
};
AesGcmCryptoParams.prototype.init = function () {
	this.iv = getRandomValues(12);
	this.tagLength = 128;
};


function AesKeyGenParams(length) {
	AlgorithmParams.call(this);
	this.length = length;
}
initClass(AesKeyGenParams, AlgorithmParams);
AesKeyGenParams.prototype.getImportParams = function() {
	return new AlgorithmParams();
};

export function AesGcmKeyGenParams() {
	AesKeyGenParams.call(this,  256);
}
initClass(AesGcmKeyGenParams, AlgorithmParams, c_oAscKeyStorageType.AesGCMKeyGenParams);

const PBKDFSaltLength = 16;
export function PBKDF2Params() {
	AlgorithmParams.call(this);
	this.iterations = 600000;
	this.hash = c_oAscDigestType.SHA256;
	this.salt = null;
}
initClass(PBKDF2Params, AlgorithmParams, c_oAscKeyStorageType.PBKDF2Params);
PBKDF2Params.import = function(reader) {
	const params = new PBKDF2Params();
	params.setVerison(readLong(reader));
	switch (params.version) {
		case 1: {
			params.setIterations(readLong(reader));
			params.setHash(readLong(reader));
			params.setSalt(readBuffer(reader));
			break;
		}
		default: {
			return null;
		}
	}
	return params;
}
PBKDF2Params.prototype.export = function(writer) {
	writeLong(this.version);
	switch (this.version) {
		case 1: {
			writeLong(writer, this.iterations);
			writeLong(writer, this.hash);
			writeBuffer(writer, this.salt);
			break;
		}
		default: {
			break;
		}
	}
}
PBKDF2Params.prototype.getCryptoParams = function() {
	return {
		name: 'PBKDF2',
		salt: this.salt,
		iterations: this.iterations,
		hash: c_oAscCryptoDigestType[this.hash]
	};
};
PBKDF2Params.prototype.setIterations = function(reader) {
	this.iterations = reader;
};
PBKDF2Params.prototype.setSalt = function(reader) {
	this.iterations = reader;
};
PBKDF2Params.prototype.setHash = function(reader) {
	this.iterations = reader;
};


