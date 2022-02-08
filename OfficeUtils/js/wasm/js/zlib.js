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
    return { length : outputIndex, buf : tmpBuffer };
};

function Zlib()
{
    this.zipFile = null;
	this.memory  = null;
    this.isInit  = false;
    this.files = [];
	
	this.GetZip = function()
	{
		if (!this.isInit)  return null;
		if (!this.zipFile) return null;
		this.GetFilesInZip();
		
		// вычисление длины дерева
		var _paths = [];
		var nLength = 4;
		for (var i = 0; i < this.files.length; i++)
		{
			_paths.push(allocString(this.files[i].path));
			nLength += 4;
			nLength += _paths[i].length;

			nLength += 4;
			nLength += this.files[i].length;
		}
		// создание дерева файлов
		var tmpBuffer = new Uint8Array(nLength);
		tmpBuffer.set([
			(nLength >>  0) & 0xFF,
			(nLength >>  8) & 0xFF,
			(nLength >> 16) & 0xFF,
			(nLength >> 24) & 0xFF
		], 0);
		var index = 4;
		for (var i = 0; i < this.files.length; i++)
		{
			tmpBuffer.set([
				(_paths[i].length >>  0) & 0xFF,
				(_paths[i].length >>  8) & 0xFF,
				(_paths[i].length >> 16) & 0xFF,
				(_paths[i].length >> 24) & 0xFF
			], index);
			index += 4;
			tmpBuffer.set(_paths[i].buf, index);
			index += _paths[i].length;
			
			tmpBuffer.set([
				(this.files[i].length >>  0) & 0xFF,
				(this.files[i].length >>  8) & 0xFF,
				(this.files[i].length >> 16) & 0xFF,
				(this.files[i].length >> 24) & 0xFF
			], index);
			index += 4;
			tmpBuffer.set(this.files[i].file, index);
			index += this.files[i].length;
		}
		
		var pointer = Module["_Zlib_Malloc"](tmpBuffer.length);
		if (!pointer) return null;
		Module["HEAP8"].set(tmpBuffer, pointer);
		var pointerZip = Module["_Zlib_CompressFiles"](this.zipFile, pointer);
		if (!pointerZip) return null;
		var _lenFile = new Int32Array(Module["HEAP8"].buffer, pointerZip, 4);
        var len = _lenFile[0];
		var zip = new Uint8Array(Module["HEAP8"].buffer, pointerZip + 4, len);
		Module["_Zlib_Free"](pointer);
		return zip;
	}
	
	this.CreateZipFromFiles = function(_files)
	{
		if (!this.isInit) return null;
		if (this.zipFile) this.CloseZip();
		this.zipFile = Module["_Zlib_Create"]();
		for (var i = 0; i < _files.length; i++)
			this.files.push({
				path   : _files[i].path,
				length : _files[i].length,
				file   : _files[i].file
			});
	}

	this.AddFileInZip = function(_file)
	{
		if (!this.isInit)  return false;
		if (!this.zipFile) return false;
		if (!this.files.length) this.GetFilesInZip();
		else if (!this.files[0].length) this.GetFilesInZip();
		
		var findFile = this.files.find(o => o.path == _file.path);
		if (findFile && findFile.length != _file.length)
		{
			this.DeleteFileInZip(findFile.path)
			this.files.push(_file);
		}
		else if (!findFile)
			this.files.push(_file);
		return true;
	}
	
	this.DeleteFileInZip = function(_path)
	{
		if (!this.isInit)  return false;
		if (!this.zipFile) return false;
		if (!this.files.length) this.GetFilesInZip();
		else if (!this.files[0].length) this.GetFilesInZip();
		
		for (var i = 0; i < this.files.length; i++)
		{
			if (this.files[i].path == _path)
			{
				this.files.splice(i, 1);
				return true;
			}
		}
		return false;
	}

    this.GetPathsInZip = function()
    {
		if (!this.isInit || !this.zipFile) return null;
		if (this.files.length)
		{
			var _paths = [];
			for (var i = 0; i < this.files.length; i++)
				_paths.push(this.files[i].path);
			return _paths;
		}
		
		// получаем пути в архиве
        var pointer = Module["_Zlib_GetPathsInArchive"](this.zipFile);
        if (!pointer)
        {
            Module["_Zlib_Destroy"](this.zipFile);
            Module["_Zlib_Free"](this.memory);
            return null;
        }
        var lenArray = new Int32Array(Module["HEAP8"].buffer, pointer, 4);
        var len = lenArray[0];
        len -= 4;
        
        var buffer = new Uint8Array(Module["HEAP8"].buffer, pointer + 4, len);
        var index = 0;
		var _paths = [];
        while (index < len)
        {
            var lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
            index += 4;
			var _path = readFromUtf8(buffer, index, lenRec);			
			var findPath = this.files.find(o => o.path === _path);
			if (!findPath)
			{
				this.files.push({
					path   : _path,
					length : 0,
					file   : null
				});
				_paths.push(_path);
			}
			index += lenRec;
        }
		return _paths;
    }
    
	this.GetFilesInZip = function()
	{
		if (!this.isInit)  return [];
		if (!this.zipFile) return [];
		
		var _paths = this.GetPathsInZip();
		var _files = [];
		for (var i = 0; i < _paths.length; i++)
			_files.push(this.GetFileInZip(_paths[i]));
		return _files;
	}
	
    this.GetFileInZip = function(_path)
    {
        if (!this.isInit)  return null;
        if (!this.zipFile) return null;
        
        var findFile = this.files.find(o => o.path === _path);
        if (findFile && findFile.file) return findFile;
        
        var tmp = allocString(_path);
        var pointer = Module["_Zlib_Malloc"](tmp.length);
		if (!pointer) return null;
        Module["HEAP8"].set(tmp.buf, pointer);
        
        var pointerFile = Module["_Zlib_GetFileFromArchive"](this.zipFile, pointer);
        if (!pointerFile) 
        {
            Module["_Zlib_Free"](pointer);
            return null;
        }
            
        var _lenFile = new Int32Array(Module["HEAP8"].buffer, pointerFile, 4);
        var len = _lenFile[0];
        
        var buffer = new Uint8Array(Module["HEAP8"].buffer, pointerFile + 4, len);
        if (findFile)
		{
			findFile.length = len;
			findFile.file = buffer;
			findFile = {
				path   : _path,
				length : len,
				file   : buffer
			};
		}
		else
		{
			findFile = {
				path   : _path,
				length : len,
				file   : buffer
			};
			this.files.push(findFile);
		}

        Module["_Zlib_Free"](pointer);
        return findFile;
    }
    
    this.OpenZipFromBuffer = function(dataBuffer)
    {
		var uint8DataBuffer = new Uint8Array(dataBuffer);
		return this.OpenZipFromUint8Array(uint8DataBuffer);
    }
	
	this.OpenZipFromUint8Array = function(dataBuffer)
    {
        if (!this.isInit) return null;
		if (this.zipFile) this.CloseZip();

        // копируем память в память webasm
        var FileRawDataSize = dataBuffer.length;
        this.memory = Module["_Zlib_Malloc"](FileRawDataSize);
        if (!this.memory) return null;
        Module["HEAP8"].set(dataBuffer, this.memory);

        // грузим данные
        this.zipFile = Module["_Zlib_Load"](this.memory, FileRawDataSize);
        if (!this.zipFile)
        {
            Module["_Zlib_Free"](this.memory);
            return null;
        }
		this.GetPathsInZip();
        return this;
    }

    this.CloseZip = function()
    {
        if (!this.isInit) return;
        if (this.zipFile) Module["_Zlib_Destroy"](this.zipFile);
		if (this.memory)  Module["_Zlib_Free"](this.memory);
        this.zipFile = null;
		this.memory  = null;
        this.files = [];
    }
}

window.nativeZlibEngine = new Zlib();
window.onEngineInit = function()
{
    window.nativeZlibEngine.isInit = true;
};

})(window, undefined);
