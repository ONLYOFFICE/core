const algorithmTypes = {
	AES_GCM          : 1,
	ED25519          : 2,
	RSA_OAEP         : 3
};
export const digestTypes = {
	SHA1: 1,
	SHA256: 2,
	SHA384: 3,
	SHA512: 4,
};
function AlgorithmParams(type) {
	this.type = type;
	this.version = 1;
}
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
AlgorithmParams.prototype.export = function() {
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

function RsaImportParams(name, hash) {
	AlgorithmParams.call(this, name);
	this.hash = hash;
}
RsaImportParams.prototype = Object.create(AlgorithmParams.prototype);
RsaImportParams.prototype.constructor = RsaImportParams;
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
}

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
Ed25519ImportParams.import = AlgorithmParams.import;

function Ed25519KeyGenParams() {
	Ed25519ImportParams.call(this);
}
Ed25519KeyGenParams.prototype = Object.create(AlgorithmParams.prototype);
Ed25519KeyGenParams.prototype.constructor = Ed25519KeyGenParams;
Ed25519KeyGenParams.prototype.getImportParams = function() {
	return new Ed25519ImportParams();
};

function AesGcmCryptoParams(iv, tagLength) {
	AlgorithmParams.call(this, algorithmTypes.AES_GCM);
	this.iv = iv || null;
	this.tagLength = typeof tagLength === "number" ? tagLength : null;
}
AesGcmCryptoParams.prototype = Object.create(AlgorithmParams.prototype);
AesGcmCryptoParams.prototype.constructor = AesGcmCryptoParams;
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
	AesKeyGenParams.call(this, algorithmTypes.AES_GCM, 256);
}
AesGcmGenParams.prototype = Object.create(AlgorithmParams.prototype);
AesGcmGenParams.prototype.constructor = AesGcmGenParams;


