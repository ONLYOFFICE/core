(function(window, undefined) {

    function CFile()
    {
    	this.nativeFile = 0;
    	this.pages = [];
    	this.zoom = 1;
    	this.isUse3d = false;
    	this.cacheManager = null;
    	this.logging = true;
    }

    // interface
    CFile.prototype.loadFromData = function(data) {};
    CFile.prototype.load = function(arrayBuffer)
    {
        return this.loadFromData(new Uint8Array(arrayBuffer));
    };

    CFile.prototype.getInfo = function() {};
    CFile.prototype.getPagePixmap = function(pageIndex, width, height) {};
    CFile.prototype.close = function() {};
    CFile.prototype.memory = function() {};
    CFile.prototype.free = function(pointer) {};

	CFile.prototype.getPage = function(pageIndex, width, height)
	{
		if (pageIndex < 0 || pageIndex >= this.pages.length)
			return null;
		if (!width) width = this.pages[pageIndex].W;
		if (!height) height = this.pages[pageIndex].H;
		var t0 = performance.now();
		var pixels = this.getPagePixmap(pageIndex, width, height);
		if (!pixels)
			return null;
		
		if (!this.logging)
		{
			var image = this._pixelsToCanvas(pixels, width, height);
		}
		else
		{
			var t1 = performance.now();
			var image = this._pixelsToCanvas(pixels, width, height);
			var t2 = performance.now();
			//console.log("time: " + (t1 - t0) + ", " + (t2 - t1));
		}

        this.free(pixels);
        return image;
	};

    CFile.prototype.getPageBase64 = function(pageIndex, width, height)
	{
		var _canvas = this.getPage(pageIndex, width, height);
		if (!_canvas)
			return "";
		
		try
		{
			return _canvas.toDataURL("image/png");
		}
		catch (err)
		{
		}
		
		return "";
	};
	CFile.prototype.isValid = function()
	{
		return this.pages.length > 0;
	};

	// private functions
	CFile.prototype._pixelsToCanvas2d = function(pixels, width, height)
    {        
        var canvas = null;
        if (this.cacheManager)
        {
            canvas = this.cacheManager.lock(width, height);
        }
        else
        {
            canvas = document.createElement("canvas");
            canvas.width = width;
            canvas.height = height;
        }
        
        var mappedBuffer = new Uint8ClampedArray(this.memory().buffer, pixels, 4 * width * height);
        var imageData = new ImageData(mappedBuffer, width, height);
        var ctx = canvas.getContext("2d");
        if (ctx)
            ctx.putImageData(imageData, 0, 0);
        return canvas;
    };

	var vs_source = "\
attribute vec2 aVertex;\n\
attribute vec2 aTex;\n\
varying vec2 vTex;\n\
void main() {\n\
	gl_Position = vec4(aVertex, 0.0, 1.0);\n\
	vTex = aTex;\n\
}";

    var fs_source = "\
precision mediump float;\n\
uniform sampler2D uTexture;\n\
varying vec2 vTex;\n\
void main() {\n\
	gl_FragColor = texture2D(uTexture, vTex);\n\
}";

	CFile.prototype._pixelsToCanvas3d = function(pixels, width, height) 
    {
        var canvas = null;
        if (this.cacheManager)
        {
            canvas = this.cacheManager.lock(width, height);
        }
        else
        {
            canvas = document.createElement("canvas");
            canvas.width = width;
            canvas.height = height;
        }

        var gl = canvas.getContext('webgl', { preserveDrawingBuffer : true });
        if (!gl)
            throw new Error('FAIL: could not create webgl canvas context');

        var colorCorrect = gl.BROWSER_DEFAULT_WEBGL;
        gl.pixelStorei(gl.UNPACK_COLORSPACE_CONVERSION_WEBGL, colorCorrect);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);

        gl.viewport(0, 0, canvas.width, canvas.height);
        gl.clearColor(0, 0, 0, 1);
        gl.clear(gl.COLOR_BUFFER_BIT);

        if (gl.getError() != gl.NONE)
            throw new Error('FAIL: webgl canvas context setup failed');

        function createShader(source, type) {
            var shader = gl.createShader(type);
            gl.shaderSource(shader, source);
            gl.compileShader(shader);
            if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS))
                throw new Error('FAIL: shader ' + id + ' compilation failed');
            return shader;
        }

        var program = gl.createProgram();
        gl.attachShader(program, createShader(vs_source, gl.VERTEX_SHADER));
        gl.attachShader(program, createShader(fs_source, gl.FRAGMENT_SHADER));
        gl.linkProgram(program);
        if (!gl.getProgramParameter(program, gl.LINK_STATUS))
            throw new Error('FAIL: webgl shader program linking failed');
        gl.useProgram(program);

        var texture = gl.createTexture();
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, new Uint8Array(this.memory().buffer, pixels, 4 * width * height));

        if (gl.getError() != gl.NONE)
            throw new Error('FAIL: creating webgl image texture failed');

        function createBuffer(data) {
            var buffer = gl.createBuffer();
            gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
            gl.bufferData(gl.ARRAY_BUFFER, data, gl.STATIC_DRAW);
            return buffer;
        }

        var vertexCoords = new Float32Array([-1, 1, -1, -1, 1, -1, 1, 1]);
        var vertexBuffer = createBuffer(vertexCoords);
        var location = gl.getAttribLocation(program, 'aVertex');
        gl.enableVertexAttribArray(location);
        gl.vertexAttribPointer(location, 2, gl.FLOAT, false, 0, 0);

        if (gl.getError() != gl.NONE)
            throw new Error('FAIL: vertex-coord setup failed');

        var texCoords = new Float32Array([0, 1, 0, 0, 1, 0, 1, 1]);
        var texBuffer = createBuffer(texCoords);
        var location = gl.getAttribLocation(program, 'aTex');
        gl.enableVertexAttribArray(location);
        gl.vertexAttribPointer(location, 2, gl.FLOAT, false, 0, 0);

        if (gl.getError() != gl.NONE)
            throw new Error('FAIL: tex-coord setup setup failed');

        gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
        return canvas;
    };
            
    CFile.prototype._pixelsToCanvas = function(pixels, width, height)
    {
        if (!this.isUse3d)
        {
            return this._pixelsToCanvas2d(pixels, width, height);
        }

        try
        {
            return this._pixelsToCanvas3d(pixels, width, height);
        }
        catch (err)
        {
            this.isUse3d = false;
            if (this.cacheManager)
                this.cacheManager.clear();
            return this._pixelsToCanvas(pixels, width, height);
        }
    };

 	window["AscViewer"] = window["AscViewer"] || {};
    window["AscViewer"].IFile = CFile;

    window["AscViewer"].createFile = function(buffer)
    {
        var data = new Uint8Array(buffer);
        var file = null;
        if (data.length > 7 &&
            0x41 == data[0] && 0x54 == data[1] && 0x26 == data[2] && 0x54 == data[3] &&
            0x46 == data[4] && 0x4f == data[5] && 0x52 == data[6] && 0x4d == data[7])
        {
            file = new window["AscViewer"].DjVuFile();
        }

        if (!file)
        {
            var maxCheck = Math.min(100, data.length - 5);
            var pdfCheck = [
                "%".charCodeAt(0),
                "P".charCodeAt(0),
                "D".charCodeAt(0),
                "F".charCodeAt(0),
                "-".charCodeAt(0)
            ];
            var isPdf = false;
            for (var i = 0; i < maxCheck; i++)
            {
                if (data[i + 0] == pdfCheck[0] &&
                    data[i + 1] == pdfCheck[1] &&
                    data[i + 2] == pdfCheck[2] &&
                    data[i + 3] == pdfCheck[3] &&
                    data[i + 4] == pdfCheck[4])
                {
                    isPdf = true;
                    break;
                }
            }

            if (isPdf)
            {
                file = new window["AscViewer"].PdfFile();
                file.type = 0;
            }
        }

        if (!file)
        {
            file = new window["AscViewer"].PdfFile();
            file.type = 1;
        }

        file.loadFromData(data);
        return file;
    };

})(window, undefined);
