(function(){

	if (undefined !== String.prototype.fromUtf8 &&
		undefined !== String.prototype.toUtf8)
		return;

	/**
	 * Read string from utf8
	 * @param {Uint8Array} buffer
	 * @param {number} [start=0]
	 * @param {number} [len]
	 * @returns {string}
	 */
	String.prototype.fromUtf8 = function(buffer, start, len) {
		if (undefined === start)
			start = 0;
		if (undefined === len)
			len = buffer.length - start;

		var result = "";
		var index  = start;
		var end = start + len;
		while (index < end)
		{
			var u0 = buffer[index++];
			if (!(u0 & 128))
			{
				result += String.fromCharCode(u0);
				continue;
			}
			var u1 = buffer[index++] & 63;
			if ((u0 & 224) == 192)
			{
				result += String.fromCharCode((u0 & 31) << 6 | u1);
				continue;
			}
			var u2 = buffer[index++] & 63;
			if ((u0 & 240) == 224)
				u0 = (u0 & 15) << 12 | u1 << 6 | u2;
			else
				u0 = (u0 & 7) << 18 | u1 << 12 | u2 << 6 | buffer[index++] & 63;
			if (u0 < 65536)
				result += String.fromCharCode(u0);
			else
			{
				var ch = u0 - 65536;
				result += String.fromCharCode(55296 | ch >> 10, 56320 | ch & 1023);
			}
		}
		return result;
	};

	/**
	 * Convert string to utf8 array
	 * @returns {Uint8Array}
	 */
	String.prototype.toUtf8 = function(isNoEndNull) {
		var inputLen = this.length;
		var testLen  = 6 * inputLen + 1;
		var tmpStrings = new ArrayBuffer(testLen);

		var code  = 0;
		var index = 0;

		var outputIndex = 0;
		var outputDataTmp = new Uint8Array(tmpStrings);
		var outputData = outputDataTmp;

		while (index < inputLen)
		{
			code = this.charCodeAt(index++);
			if (code >= 0xD800 && code <= 0xDFFF && index < inputLen)
				code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & this.charCodeAt(index++)));

			if (code < 0x80)
				outputData[outputIndex++] = code;
			else if (code < 0x0800)
			{
				outputData[outputIndex++] = 0xC0 | (code >> 6);
				outputData[outputIndex++] = 0x80 | (code & 0x3F);
			}
			else if (code < 0x10000)
			{
				outputData[outputIndex++] = 0xE0 | (code >> 12);
				outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
				outputData[outputIndex++] = 0x80 | (code & 0x3F);
			}
			else if (code < 0x1FFFFF)
			{
				outputData[outputIndex++] = 0xF0 | (code >> 18);
				outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
				outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
				outputData[outputIndex++] = 0x80 | (code & 0x3F);
			}
			else if (code < 0x3FFFFFF)
			{
				outputData[outputIndex++] = 0xF8 | (code >> 24);
				outputData[outputIndex++] = 0x80 | ((code >> 18) & 0x3F);
				outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
				outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
				outputData[outputIndex++] = 0x80 | (code & 0x3F);
			}
			else if (code < 0x7FFFFFFF)
			{
				outputData[outputIndex++] = 0xFC | (code >> 30);
				outputData[outputIndex++] = 0x80 | ((code >> 24) & 0x3F);
				outputData[outputIndex++] = 0x80 | ((code >> 18) & 0x3F);
				outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
				outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
				outputData[outputIndex++] = 0x80 | (code & 0x3F);
			}
		}

		if (isNoEndNull !== true)
			outputData[outputIndex++] = 0;

		return new Uint8Array(tmpStrings, 0, outputIndex);
	};

	function StringPointer(pointer, len)
	{
		this.ptr = pointer;
		this.length = len;
	}
	StringPointer.prototype.free = function()
	{
		if (0 !== this.ptr)
			Module["_free"](this.ptr);
	};

	String.prototype.toUtf8Pointer = function(isNoEndNull) {
		var tmp = this.toUtf8(isNoEndNull);
		var pointer = Module["_malloc"](tmp.length);
		if (0 == pointer)
			return null;

		Module["HEAP8"].set(tmp, pointer);
		return new StringPointer(pointer, tmp.length);		
	};

})();
