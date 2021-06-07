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

//module

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
        var imageFileRawData = Module["_Raster_Malloc"](imageFileRawDataSize);
        if (0 === imageFileRawData)
            return null;

        var uint8DataBuffer = new Uint8Array(dataBuffer);
        Module["HEAP8"].set(uint8DataBuffer, imageFileRawData);

        // грузим картинку
        var imageFile = Module["_Raster_Load"](imageFileRawData, imageFileRawDataSize);
        if (0 === imageFile)
        {
            Module["_Raster_Free"](imageFileRawData);
            return null;
        }

        // получаем данные картинки
        var imageW = Module["_Raster_GetWidth"](imageFile);
        var imageH = Module["_Raster_GetHeight"](imageFile);
        var imageRGBA = Module["_Raster_GetRGBA"](imageFile);

        if (imageW <= 0 || imageH <= 0 || 0 === imageRGBA)
        {
            Module["_Raster_Destroy"](imageFile);
            Module["_Raster_Free"](imageFileRawData);
            return null;
        }

        // быстро копируем память в канву
        var canvas = document.createElement("canvas");
        canvas.width = imageW;
        canvas.height = imageH;

        var canvasCtx = canvas.getContext("2d");
        var imageRGBAClampedArray = new Uint8ClampedArray(Module["HEAP8"].buffer, imageRGBA, 4 * imageW * imageH);
        var canvasData = new ImageData(imageRGBAClampedArray, imageW, imageH);

        canvasCtx.putImageData(canvasData, 0, 0);

        Module["_Raster_Destroy"](imageFile);
        Module["_Raster_Free"](imageFileRawData);

        return canvas;
    }
}

window.nativeRasterEngine = new Raster();
window.onEngineInit = function()
{
    window.nativeRasterEngine.isInit = true;
};

})(window, undefined);
