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



(function(window, undefined){
    WebAssembly.instantiateStreaming = undefined;

    function MemoryData(ptr) {
        this.ptr = ptr;
    }
    MemoryData.prototype.isValid = function() {
        return (this.ptr === 0) ? false : true;
    };
    MemoryData.prototype.free = function() {
        if (0 != this.ptr)
            Module["_Crypto_Free"](this.ptr);
    };
    MemoryData.prototype.getData = function() {
        let lenArray = new Int32Array(Module["HEAP8"].buffer, this.ptr, 4);
        let len = lenArray[0];
        return new Uint8Array(Module["HEAP8"].buffer, this.ptr + 4, len);
    };

    function StringPointer(pointer, len) {
        this.ptr = pointer;
        this.length = len;
    }
    StringPointer.prototype.free = function() {
        if (0 !== this.ptr)
            Module["_free"](this.ptr);
    };

    String.prototype.toUtf8Pointer = function(isNoEndNull) {
        var tmp = this.toUtf8(isNoEndNull, true);
        var pointer = Module["_malloc"](tmp.length);
        if (0 == pointer)
            return null;

        Module["HEAP8"].set(tmp, pointer);
        return new StringPointer(pointer,tmp.length);
    };

    function typedArrayToMemory(data)
    {
        var pointer = Module["_malloc"](data.length);
		Module["HEAP8"].set(data, langBuffer);
        return pointer;
    }

    function Engine()
    {
        this.isInit = false;
        this.waitResolvers = [];
    }

    Engine.prototype.onLoad = function() 
    {
        this.isInit = true;

        for (let i = 0, len = this.waitResolvers.length; i < len; i++)
            this.waitResolvers[i]();

        this.waitResolvers = [];
    };

    Engine.prototype.init = async function()
    {
        if (this.isInit)
            return;

        return new Promise(resolve => (function(){
            window.CryptoJS.waitResolvers.push(resolve);
		})());
    };

    // SIGN
    Engine.prototype.generateKeys = async function(alg, password, salt) 
    {
        await this.init();

        if (!salt)
            salt = window.UtilsJS.toBase64(window.UtilsJS.random(32));

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
            "salt": salt,
            "privateKey": privateKey,
            "publicKey": publicKey
        };
    };

    Engine.prototype.changePassword = async function(privateKey, passwordOld, passwordNew, salt) 
    {
        await this.init();

        let privateKeyPtr = privateKey.toUtf8Pointer();
        let passwordOldPtr = passwordOld.toUtf8Pointer();
        let passwordNewPtr = passwordNew.toUtf8Pointer();
        let saltPtr = salt.toUtf8Pointer();

        let privateKeyEnc = Module["_Crypto_ChangePassword"](privateKeyPtr.ptr, passwordOldPtr.ptr, passwordNewPtr.ptr, saltPtr.ptr);

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

    Engine.prototype.sign = async function(privateKey, password, salt, xml) 
    {
        await this.init();

        let privateKeyPtr = privateKey.toUtf8Pointer();
        let passwordPtr = password.toUtf8Pointer();
        let saltPtr = salt.toUtf8Pointer();
        let xmlPtr = xml.toUtf8Pointer();

        let signData = Module["_Crypto_Sign"](privateKeyPtr.ptr, passwordPtr.ptr, saltPtr.ptr, xmlPtr.ptr, xmlPtr.length);

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

    // ENCRYPT
    Engine.prototype.decrypt = async function(privateKeyEnc, password, salt, data) 
    {
        await this.init();

        let privateKeyEncPtr = privateKeyEnc.toUtf8Pointer();
        let passwordPtr = password.toUtf8Pointer();
        let saltPtr = salt.toUtf8Pointer();

        let dataPtr = typedArrayToMemory(data);

        let decryptData = Module["_Crypto_Decrypt"](privateKeyEncPtr, passwordPtr, saltPtr.ptr, dataPtr, data.length);
        let memoryData = new CMemoryData(decryptData);

        privateKeyEncPtr.free();
        passwordPtr.free();
        saltPtr.free();
        
        Module["_free"](dataPtr);

        return memoryData;
    };

    Engine.prototype.encrypt = async function(publicKey, data) 
    {
        await this.init();

        let publicKeyEncPtr = publicKey.toUtf8Pointer();
        let dataPtr = typedArrayToMemory(data);

        let encryptData = Module["_Crypto_Encrypt"](publicKeyEncPtr, dataPtr, data.length);
        let memoryData = new CMemoryData(decryptData);

        publicKeyEncPtr.free();
        Module["_free"](dataPtr);

        return memoryData;
    };

    window.cryptoJS = new Engine();

    var Module=typeof Module!="undefined"?Module:{};var moduleOverrides=Object.assign({},Module);var arguments_=[];var thisProgram="./this.program";var quit_=(status,toThrow)=>{throw toThrow};var ENVIRONMENT_IS_WEB=true;var ENVIRONMENT_IS_WORKER=false;var ENVIRONMENT_IS_NODE=false;var ENVIRONMENT_IS_SHELL=false;if(Module["ENVIRONMENT"]){throw new Error("Module.ENVIRONMENT has been deprecated. To force the environment, use the ENVIRONMENT compile-time option (for example, -sENVIRONMENT=web or -sENVIRONMENT=node)")}var scriptDirectory="";function locateFile(path){if(Module["locateFile"]){return Module["locateFile"](path,scriptDirectory)}return scriptDirectory+path}var read_,readAsync,readBinary;if(ENVIRONMENT_IS_SHELL){if(typeof process=="object"&&typeof require==="function"||typeof window=="object"||typeof importScripts=="function")throw new Error("not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)");if(typeof read!="undefined"){read_=read}readBinary=f=>{if(typeof readbuffer=="function"){return new Uint8Array(readbuffer(f))}let data=read(f,"binary");assert(typeof data=="object");return data};readAsync=(f,onload,onerror)=>{setTimeout(()=>onload(readBinary(f)))};if(typeof clearTimeout=="undefined"){globalThis.clearTimeout=id=>{}}if(typeof setTimeout=="undefined"){globalThis.setTimeout=f=>typeof f=="function"?f():abort()}if(typeof scriptArgs!="undefined"){arguments_=scriptArgs}else if(typeof arguments!="undefined"){arguments_=arguments}if(typeof quit=="function"){quit_=(status,toThrow)=>{setTimeout(()=>{if(!(toThrow instanceof ExitStatus)){let toLog=toThrow;if(toThrow&&typeof toThrow=="object"&&toThrow.stack){toLog=[toThrow,toThrow.stack]}err(`exiting due to exception: ${toLog}`)}quit(status)});throw toThrow}}if(typeof print!="undefined"){if(typeof console=="undefined")console={};console.log=print;console.warn=console.error=typeof printErr!="undefined"?printErr:print}}else if(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER){if(ENVIRONMENT_IS_WORKER){scriptDirectory=self.location.href}else if(typeof document!="undefined"&&document.currentScript){scriptDirectory=document.currentScript.src}if(scriptDirectory.indexOf("blob:")!==0){scriptDirectory=scriptDirectory.substr(0,scriptDirectory.replace(/[?#].*/,"").lastIndexOf("/")+1)}else{scriptDirectory=""}if(!(typeof window=="object"||typeof importScripts=="function"))throw new Error("not compiled for this environment (did you build to HTML and try to run it not on the web, or set ENVIRONMENT to something - like node - and run it someplace else - like on the web?)");{read_=url=>{var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.send(null);return xhr.responseText};if(ENVIRONMENT_IS_WORKER){readBinary=url=>{var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.responseType="arraybuffer";xhr.send(null);return new Uint8Array(xhr.response)}}readAsync=(url,onload,onerror)=>{var xhr=new XMLHttpRequest;xhr.open("GET",url,true);xhr.responseType="arraybuffer";xhr.onload=()=>{if(xhr.status==200||xhr.status==0&&xhr.response){onload(xhr.response);return}onerror()};xhr.onerror=onerror;xhr.send(null)}}}else{throw new Error("environment detection error")}var out=Module["print"]||console.log.bind(console);var err=Module["printErr"]||console.error.bind(console);Object.assign(Module,moduleOverrides);moduleOverrides=null;checkIncomingModuleAPI();if(Module["arguments"])arguments_=Module["arguments"];legacyModuleProp("arguments","arguments_");if(Module["thisProgram"])thisProgram=Module["thisProgram"];legacyModuleProp("thisProgram","thisProgram");if(Module["quit"])quit_=Module["quit"];legacyModuleProp("quit","quit_");assert(typeof Module["memoryInitializerPrefixURL"]=="undefined","Module.memoryInitializerPrefixURL option was removed, use Module.locateFile instead");assert(typeof Module["pthreadMainPrefixURL"]=="undefined","Module.pthreadMainPrefixURL option was removed, use Module.locateFile instead");assert(typeof Module["cdInitializerPrefixURL"]=="undefined","Module.cdInitializerPrefixURL option was removed, use Module.locateFile instead");assert(typeof Module["filePackagePrefixURL"]=="undefined","Module.filePackagePrefixURL option was removed, use Module.locateFile instead");assert(typeof Module["read"]=="undefined","Module.read option was removed (modify read_ in JS)");assert(typeof Module["readAsync"]=="undefined","Module.readAsync option was removed (modify readAsync in JS)");assert(typeof Module["readBinary"]=="undefined","Module.readBinary option was removed (modify readBinary in JS)");assert(typeof Module["setWindowTitle"]=="undefined","Module.setWindowTitle option was removed (modify emscripten_set_window_title in JS)");assert(typeof Module["TOTAL_MEMORY"]=="undefined","Module.TOTAL_MEMORY has been renamed Module.INITIAL_MEMORY");legacyModuleProp("asm","wasmExports");legacyModuleProp("read","read_");legacyModuleProp("readAsync","readAsync");legacyModuleProp("readBinary","readBinary");legacyModuleProp("setWindowTitle","setWindowTitle");assert(!ENVIRONMENT_IS_WORKER,"worker environment detected but not enabled at build time.  Add 'worker' to `-sENVIRONMENT` to enable.");assert(!ENVIRONMENT_IS_NODE,"node environment detected but not enabled at build time.  Add 'node' to `-sENVIRONMENT` to enable.");assert(!ENVIRONMENT_IS_SHELL,"shell environment detected but not enabled at build time.  Add 'shell' to `-sENVIRONMENT` to enable.");var wasmBinary;if(Module["wasmBinary"])wasmBinary=Module["wasmBinary"];legacyModuleProp("wasmBinary","wasmBinary");if(typeof WebAssembly!="object"){abort("no native wasm support detected")}var wasmMemory;var ABORT=false;var EXITSTATUS;function assert(condition,text){if(!condition){abort("Assertion failed"+(text?": "+text:""))}}var HEAP8,HEAPU8,HEAP16,HEAPU16,HEAP32,HEAPU32,HEAPF32,HEAPF64;function updateMemoryViews(){var b=wasmMemory.buffer;Module["HEAP8"]=HEAP8=new Int8Array(b);Module["HEAP16"]=HEAP16=new Int16Array(b);Module["HEAPU8"]=HEAPU8=new Uint8Array(b);Module["HEAPU16"]=HEAPU16=new Uint16Array(b);Module["HEAP32"]=HEAP32=new Int32Array(b);Module["HEAPU32"]=HEAPU32=new Uint32Array(b);Module["HEAPF32"]=HEAPF32=new Float32Array(b);Module["HEAPF64"]=HEAPF64=new Float64Array(b)}assert(!Module["STACK_SIZE"],"STACK_SIZE can no longer be set at runtime.  Use -sSTACK_SIZE at link time");assert(typeof Int32Array!="undefined"&&typeof Float64Array!=="undefined"&&Int32Array.prototype.subarray!=undefined&&Int32Array.prototype.set!=undefined,"JS engine does not provide full typed array support");assert(!Module["wasmMemory"],"Use of `wasmMemory` detected.  Use -sIMPORTED_MEMORY to define wasmMemory externally");assert(!Module["INITIAL_MEMORY"],"Detected runtime INITIAL_MEMORY setting.  Use -sIMPORTED_MEMORY to define wasmMemory dynamically");function writeStackCookie(){var max=_emscripten_stack_get_end();assert((max&3)==0);if(max==0){max+=4}HEAPU32[max>>2]=34821223;HEAPU32[max+4>>2]=2310721022;HEAPU32[0>>2]=1668509029}function checkStackCookie(){if(ABORT)return;var max=_emscripten_stack_get_end();if(max==0){max+=4}var cookie1=HEAPU32[max>>2];var cookie2=HEAPU32[max+4>>2];if(cookie1!=34821223||cookie2!=2310721022){abort(`Stack overflow! Stack cookie has been overwritten at ${ptrToString(max)}, expected hex dwords 0x89BACDFE and 0x2135467, but received ${ptrToString(cookie2)} ${ptrToString(cookie1)}`)}if(HEAPU32[0>>2]!=1668509029){abort("Runtime error: The application has corrupted its heap memory area (address zero)!")}}(function(){var h16=new Int16Array(1);var h8=new Int8Array(h16.buffer);h16[0]=25459;if(h8[0]!==115||h8[1]!==99)throw"Runtime error: expected the system to be little-endian! (Run with -sSUPPORT_BIG_ENDIAN to bypass)"})();var __ATPRERUN__=[];var __ATINIT__=[];var __ATPOSTRUN__=[function(){window.cryptoJS.onLoad();}];var runtimeInitialized=false;function preRun(){if(Module["preRun"]){if(typeof Module["preRun"]=="function")Module["preRun"]=[Module["preRun"]];while(Module["preRun"].length){addOnPreRun(Module["preRun"].shift())}}callRuntimeCallbacks(__ATPRERUN__)}function initRuntime(){assert(!runtimeInitialized);runtimeInitialized=true;checkStackCookie();callRuntimeCallbacks(__ATINIT__)}function postRun(){checkStackCookie();if(Module["postRun"]){if(typeof Module["postRun"]=="function")Module["postRun"]=[Module["postRun"]];while(Module["postRun"].length){addOnPostRun(Module["postRun"].shift())}}callRuntimeCallbacks(__ATPOSTRUN__)}function addOnPreRun(cb){__ATPRERUN__.unshift(cb)}function addOnInit(cb){__ATINIT__.unshift(cb)}function addOnPostRun(cb){__ATPOSTRUN__.unshift(cb)}assert(Math.imul,"This browser does not support Math.imul(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill");assert(Math.fround,"This browser does not support Math.fround(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill");assert(Math.clz32,"This browser does not support Math.clz32(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill");assert(Math.trunc,"This browser does not support Math.trunc(), build with LEGACY_VM_SUPPORT or POLYFILL_OLD_MATH_FUNCTIONS to add in a polyfill");var runDependencies=0;var runDependencyWatcher=null;var dependenciesFulfilled=null;var runDependencyTracking={};function addRunDependency(id){runDependencies++;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}if(id){assert(!runDependencyTracking[id]);runDependencyTracking[id]=1;if(runDependencyWatcher===null&&typeof setInterval!="undefined"){runDependencyWatcher=setInterval(()=>{if(ABORT){clearInterval(runDependencyWatcher);runDependencyWatcher=null;return}var shown=false;for(var dep in runDependencyTracking){if(!shown){shown=true;err("still waiting on run dependencies:")}err(`dependency: ${dep}`)}if(shown){err("(end of list)")}},1e4)}}else{err("warning: run dependency added without ID")}}function removeRunDependency(id){runDependencies--;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}if(id){assert(runDependencyTracking[id]);delete runDependencyTracking[id]}else{err("warning: run dependency removed without ID")}if(runDependencies==0){if(runDependencyWatcher!==null){clearInterval(runDependencyWatcher);runDependencyWatcher=null}if(dependenciesFulfilled){var callback=dependenciesFulfilled;dependenciesFulfilled=null;callback()}}}function abort(what){if(Module["onAbort"]){Module["onAbort"](what)}what="Aborted("+what+")";err(what);ABORT=true;EXITSTATUS=1;var e=new WebAssembly.RuntimeError(what);throw e}var FS={error(){abort("Filesystem support (FS) was not included. The problem is that you are using files from JS, but files were not used from C/C++, so filesystem support was not auto-included. You can force-include filesystem support with -sFORCE_FILESYSTEM")},init(){FS.error()},createDataFile(){FS.error()},createPreloadedFile(){FS.error()},createLazyFile(){FS.error()},open(){FS.error()},mkdev(){FS.error()},registerDevice(){FS.error()},analyzePath(){FS.error()},ErrnoError(){FS.error()}};Module["FS_createDataFile"]=FS.createDataFile;Module["FS_createPreloadedFile"]=FS.createPreloadedFile;var dataURIPrefix="data:application/octet-stream;base64,";var isDataURI=filename=>filename.startsWith(dataURIPrefix);var isFileURI=filename=>filename.startsWith("file://");function createExportWrapper(name){return function(){assert(runtimeInitialized,`native function \`${name}\` called before runtime initialization`);var f=wasmExports[name];assert(f,`exported native function \`${name}\` not found`);return f.apply(null,arguments)}}var wasmBinaryFile;wasmBinaryFile="engine.wasm";if(!isDataURI(wasmBinaryFile)){wasmBinaryFile=locateFile(wasmBinaryFile)}function getBinarySync(file){if(file==wasmBinaryFile&&wasmBinary){return new Uint8Array(wasmBinary)}if(readBinary){return readBinary(file)}throw"both async and sync fetching of the wasm failed"}function getBinaryPromise(binaryFile){if(!wasmBinary&&(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER)){if(typeof fetch=="function"){return fetch(binaryFile,{credentials:"same-origin"}).then(response=>{if(!response["ok"]){throw"failed to load wasm binary file at '"+binaryFile+"'"}return response["arrayBuffer"]()}).catch(()=>getBinarySync(binaryFile))}}return Promise.resolve().then(()=>getBinarySync(binaryFile))}function instantiateArrayBuffer(binaryFile,imports,receiver){return getBinaryPromise(binaryFile).then(binary=>WebAssembly.instantiate(binary,imports)).then(instance=>instance).then(receiver,reason=>{err(`failed to asynchronously prepare wasm: ${reason}`);if(isFileURI(wasmBinaryFile)){err(`warning: Loading from a file URI (${wasmBinaryFile}) is not supported in most browsers. See https://emscripten.org/docs/getting_started/FAQ.html#how-do-i-run-a-local-webserver-for-testing-why-does-my-program-stall-in-downloading-or-preparing`)}abort(reason)})}function instantiateAsync(binary,binaryFile,imports,callback){if(!binary&&typeof WebAssembly.instantiateStreaming=="function"&&!isDataURI(binaryFile)&&typeof fetch=="function"){return fetch(binaryFile,{credentials:"same-origin"}).then(response=>{var result=WebAssembly.instantiateStreaming(response,imports);return result.then(callback,function(reason){err(`wasm streaming compile failed: ${reason}`);err("falling back to ArrayBuffer instantiation");return instantiateArrayBuffer(binaryFile,imports,callback)})})}return instantiateArrayBuffer(binaryFile,imports,callback)}function createWasm(){var info={"env":wasmImports,"wasi_snapshot_preview1":wasmImports};function receiveInstance(instance,module){wasmExports=instance.exports;wasmMemory=wasmExports["memory"];assert(wasmMemory,"memory not found in wasm exports");updateMemoryViews();addOnInit(wasmExports["__wasm_call_ctors"]);removeRunDependency("wasm-instantiate");return wasmExports}addRunDependency("wasm-instantiate");var trueModule=Module;function receiveInstantiationResult(result){assert(Module===trueModule,"the Module object should not be replaced during async compilation - perhaps the order of HTML elements is wrong?");trueModule=null;receiveInstance(result["instance"])}if(Module["instantiateWasm"]){try{return Module["instantiateWasm"](info,receiveInstance)}catch(e){err(`Module.instantiateWasm callback failed with error: ${e}`);return false}}instantiateAsync(wasmBinary,wasmBinaryFile,info,receiveInstantiationResult);return{}}function legacyModuleProp(prop,newName,incomming=true){if(!Object.getOwnPropertyDescriptor(Module,prop)){Object.defineProperty(Module,prop,{configurable:true,get(){let extra=incomming?" (the initial value can be provided on Module, but after startup the value is only looked for on a local variable of that name)":"";abort(`\`Module.${prop}\` has been replaced by \`${newName}\``+extra)}})}}function ignoredModuleProp(prop){if(Object.getOwnPropertyDescriptor(Module,prop)){abort(`\`Module.${prop}\` was supplied but \`${prop}\` not included in INCOMING_MODULE_JS_API`)}}function isExportedByForceFilesystem(name){return name==="FS_createPath"||name==="FS_createDataFile"||name==="FS_createPreloadedFile"||name==="FS_unlink"||name==="addRunDependency"||name==="FS_createLazyFile"||name==="FS_createDevice"||name==="removeRunDependency"}function missingGlobal(sym,msg){if(typeof globalThis!=="undefined"){Object.defineProperty(globalThis,sym,{configurable:true,get(){warnOnce("`"+sym+"` is not longer defined by emscripten. "+msg);return undefined}})}}missingGlobal("buffer","Please use HEAP8.buffer or wasmMemory.buffer");missingGlobal("asm","Please use wasmExports instead");function missingLibrarySymbol(sym){if(typeof globalThis!=="undefined"&&!Object.getOwnPropertyDescriptor(globalThis,sym)){Object.defineProperty(globalThis,sym,{configurable:true,get(){var msg="`"+sym+"` is a library symbol and not included by default; add it to your library.js __deps or to DEFAULT_LIBRARY_FUNCS_TO_INCLUDE on the command line";var librarySymbol=sym;if(!librarySymbol.startsWith("_")){librarySymbol="$"+sym}msg+=" (e.g. -sDEFAULT_LIBRARY_FUNCS_TO_INCLUDE='"+librarySymbol+"')";if(isExportedByForceFilesystem(sym)){msg+=". Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you"}warnOnce(msg);return undefined}})}unexportedRuntimeSymbol(sym)}function unexportedRuntimeSymbol(sym){if(!Object.getOwnPropertyDescriptor(Module,sym)){Object.defineProperty(Module,sym,{configurable:true,get(){var msg="'"+sym+"' was not exported. add it to EXPORTED_RUNTIME_METHODS (see the Emscripten FAQ)";if(isExportedByForceFilesystem(sym)){msg+=". Alternatively, forcing filesystem support (-sFORCE_FILESYSTEM) can export this for you"}abort(msg)}})}}function js_random(buf,num){var byteArray=new Uint8Array(num);var engine=self.crypto||self.msCrypto;engine.getRandomValues(byteArray);Module["HEAP8"].set(byteArray,buf);return 1}function ExitStatus(status){this.name="ExitStatus";this.message=`Program terminated with exit(${status})`;this.status=status}var callRuntimeCallbacks=callbacks=>{while(callbacks.length>0){callbacks.shift()(Module)}};var noExitRuntime=Module["noExitRuntime"]||true;var ptrToString=ptr=>{assert(typeof ptr==="number");ptr>>>=0;return"0x"+ptr.toString(16).padStart(8,"0")};var warnOnce=text=>{if(!warnOnce.shown)warnOnce.shown={};if(!warnOnce.shown[text]){warnOnce.shown[text]=1;err(text)}};function ExceptionInfo(excPtr){this.excPtr=excPtr;this.ptr=excPtr-24;this.set_type=function(type){HEAPU32[this.ptr+4>>2]=type};this.get_type=function(){return HEAPU32[this.ptr+4>>2]};this.set_destructor=function(destructor){HEAPU32[this.ptr+8>>2]=destructor};this.get_destructor=function(){return HEAPU32[this.ptr+8>>2]};this.set_caught=function(caught){caught=caught?1:0;HEAP8[this.ptr+12>>0]=caught};this.get_caught=function(){return HEAP8[this.ptr+12>>0]!=0};this.set_rethrown=function(rethrown){rethrown=rethrown?1:0;HEAP8[this.ptr+13>>0]=rethrown};this.get_rethrown=function(){return HEAP8[this.ptr+13>>0]!=0};this.init=function(type,destructor){this.set_adjusted_ptr(0);this.set_type(type);this.set_destructor(destructor)};this.set_adjusted_ptr=function(adjustedPtr){HEAPU32[this.ptr+16>>2]=adjustedPtr};this.get_adjusted_ptr=function(){return HEAPU32[this.ptr+16>>2]};this.get_exception_ptr=function(){var isPointer=___cxa_is_pointer_type(this.get_type());if(isPointer){return HEAPU32[this.excPtr>>2]}var adjusted=this.get_adjusted_ptr();if(adjusted!==0)return adjusted;return this.excPtr}}var exceptionLast=0;var uncaughtExceptionCount=0;var ___cxa_throw=(ptr,type,destructor)=>{var info=new ExceptionInfo(ptr);info.init(type,destructor);exceptionLast=ptr;uncaughtExceptionCount++;assert(false,"Exception thrown, but exception catching is not enabled. Compile with -sNO_DISABLE_EXCEPTION_CATCHING or -sEXCEPTION_CATCHING_ALLOWED=[..] to catch.")};var UTF8Decoder=typeof TextDecoder!="undefined"?new TextDecoder("utf8"):undefined;var UTF8ArrayToString=(heapOrArray,idx,maxBytesToRead)=>{var endIdx=idx+maxBytesToRead;var endPtr=idx;while(heapOrArray[endPtr]&&!(endPtr>=endIdx))++endPtr;if(endPtr-idx>16&&heapOrArray.buffer&&UTF8Decoder){return UTF8Decoder.decode(heapOrArray.subarray(idx,endPtr))}var str="";while(idx<endPtr){var u0=heapOrArray[idx++];if(!(u0&128)){str+=String.fromCharCode(u0);continue}var u1=heapOrArray[idx++]&63;if((u0&224)==192){str+=String.fromCharCode((u0&31)<<6|u1);continue}var u2=heapOrArray[idx++]&63;if((u0&240)==224){u0=(u0&15)<<12|u1<<6|u2}else{if((u0&248)!=240)warnOnce("Invalid UTF-8 leading byte "+ptrToString(u0)+" encountered when deserializing a UTF-8 string in wasm memory to a JS string!");u0=(u0&7)<<18|u1<<12|u2<<6|heapOrArray[idx++]&63}if(u0<65536){str+=String.fromCharCode(u0)}else{var ch=u0-65536;str+=String.fromCharCode(55296|ch>>10,56320|ch&1023)}}return str};var UTF8ToString=(ptr,maxBytesToRead)=>{assert(typeof ptr=="number",`UTF8ToString expects a number (got ${typeof ptr})`);return ptr?UTF8ArrayToString(HEAPU8,ptr,maxBytesToRead):""};var SYSCALLS={varargs:undefined,get(){assert(SYSCALLS.varargs!=undefined);var ret=HEAP32[+SYSCALLS.varargs>>2];SYSCALLS.varargs+=4;return ret},getp(){return SYSCALLS.get()},getStr(ptr){var ret=UTF8ToString(ptr);return ret}};function ___syscall_fcntl64(fd,cmd,varargs){SYSCALLS.varargs=varargs;return 0}var ___syscall_fstat64=(fd,buf)=>{abort("it should not be possible to operate on streams when !SYSCALLS_REQUIRE_FILESYSTEM")};var stringToUTF8Array=(str,heap,outIdx,maxBytesToWrite)=>{assert(typeof str==="string",`stringToUTF8Array expects a string (got ${typeof str})`);if(!(maxBytesToWrite>0))return 0;var startIdx=outIdx;var endIdx=outIdx+maxBytesToWrite-1;for(var i=0;i<str.length;++i){var u=str.charCodeAt(i);if(u>=55296&&u<=57343){var u1=str.charCodeAt(++i);u=65536+((u&1023)<<10)|u1&1023}if(u<=127){if(outIdx>=endIdx)break;heap[outIdx++]=u}else if(u<=2047){if(outIdx+1>=endIdx)break;heap[outIdx++]=192|u>>6;heap[outIdx++]=128|u&63}else if(u<=65535){if(outIdx+2>=endIdx)break;heap[outIdx++]=224|u>>12;heap[outIdx++]=128|u>>6&63;heap[outIdx++]=128|u&63}else{if(outIdx+3>=endIdx)break;if(u>1114111)warnOnce("Invalid Unicode code point "+ptrToString(u)+" encountered when serializing a JS string to a UTF-8 string in wasm memory! (Valid unicode code points should be in range 0-0x10FFFF).");heap[outIdx++]=240|u>>18;heap[outIdx++]=128|u>>12&63;heap[outIdx++]=128|u>>6&63;heap[outIdx++]=128|u&63}}heap[outIdx]=0;return outIdx-startIdx};var ___syscall_getdents64=(fd,dirp,count)=>{abort("it should not be possible to operate on streams when !SYSCALLS_REQUIRE_FILESYSTEM")};function ___syscall_ioctl(fd,op,varargs){SYSCALLS.varargs=varargs;return 0}var ___syscall_lstat64=(path,buf)=>{abort("it should not be possible to operate on streams when !SYSCALLS_REQUIRE_FILESYSTEM")};var ___syscall_newfstatat=(dirfd,path,buf,flags)=>{abort("it should not be possible to operate on streams when !SYSCALLS_REQUIRE_FILESYSTEM")};function ___syscall_openat(dirfd,path,flags,varargs){SYSCALLS.varargs=varargs;abort("it should not be possible to operate on streams when !SYSCALLS_REQUIRE_FILESYSTEM")}var ___syscall_stat64=(path,buf)=>{abort("it should not be possible to operate on streams when !SYSCALLS_REQUIRE_FILESYSTEM")};var nowIsMonotonic=true;var __emscripten_get_now_is_monotonic=()=>nowIsMonotonic;var convertI32PairToI53Checked=(lo,hi)=>{assert(lo==lo>>>0||lo==(lo|0));assert(hi===(hi|0));return hi+2097152>>>0<4194305-!!lo?(lo>>>0)+hi*4294967296:NaN};function __munmap_js(addr,len,prot,flags,fd,offset_low,offset_high){var offset=convertI32PairToI53Checked(offset_low,offset_high)}var _abort=()=>{abort("native code called abort()")};var _emscripten_date_now=()=>Date.now();var _emscripten_get_now;_emscripten_get_now=()=>performance.now();var _emscripten_memcpy_js=(dest,src,num)=>HEAPU8.copyWithin(dest,src,src+num);var getHeapMax=()=>2147483648;var growMemory=size=>{var b=wasmMemory.buffer;var pages=(size-b.byteLength+65535)/65536;try{wasmMemory.grow(pages);updateMemoryViews();return 1}catch(e){err(`growMemory: Attempted to grow heap from ${b.byteLength} bytes to ${size} bytes, but got error: ${e}`)}};var _emscripten_resize_heap=requestedSize=>{var oldSize=HEAPU8.length;requestedSize>>>=0;assert(requestedSize>oldSize);var maxHeapSize=getHeapMax();if(requestedSize>maxHeapSize){err(`Cannot enlarge memory, requested ${requestedSize} bytes, but the limit is ${maxHeapSize} bytes!`);return false}var alignUp=(x,multiple)=>x+(multiple-x%multiple)%multiple;for(var cutDown=1;cutDown<=4;cutDown*=2){var overGrownHeapSize=oldSize*(1+.2/cutDown);overGrownHeapSize=Math.min(overGrownHeapSize,requestedSize+100663296);var newSize=Math.min(maxHeapSize,alignUp(Math.max(requestedSize,overGrownHeapSize),65536));var replacement=growMemory(newSize);if(replacement){return true}}err(`Failed to grow the heap from ${oldSize} bytes to ${newSize} bytes, not enough memory!`);return false};var ENV={};var getExecutableName=()=>thisProgram||"./this.program";var getEnvStrings=()=>{if(!getEnvStrings.strings){var lang=(typeof navigator=="object"&&navigator.languages&&navigator.languages[0]||"C").replace("-","_")+".UTF-8";var env={"USER":"web_user","LOGNAME":"web_user","PATH":"/","PWD":"/","HOME":"/home/web_user","LANG":lang,"_":getExecutableName()};for(var x in ENV){if(ENV[x]===undefined)delete env[x];else env[x]=ENV[x]}var strings=[];for(var x in env){strings.push(`${x}=${env[x]}`)}getEnvStrings.strings=strings}return getEnvStrings.strings};var stringToAscii=(str,buffer)=>{for(var i=0;i<str.length;++i){assert(str.charCodeAt(i)===(str.charCodeAt(i)&255));HEAP8[buffer++>>0]=str.charCodeAt(i)}HEAP8[buffer>>0]=0};var _environ_get=(__environ,environ_buf)=>{var bufSize=0;getEnvStrings().forEach((string,i)=>{var ptr=environ_buf+bufSize;HEAPU32[__environ+i*4>>2]=ptr;stringToAscii(string,ptr);bufSize+=string.length+1});return 0};var _environ_sizes_get=(penviron_count,penviron_buf_size)=>{var strings=getEnvStrings();HEAPU32[penviron_count>>2]=strings.length;var bufSize=0;strings.forEach(string=>bufSize+=string.length+1);HEAPU32[penviron_buf_size>>2]=bufSize;return 0};var _fd_close=fd=>{abort("fd_close called without SYSCALLS_REQUIRE_FILESYSTEM")};var _fd_read=(fd,iov,iovcnt,pnum)=>{abort("fd_read called without SYSCALLS_REQUIRE_FILESYSTEM")};function _fd_seek(fd,offset_low,offset_high,whence,newOffset){var offset=convertI32PairToI53Checked(offset_low,offset_high);return 70}var printCharBuffers=[null,[],[]];var printChar=(stream,curr)=>{var buffer=printCharBuffers[stream];assert(buffer);if(curr===0||curr===10){(stream===1?out:err)(UTF8ArrayToString(buffer,0));buffer.length=0}else{buffer.push(curr)}};var flush_NO_FILESYSTEM=()=>{_fflush(0);if(printCharBuffers[1].length)printChar(1,10);if(printCharBuffers[2].length)printChar(2,10)};var _fd_write=(fd,iov,iovcnt,pnum)=>{var num=0;for(var i=0;i<iovcnt;i++){var ptr=HEAPU32[iov>>2];var len=HEAPU32[iov+4>>2];iov+=8;for(var j=0;j<len;j++){printChar(fd,HEAPU8[ptr+j])}num+=len}HEAPU32[pnum>>2]=num;return 0};function checkIncomingModuleAPI(){ignoredModuleProp("fetchSettings")}var wasmImports={__cxa_throw:___cxa_throw,__syscall_fcntl64:___syscall_fcntl64,__syscall_fstat64:___syscall_fstat64,__syscall_getdents64:___syscall_getdents64,__syscall_ioctl:___syscall_ioctl,__syscall_lstat64:___syscall_lstat64,__syscall_newfstatat:___syscall_newfstatat,__syscall_openat:___syscall_openat,__syscall_stat64:___syscall_stat64,_emscripten_get_now_is_monotonic:__emscripten_get_now_is_monotonic,_munmap_js:__munmap_js,abort:_abort,emscripten_date_now:_emscripten_date_now,emscripten_get_now:_emscripten_get_now,emscripten_memcpy_js:_emscripten_memcpy_js,emscripten_resize_heap:_emscripten_resize_heap,environ_get:_environ_get,environ_sizes_get:_environ_sizes_get,fd_close:_fd_close,fd_read:_fd_read,fd_seek:_fd_seek,fd_write:_fd_write,js_random:js_random};var wasmExports=createWasm();var ___wasm_call_ctors=createExportWrapper("__wasm_call_ctors");var _malloc=Module["_malloc"]=createExportWrapper("malloc");var _free=Module["_free"]=createExportWrapper("free");var ___errno_location=createExportWrapper("__errno_location");var _ntohs=createExportWrapper("ntohs");var _fflush=Module["_fflush"]=createExportWrapper("fflush");var _Crypto_Malloc=Module["_Crypto_Malloc"]=createExportWrapper("Crypto_Malloc");var _Crypto_Free=Module["_Crypto_Free"]=createExportWrapper("Crypto_Free");var _Crypto_CreateKeys=Module["_Crypto_CreateKeys"]=createExportWrapper("Crypto_CreateKeys");var _Crypto_Sign=Module["_Crypto_Sign"]=createExportWrapper("Crypto_Sign");var _Crypto_Decrypt=Module["_Crypto_Decrypt"]=createExportWrapper("Crypto_Decrypt");var _Crypto_Encrypt=Module["_Crypto_Encrypt"]=createExportWrapper("Crypto_Encrypt");var _Crypto_ChangePassword=Module["_Crypto_ChangePassword"]=createExportWrapper("Crypto_ChangePassword");var _htonl=createExportWrapper("htonl");var _htons=createExportWrapper("htons");var setTempRet0=createExportWrapper("setTempRet0");var _emscripten_stack_init=()=>(_emscripten_stack_init=wasmExports["emscripten_stack_init"])();var _emscripten_stack_get_free=()=>(_emscripten_stack_get_free=wasmExports["emscripten_stack_get_free"])();var _emscripten_stack_get_base=()=>(_emscripten_stack_get_base=wasmExports["emscripten_stack_get_base"])();var _emscripten_stack_get_end=()=>(_emscripten_stack_get_end=wasmExports["emscripten_stack_get_end"])();var stackSave=createExportWrapper("stackSave");var stackRestore=createExportWrapper("stackRestore");var stackAlloc=createExportWrapper("stackAlloc");var _emscripten_stack_get_current=()=>(_emscripten_stack_get_current=wasmExports["emscripten_stack_get_current"])();var ___cxa_is_pointer_type=createExportWrapper("__cxa_is_pointer_type");var dynCall_jiiii=Module["dynCall_jiiii"]=createExportWrapper("dynCall_jiiii");var dynCall_jiji=Module["dynCall_jiji"]=createExportWrapper("dynCall_jiji");var ___start_em_js=Module["___start_em_js"]=317660;var ___stop_em_js=Module["___stop_em_js"]=317859;var missingLibrarySymbols=["writeI53ToI64","writeI53ToI64Clamped","writeI53ToI64Signaling","writeI53ToU64Clamped","writeI53ToU64Signaling","readI53FromI64","readI53FromU64","convertI32PairToI53","convertU32PairToI53","zeroMemory","exitJS","isLeapYear","ydayFromDate","arraySum","addDays","inetPton4","inetNtop4","inetPton6","inetNtop6","readSockaddr","writeSockaddr","getHostByName","initRandomFill","randomFill","getCallstack","emscriptenLog","convertPCtoSourceLocation","readEmAsmArgs","jstoi_q","jstoi_s","listenOnce","autoResumeAudioContext","dynCallLegacy","getDynCaller","dynCall","handleException","keepRuntimeAlive","runtimeKeepalivePush","runtimeKeepalivePop","callUserCallback","maybeExit","asmjsMangle","asyncLoad","alignMemory","mmapAlloc","handleAllocatorInit","HandleAllocator","getNativeTypeSize","STACK_SIZE","STACK_ALIGN","POINTER_SIZE","ASSERTIONS","getCFunc","ccall","cwrap","uleb128Encode","sigToWasmTypes","generateFuncType","convertJsFunctionToWasm","getEmptyTableSlot","updateTableMap","getFunctionAddress","addFunction","removeFunction","reallyNegative","unSign","strLen","reSign","formatString","lengthBytesUTF8","intArrayFromString","intArrayToString","AsciiToString","UTF16ToString","stringToUTF16","lengthBytesUTF16","UTF32ToString","stringToUTF32","lengthBytesUTF32","stringToNewUTF8","stringToUTF8OnStack","writeArrayToMemory","registerKeyEventCallback","maybeCStringToJsString","findEventTarget","findCanvasEventTarget","getBoundingClientRect","fillMouseEventData","registerMouseEventCallback","registerWheelEventCallback","registerUiEventCallback","registerFocusEventCallback","fillDeviceOrientationEventData","registerDeviceOrientationEventCallback","fillDeviceMotionEventData","registerDeviceMotionEventCallback","screenOrientation","fillOrientationChangeEventData","registerOrientationChangeEventCallback","fillFullscreenChangeEventData","registerFullscreenChangeEventCallback","JSEvents_requestFullscreen","JSEvents_resizeCanvasForFullscreen","registerRestoreOldStyle","hideEverythingExceptGivenElement","restoreHiddenElements","setLetterbox","softFullscreenResizeWebGLRenderTarget","doRequestFullscreen","fillPointerlockChangeEventData","registerPointerlockChangeEventCallback","registerPointerlockErrorEventCallback","requestPointerLock","fillVisibilityChangeEventData","registerVisibilityChangeEventCallback","registerTouchEventCallback","fillGamepadEventData","registerGamepadEventCallback","registerBeforeUnloadEventCallback","fillBatteryEventData","battery","registerBatteryEventCallback","setCanvasElementSize","getCanvasElementSize","demangle","demangleAll","jsStackTrace","stackTrace","checkWasiClock","wasiRightsToMuslOFlags","wasiOFlagsToMuslOFlags","createDyncallWrapper","safeSetTimeout","setImmediateWrapped","clearImmediateWrapped","polyfillSetImmediate","getPromise","makePromise","idsToPromises","makePromiseCallback","findMatchingCatch","setMainLoop","getSocketFromFD","getSocketAddress","heapObjectForWebGLType","heapAccessShiftForWebGLHeap","webgl_enable_ANGLE_instanced_arrays","webgl_enable_OES_vertex_array_object","webgl_enable_WEBGL_draw_buffers","webgl_enable_WEBGL_multi_draw","emscriptenWebGLGet","computeUnpackAlignedImageSize","colorChannelsInGlTextureFormat","emscriptenWebGLGetTexPixelData","__glGenObject","emscriptenWebGLGetUniform","webglGetUniformLocation","webglPrepareUniformLocationsBeforeFirstUse","webglGetLeftBracePos","emscriptenWebGLGetVertexAttrib","__glGetActiveAttribOrUniform","writeGLArray","registerWebGlEventCallback","runAndAbortIfError","SDL_unicode","SDL_ttfContext","SDL_audio","ALLOC_NORMAL","ALLOC_STACK","allocate","writeStringToMemory","writeAsciiToMemory"];missingLibrarySymbols.forEach(missingLibrarySymbol);var unexportedSymbols=["run","addOnPreRun","addOnInit","addOnPreMain","addOnExit","addOnPostRun","addRunDependency","removeRunDependency","FS_createFolder","FS_createPath","FS_createLazyFile","FS_createLink","FS_createDevice","FS_readFile","out","err","callMain","abort","wasmMemory","wasmExports","stackAlloc","stackSave","stackRestore","getTempRet0","setTempRet0","writeStackCookie","checkStackCookie","convertI32PairToI53Checked","ptrToString","getHeapMax","growMemory","ENV","MONTH_DAYS_REGULAR","MONTH_DAYS_LEAP","MONTH_DAYS_REGULAR_CUMULATIVE","MONTH_DAYS_LEAP_CUMULATIVE","ERRNO_CODES","ERRNO_MESSAGES","setErrNo","DNS","Protocols","Sockets","timers","warnOnce","UNWIND_CACHE","readEmAsmArgsArray","getExecutableName","wasmTable","noExitRuntime","freeTableIndexes","functionsInTableMap","setValue","getValue","PATH","PATH_FS","UTF8Decoder","UTF8ArrayToString","UTF8ToString","stringToUTF8Array","stringToUTF8","stringToAscii","UTF16Decoder","JSEvents","specialHTMLTargets","currentFullscreenStrategy","restoreOldWindowedStyle","ExitStatus","getEnvStrings","flush_NO_FILESYSTEM","promiseMap","uncaughtExceptionCount","exceptionLast","exceptionCaught","ExceptionInfo","Browser","wget","SYSCALLS","tempFixedLengthArray","miniTempWebGLFloatBuffers","miniTempWebGLIntBuffers","GL","emscripten_webgl_power_preferences","AL","GLUT","EGL","GLEW","IDBStore","SDL","SDL_gfx","allocateUTF8","allocateUTF8OnStack"];unexportedSymbols.forEach(unexportedRuntimeSymbol);var calledRun;dependenciesFulfilled=function runCaller(){if(!calledRun)run();if(!calledRun)dependenciesFulfilled=runCaller};function stackCheckInit(){_emscripten_stack_init();writeStackCookie()}function run(){if(runDependencies>0){return}stackCheckInit();preRun();if(runDependencies>0){return}function doRun(){if(calledRun)return;calledRun=true;Module["calledRun"]=true;if(ABORT)return;initRuntime();if(Module["onRuntimeInitialized"])Module["onRuntimeInitialized"]();assert(!Module["_main"],'compiled without a main, but one is present. if you added it from JS, use Module["onRuntimeInitialized"]');postRun()}if(Module["setStatus"]){Module["setStatus"]("Running...");setTimeout(function(){setTimeout(function(){Module["setStatus"]("")},1);doRun()},1)}else{doRun()}checkStackCookie()}if(Module["preInit"]){if(typeof Module["preInit"]=="function")Module["preInit"]=[Module["preInit"]];while(Module["preInit"].length>0){Module["preInit"].pop()()}}run();


});


function KeyStorage()
{
    
}

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
    console.log("reseived in background:", message);

    // Отображение всплывающего окна
    chrome.action.openPopup();

    // Посылаем событие обратно на страницу
    if (sender.tab) {
        chrome.scripting.executeScript({
            target: { tabId: sender.tab.id },
            func: (msg) => {
                document.dispatchEvent(new CustomEvent("customEventFromExtension", { detail: msg }));
            },
            args: [message]
        });
    }

    sendResponse({ status: "received" });
});
