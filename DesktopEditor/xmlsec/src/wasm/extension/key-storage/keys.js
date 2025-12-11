import getCrypto from "../crypto.ts";
function writeString(memory) {

}
function writeBool() {

}
function writeLong() {

}
function writeByte() {

}
function writeDouble() {

}
function writeBuffer() {

}
function writeObject() {

}
function PromiseManager(initPromise) {
	this.data = null;
	this.error = null;
	this.isResolved = false;
	this.isRejected = false;
	this.resolvers = [];
	const oThis= this;
	initPromise.then(function(data) {
		oThis.isResolved = true;
		oThis.data = data;
	}).catch(function(error) {
		oThis.isRejected = true;
		oThis.error = error;
	}).finally(function() {
		oThis.handleResolvers();
	});
}
PromiseManager.prototype.getPromise = function() {
	const oThis = this;
	return new Promise(function(resolve, reject) {
		if (oThis.isResolved) {
			resolve(oThis.data);
		} else if (oThis.isRejected) {
			reject(oThis.error);
		} else {
			oThis.resolvers.push({resolve: resolve, reject: reject});
		}
	});
};
PromiseManager.prototype.handleResolvers = function() {
	while (this.resolvers.length) {
		const resolver = this.resolvers.pop();
		if (this.isResolved) {
			resolver.resolve(this.data);
		} else if (this.isRejected) {
			resolver.reject(this.error);
		}
	}
};
const c_oAscCipherKeyType = {
	NoType: 0,
	WebSymmetricKey: 1,
	WebSignKeyPair: 2,
	WebEncryptKeyPair: 3,
	WebPublicKey: 4,
	WebPrivateKey: 5,
};


function CryptoKeyBase() {
	this.date = null;
	this.isValid = false;
	this.uid = null;
	this.version = 1;
	this.type = c_oAscCipherKeyType.NoType;
}
CryptoKeyBase.import = function(reader, version, symmetricKey) {};
CryptoKeyBase.prototype.init = function() {
	const crypto = getCrypto();
	this.setDate(new Date());
	this.setIsValid(true);
	this.setUID(crypto.randomUUID());
}
CryptoKeyBase.prototype.export = function(writer) {

};
CryptoKeyBase.prototype.encrypt = function() {

};
CryptoKeyBase.prototype.decrypt = function() {

};
CryptoKeyBase.prototype.setDate = function(date) {
	this.date = new Date(date);
};
CryptoKeyBase.prototype.setIsValid = function(isValid) {
	this.isValid = isValid;
};
CryptoKeyBase.prototype.setUID = function(uid) {
	this.uid = uid;
};
CryptoKeyBase.prototype.setVersion = function(version) {
	this.version = version;
};
CryptoKeyBase.prototype.setType = function(type) {
	this.type = type;
};


function WebKeyPair() {
	CryptoKeyBase.call(this);
	this.privateKey = null;
	this.publicKey = null;
	this.version = 1;
}
WebKeyPair.prototype = Object.create(CryptoKeyBase.prototype);
WebKeyPair.prototype.constructor = WebKeyPair;
WebKeyPair.prototype.setPublicKey = function(publicKey) {
	this.publicKey = publicKey;
};
WebKeyPair.prototype.setPrivateKey = function(privateKey) {
	this.privateKey = privateKey;
};

WebKeyPair.import = function(reader) {
	const keyPair = new this();
	keyPair.setVersion(readLong(reader));
	switch (keyPair.version) {
		case 1: {
			keyPair.setDate(readString(reader));
			keyPair.setUID(readString(reader));
			keyPair.setIsValid(readBool(reader));
			keyPair.setPublicKey(readObject(reader));
			keyPair.setPrivateKey(readObject(reader));
			break;
		}
		default: {
			return null;
		}
	}
};
WebKeyPair.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeString(writer, this.date.toISOString());
			writeString(writer, this.uid);
			writeBool(writer, this.isValid);
			writeObject(writer, this.publicKey);
			writeObject(writer, this.privateKey);
			break;
		}
		default: {
			break;
		}
	}
};


function WebSignKeyPair() {
	WebKeyPair.call(this);
	this.type = c_oAscCipherKeyType.WebSignKeyPair;
}
WebSignKeyPair.prototype = Object.create(WebKeyPair.prototype);
WebSignKeyPair.prototype.constructor = WebSignKeyPair;
WebSignKeyPair.import = WebKeyPair.import;
WebSignKeyPair.fromCryptoBuffer = function(publicKeyBuffer, privateKeyBuffer) {
	const keyPair = new WebSignKeyPair();
	keyPair.init();
	const publicKey = new WebPublicKey();
	publicKey.setBinaryKey(publicKeyBuffer);
	const privateKey = new WebPrivateKey();
	privateKey.setBinaryKey(privateKeyBuffer);
	keyPair.setPublicKey(publicKey);
	keyPair.setPrivateKey(privateKey);
	return keyPair;
};

function WebEncryptKeyPair() {
	WebKeyPair.call(this);
	this.privateKey = null;
	this.publicKey = null;
	this.type = c_oAscCipherKeyType.WebEncryptKeyPair;
}
WebEncryptKeyPair.prototype = Object.create(WebKeyPair.prototype);
WebEncryptKeyPair.prototype.constructor = WebEncryptKeyPair;
WebEncryptKeyPair.import = WebKeyPair.import;
WebEncryptKeyPair.fromCryptoBuffer = function(publicKeyBuffer, privateKeyBuffer) {
	const keyPair = new WebEncryptKeyPair();
	keyPair.init();
	const publicKey = new WebPublicKey();
	publicKey.setBinaryKey(publicKeyBuffer);
	const privateKey = new WebPrivateKey();
	privateKey.setBinaryKey(privateKeyBuffer);
	keyPair.setPublicKey(publicKey);
	keyPair.setPrivateKey(privateKey);
	return keyPair;
};

function AsymmetricKey() {
	this.binaryKey = null;
	this.cryptoKey = null;
	this.version = 1;
	this.type = c_oAscCipherKeyType.NoType;
}
AsymmetricKey.prototype.setBinaryKey = function(binaryKey) {
	this.binaryKey = binaryKey;
};
AsymmetricKey.prototype.setCryptoKey = function(cryptoKey) {
	this.cryptoKey = cryptoKey;
};
AsymmetricKey.prototype.setVersion = function(version) {
	this.version = version;
};
AsymmetricKey.prototype.changeMasterPassword = function(oldMasterPassword, newMasterPassword) {};

function WebPrivateKey() {
	AsymmetricKey.call(this);
	this.type = c_oAscCipherKeyType.WebPrivateKey;
	this.salt = null;
}
WebPrivateKey.prototype = Object.create(AsymmetricKey.prototype);
WebPrivateKey.prototype.constructor = WebPrivateKey;


WebPrivateKey.import = function(reader) {
	const key = new WebPrivateKey();
	key.setVersion(readLong(reader));
	switch (key.version) {
		case 1: {
			key.setBinaryKey(readBuffer(reader));
			key.setSalt(readBuffer(reader));
			break;
		}
		default:
			return null;
	}
	return key;
};
WebPrivateKey.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeBuffer(writer, this.binaryKey);
			writeBuffer(writer, this.salt);
			break;
		}
		default: {
			break;
		}
	}
};
WebPrivateKey.prototype.setSalt = function(salt) {
	this.salt = salt;
};
WebPrivateKey.prototype.changeMasterPassword = function(oldMasterPassword, newMasterPassword) {
	const oldPasswordKey = WebSymmetricKey.getFromPassword(oldMasterPassword, this.salt);

}



function WebPublicKey() {
	AsymmetricKey.call(this);
	this.binaryKey = null;
	this.cryptoKey = null;
}
WebPublicKey.prototype = Object.create(AsymmetricKey.prototype);
WebPublicKey.prototype.constructor = WebPublicKey;
WebPublicKey.import = function(reader) {
	const key = new WebPublicKey();
	key.setVersion(readLong(reader));
	switch (key.version) {
		case 1: {
			key.setBinaryKey(readBuffer(reader));
			break;
		}
		default:
			return null;
	}
	return key;
};
WebPublicKey.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeBuffer(writer, this.binaryKey);
			break;
		}
		default: {
			break;
		}
	}
};

function WebSymmetricKey() {
	CryptoKeyBase.call(this);
	this.version = 1;
	this.type = c_oAscCipherKeyType.WebSymmetricKey;
	this.cryptoKey = null;
	this.binaryKey = null;
}
WebSymmetricKey.prototype = Object.create(CryptoKeyBase.prototype);
WebSymmetricKey.prototype.constructor = WebSymmetricKey;
WebSymmetricKey.import = function(reader) {
	const symmetricKey = new WebSymmetricKey();
	symmetricKey.setVersion(readLong(reader));
	switch (symmetricKey.version) {
		case 1: {
			symmetricKey.setDate(readString(reader));
			symmetricKey.setUID(readString(reader));
			symmetricKey.setIsValid(readBool(reader));
			symmetricKey.setBinaryKey(readBuffer(reader));
			break;
		}
		default:
			return null;
	}
	return symmetricKey;
};
WebSymmetricKey.fromCryptoBuffer = function(symmetricKeyBuffer) {
	const key = new WebSymmetricKey();
	key.init();
	key.setBinaryKey(symmetricKeyBuffer);
	return key;
};
WebSymmetricKey.prototype.setBinaryKey = function(buffer) {
	this.binaryKey = buffer;
};
WebSymmetricKey.prototype.getCryptoKey = function() {
	if (!this.cryptoKey && this.binaryKey) {
		const crypto = getCrypto();
		this.cryptoKey = new PromiseManager();
	}
	return this.cryptoKey ? this.cryptoKey.getPromise() : Promise.resolve(null);
};


WebSymmetricKey.prototype.export = function(writer) {
	writeLong(writer, this.version);
	switch (this.version) {
		case 1: {
			writeString(writer, this.date.toISOString());
			writeString(writer, this.uid);
			writeBool(writer, this.isValid);
			writeBuffer(writer, this.binaryKey);
			break;
		}
		default: {
			break;
		}
	}
};



