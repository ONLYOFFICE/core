(function(window, undefined){

var printErr = undefined;
var print    = undefined;

var fetch = self.fetch;
var getBinaryPromise = null;
if (self.AscDesktopEditor && document.currentScript && 0 == document.currentScript.src.indexOf("file:///"))
{
    fetch = undefined; // fetch not support file:/// scheme
    getBinaryPromise = function() {
        var wasmPath = "ascdesktop://zlib/" + wasmBinaryFile.substr(8);
        return new Promise(function (resolve, reject) {

            var xhr = new XMLHttpRequest();
            xhr.open('GET', wasmPath, true);
            xhr.responseType = 'arraybuffer';

            if (xhr.overrideMimeType)
                xhr.overrideMimeType('text/plain; charset=x-user-defined');
            else
                xhr.setRequestHeader('Accept-Charset', 'x-user-defined');

            xhr.onload = function () {
                if (this.status == 200) {
                    resolve(new Uint8Array(this.response));
                }
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

function Zlib()
{
    this.isInit = false;
	
	this.readFromUtf8 = function(buffer, start, len)
	{
		var result = "";
		var index = start;
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
	        {
	          	u0 = (u0 & 15) << 12 | u1 << 6 | u2;
	        } 
	        else 
	        {
	          	u0 = (u0 & 7) << 18 | u1 << 12 | u2 << 6 | buffer[index++] & 63;
	        }
	        if (u0 < 65536) 
	        {
	          	result += String.fromCharCode(u0);
	        } 
	        else 
	        {
	          	var ch = u0 - 65536;
	          	result += String.fromCharCode(55296 | ch >> 10, 56320 | ch & 1023);
	        }
      	}
      	return result;
	};

    this.openZip = function(dataBuffer)
    {
        if (!this.isInit)
        {
            // не подгружен еще модуль
            return null;
        }

        // копируем память в память webasm
        var FileRawDataSize = dataBuffer.byteLength;
        var FileRawData = Module["_Zlib_Malloc"](FileRawDataSize);
        if (0 === FileRawData)
            return null;

		var uint8DataBuffer = new Uint8Array(dataBuffer);
        Module["HEAP8"].set(uint8DataBuffer, FileRawData);

        // грузим данные
        var zipFile = Module["_Zlib_Load"](FileRawData, FileRawDataSize);
        if (0 === zipFile)
        {
            Module["_Zlib_Free"](FileRawData);
            return null;
        }

        // получаем пути в архиве
        var nPaths = Module["_Zlib_GetNumberPaths"](zipFile);
		var sPaths = Module["_Zlib_GetPaths"](zipFile);
        if (nPaths <= 0)
        {
            Module["_Zlib_Destroy"](zipFile);
            Module["_Zlib_Free"](FileRawData);
            return null;
        }
		
		var res = []
        // получаем содержимое файла по пути
		for (var i = 0; i < nPaths; i++)
		{
			var nSizeFile   = Module["_Zlib_GetSizeFileByPath"](zipFile, sPaths + i);
			var pointerFile = Module["_Zlib_GetLastFileByPath"](zipFile);
			
			var buffer = new Uint8Array(Module["HEAP8"].buffer, pointerFile, nSizeFile);
			var index = 0;
			var File = [];
			while (index < nSizeFile)
			{
				var lenRec = buffer[index] | buffer[index + 1] << 8 | buffer[index + 2] << 16 | buffer[index + 3] << 24;
				index += 4;
				File.push(this.readFromUtf8(buffer, index, lenRec));
				index += lenRec;
			}
			res.push(File);
		}
		return res;
    }
}

window.nativeZlibEngine = new Zlib();
window.onEngineInit = function()
{
    window.nativeZlibEngine.isInit = true;
};

})(window, undefined);
