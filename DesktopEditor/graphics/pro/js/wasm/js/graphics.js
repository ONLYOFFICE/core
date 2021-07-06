(function(window, undefined){

	var printErr = undefined;
    var FS = undefined;
    var print = undefined;

    var getBinaryPromise = null;
    if (window["AscDesktopEditor"] && document.currentScript && 0 == document.currentScript.src.indexOf("file:///"))
    {
        // fetch not support file:/// scheme
        window.fetch = undefined;

        getBinaryPromise = function() {

            var wasmPath = "ascdesktop://fonts/" + wasmBinaryFile.substr(8);
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
        getBinaryPromise = function() {
            return getBinaryPromise2();
        }
    }

    //module

    function Graphics()
    {
        this.isInit = false;
		this.engine = 0;
    
        this.testImage = function(dataBuffer)
        {
            if (!this.isInit)
                return null;
    
            if (this.engine)
				this.close();

			var imageFileRawDataSize = dataBuffer.byteLength;
			var imageFileRawData = Module["_malloc"](imageFileRawDataSize);
			if (0 === imageFileRawData)
				return null;
			
			var uint8DataBuffer = new Uint8Array(dataBuffer);
			Module["HEAP8"].set(uint8DataBuffer, imageFileRawData);
			this.engine = Module["_XPS_Load"](imageFileRawData, imageFileRawDataSize);
			Module["_free"](imageFileRawData);
			if (0 === this.engine)
				return null;
			
			var _info = Module["_XPS_GetInfo"](this.engine);
			if (!_info)
				return false;
			
			var _pages = Module["HEAP32"][_info >> 2];
			var _buffer = new Int32Array(Module["HEAP8"].buffer, _info, 1 + 3 * _pages);
			
			var _cur = 1;
			for (var i = 0; i < _pages; i++)
			{
				var imageW = Math.round(_buffer[_cur++] * 96 / 25.4);
				var imageH = Math.round(_buffer[_cur++] * 96 / 25.4);
				var imageRGBA = Module["_XPS_GetPixmap"](this.engine, i, imageW, imageH);
			}
			Module["_XPS_Delete"](_info);

			if (imageW <= 0 || imageH <= 0 || 0 === imageRGBA)
			{
				Module["_XPS_Close"](this.engine);
				return null;
			}
			
			var canvas = document.createElement("canvas");
			canvas.width = imageW;
			canvas.height = imageH;
	
			var canvasCtx = canvas.getContext("2d");
			var imageRGBAClampedArray = new Uint8ClampedArray(Module["HEAP8"].buffer, imageRGBA, 4 * imageW * imageH);
			var canvasData = new ImageData(imageRGBAClampedArray, imageW, imageH);
			
			canvasCtx.putImageData(canvasData, 0, 0);
	
			this.close();
			return canvas;
        }
		
		this.close = function()
		{
			if (!this.isInit || !this.engine)
				return;
			
			if (this.engine)
				Module["_XPS_Close"](this.engine);
			this.engine = 0;
		}
    }
    
    window.nativeGraphics = new Graphics();
    window.onEngineGraphicsInit = function()
    {
        window.nativeGraphics.isInit = true;
    };

})(window, undefined);
