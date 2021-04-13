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

    this.openZip = function(dataBuffer)
    {
        if (!this.isInit)
        {
            // не подгружен еще модуль
            return null;
        }

        // копируем память в память webasm
        var FileRawDataSize = dataBuffer.byteLength;
        var FileRawData = Module["_Raster_Malloc"](FileRawDataSize);
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

        // получаем содержимое файла по пути
		/*
        var canvas = document.createElement("canvas");
        canvas.width = imageW;
        canvas.height = imageH;

        var canvasCtx = canvas.getContext("2d");
        var imageRGBAClampedArray = new Uint8ClampedArray(Module["HEAP8"].buffer, imageRGBA, 4 * imageW * imageH);
        var canvasData = new ImageData(imageRGBAClampedArray, imageW, imageH);
		
        canvasCtx.putImageData(canvasData, 0, 0);

        Module["_Raster_Destroy"](zipFile);
        Module["_Raster_Free"](FileRawData);

        return canvas;
		*/
    }
}

window.nativeZlibEngine = new Zlib();
window.onEngineInit = function()
{
    window.nativeZlibEngine.isInit = true;
};

})(window, undefined);
