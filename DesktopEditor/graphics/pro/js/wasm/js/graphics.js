(function(window, undefined){

	//desktop_fetch
	//string_utf8

    //module

    function Graphics()
    {
        this.isInit = false;
		this.engine = 0;
    
        this.testImage = function(wF, hF, width, height, dataBuffer)
        {
            if (!this.isInit)
                return null;
    
            if (this.engine)
				this.close();
			
			this.engine = Module["_Graphics_Create"](wF, hF, width, height);
			if (0 === this.engine)
				return null;
			var fonts = Module["_Fonts_Create"]();
			if (0 === fonts)
				return null;
				
			var imageFileRawDataSize = dataBuffer.byteLength;
			var imageFileRawData = Module["_Graphics_Malloc"](imageFileRawDataSize);
			if (0 === imageFileRawData)
				return null;
			
			var uint8DataBuffer = new Uint8Array(dataBuffer);
			Module["HEAP8"].set(uint8DataBuffer, imageFileRawData);
			
			var tmp = "Arial".toUtf8();
			var pointer = Module["_Graphics_Malloc"](tmp.length);
			if (0 === pointer)
				return null;
			Module["HEAP8"].set(tmp, pointer);

			Module["_Fonts_Add"](fonts, pointer, imageFileRawData, imageFileRawDataSize);	
			Module["_Graphics_TEST"](this.engine);
			
			Module["_Graphics_Free"](pointer);
			Module["_Graphics_Free"](imageFileRawData);
			
			var imageW = Module["_Graphics_GetPageWidth"](this.engine, 1);
			var imageH = Module["_Graphics_GetPageHeight"](this.engine, 1);
			var imageRGBA = Module["_Graphics_GetPage"](this.engine, imageW, imageH);
			
			if (imageW <= 0 || imageH <= 0 || 0 === imageRGBA)
			{
				Module["_Graphics_Destroy"](this.engine);
				return null;
			}
			
			var canvas = document.createElement("canvas");
			canvas.width = imageW;
			canvas.height = imageH;
	
			var canvasCtx = canvas.getContext("2d");
			var imageRGBAClampedArray = new Uint8ClampedArray(Module["HEAP8"].buffer, imageRGBA, 4 * imageW * imageH);
			var canvasData = new ImageData(imageRGBAClampedArray, imageW, imageH);
			
			canvasCtx.putImageData(canvasData, 0, 0);
	
			Module["_Fonts_Destroy"]();
			this.close();
			return canvas;
        }
		
		this.close = function()
		{
			if (!this.isInit || !this.engine)
				return;
			
			if (this.engine)
				Module["_Graphics_Destroy"](this.engine);
			this.engine = 0;
		}
    }
    
    window.nativeGraphics = new Graphics();
    window.onEngineGraphicsInit = function()
    {
        window.nativeGraphics.isInit = true;
    };

})(window, undefined);
