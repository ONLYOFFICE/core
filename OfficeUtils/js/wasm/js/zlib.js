(function(window, undefined){

var printErr = undefined;
var print    = undefined;

var fetch = self.fetch;
var getBinaryPromise = null;
if (self.AscDesktopEditor && document.currentScript && 0 == document.currentScript.src.indexOf("file:///"))
{
    fetch = undefined; // fetch not support file:/// scheme
    getBinaryPromise = function()
	{
        var wasmPath = "ascdesktop://zlib/" + wasmBinaryFile.substr(8);
        return new Promise(function (resolve, reject)
		{
            var xhr = new XMLHttpRequest();
            xhr.open('GET', wasmPath, true);
            xhr.responseType = 'arraybuffer';

            if (xhr.overrideMimeType)
                xhr.overrideMimeType('text/plain; charset=x-user-defined');
            else
                xhr.setRequestHeader('Accept-Charset', 'x-user-defined');

            xhr.onload = function ()
			{
                if (this.status == 200)
                    resolve(new Uint8Array(this.response));
            };
            xhr.send(null);
        });
    }
}
else
{
    getBinaryPromise = function() { return getBinaryPromise2(); }
}

//module

var readFromUtf8 = function(buffer, start, len)
{
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

var allocString = function(string)
{
    var inputLen = string.length;
    var testLen  = 6 * inputLen + 1;
    var tmpStrings = new ArrayBuffer(testLen);

    var code  = 0;
    var index = 0;

    var outputIndex = 0;
    var outputDataTmp = new Uint8Array(tmpStrings);
    var outputData = outputDataTmp;

    while (index < inputLen)
    {
        code = string.charCodeAt(index++);
        if (code >= 0xD800 && code <= 0xDFFF && index < inputLen)
            code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & string.charCodeAt(index++)));

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

    outputData[outputIndex++] = 0;

    var tmpBuffer = new Uint8Array(tmpStrings, 0, outputIndex);
    return { len : outputIndex, buf : tmpBuffer };
};

function Zlib()
{
    this.zipFile = null;
    this.isInit  = false;
    this.paths = [];
    this.files = [];
	
	this.CreateArchiveFromFiles(_files)
	{
		if (!this.isInit) return null;
		// создание дерева файлов
		var tmpBuffer = new Uint8Array();
		var index = 4;
		tmpBuffer.set([0,0,0,0], 0); // SkipLen
		for (var i = 0; i < _files.length; i++)
		{
			var path = allocString(_files[i].path);
			tmpBuffer.set([
				(path.len >> 24) & 0xFF,
				(path.len >> 16) & 0xFF,
				(path.len >>  8) & 0xFF,
				(path.len >>  0) & 0xFF
			], index);
			index += 4;
			tmpBuffer.set(path.buf, index);
			index += path.len;
			
			tmpBuffer.set([
				(_files[i].length >> 24) & 0xFF,
				(_files[i].length >> 16) & 0xFF,
				(_files[i].length >>  8) & 0xFF,
				(_files[i].length >>  0) & 0xFF
			], index);
			index += 4;
			tmpBuffer.set(_files[i].file, index);
			index += _files[i].length;
		}
		tmpBuffer.set([
			(index >> 24) & 0xFF,
			(index >> 16) & 0xFF,
			(index >>  8) & 0xFF,
			(index >>  0) & 0xFF
		], 0);
	}

    this.GetPathsInArchive = function()
    {
		return (this.isInit && this.zipFile != 0 ? this.paths : []);
    }
    
	this.GetFilesInArchive = function()
	{
		if (!this.isInit) return [];
		if (this.zipFile == 0) return [];
		
		var _paths = this.GetPathsInArchive();
		for (var i = 0; i < _paths.length; i++)
			this.GetFileInArchive(_paths[i]);
		return this.files;
	}
	
    this.GetFileInArchive = function(_path)
    {
        if (!this.isInit) return null;
        if (this.zipFile == 0) return null;
        
        var findFile = this.files.find(o => o.path === _path);
        if (findFile) return findFile;
        
        var tmp = allocString(_path);
        var pointer = Module["_Zlib_Malloc"](tmp.len);
        Module["HEAP8"].set(tmp.buf, pointer);
        
        var pointerFile = Module["_Zlib_GetFileFromArchive"](this.zipFile, pointer);
        if (pointerFile == 0) 
        {
            Module["_Zlib_Free"](pointer);
            return null;
        }
            
        var _lenFile = new Int32Array(Module["HEAP8"].buffer, pointerFile, 4);
        var len = _lenFile[0];
        
        var buffer = new Uint8Array(Module["HEAP8"].buffer, pointerFile + 4, len);
        var file = {
            path   : _path,
            length : len,
            file   : buffer
        };
        this.files.push(file);
        
        Module["_Zlib_Free"](pointer);
        return file;
    }
    
    this.openZip = function(dataBuffer)
    {
        if (!this.isInit) return null;

        // копируем память в память webasm
        var FileRawDataSize = dataBuffer.byteLength;
        var FileRawData = Module["_Zlib_Malloc"](FileRawDataSize);
        if (0 === FileRawData) return null;

        var uint8DataBuffer = new Uint8Array(dataBuffer);
        Module["HEAP8"].set(uint8DataBuffer, FileRawData);

        // грузим данные
        this.zipFile = Module["_Zlib_Load"](FileRawData, FileRawDataSize);
        if (this.zipFile == 0)
        {
            Module["_Zlib_Free"](FileRawData);
            return null;
        }

        // получаем пути в архиве
        var pointer = Module["_Zlib_GetPathsInArchive"](this.zipFile);
        if (pointer == 0)
        {
            Module["_Zlib_Destroy"](this.zipFile);
            Module["_Zlib_Free"](FileRawData);
            return null;
        }
        var lenArray = new Int32Array(Module["HEAP8"].buffer, pointer, 4);
        var len = lenArray[0];
        len -= 4;
        
        var buffer = new Uint8Array(Module["HEAP8"].buffer, pointer + 4, len);
        var index = 0;
        while (index < len)
        {
            var lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
            this.paths.push(readFromUtf8(buffer, index, lenRec));
            index += lenRec;
        }
        return this;
    }

    this.closeZip = function()
    {
        if (!this.isInit) return;
        if (this.zipFile != 0) Module["_Zlib_Destroy"](this.zipFile);
        this.paths = [];
        this.files = [];
    }
}

window.nativeZlibEngine = new Zlib();
window.onEngineInit = function()
{
    window.nativeZlibEngine.isInit = true;
};

})(window, undefined);
