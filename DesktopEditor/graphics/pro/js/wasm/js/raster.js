(function(window, undefined){

var printErr = undefined;
var print    = undefined;

var fetch = self.fetch;
var getBinaryPromise = null;
if (self.AscDesktopEditor && document.currentScript && 0 == document.currentScript.src.indexOf("file:///"))
{
    fetch = undefined; // fetch not support file:/// scheme
    getBinaryPromise = function() {
        var wasmPath = "ascdesktop://raster/" + wasmBinaryFile.substr(8);
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

// module

function Raster()
{
    this.isInit = false;

    this.openImage = function(dataBuffer)
    {
        if (!this.isInit)
        {
            // не подгружен еще модуль
            return null;
        }

        // копируем память в память webasm
        var imageFileRawDataSize = dataBuffer.byteLength;
        var imageFileRawData = Module["_CxImage_Malloc"](imageFileRawDataSize);
        if (0 === imageFileRawData)
            return null;

		var uint8DataBuffer = new Uint8Array(dataBuffer);
        Module["HEAP8"].set(uint8DataBuffer, imageFileRawData);

        // грузим картинку
        var imageFile = Module["_CxImage_Load"](imageFileRawData, imageFileRawDataSize, 3);
        if (0 === imageFile)
        {
            Module["_CxImage_Free"](imageFileRawData);
            return null;
        }

        // получаем данные картинки
        var imageW = Module["_CxImage_GetWidth"](imageFile);
        var imageH = Module["_CxImage_GetHeight"](imageFile);
        var imageRGBA = Module["_CxImage_GetRGBA"](imageFile);

        if (imageW <= 0 || imageH <= 0 || 0 === imageRGBA)
        {
            Module["_CxImage_Destroy"](imageFile);
            Module["_CxImage_Free"](imageFileRawData);
            return null;
        }

        // быстро копируем память в канву
        var canvas = document.createElement("canvas");
        canvas.width = imageW;
        canvas.height = imageH;

        var canvasCtx = canvas.getContext("2d");
        var imageRGBAClampedArray = new Uint8ClampedArray(Module["HEAP8"].buffer, imageRGBA, 4 * imageW * imageH);
        var canvasData = new ImageData(imageRGBAClampedArray, imageW, imageH);
		
		// correct
		var alphaIndex = 4 * imageW * imageH - 1;
		while (alphaIndex > 0)
		{
			canvasData.data[alphaIndex] = 255;
			alphaIndex -= 4;
		}
		
        canvasCtx.putImageData(canvasData, 0, 0);

        Module["_CxImage_Destroy"](imageFile);
        Module["_CxImage_Free"](imageFileRawData);

        return canvas;
    }
}

window.nativeRasterEngine = new Raster();
window.onEngineInit = function()
{
    window.nativeRasterEngine.isInit = true;
};

})(window, undefined);
