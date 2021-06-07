(function(window, undefined){

	//desktop_fetch
	//string_utf8

    //module

    function Graphics()
    {
        this.isInit = false;
		this.engine = 0;
    
        this.testImage = function(wF, hF, width, height)
        {
            if (!this.isInit)
                return null;
    
            if (this.engine)
				this.close();
			
			var pFrame = Module["_Graphics_InitFrame"](wF, hF);
			
			this.engine = Module["_Graphics_Init"](pFrame, width, height);
			if (0 === this.engine)
				return null;
			
			Module["_Graphics_CoordTransformOffset"](this.engine, -160.294, -109.826);
			Module["_Graphics_transform"](this.engine, 1, 0, 0, 1, 0, 0);
			Module["_Graphics_SetIntegerGrid"](this.engine, false);
			Module["_Graphics_transform"](this.engine, 1, 0, 0, -1, 42.625, 68.6708);
			Module["_Graphics_p_width"](this.engine, 352.75);
			Module["_Graphics_s"](this.engine);
			Module["_Graphics_m"](this.engine, 0, 0);
			Module["_Graphics_l"](this.engine, 108.744, 39.4229);
			Module["_Graphics_p_color"](this.engine, 255, 0, 0, 255);
			Module["_Graphics_DrawPath"](this.engine, 1);
			Module["_Graphics_e"](this.engine);
			Module["_Graphics_p_dash"](this.engine, 0, null);
			Module["_Graphics_SetIntegerGrid"](this.engine, true);
			Module["_Graphics_reset"](this.engine);
			
			var imageW = Module["_Graphics_GetFrameWidth"](pFrame);
			var imageH = Module["_Graphics_GetFrameHeight"](pFrame);
			var imageRGBA = Module["_Graphics_GetRGBA"](pFrame);
			
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
	
			Module["_Graphics_DestroyFrame"](pFrame);
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
