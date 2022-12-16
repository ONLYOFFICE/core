(function(window, undefined){

	//desktop_fetch

    //polyfill

    //string_utf8

    function toBase64(buf)
	{
		if(typeof buf === "string") 
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

    function CryptoJS()
	{
		this.isModuleInit = false;
	};
	CryptoJS.prototype.onLoad = function()
	{
		this.isModuleInit = true;
	};

	CryptoJS.prototype.generateKeys = function(password, salt)
	{
		if (!this.isModuleInit)
			return null;

		if (!salt)
			salt = toBase64(random(32));

		let algPtr = "ed25519".toUtf8Pointer();
		let passwordPtr = password.toUtf8Pointer();
		let saltPtr = salt.toUtf8Pointer();

		let keys = Module["_Crypto_CreateKeys"](algPtr.ptr, passwordPtr.ptr, saltPtr.ptr);

		algPtr.free();
		passwordPtr.free();
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
			"salt" : salt,
			"privateKey" : privateKey,
			"publicKey" : publicKey
		};
	};

	CryptoJS.prototype.sign = function(privateKey, password, salt, xml)
	{
		if (!this.isModuleInit)
			return null;

		let privateKeyPtr = privateKey.toUtf8Pointer();
		let passwordPtr = password.toUtf8Pointer();
		let saltPtr = salt.toUtf8Pointer();
		let xmlPtr = xml.toUtf8Pointer();

		let signData = Module["_Crypto_Sign"](privateKeyPtr.ptr, passwordPtr.ptr, saltPtr.ptr, 
			xmlPtr.ptr, xmlPtr.length);

		privateKeyPtr.free();
		passwordPtr.free();
		saltPtr.free();
		xmlPtr.free();

		if (signData === 0)
			return null;

		let heap = Module["HEAP8"];

		let currentStart = signData;
		let currentEnd = currentStart;
		while (heap[currentEnd] != 0)
			currentEnd++;
		
		let signString = "".fromUtf8(heap, currentStart, currentEnd - currentStart);

		Module["_Crypto_Free"](signData);
		return signString;
	};

	CryptoJS.prototype.changePassword = function(privateKey, passwordOld, passwordNew, salt)
	{
		if (!this.isModuleInit)
			return null;

		let privateKeyPtr = privateKey.toUtf8Pointer();
		let passwordOldPtr = passwordOld.toUtf8Pointer();
		let passwordNewPtr = passwordNew.toUtf8Pointer();
		let saltPtr = salt.toUtf8Pointer();

		let privateKeyEnc = Module["_Crypto_ChangePassword"](privateKeyPtr.ptr, 
			passwordOldPtr.ptr, passwordNewPtr.ptr, saltPtr.ptr);

		privateKeyPtr.free();
		passwordOldPtr.free();
		passwordNewPtr.free();
		saltPtr.free();	

		if (privateKeyEnc === 0)
			return null;

		let heap = Module["HEAP8"];

		let currentStart = privateKeyEnc;
		let currentEnd = currentStart;
		while (heap[currentEnd] != 0)
			currentEnd++;
		
		let privateKeyString = "".fromUtf8(heap, currentStart, currentEnd - currentStart);

		Module["_Crypto_Free"](privateKeyEnc);
		return privateKeyString;
	};

	window.cryptoJS = new CryptoJS();

    //module

})(window, undefined);

