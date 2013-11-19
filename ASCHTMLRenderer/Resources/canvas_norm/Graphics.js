function CColor()
{
    this.R = 255;
    this.G = 255;
    this.B = 255;
    this.A = 255;
}
function CPen()
{
    this.Color      = new CColor();
    this.Style      = 0;
    this.LineCap    = 0;
    this.LineJoin   = 0;
}

function CBrush()
{
    this.Color1     = new CColor();
    this.Color2     = new CColor();
    this.Type       = 0;
}

var MATRIX_ORDER_PREPEND    = 0;
var MATRIX_ORDER_APPEND     = 1;

var memoryCanvasText 	= null;
var memoryCanvasContext = null;
var memoryImageDataText	= null;
var memoryCanvasW 		= 200;
var memoryCanvasH 		= 200;

var bIsIE = /*@cc_on ! @*/ false;
var bIsChrome = (null == window.chrome) ? 0 : 1;
var bIsSafari = (null == window.safari) ? 0 : 1;

function deg2rad(deg){
    return deg * Math.PI / 180.0;
}
function rad2deg(rad){
    return rad * 180.0 / Math.PI;
}

function CMatrix()
{
    this.sx     = 1.0;
    this.shx    = 0.0;
    this.shy    = 0.0;
    this.sy     = 1.0;
    this.tx     = 0.0;
    this.ty     = 0.0;

    this.Reset = function(){
        this.sx     = 1.0;
        this.shx    = 0.0;
        this.shy    = 0.0;
        this.sy     = 1.0;
        this.tx     = 0.0;
        this.ty     = 0.0;
    }
    // трансформ
    this.Multiply = function(matrix,order){
        if (MATRIX_ORDER_PREPEND == order)
        {
            var m = new CMatrix();
            m.sx     = matrix.sx;
            m.shx    = matrix.shx;
            m.shy    = matrix.shy;
            m.sy     = matrix.sy;
            m.tx     = matrix.tx;
            m.ty     = matrix.ty;
            m.Multiply(this,MATRIX_ORDER_APPEND);
            this.sx     = m.sx;
            this.shx    = m.shx;
            this.shy    = m.shy;
            this.sy     = m.sy;
            this.tx     = m.tx;
            this.ty     = m.ty;
        }
        else
        {
            var t0 = this.sx  * matrix.sx + this.shy * matrix.shx;
            var t2 = this.shx * matrix.sx + this.sy  * matrix.shx;
            var t4 = this.tx  * matrix.sx + this.ty  * matrix.shx + matrix.tx;
            this.shy = this.sx * matrix.shy + this.shy * matrix.sy;
            this.sy  = this.shx * matrix.shy + this.sy * matrix.sy;
            this.ty  = this.tx  * matrix.shy + this.ty * matrix.sy + matrix.ty;
            this.sx  = t0;
            this.shx = t2;
            this.tx  = t4;
        }
    }
    // а теперь частные случаи трансформа (для удобного пользования)
    this.Translate = function(x,y,order){
        var m = new CMatrix();
        m.tx  = x;
        m.ty  = y;
        this.Multiply(m,order);
    }
    this.Scale = function(x,y,order){
        var m = new CMatrix();
        m.sx  = x;
        m.sy  = y;
        this.Multiply(m,order);
    }
    this.Rotate = function(a,order){
        var m = new CMatrix();
        var rad = deg2rad(a);
        m.sx  = Math.cos(rad);
        m.shx = Math.sin(rad);
        m.shy = -Math.sin(rad);
        m.sy  = Math.cos(rad);
        this.Multiply(m,order);
    }
    this.RotateAt = function(a,x,y,order){
        this.Translate(-x,-y,order);
        this.Rotate(a,order);
        this.Translate(x,y,order);
    }
    // determinant
    this.Determinant = function(){
        return this.sx * this.sy - this.shy * this.shx;
    }
    // invert
    this.Invert = function(){
        var det = this.Determinant();
        if (0.0001 > det)
            return;
        var d = 1 / det;

        var t0 = this.sy * d;
        this.sy =  this.sx * d;
        this.shy = -this.shy * d;
        this.shx = -this.shx * d;

        var t4 = -this.tx * t0  - this.ty * this.shx;
        this.ty = -this.tx * this.shy - this.ty * this.sy;

        this.sx = t0;
        this.tx = t4;
    }
    // transform point
    this.TransformPointX = function(x,y){
        return x * this.sx  + y * this.shx + this.tx;
    }
    this.TransformPointY = function(x,y){
        return y * this.shy + y * this.sy  + this.ty;
    }
    // calculate rotate angle
    this.GetRotation = function(){
        var x1 = 0.0;
        var y1 = 0.0;
        var x2 = 1.0;
        var y2 = 0.0;
        this.TransformPoint(x1, y1);
        this.TransformPoint(x2, y2);
        var a = Math.atan2(y2-y1,x2-x1);
        return rad2deg(a);
    }
    // сделать дубликата
    this.CreateDublicate = function(){
        var m = new CMatrix();
        m.sx     = this.sx;
        m.shx    = this.shx;
        m.shy    = this.shy;
        m.sy     = this.sy;
        m.tx     = this.tx;
        m.ty     = this.ty;
        return m;
    }
}

function CGraphics(){
    this.m_pPixels      = null;                 // пикселы, пока только для текста
    this.m_oContext     = null;                 // контекст устройства (канвы)
    this.m_dWidthMM     = 0;
    this.m_dHeightMM    = 0;
    this.m_lWidthPix    = 0;
    this.m_lHeightPix   = 0;
    this.m_dDpiX        = 96.0;
    this.m_dDpiY        = 96.0; 
    this.m_bIsBreak 	= false;

    this.textBB_l       = 10000;
    this.textBB_t       = 10000;
    this.textBB_r       = -10000;
    this.textBB_b       = -10000;
	
	this.m_bIsTextPrevCommand = false;

    this.m_oPen     = new CPen();
    this.m_oBrush   = new CBrush();

    this.m_oFontManager = null;             // ссылка на менеджер шрифтов.
	this.m_bIsFillTextCanvasColor = 0;

    this.m_oCoordTransform  = new CMatrix();
    this.m_oTransform       = new CMatrix();
    this.m_oFullTransform   = new CMatrix();
    this.m_oInvertFullTransform = new CMatrix();

    // init function
    this.init = function(context,width_px,height_px,width_mm,height_mm)
    {
        this.m_oContext     = context;
        this.m_lHeightPix   = height_px;
        this.m_lWidthPix    = width_px;
        this.m_dWidthMM     = width_mm;
        this.m_dHeightMM    = height_mm;
        this.m_dDpiX        = 25.4 * this.m_lWidthPix / this.m_dWidthMM;
        this.m_dDpiY        = 25.4 * this.m_lHeightPix / this.m_dHeightMM;

        this.m_oCoordTransform.sx   = this.m_dDpiX / 25.4;
        this.m_oCoordTransform.sy   = this.m_dDpiY / 25.4;
		
		if (true == this.m_oContext.mozImageSmoothingEnabled)
			this.m_oContext.mozImageSmoothingEnabled = false; 
    }
    this.EndDraw = function(){
        this.DumpText();
    }
    this.DumpText = function(){
		return;
        if (null == memoryAllCanvasImageData || (true == this.m_bIsBreak))
		    return;

        var _x = Math.max(0,this.textBB_l);
        var _y = Math.max(0,this.textBB_t);
        var _r = Math.min(this.m_lWidthPix - 1,this.textBB_r);
        var _b = Math.min(this.m_lHeightPix - 1,this.textBB_b);

        this.textBB_l       = 10000;
        this.textBB_t       = 10000;
        this.textBB_r       = -10000;
        this.textBB_b       = -10000;

        // проставляем цвет
        var r = this.m_oBrush.Color1.R;
        var g = this.m_oBrush.Color1.G;
        var b = this.m_oBrush.Color1.B;
        if ((0 != r) || (0 != g) || (0 != b))
        {
            var index = 4 * (_y * this.m_lWidthPix + _x);
            var stride = 4 * (this.m_lWidthPix - _r + _x);
            for (var j = _y; j <= _b; ++j)
            {
                for (var i = _x; i <= _r; ++i)
                {
                    pPixelsImageData[index] = r;
                    pPixelsImageData[index + 1] = g;
                    pPixelsImageData[index + 2] = b;
                    index += 4;
                }

                index += stride;
            }
        }

        var _w = _r - _x + 1;
        var _h = _b - _y + 1;
        memoryAllCanvasContext.putImageData(memoryAllCanvasImageData,0,0,_x,_y,_w,_h);
        //memoryAllCanvasContext.putImageData(memoryAllCanvasImageData,0,0,0,0,this.m_lWidthPix,this.m_lHeightPix);

        this.m_oContext.setTransform(1,0,0,1,0,0);

        this.m_oContext.drawImage(memotyAllCanvas,_x,_y,_w,_h,_x,_y,_w,_h);
        //this.m_oContext.drawImage(memotyAllCanvas,0,0,this.m_lWidthPix,this.m_lHeightPix,0,0,this.m_lWidthPix,this.m_lHeightPix);

        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shx,this.m_oFullTransform.shy,
            this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
        
        delete memoryAllCanvasImageData;
        memoryAllCanvasImageData = null;
        pPixelsImageData =null;
	    //clearAllCanvasText();
    }

    // pen methods
    this.p_color = function(r,g,b,a){
        this.m_oPen.Color.R = r;
        this.m_oPen.Color.G = g;
        this.m_oPen.Color.B = b;
        this.m_oPen.Color.A = a;

        // выставить стиль канве
        this.m_oContext.strokeStyle = "rgba(" + this.m_oPen.Color.R + "," + this.m_oPen.Color.G + "," +
            this.m_oPen.Color.B + "," + (this.m_oPen.Color.A / 255) + ")";
    }
    this.p_width = function(w){
        this.m_oContext.lineWidth = w / 1000;
    }
    // brush methods
    this.b_color1 = function(r,g,b,a){
        this.DumpText();

        this.m_oBrush.Color1.R = r;
        this.m_oBrush.Color1.G = g;
        this.m_oBrush.Color1.B = b;
        this.m_oBrush.Color1.A = a;

        // выставить стиль канве
        this.m_oContext.fillStyle = "rgba(" + this.m_oBrush.Color1.R + "," + this.m_oBrush.Color1.G + "," +
            this.m_oBrush.Color1.B + "," + (this.m_oBrush.Color1.A / 255) + ")";
			
		this.m_bIsFillTextCanvasColor = 0;
    }
    this.b_color2 = function(r,g,b,a){
        this.m_oBrush.Color2.R = r;
        this.m_oBrush.Color2.G = g;
        this.m_oBrush.Color2.B = b;
        this.m_oBrush.Color2.A = a;

        // выставить стиль канве
    }
    // методы трансформации
    this.transform = function(sx,shy,shx,sy,tx,ty){
        this.m_oTransform.sx    = sx;
        this.m_oTransform.shx   = shx;
        this.m_oTransform.shy   = shy;
        this.m_oTransform.sy    = sy;
        this.m_oTransform.tx    = tx;
        this.m_oTransform.ty    = ty;

        // теперь проставим трансформ канве
        this.CalculateFullTransform();
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
            this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);

        this.m_oFontManager.SetTextMatrix(this.m_oTransform.sx,this.m_oTransform.shy,this.m_oTransform.shx,
            this.m_oTransform.sy,this.m_oTransform.tx,this.m_oTransform.ty);
    }
    this.CalculateFullTransform = function(){
        this.m_oFullTransform	= this.m_oCoordTransform.CreateDublicate();
        this.m_oFullTransform.Multiply(this.m_oTransform, MATRIX_ORDER_PREPEND);

        this.m_oInvertFullTransform.sx = this.m_oTransform.sx;
        this.m_oInvertFullTransform.shx = this.m_oTransform.shx;
        this.m_oInvertFullTransform.shy = this.m_oTransform.shy;
        this.m_oInvertFullTransform.sy = this.m_oTransform.sy;
        this.m_oInvertFullTransform.tx = this.m_oTransform.tx;
        this.m_oInvertFullTransform.ty = this.m_oTransform.ty;
        this.m_oInvertFullTransform.Invert();
        this.m_oInvertFullTransform.Multiply(this.m_oFullTransform,MATRIX_ORDER_PREPEND);
    }
    // path commands
    this._s = function(){
        this.m_oContext.beginPath();
    }
    this._z = function(){
        this.m_oContext.closePath();
    }
    this._m = function(x,y){
        this.m_oContext.moveTo(x/100,y/100);
    }
    this._l = function(x,y){
        this.m_oContext.lineTo(x/100,y/100);
    }
    this._c = function(x1,y1,x2,y2,x3,y3){
        this.m_oContext.bezierCurveTo(x1/100,y1/100,x2/100,y2/100,x3/100,y3/100);
    }
    this.ds = function(){
        this.m_oContext.stroke();
    }
    this.df = function(){
        this.m_oContext.fill();
    }

    // canvas state
    this.save = function(){
        this.m_oContext.save();
    }
    this.restore = function(){
        this.m_oContext.restore();
    }
    this.clip = function(){
        this.m_oContext.clip();
    }

    // images
    this.drawImage = function(img,x,y,w,h){
		if (this.m_bIsTextPrevCommand)
		{
			this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
				this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
				
			this.m_bIsTextPrevCommand = false;
		}
        this.m_oContext.drawImage(img,x,y,w,h);
    }

    // text
    this.font = function(font_id,font_size){
        this.m_oFontManager.LoadFontFromFile(font_id, font_size, this.m_dDpiX, this.m_dDpiY);
    }
    this.t = function(text,x,y){
		if (this.m_bIsBreak)
			return;
	
		var _x = this.m_oInvertFullTransform.TransformPointX(x/100,y/100);
		var _y = this.m_oInvertFullTransform.TransformPointY(x/100,y/100);

		try
		{
			this.m_oFontManager.LoadString2(text,_x,_y);
		}
		catch(err)
		{
		}
		
		this.m_oContext.setTransform(1,0,0,1,0,0);
		
		var arrayGlyphs = this.m_oFontManager.m_oGlyphString.m_pGlyphsBuffer;
		var countGlyphs = this.m_oFontManager.m_oGlyphString.m_nGlyphsCount;
		for (var i = 0; i < countGlyphs; i++)
		{
			var pGlyph = arrayGlyphs[i];
			if (null != pGlyph.oBitmap.pData)
			{
				var nW = pGlyph.oBitmap.nWidth;
				var nH = pGlyph.oBitmap.nHeight;

				if (0 == nW || 0 == nH)
					continue;

				var nX = parseInt(this.m_oFontManager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX);
				var nY = parseInt(this.m_oFontManager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY);
				
				pGlyph.oBitmap.oGlyphData.checkColor(this.m_oBrush.Color1.R,this.m_oBrush.Color1.G,this.m_oBrush.Color1.B,nW,nH);
				this.m_oContext.drawImage(pGlyph.oBitmap.oGlyphData.m_oCanvas,0,0,nW,nH,nX,nY,nW,nH);
			}
		}
        
		/*
		while (true)
        {
            var pGlyph = this.m_oFontManager.GetNextChar2();
            if (null == pGlyph)
                break;

            if (null != pGlyph.oBitmap.pData)
            {
                this.private_FillGlyph(pGlyph);
            }
        }
		*/

		this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
				this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
	this.tg = function(text,x,y){
		if (this.m_bIsBreak)
			return;
	
		var _x = this.m_oInvertFullTransform.TransformPointX(x/100,y/100);
		var _y = this.m_oInvertFullTransform.TransformPointY(x/100,y/100);

		try
		{
			this.m_oFontManager.LoadString3(text,_x,_y);
		}
		catch(err)
		{
		}
		
		this.m_oContext.setTransform(1,0,0,1,0,0);
		
		var arrayGlyphs = this.m_oFontManager.m_oGlyphString.m_pGlyphsBuffer;
		var countGlyphs = this.m_oFontManager.m_oGlyphString.m_nGlyphsCount;
		for (var i = 0; i < countGlyphs; i++)
		{
			var pGlyph = arrayGlyphs[i];
			if (null != pGlyph.oBitmap.pData)
			{
				var nW = pGlyph.oBitmap.nWidth;
				var nH = pGlyph.oBitmap.nHeight;

				if (0 == nW || 0 == nH)
					continue;

				var nX = parseInt(this.m_oFontManager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX);
				var nY = parseInt(this.m_oFontManager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY);
				
				pGlyph.oBitmap.oGlyphData.checkColor(this.m_oBrush.Color1.R,this.m_oBrush.Color1.G,this.m_oBrush.Color1.B,nW,nH);
				this.m_oContext.drawImage(pGlyph.oBitmap.oGlyphData.m_oCanvas,0,0,nW,nH,nX,nY,nW,nH);
			}
		}
		this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
				this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    this.charspace = function(space){
    }

    // private methods
    this.private_FillGlyph = function(pGlyph){

        // new scheme
        var nW = pGlyph.oBitmap.nWidth;
        var nH = pGlyph.oBitmap.nHeight;

        if (0 == nW || 0 == nH)
            return;

        var nX = parseInt(this.m_oFontManager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX);
        var nY = parseInt(this.m_oFontManager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY);

        pGlyph.oBitmap.oGlyphData.checkColor(this.m_oBrush.Color1.R,this.m_oBrush.Color1.G,this.m_oBrush.Color1.B,nW,nH);
        this.m_oContext.drawImage(pGlyph.oBitmap.oGlyphData.m_oCanvas,0,0,nW,nH,nX,nY,nW,nH);

        return;

        /*
        if (bIsChrome)
            this.private_FillGlyph3(pGlyph);
        else if (bIsIE)
            this.private_FillGlyph3(pGlyph);
	    else
	        this.private_FillGlyph1(pGlyph);
	    */
    }

    this.private_FillGlyph1 = function(pGlyph){
        var i = 0;
        var j = 0;

        var nW = pGlyph.oBitmap.nWidth;
        var nH = pGlyph.oBitmap.nHeight;

        if (0 == nW || 0 == nH)
            return;

        var nX = parseInt(this.m_oFontManager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX);
        var nY = parseInt(this.m_oFontManager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY);

        var pPixels = memoryImageDataText.data;

        if (0 == this.m_bIsFillTextCanvasColor)
        {
            var r = this.m_oBrush.Color1.R;
            var g = this.m_oBrush.Color1.G;
            var b = this.m_oBrush.Color1.B;

            var ind = 0;
            var count = memoryCanvasW * memoryCanvasH;
            for (var i = 0; i < count; ++i)
            {
                pPixels[ind] 	 = r;
                pPixels[ind + 1] = g;
                pPixels[ind + 2] = b;
                ind += 4;
            }
            this.m_bIsFillTextCanvasColor = 1;
        }

	    var indexG = 0;
		var pDataSrc = pGlyph.oBitmap.pData;
        for (j = 0; j < nH; j++)
        {
            var indx = 4 * j * memoryCanvasW + 3;
            for (i = 0; i < nW; i++)
            {
                pPixels[indx] = pDataSrc[indexG];
		        indexG++;
                indx += 4;
            }
        }
        // теперь быстро копируем пикселы на текстовую канву
        memoryCanvasContext.putImageData(memoryImageDataText,0,0,0,0,nW,nH);
        this.m_oContext.drawImage(memoryCanvasText,0,0,nW,nH,nX,nY,nW,nH);
    }
    this.private_FillGlyph2 = function(pGlyph){
        var i = 0;
        var j = 0;

        var nW = pGlyph.oBitmap.nWidth;
        var nH = pGlyph.oBitmap.nHeight;

        if (0 == nW || 0 == nH)
            return;

        var nX = parseInt(this.m_oFontManager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX);
        var nY = parseInt(this.m_oFontManager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY);

        var imageData = this.m_oContext.getImageData(nX,nY,nW,nH);
        var pPixels = imageData.data;
	
        var _r = this.m_oBrush.Color1.R;
        var _g = this.m_oBrush.Color1.G;
        var _b = this.m_oBrush.Color1.B;

        for (; j < nH; ++j)
        {
            var indx = 4 * j * nW;
            for (i = 0; i < nW; ++i)
            {
                var weight  = pGlyph.oBitmap.pData[j * pGlyph.oBitmap.nWidth + i];

                if (255 == weight)
                {
                    pPixels[indx]     = _r;
                    pPixels[indx + 1] = _g;
                    pPixels[indx + 2] = _b;
                    pPixels[indx + 3] = 255;
                }
                else
                {
                    var r = pPixels[indx];
                    var g = pPixels[indx + 1];
                    var b = pPixels[indx + 2];
                    var a = pPixels[indx + 3];

                    pPixels[indx]     = ((_r - r) * weight + (r << 8)) >> 8;
                    pPixels[indx + 1] = ((_g - g) * weight + (g << 8)) >> 8;
                    pPixels[indx + 2] = ((_b - b) * weight + (b << 8)) >> 8;
                    pPixels[indx + 3] = (weight + a) - ((weight * a + 256) >> 8);
                }

                indx += 4;
            }
        }
        this.m_oContext.putImageData(imageData,nX,nY);
    }
    this.private_FillGlyph3 = function(pGlyph){
        var i = 0;
        var j = 0;

        var nW = pGlyph.oBitmap.nWidth;
        var nH = pGlyph.oBitmap.nHeight;

        if (0 == nW || 0 == nH)
            return;

        if (null == memoryAllCanvasImageData)
        {
            memoryAllCanvasImageData = memoryAllCanvasContext.createImageData(this.m_lWidthPix,this.m_lHeightPix);
	        pPixelsImageData = memoryAllCanvasImageData.data;
            this.m_bIsFillTextCanvasColor = 1;
        }

        var nX = parseInt(this.m_oFontManager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX);
        var nY = parseInt(this.m_oFontManager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY);

        if (this.textBB_l > nX)
        {
            this.textBB_l = nX;
        }
        if (this.textBB_t > nY)
        {
            this.textBB_t = nY;
        }
        if (this.textBB_r < (nX + nW - 1))
        {
            this.textBB_r = nX + nW - 1;
        }
        if (this.textBB_b < (nY + nH - 1))
        {
            this.textBB_b = nY + nH - 1;
        }
        
	    var indexG = 0;
		var pDataSrc = pGlyph.oBitmap.pData;
        for (; j < nH; ++j)
        {
            var indx = 4 * ((j + nY) * this.m_lWidthPix + nX) + 3;
            for (i = 0; i < nW; ++i)
            {
                pPixelsImageData[indx] += pDataSrc[indexG];
                if (pPixelsImageData[indx] > 255)
                    pPixelsImageData[indx] = 255;
		        indexG++;
                indx += 4;
            }
        }
    }
}
