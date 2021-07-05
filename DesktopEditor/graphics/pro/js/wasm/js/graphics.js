(function(window, undefined){

	//desktop_fetch
	//string_utf8

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
			
			this.engine = Module["_Graphics_Create"]();
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
			Module["_Graphics_Load"](this.engine, imageFileRawData, imageFileRawDataSize);
			
			var imageW = Module["_Graphics_GetPageWidth"](this.engine, 0);
			var imageH = Module["_Graphics_GetPageHeight"](this.engine, 0);
			var imageRGBA = Module["_Graphics_GetPage"](this.engine, 0, imageW, imageH);
			
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
	
			Module["_Graphics_Free"](imageFileRawData);
			this.close();
			Module["_Fonts_Destroy"]();
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
