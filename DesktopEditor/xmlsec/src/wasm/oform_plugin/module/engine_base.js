(function(window, undefined){

	//desktop_fetch

    //polyfill

    //string_utf8

    function toBase64(buf)
	{
		if (typeof buf === "string")
		{
			let old = buf;
			buf = [];
			for (let i = 0, len = old.length; i < len; i++)
				buf.push(old.charCodeAt(i));
		}

		const chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
		const chars_map = [
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
			-1,  9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
			22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
			-1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
			47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
		];

		let result = [];
		for (let i = 0, len = buf.length; i < len; i++)
		{
			let carry = buf[i];
			for (let j = 0; j < result.length; ++j) 
			{
				const x = (chars_map[result[j]] << 8) + carry;
				result[j] = chars.charCodeAt(x % 58);
				carry = (x / 58) >> 0;
			}
			while (carry)
			{
				result.push(chars.charCodeAt(carry % 58));
				carry = (carry / 58) >> 0;
			}
		}

		let char1 = "1".charCodeAt(0);
		for (let i = 0, len = buf.length; i < len; i++)
		{
			if (buf[i])
				break;
			else
				result.push(char1);
		}

		result.reverse();
		return String.fromCharCode.apply(null, result);
	}

	function random(length)
	{
		let byteArray = new Uint8Array(length);
		let engine = window.crypto || window.msCrypto;
		if (engine)
			engine.getRandomValues(byteArray);
		else
		{
			for (let i = 0; i < length; i++)
				byteArray[i] = (Math.random() * 256) >> 0;
		}
		return byteArray;
	}

	function pointerToString(ptr, isDestroy)
	{
		if (0 === ptr)
			return "";

		let end = ptr;
		let heap = Module["HEAP8"];
		while (heap[end] !== 0)
			++end;

		let res = "".fromUtf8(heap, ptr, end - ptr);

		if (isDestroy === true)
			Module["_Crypto_Free"](ptr);

		return res;
	}

    function CryptoJS()
	{
		this.isModuleInit = false;
	};

	CryptoJS.prototype.onLoad = function()
	{
		this.isModuleInit = true;
	};

	CryptoJS.prototype.generateKeys = function(alg)
	{
		if (!this.isModuleInit)
			return null;

		let salt = this.createSalt();

		let algPtr  = alg.toUtf8Pointer();
		let saltPtr = salt.toUtf8Pointer();

		let keys = Module["_Crypto_CreateKeys"](algPtr.ptr, saltPtr.ptr);

		algPtr.free();
		saltPtr.free();

		if (keys === 0)
			return null;

		let heap = Module["HEAP8"];

		let currentStart = keys;
		let currentEnd = currentStart;
		while (heap[currentEnd] != 0)
			currentEnd++;
		let publicKey = "".fromUtf8(heap, currentStart, currentEnd - currentStart);

		currentStart = currentEnd + 1;
		currentEnd = currentStart;
		while (heap[currentEnd] != 0)
			currentEnd++;
		let privateKey = "".fromUtf8(heap, currentStart, currentEnd - currentStart);

		Module["_Crypto_Free"](keys);

		return {
			"salt"       : salt,
			"privateKey" : privateKey,
			"publicKey"  : publicKey
		};
	};

	CryptoJS.prototype.sign = function(privateKeyEnc, salt, version, xml)
	{
		if (!this.isModuleInit)
			return null;

		let privateKeyPtr = privateKeyEnc.toUtf8Pointer();
		let saltPtr = salt.toUtf8Pointer();
		let xmlPtr = xml.toUtf8Pointer();

		let signData = Module["_Crypto_Sign"](privateKeyPtr.ptr, saltPtr.ptr, 
			version, xmlPtr.ptr, xmlPtr.length);

		privateKeyPtr.free();
		saltPtr.free();
		xmlPtr.free();

		return pointerToString(signData, true);
	};

	CryptoJS.prototype.changePassword = function(privateKeyEnc, salt, version, passwordNew)
	{
		if (!this.isModuleInit)
			return null;

		let privateKeyPtr = privateKeyEnc.toUtf8Pointer();
		let saltPtr = salt.toUtf8Pointer();
		let passwordNewPtr = passwordNew.toUtf8Pointer();
		
		let privateKeyEncPtr = Module["_Crypto_ChangePassword"](privateKeyPtr.ptr, saltPtr.ptr, version, passwordNewPtr.ptr);

		privateKeyPtr.free();
		saltPtr.free();
		passwordNewPtr.free();

		return pointerToString(privateKeyEncPtr, true);
	};

	CryptoJS.prototype.setMasterPassword = function(password)
	{
		let passwordPtr = password.toUtf8Pointer();
		Module["_SetMasterPassword"](passwordPtr);
		Module["_Crypto_Free"](passwordPtr);
	};

	CryptoJS.prototype.createSalt = function()
	{
		return toBase64(random(32));
	};

	CryptoJS.prototype.generateSignKeys = function()
	{
		return this.generateKeys("ed25519");
	};

	CryptoJS.prototype.generateCryptKeys = function()
	{
		return this.generateKeys("x25519");
	};

	CryptoJS.prototype.aesDecrypt = function(version, salt, data) 
	{
		let saltPtr = salt.toUtf8Pointer();
		let dataPtr = Module["_malloc"](data.length);
		Module["HEAP8"].set(dataPtr, data);
		
		let buffer = Module["_AES_Decode"](version, 0, saltPtr.ptr, dataPtr, data.length);
		
		saltPtr.free();
		Module["_free"](dataPtr);

		if (!buffer)
			return null;		

		let result = new Uint8Array(Module["HEAPU8"].buffer, 
			Module["_MemoryBlockGetData"](buffer), 
			Module["_MemoryBlockGetSize"](buffer));

		Module["_MemoryBlockDestroy"](buffer);
		return result;
	};
	CryptoJS.prototype.aesDecryptString = function(version, salt, data) 
	{
		let saltPtr = salt.toUtf8Pointer();
		let dataPtr = data.toUtf8Pointer();
		
		let result = Module["_AES_Decode"](version, 0, saltPtr.ptr, dataPtr.ptr, dataPtr.length);
		
		saltPtr.free();
		dataPtr.free();

		return pointerToString(result, true);
	};
	CryptoJS.prototype.aesDecryptBase64 = function(version, salt, data) 
	{
		let saltPtr = salt.toUtf8Pointer();
		let dataPtr = data.toUtf8Pointer();
		
		let result = Module["_AES_DecodeBase64"](version, saltPtr.ptr, dataPtr.ptr);
		
		saltPtr.free();
		dataPtr.free();

		return pointerToString(result, true);
	};

	CryptoJS.prototype.aesEncrypt = function(version, salt, data) 
	{
		let saltPtr = salt.toUtf8Pointer();
		let dataPtr = Module["_malloc"](data.length);
		Module["HEAP8"].set(data, dataPtr);
		
		let buffer = Module["_AES_Encode"](version, 0, saltPtr.ptr, dataPtr, data.length);
		
		saltPtr.free();
		Module["_free"](dataPtr);

		if (!buffer)
			return null;		

		let result = new Uint8Array(Module["HEAPU8"].buffer, 
			Module["_MemoryBlockGetData"](buffer), 
			Module["_MemoryBlockGetSize"](buffer));

		Module["_MemoryBlockDestroy"](buffer);
		return result;
	};
	CryptoJS.prototype.aesEncryptString = function(version, salt, data) 
	{
		let saltPtr = salt.toUtf8Pointer();
		let dataPtr = data.toUtf8Pointer();
		
		let result = Module["_AES_Encode"](version, 0, saltPtr.ptr, dataPtr.ptr, dataPtr.length);
		
		saltPtr.free();
		dataPtr.free();

		return pointerToString(result, true);
	};
	CryptoJS.prototype.aesEncryptBase64 = function(version, salt, data) 
	{
		let saltPtr = salt.toUtf8Pointer();
		let dataPtr = data.toUtf8Pointer();
		
		let result = Module["_AES_EncodeBase64"](version, 0, saltPtr, dataPtr);
		
		saltPtr.free();
		dataPtr.free();

		return pointerToString(result, true);
	};

	// Crypt/Decrypt 
	CryptoJS.prototype.decryptWithPrivateKey = function(version, privateKeyEnc, salt, data)
	{
		let keyEncPtr = privateKeyEnc.toUtf8Pointer();
		let saltPtr = salt.toUtf8Pointer();
		let buffer = null;

		if (typeof data === "string")
		{
			let dataPtr = data.toUtf8Pointer();
			buffer = Module["_DecryptWithPrivateKeyBase64"](version, keyEncPtr.ptr, saltPtr.ptr, data.ptr, data.length);
			dataPtr.free();
		}
		else
		{
			let dataPtr = Module["_malloc"](data.length);
			Module["HEAP8"].set(data, dataPtr);
			buffer = Module["_DecryptWithPrivateKey"](version, keyEncPtr.ptr, saltPtr.ptr, dataPtr, data.length);
			Module["_free"](dataPtr);
		}

		keyEncPtr.free();
		return pointerToString(buffer, true);
	};

	CryptoJS.prototype.cryptWithPublicKey = function(version, publicKey, data)
	{
		let keyPtr = publicKey.toUtf8Pointer();
		let buffer = null;

		if (typeof data === "string")
		{
			let dataPtr = data.toUtf8Pointer();
			buffer = Module["_EncryptWithPublicKeyBase64"](version, keyPtr.ptr, data.ptr, data.length);
			dataPtr.free();
		}
		else
		{
			let dataPtr = Module["_malloc"](data.length);
			Module["HEAP8"].set(data, dataPtr);
			buffer = Module["_EncryptWithPublicKey"](version, keyPtr.ptr, dataPtr, data.length);
			Module["_free"](dataPtr);
		}

		keyPtr.free();
		return pointerToString(buffer, true);
	};

	window.cryptoJS = new CryptoJS();

	window.CryptoJS = {
		
		Version : {
			"1" : 0,
			"Error" : 1
		},

		Algorithm : {
			ED25519 : "ed25519",
			X25519  : "x25519",
		}

	};

    //module

})(window, undefined);

