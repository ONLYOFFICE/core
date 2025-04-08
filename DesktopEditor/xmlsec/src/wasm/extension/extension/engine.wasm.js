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

    // module

});
