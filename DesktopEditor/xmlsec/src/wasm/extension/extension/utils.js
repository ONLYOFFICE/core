(function(window, undefined) {
    if (undefined !== String.prototype.fromUtf8 && undefined !== String.prototype.toUtf8)
        return;

    var STRING_UTF8_BUFFER_LENGTH = 1024;
    var STRING_UTF8_BUFFER = new ArrayBuffer(STRING_UTF8_BUFFER_LENGTH);

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
        var index = start;
        var end = start + len;
        while (index < end) {
            var u0 = buffer[index++];
            if (!(u0 & 128)) {
                result += String.fromCharCode(u0);
                continue;
            }
            var u1 = buffer[index++] & 63;
            if ((u0 & 224) == 192) {
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
            else {
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
    String.prototype.toUtf8 = function(isNoEndNull, isUseBuffer) {
        var inputLen = this.length;
        var testLen = 6 * inputLen + 1;
        var tmpStrings = (isUseBuffer && testLen < STRING_UTF8_BUFFER_LENGTH) ? STRING_UTF8_BUFFER : new ArrayBuffer(testLen);

        var code = 0;
        var index = 0;

        var outputIndex = 0;
        var outputDataTmp = new Uint8Array(tmpStrings);
        var outputData = outputDataTmp;

        while (index < inputLen) {
            code = this.charCodeAt(index++);
            if (code >= 0xD800 && code <= 0xDFFF && index < inputLen)
                code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & this.charCodeAt(index++)));

            if (code < 0x80)
                outputData[outputIndex++] = code;
            else if (code < 0x0800) {
                outputData[outputIndex++] = 0xC0 | (code >> 6);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            } else if (code < 0x10000) {
                outputData[outputIndex++] = 0xE0 | (code >> 12);
                outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            } else if (code < 0x1FFFFF) {
                outputData[outputIndex++] = 0xF0 | (code >> 18);
                outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            } else if (code < 0x3FFFFFF) {
                outputData[outputIndex++] = 0xF8 | (code >> 24);
                outputData[outputIndex++] = 0x80 | ((code >> 18) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 12) & 0x3F);
                outputData[outputIndex++] = 0x80 | ((code >> 6) & 0x3F);
                outputData[outputIndex++] = 0x80 | (code & 0x3F);
            } else if (code < 0x7FFFFFFF) {
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

        return new Uint8Array(tmpStrings,0,outputIndex);
    };

    window.UtilsJS = {};

    var charA = "A".charCodeAt(0);
    var charZ = "Z".charCodeAt(0);
    var chara = "a".charCodeAt(0);
    var charz = "z".charCodeAt(0);
    var char0 = "0".charCodeAt(0);
    var char9 = "9".charCodeAt(0);
    var charp = "+".charCodeAt(0);
    var chars = "/".charCodeAt(0);
    var char_break = ";".charCodeAt(0);

    function decodeBase64Char(ch)
    {
        if (ch >= charA && ch <= charZ)
            return ch - charA + 0;
        if (ch >= chara && ch <= charz)
            return ch - chara + 26;
        if (ch >= char0 && ch <= char9)
            return ch - char0 + 52;
        if (ch == charp)
            return 62;
        if (ch == chars)
            return 63;
        return -1;
    }

    var stringBase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    var arrayBase64  = [];
    for (var index64 = 0; index64 < stringBase64.length; index64++)
    {
        arrayBase64.push(stringBase64.charAt(index64));
    }

    window.UtilsJS.Base64 = {};

    window.UtilsJS.Base64.decodeData = function(input, input_offset, input_len, output, output_offset)
    {
        var isBase64 = typeof input === "string";
        if (undefined === input_len) input_len = input.length;
        var writeIndex = (undefined === output_offset) ? 0 : output_offset;
        var index = (undefined === input_offset) ? 0 : input_offset;

        while (index < input_len)
        {
            var dwCurr = 0;
            var i;
            var nBits = 0;
            for (i=0; i<4; i++)
            {
                if (index >= input_len)
                    break;
                var nCh = decodeBase64Char(isBase64 ? input.charCodeAt(index) : input[index]);
                index++;
                if (nCh == -1)
                {
                    i--;
                    continue;
                }
                dwCurr <<= 6;
                dwCurr |= nCh;
                nBits += 6;
            }

            dwCurr <<= 24-nBits;
            for (i=0; i<(nBits>>3); i++)
            {
                output[writeIndex++] = ((dwCurr & 0x00ff0000) >>> 16);
                dwCurr <<= 8;
            }
        }
        return writeIndex;
    };

    window.UtilsJS.Base64.decode = function(input, isUsePrefix, dstlen, offset)
    {
        var srcLen = input.length;
        var index = (undefined === offset) ? 0 : offset;
        var dstLen = (undefined === dstlen) ? srcLen : dstlen;

        var isBase64 = typeof input === "string";

        if (isUsePrefix && isBase64)
        {
            dstLen = 0;
            var maxLen = Math.max(11, srcLen); // > 4 Gb
            while (index < maxLen)
            {
                var c = input.charCodeAt(index++);
                if (c == char_break)
                    break;

                dstLen *= 10;
                dstLen += (c - char0);
            }

            if (index == maxLen)
            {
                index = 0;
                dstLen = srcLen;
            }
        }

        var dst = new Uint8Array(dstLen);
        var writeIndex = window.AscCommon.Base64.decodeData(input, index, srcLen, dst, 0);

        if (writeIndex == dstLen)
            return dst;

        return new Uint8Array(dst.buffer, 0, writeIndex);
    };

    window.UtilsJS.Base64.encode = function(input, offset, length, isUsePrefix)
    {
        var srcLen = (undefined === length) ? input.length : length;
        var index = (undefined === offset) ? 0 : offset;

        var len1 = (((srcLen / 3) >> 0) * 4);
        var len2 = (len1 / 76) >> 0;
        var len3 = 19;
        var dstArray = [];

        var sTemp = "";
        var dwCurr = 0;
        for (var i = 0; i <= len2; i++)
        {
            if (i == len2)
                len3 = ((len1 % 76) / 4) >> 0;

            for (var j = 0; j < len3; j++)
            {
                dwCurr = 0;
                for (var n = 0; n < 3; n++)
                {
                    dwCurr |= input[index++];
                    dwCurr <<= 8;
                }

                sTemp = "";
                for (var k = 0; k < 4; k++)
                {
                    var b = (dwCurr >>> 26) & 0xFF;
                    sTemp += arrayBase64[b];
                    dwCurr <<= 6;
                    dwCurr &= 0xFFFFFFFF;
                }
                dstArray.push(sTemp);
            }
        }
        len2 = (srcLen % 3 != 0) ? (srcLen % 3 + 1) : 0;
        if (len2)
        {
            dwCurr = 0;
            for (var n = 0; n < 3; n++)
            {
                if (n < (srcLen % 3))
                    dwCurr |= input[index++];
                dwCurr <<= 8;
            }

            sTemp = "";
            for (var k = 0; k < len2; k++)
            {
                var b = (dwCurr >>> 26) & 0xFF;
                sTemp += arrayBase64[b];
                dwCurr <<= 6;
            }

            len3 = (len2 != 0) ? 4 - len2 : 0;
            for (var j = 0; j < len3; j++)
            {
                sTemp += '=';
            }
            dstArray.push(sTemp);
        }

        return isUsePrefix ? (("" + srcLen + ";") + dstArray.join("")) : dstArray.join("");
    };

    window.UtilsJS.Hex = {};

    window.UtilsJS.Hex.decode = function(input)
    {
        var hexToByte = function(c) {
            if (c >= 48 && c <= 57) return c - 48; // 0..9
            if (c >= 97 && c <= 102) return c - 87;
            if (c >= 65 && c <= 70) return c - 55;
            return 0;
        };

        var len = input.length;
        if (len & 0x01) len -= 1;
        var result = new Uint8Array(len >> 1);
        var resIndex = 0;
        for (var i = 0; i < len; i += 2)
        {
            result[resIndex++] = hexToByte(input.charCodeAt(i)) << 4 | hexToByte(input.charCodeAt(i + 1));
        }
        return result;
    };

    window.UtilsJS.Hex.encode = function(input, isUpperCase)
    {
        var byteToHex = new Array(256);
        for (var i = 0; i < 16; i++)
            byteToHex[i] = "0" + (isUpperCase ? i.toString(16).toUpperCase() : i.toString(16));
        for (var i = 16; i < 256; i++)
            byteToHex[i] = isUpperCase ? i.toString(16).toUpperCase() : i.toString(16);

        var result = "";
        for (var i = 0, len = input.length; i < len; i++)
            result += byteToHex[input[i]];

        return result;
    };    

    window.UtilsJS.random = function(length) {
        let byteArray = new Uint8Array(length);
        let engine = window.crypto || window.msCrypto;
        if (engine)
            engine.getRandomValues(byteArray);
        else {
            for (let i = 0; i < length; i++)
                byteArray[i] = (Math.random() * 256) >> 0;
        }
        return byteArray;
    };
})(self);