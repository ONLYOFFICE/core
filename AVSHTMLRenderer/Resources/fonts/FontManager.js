var gFontArraysObj = new Array();
var gFontNamesObj = new Array();

var FONT_ITALIC_ANGLE = 0.3090169943749;
var FT_ENCODING_UNICODE = 1970170211;
var FT_ENCODING_NONE = 0;
var FT_ENCODING_MS_SYMBOL = 1937337698;
var FT_ENCODING_APPLE_ROMAN = 1634889070;

var LOAD_MODE = 40970;
var REND_MODE = 0;

var ft_glyph_bbox_gridfit = 1;

var FT_FACE_FLAG_KERNING = 1<<6;

FT_Kerning_Mode = 
{
    FT_KERNING_DEFAULT:   0,
    FT_KERNING_UNFITTED:  1,
    FT_KERNING_UNSCALED:  2

};

FontStyle =
{
    FontStyleRegular:    0,
    FontStyleBold:       1,
    FontStyleItalic:     2,
    FontStyleBoldItalic: 3,
    FontStyleUnderline:  4,
    FontStyleStrikeout:  8
};

EGlyphState = 
{
	glyphstateNormal:   0,  // символ отрисовался в нужном шрифте
	glyphstateDeafault: 1,  // символ отрисовался в дефолтовом шрифте
	glyphstateMiss:     2   // символ не отрисовался
};

function CPoint1()
{
	this.fX = 0;
	this.fY = 0;
	this.fWidth = 0;
	this.fHeight = 0;
};

function CPoint2()
{
	this.fLeft = 0;
	this.fTop = 0;
	this.fRight = 0;
	this.fBottom = 0;
};

var g_staticImageData       = null;
var g_staticImageDataPixels = null;
var g_staticImageDataW      = 200;
var g_staticImageDataH      = 200;
function CGlyphData()
{
    this.m_oCanvas  = null;
    this.m_oContext = null;
    this.R          = 0;
    this.G          = 0;
    this.B          = 0;

    this.init = function(width,height)
    {
        this.m_oCanvas = document.createElement('canvas');
        this.m_oContext = this.m_oCanvas.getContext('2d');
        this.m_oContext.globalCompositeOperation = "source-in";

        if (null == g_staticImageData)
        {
            g_staticImageData = this.m_oContext.createImageData(g_staticImageDataW,g_staticImageDataH);
            g_staticImageDataPixels = g_staticImageData.data;
        }
    }
    this.checkColor = function(r,g,b,w,h)
    {
        if ((r == this.R) && (g == this.G) && (b == this.B))
            return;
        this.R = r;
        this.G = g;
        this.B = b;
        this.m_oContext.fillStyle = "rgb(" + this.R + "," + this.G + "," + this.B + ")";
        this.m_oContext.fillRect(0,0,w,h)
    }
};

function TGlyphBitmap()
{
	this.nX = 0;            // Сдвиг по X начальной точки для рисования символа
    this.nY = 0;            // Сдвиг по Y начальной точки для рисования символа
    this.nWidth = 0;        // Ширина символа
    this.nHeight = 0;       // Высота символа
    this.bAA = false;       // Anti-aliased: True означает, что Bitmap 8-битный(т.е. с альфой); False - Bitmap 1-битный
    this.pData = null;      // Bitmap data(картинка с символом)
    this.bFreeData = false; // True, если память в pData нужно освободить

    this.oGlyphData = new CGlyphData();

    // пока сделаю, чтобы работала и обычная альфа маска. Чтобы тестировать.
    // если все будет хорошо - то надо убрать заполнение this.pData
    this.fromAlphaMask = function()
    {
        if (null == this.pData)
            return;
			
        this.oGlyphData.init(this.nWidth,this.nHeight);
		var nIndexSrc = 0;
        var nIndexDst = 3;
        var nPitch = 4 * (g_staticImageDataW - this.nWidth);
        for (var j = 0; j < this.nHeight; j++)
        {
            for (var i = 0; i < this.nWidth; i++)
            {
                g_staticImageDataPixels[nIndexDst] = this.pData[nIndexSrc];
                nIndexSrc++;
                nIndexDst += 4;
            }
            nIndexDst += nPitch;
        }
        this.oGlyphData.m_oContext.putImageData(g_staticImageData,0,0,0,0,this.nWidth,this.nHeight);
    }
};

function TMetrics()
{
    this.fWidth = 0;
    this.fHeight = 0;

    this.fHoriBearingX = 0;
    this.fHoriBearingY = 0;
    this.fHoriAdvance = 0;

    this.fVertBearingX = 0;
    this.fVertBearingY = 0;
    this.fVertAdvance = 0;
};

function TGlyph()
{
	this.lUnicode = 0; // Юникод
	this.fX = 0;       // Позиция глифа
	this.fY = 0;       // на BaseLine

	this.fLeft = 0;    //
	this.fTop = 0;     // BBox
	this.fRight = 0;   //
	this.fBottom = 0;  //
	
	this.oMetrics = null;//new TMetrics();

	this.eState = EGlyphState.glyphstateNormal;

	this.bBitmap = false;
	this.oBitmap = null;//new TGlyphBitmap();
	
	this.Clear = function()
	{
		this.bBitmap = false;
		this.eState = EGlyphState.glyphstateNormal;
	}
};

function TBBox()
{ 
	this.fMinX;
	this.fMaxX;
	this.fMinY;
	this.fMaxY;
};

function TFontCacheSizes()
{
	this.ushUnicode; // Значение символа в юникоде
	this.eState;     // Есть ли символ в шрифте/стандартном шрифте
	this.nCMapIndex; // Номер таблицы 'cmap', в которой был найден данный символ 

	this.ushGID;     

	this.fAdvanceX; 

	this.oBBox = new TBBox();
	this.oMetrics = new TMetrics();

	this.bBitmap;
	this.oBitmap = new TGlyphBitmap();
};

function CGlyphString()
{
	this.m_fX = 0;
	this.m_fY = 0;

	this.m_fEndX = 0;
	this.m_fEndY = 0;

	this.m_nGlyphIndex   = -1;
	this.m_nGlyphsCount  = 0;
	this.m_pGlyphsBuffer = new Array(100);//null;

	this.m_arrCTM = new Array();
	this.m_dIDet = 1;

	this.m_fTransX = 0;
	this.m_fTransY = 0;
	
	this.SetString = function(wsString, fX, fY)
	{
		this.m_fX = fX + this.m_fTransX;
		this.m_fY = fY + this.m_fTransY;

		this.m_nGlyphsCount = wsString.length;
		this.m_nGlyphIndex  = 0;
		
		//this.m_pGlyphsBuffer = new Array(this.m_nGlyphsCount);

		for (var nIndex = 0; nIndex < this.m_nGlyphsCount; ++nIndex)
		{
			if (undefined == this.m_pGlyphsBuffer[nIndex])
				this.m_pGlyphsBuffer[nIndex] = new TGlyph();
			else
				this.m_pGlyphsBuffer[nIndex].Clear();
			this.m_pGlyphsBuffer[nIndex].lUnicode = wsString.charCodeAt(nIndex);
			this.m_pGlyphsBuffer[nIndex].bBitmap  = false;
		}
	}
	this.SetStringGID = function(gid, fX, fY)
	{
		this.m_fX = fX + this.m_fTransX;
		this.m_fY = fY + this.m_fTransY;

		this.m_nGlyphsCount = 1;
		this.m_nGlyphIndex  = 0;
		
		//this.m_pGlyphsBuffer = new Array(this.m_nGlyphsCount);

		for (var nIndex = 0; nIndex < this.m_nGlyphsCount; ++nIndex)
		{
			if (undefined == this.m_pGlyphsBuffer[nIndex])
				this.m_pGlyphsBuffer[nIndex] = new TGlyph();
			else
				this.m_pGlyphsBuffer[nIndex].Clear();
			this.m_pGlyphsBuffer[nIndex].lUnicode = gid;
			this.m_pGlyphsBuffer[nIndex].bBitmap  = false;
		}
	}
	
	this.GetLength = function()
	{
		return this.m_nGlyphsCount;
	}
	
	this.GetAt = function(nIndex)
	{
		if (this.m_nGlyphsCount <= 0)
			return null;

		var nCurIndex = Math.min (this.m_nGlyphsCount - 1, Math.max(0, nIndex));

		return this.m_pGlyphsBuffer[nCurIndex];
	}
	
	this.SetStartPoint = function(nIndex, fX, fY)
	{
		if (this.m_nGlyphsCount <= 0)
			return;

		var nCurIndex = Math.min (this.m_nGlyphsCount - 1, Math.max(0, nIndex));

		this.m_pGlyphsBuffer[nCurIndex].fX = fX;
		this.m_pGlyphsBuffer[nCurIndex].fY = fY;
	}
	
	this.SetState = function(nIndex, eState)
	{
		if (this.m_nGlyphsCount <= 0)
			return;

		var nCurIndex = Math.min (this.m_nGlyphsCount - 1, Math.max(0, nIndex));

		this.m_pGlyphsBuffer[nCurIndex].eState = eState;
	}
	
	this.SetBBox = function (nIndex, fLeft, fTop, fRight, fBottom)
	{
		if (this.m_nGlyphsCount <= 0)
			return;

		var nCurIndex = Math.min (this.m_nGlyphsCount - 1, Math.max (0, nIndex));

		this.m_pGlyphsBuffer[nCurIndex].fLeft   = fLeft;
		this.m_pGlyphsBuffer[nCurIndex].fTop    = fTop;
		this.m_pGlyphsBuffer[nCurIndex].fRight  = fRight;
		this.m_pGlyphsBuffer[nCurIndex].fBottom = fBottom;
	}
	
	this.SetMetrics = function (nIndex, fWidth, fHeight, fHoriAdvance, fHoriBearingX, fHoriBearingY, fVertAdvance, fVertBearingX, fVertBearingY)
	{
		if (this.m_nGlyphsCount <= 0)
			return;

		var nCurIndex = Math.min (this.m_nGlyphsCount - 1, Math.max (0, nIndex));

		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fHeight       = fHeight;
		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fHoriAdvance  = fHoriAdvance;
		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fHoriBearingX = fHoriBearingX;
		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fHoriBearingY = fHoriBearingY;
		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fVertAdvance  = fVertAdvance;
		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fVertBearingX = fVertBearingX;
		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fVertBearingY = fVertBearingY;
		this.m_pGlyphsBuffer[nCurIndex].oMetrics.fWidth        = fWidth;
	}
	
	this.ResetCTM = function()
	{
		this.m_arrCTM[0] = 1;
		this.m_arrCTM[1] = 0;
		this.m_arrCTM[2] = 0;
		this.m_arrCTM[3] = 1;
		this.m_arrCTM[4] = 0;
		this.m_arrCTM[5] = 0;

		this.m_dIDet      = 1;
	}
	
	this.GetBBox = function(nIndex, nType)
	{
		var oPoint = new CPoint2();
		if (typeof nIndex == "undefined")
			nIndex = -1;
		if (typeof nType == "undefined")
			nType = 0;
			
		var nCurIndex = 0;
		if (nIndex < 0)
		{
			if (this.m_nGlyphsCount <= 0 || this.m_nGlyphIndex < 1 || this.m_nGlyphIndex > this.m_nGlyphsCount )
				return oPoint;

			nCurIndex = this.m_nGlyphIndex - 1;
		}
		else
		{
			if (this.m_nGlyphsCount <= 0)
				return oPoint;

			nCurIndex = Math.min(this.m_nGlyphsCount - 1, Math.max(0, nIndex));
		}

		var fBottom = -this.m_pGlyphsBuffer[nCurIndex].fBottom;
		var fRight  =  this.m_pGlyphsBuffer[nCurIndex].fRight;
		var fLeft   =  this.m_pGlyphsBuffer[nCurIndex].fLeft;
		var fTop    = -this.m_pGlyphsBuffer[nCurIndex].fTop;


		if (0 == nType && !(1 == this.m_arrCTM[0] && 0 == this.m_arrCTM[1] && 0 == this.m_arrCTM[2] && 1 == this.m_arrCTM[3] && 0 == this.m_arrCTM[4] && 0 == this.m_arrCTM[5]))
		{
			// Применяем глобальную матрицу преобразования и пересчитываем BBox
			var arrfX =[fLeft, fLeft, fRight, fRight];
			var arrfY = [fTop, fBottom, fBottom, fTop];

			var fMinX = (arrfX[0] * this.m_arrCTM[0] + arrfY[0] * this.m_arrCTM[2]);
			var fMinY = (arrfX[0] * this.m_arrCTM[1] + arrfY[0] * this.m_arrCTM[3]);
			var fMaxX = fMinX;
			var fMaxY = fMinY;
			
			for (var nIndex = 1; nIndex < 4; ++nIndex)
			{
				var fX = (arrfX[nIndex] * this.m_arrCTM[0] + arrfY[nIndex] * this.m_arrCTM[2]);
				var fY = (arrfX[nIndex] * this.m_arrCTM[1] + arrfY[nIndex] * this.m_arrCTM[3]);

				fMaxX = Math.max(fMaxX, fX);
				fMinX = Math.min(fMinX, fX);

				fMaxY = Math.max(fMaxY, fY);
				fMinY = Math.min(fMinY, fY);
			}

			fLeft   = fMinX;
			fRight  = fMaxX;
			fTop    = fMinY;
			fBottom = fMaxY;
		}

		oPoint.fLeft   = fLeft   + this.m_pGlyphsBuffer[nCurIndex].fX + this.m_fX;
		oPoint.fRight  = fRight  + this.m_pGlyphsBuffer[nCurIndex].fX + this.m_fX;
		oPoint.fTop    = fTop    + this.m_pGlyphsBuffer[nCurIndex].fY + this.m_fY;
		oPoint.fBottom = fBottom + this.m_pGlyphsBuffer[nCurIndex].fY + this.m_fY;
		
		return oPoint;
	}
	this.GetBBox2 = function()
	{
		var oPoint = new CPoint2();
		if (this.m_nGlyphsCount <= 0)
			return oPoint;

		var fBottom = 0;
		var fRight  = 0;
		var fLeft   = 0;
		var fTop    = 0;

		for (var nIndex = 0; nIndex < this.m_nGlyphsCount; ++nIndex)
		{
			fBottom = Math.max (fBottom, -this.m_pGlyphsBuffer[nIndex].fBottom);
			fTop    = Math.min (fTop, -this.m_pGlyphsBuffer[nIndex].fTop);
		}

		if (!(1 == this.m_arrCTM[0] && 0 == this.m_arrCTM[1] && 0 == this.m_arrCTM[2] && 1 == this.m_arrCTM[3] && 0 == this.m_arrCTM[4] && 0 == this.m_arrCTM[5]))
		{
			// Применяем глобальную матрицу преобразования и пересчитываем BBox
			var arrfX = [fLeft, fLeft, fRight, fRight];
			var arrfY = [fTop, fBottom, fBottom, fTop];

			var fMinX = (arrfX[0] * this.m_arrCTM[0] + arrfY[0] * this.m_arrCTM[2]);
			var fMinY = (arrfX[0] * this.m_arrCTM[1] + arrfY[0] * this.m_arrCTM[3]);
			var fMaxX = fMinX;
			var fMaxY = fMinY;
			
			for (var nIndex = 1; nIndex < 4; ++nIndex)
			{
				var fX = (arrfX[nIndex] * this.m_arrCTM[0] + arrfY[nIndex] * this.m_arrCTM[2]);
				var fY = (arrfX[nIndex] * this.m_arrCTM[1] + arrfY[nIndex] * this.m_arrCTM[3]);

				fMaxX = Math.max (fMaxX, fX);
				fMinX = Math.min (fMinX, fX);

				fMaxY = Math.max (fMaxY, fY);
				fMinY = Math.min (fMinY, fY);
			}

			fLeft   = fMinX;
			fRight  = fMaxX;
			fTop    = fMinY;
			fBottom = fMaxY;
		}

		fLeft   += this.m_fX;
		fRight  += this.m_fX;
		fTop    += this.m_fY;
		fBottom += this.m_fY; 

		oPoint.fLeft  = Math.min (fLeft, Math.min (this.m_fX, this.m_fEndX));
		oPoint.fRight = Math.max (fRight, Math.max (this.m_fX, this.m_fEndX));
		oPoint.fTop   = Math.min (fTop, Math.min (this.m_fY, this.m_fEndY));
		oPoint.fBottom = Math.max (fBottom, Math.max (this.m_fY, this.m_fEndY));
		
		return oPoint;
	}
	
	this.GetNext = function()
	{
		if (this.m_nGlyphIndex >= this.m_nGlyphsCount || this.m_nGlyphIndex < 0)
			return undefined;

		return this.m_pGlyphsBuffer[this.m_nGlyphIndex++];
	}
	
	this.SetTrans = function(fX, fY)
	{
		this.m_fTransX = this.m_dIDet * (fX * this.m_arrCTM[3] - this.m_arrCTM[2] * fY);
		this.m_fTransY = this.m_dIDet * (fY * this.m_arrCTM[0] - this.m_arrCTM[1] * fX);
	}
	
	this.SetCTM = function(fA, fB, fC, fD, fE , fF)
	{
		this.m_arrCTM[0] = fA;
		this.m_arrCTM[1] = fB;
		this.m_arrCTM[2] = fC;
		this.m_arrCTM[3] = fD;
		this.m_arrCTM[4] = fE;
		this.m_arrCTM[5] = fF;

		var dDet = fA * fD - fB * fC;

		if (dDet < 0.001 && dDet >= 0)
			dDet =  0.001;
		else if (dDet > - 0.001 && dDet < 0)
			dDet = -0.001;

		this.m_dIDet = 1 / dDet;
	}
};

function DrawingCanvas(id)
{
	this.id = id;
	this.canvas;
	this.ctx;

	this.startX = 0;
	this.startY = 12;

	this.width;
	this.height;

	this.m_oFontManager = null;

	this.init = function()
	{
		this.createContext();
		this.clear();

		this.m_oFontManager = new CFontManager();
		this.m_oFontManager.Initialize();
	}

	this.loadFont = function(fontName, fontSize, unHorDpi, unVerDpi)
	{
		return this.m_oFontManager.LoadFontFromFile(fontName, fontSize, unHorDpi, unVerDpi);
	}
    this.loadFont2 = function(fontName, fontSize, lStyle, unHorDpi, unVerDpi)
    {
        return this.m_oFontManager.LoadFontByName(fontName, fontSize, lStyle, unHorDpi, unVerDpi);
    }

    this.SetDefaultFont = function(sFamilyName)
    {
        this.m_oFontManager.SetDefaultFont(sFamilyName);
    }

	this.LoadString = function(wsBuffer, fX, fY)
	{
		this.m_oFontManager.LoadString(wsBuffer, fX, fY);
	}
	this.LoadString2 = function(wsBuffer, fX, fY)
	{
		this.m_oFontManager.LoadString2(wsBuffer, fX, fY);
	}
	this.MeasureString = function()
	{
		return this.m_oFontManager.MeasureString();
	}
	this.MeasureString2 = function()
	{
		return this.m_oFontManager.MeasureString2();
	}
    this.MeasureChar = function(lUnicode)
    {
        return this.m_oFontManager.MeasureChar(lUnicode);
    }

	this.createContext = function()
	{
		this.canvas = document.getElementById(this.id);
		this.ctx = this.canvas.getContext('2d');

		this.width = this.canvas.width;
		this.height = this.canvas.height;
	}
	this.createBuffer = function(width, height)
	{
		return this.ctx.createImageData(width, height);
	}
	this.drawBuffer = function(elem, x, y)
	{
		var widthChar  = elem.oBitmap.nWidth;
		var heightChar = elem.oBitmap.nHeight;

		var pBuffer = this.createBuffer (widthChar, heightChar);
		for (var nY = 0, base = 0, base2 = 0; nY < heightChar; ++nY)
        {
			for (var nX = 0; nX < widthChar; ++nX, base += 4, base2++)
            {
				var byteObj = 255 - elem.oBitmap.pData[base2];
                if ( 255 != byteObj )
                {
                    pBuffer.data[base + 0] = byteObj;
                    pBuffer.data[base + 1] = byteObj;
                    pBuffer.data[base + 2] = byteObj;
                    pBuffer.data[base + 3] = 255;
                }
			}
		}
        this.ctx.putImageData(pBuffer, x + elem.oBitmap.nX, y - elem.oBitmap.nY);
	}

	this.clear = function()
	{
		this.startX = 0;
		this.startY = 12;

		var image = this.ctx.getImageData(0, 0, this.width, this.height);

		for (var y = 0; y < this.height; y++) {
		  for (var x = 0; x < this.width; x++) {
			var base = (y*this.width + x)*4;
			image.data[base + 0] = 0;
			image.data[base + 1] = 0;
			image.data[base + 2] = 0;
			image.data[base + 3] = 0;
		  }
		}

		this.ctx.putImageData(image, 0, 0);
	}

	this.drawChar = function(charVal, x, y)
	{
        //var pGlyph = this.m_oFontManager.LoadChar( charVal );

		this.m_oFontManager.LoadString2 (charVal, x, y);
		var pGlyph = this.m_oFontManager.GetNextChar2();

		this.drawBuffer (pGlyph, x, y);
	}
};

function cacheSymbols()
{
	this.arrayCache = new Array();
	
	this.add = function(elem)
	{
		if (undefined == this.arrayCache[elem.fontName])
			this.arrayCache[elem.fontName] = elem;
	}
	this.getFont = function(fontName, nameElem)
	{
		var cacheFontObj = this.arrayCache[fontName];
		if (cacheFontObj)
			return cacheFontObj.get (nameElem);
		
		return undefined;
	}
};
function cacheFont(fontName)
{
	this.fontFace = null;
	this.fontName = fontName;
	this.arraySymbols = new Array();
	
	this.add = function(nameElem, elem)
	{
		this.arraySymbols[nameElem] = elem;
	}
	this.get = function(nameElem)
	{
		return this.arraySymbols[nameElem];
	}
};

function CFontEngine()
{
	this.m_pFreeTypeEngine = null;
	this.m_arrFontFiles = new Array();
    this.m_arrDefaultFont = new Array(); // { Regular, Bold, Italic, Bold Italic}
	
	this.Initialize = function()
	{
		this.m_pFreeTypeEngine = new CFreeTypeFontEngine();
		return this.m_pFreeTypeEngine.Initialize();
	}
	this.LoadFont = function(fontName, fontSize, bBold, bItalic)
	{
        var fontNameBI = fontName + ((bBold & bItalic)?"bi":((bBold)?"bd":(bItalic)?"i":""));
		var pFontFile = this.m_arrFontFiles[fontNameBI + fontSize];
		if (pFontFile)
			return pFontFile;
			
		if (this.m_pFreeTypeEngine)
		{
			pFontFile = this.m_arrFontFiles[fontNameBI + fontSize] = this.m_pFreeTypeEngine.LoadFont(fontNameBI);
            if (!pFontFile && fontNameBI != fontName)
                pFontFile = this.m_arrFontFiles[fontNameBI + fontSize] = this.m_pFreeTypeEngine.LoadFont(fontName);
		}
		
		return pFontFile;
	}

    this.SetDefaultFont = function(sFamilyName)
    {
        this.m_arrDefaultFont[0] = this.m_pFreeTypeEngine.LoadFont(sFamilyName);
        this.m_arrDefaultFont[0].UpdateStyles(false, false);
        this.m_arrDefaultFont[1] = this.m_pFreeTypeEngine.LoadFont(sFamilyName + "bd");
        this.m_arrDefaultFont[1].UpdateStyles(true, false);
        this.m_arrDefaultFont[2] = this.m_pFreeTypeEngine.LoadFont(sFamilyName +  "i");
        this.m_arrDefaultFont[2].UpdateStyles(false, true);
        this.m_arrDefaultFont[3] = this.m_pFreeTypeEngine.LoadFont(sFamilyName + "bi");
        this.m_arrDefaultFont[3].UpdateStyles(true, true);
    }

    this.GetDefaultFont = function(bBold, bItalic)
    {
        var nIndex = (bBold ? 1 : 0) + (bItalic ? 2 : 0);
        return this.m_arrDefaultFont[nIndex];
    }
};

function CFontManager()
{
	this.m_pFontEngine = null;
	
	this.m_pFont = null;
	this.m_oGlyphString = null;
	
	this.fontFace = null;
	this.error = 0;
	
	this.cacheSymbolsObj = new cacheSymbols();
	this.fontName = undefined;

    this.m_bUseDefaultFont = false;
    this.m_fCharSpacing = 0.0;
    this.m_bStringGID = false;
	
	this.Initialize = function()
	{
		this.m_oGlyphString = new CGlyphString();
		this.m_pFontEngine = new CFontEngine();
		return this.m_pFontEngine.Initialize();
	}
	
	this.LoadFontFromFile = function(fontName, fEmSize, dHorDpi, dVerDpi)
	{
		if (!this.m_pFontEngine)
			return false;
		
		var pFontFile = this.m_pFontEngine.LoadFont(fontName, fEmSize, false, false);
		if (!pFontFile)
			return false;
			
		this.m_pFont = pFontFile;

        this.m_pFont.UpdateStyles(false, false);
        this.m_pFont.SetDefaultFont(this.m_pFontEngine.GetDefaultFont(false, false));
		fEmSize = this.UpdateSize(fEmSize, dVerDpi, dVerDpi);
		this.m_pFont.SetSizeAndDpi(fEmSize, dHorDpi, dVerDpi);

        this.m_pFont.SetStringGID(this.m_bStringGID);
        this.m_pFont.SetUseDefaultFont(this.m_bUseDefaultFont);
        this.m_pFont.SetCharSpacing(this.m_fCharSpacing);
		
		this.m_oGlyphString.ResetCTM();
		this.m_pFont.SetTextMatrix (1, 0, 0, 1, 0, 0);
	}

    this.LoadFontByName = function(bsFamilyName, fEmSize, lStyle, dHorDpi, dVerDpi)
    {
        if (!this.m_pFontEngine)
            return false;

        var bBold   = false;
        var bItalic = false;

        if (lStyle & FontStyle.FontStyleBold)
            bBold = true;

        if (lStyle & FontStyle.FontStyleItalic)
            bItalic = true;

        var pFontFile = this.m_pFontEngine.LoadFont(bsFamilyName, fEmSize, bBold, bItalic);

        if (!pFontFile)
            pFontFile = this.m_pFontEngine.GetDefaultFont (bBold, bItalic);
        else
            pFontFile.SetDefaultFont (this.m_pFontEngine.GetDefaultFont (bBold, bItalic));

        if (!pFontFile)
            return false;

        this.m_pFont = pFontFile;

        this.m_pFont.UpdateStyles(bBold, bItalic);
        fEmSize = this.UpdateSize(fEmSize, dVerDpi, dVerDpi);
        this.m_pFont.SetSizeAndDpi(fEmSize, dHorDpi, dVerDpi);

        this.m_pFont.SetStringGID(this.m_bStringGID);
        this.m_pFont.SetUseDefaultFont(this.m_bUseDefaultFont);
        this.m_pFont.SetCharSpacing(this.m_fCharSpacing);

        this.m_oGlyphString.ResetCTM();
        this.m_pFont.SetTextMatrix (1, 0, 0, 1, 0, 0);
    }

    this.SetDefaultFont = function(sFamilyName)
    {
        if (!this.m_pFontEngine)
            return false;

        this.m_pFontEngine.SetDefaultFont(sFamilyName);
    }
	
	this.UpdateSize = function(dOldSize, dDpi, dNewDpi)
	{
		if (0 == dNewDpi)
			dNewDpi = 72.0;
		if (0 == dDpi)
			dDpi = 72.0;

		return dOldSize * dDpi / dNewDpi;
	}
	
	this.LoadString = function(wsBuffer, fX, fY)
	{
		if (!this.m_pFont)
			return false;
		
		this.m_oGlyphString.SetString (wsBuffer, fX, fY);
		this.m_pFont.GetString(this.m_oGlyphString);
		
		return true;
	}
	
	this.LoadString2 = function(wsBuffer, fX, fY)
	{
		if (!this.m_pFont)
			return false;

		this.m_oGlyphString.SetString (wsBuffer, fX, fY);
		this.m_pFont.GetString2(this.m_oGlyphString);
		
		return true;
	}
	this.LoadString3 = function(gid, fX, fY)
	{
		if (!this.m_pFont)
			return false;

		this.SetStringGID(true);
		this.m_oGlyphString.SetStringGID (gid, fX, fY);
		this.m_pFont.GetString2(this.m_oGlyphString);
		this.SetStringGID(false);
		
		return true;
	}

    this.LoadChar = function(lUnicode)
    {
        if (!this.m_pFont)
            return false;

        return this.m_pFont.GetChar2(lUnicode);
    }

    this.MeasureChar = function(lUnicode)
    {
        if (!this.m_pFont)
            return;

        return this.m_pFont.GetChar(lUnicode);
    }

    this.GetKerning = function(unPrevGID, unGID)
    {
        if (!this.m_pFont)
            return;

        return this.m_pFont.GetKerning(unPrevGID, unGID);
    }

	this.MeasureString = function()
	{
		var oPoint = new CPoint1();
		
		if (this.m_oGlyphString.GetLength() <= 0)
			return oPoint;

		var fTop = 0xFFFF, fBottom = -0xFFFF, fLeft = 0xFFFF, fRight = -0xFFFF;
		for (var nIndex = 0; nIndex < this.m_oGlyphString.GetLength(); ++nIndex)
		{
			var oSizeTmp = this.m_oGlyphString.GetBBox (nIndex);

			if (fBottom < oSizeTmp.fBottom)
				fBottom = oSizeTmp.fBottom;

			if (fTop > oSizeTmp.fTop)
				fTop = oSizeTmp.fTop;

			if (fLeft > oSizeTmp.fLeft)
				fLeft = oSizeTmp.fLeft;

			if (fRight < oSizeTmp.fRight)
				fRight = oSizeTmp.fRight;
		}

		oPoint.fX = fLeft;
		oPoint.fY = fTop;

		oPoint.fWidth  = Math.abs ((fRight - fLeft));
		oPoint.fHeight = Math.abs ((fTop - fBottom));
		
		return oPoint;
	}
	
	this.MeasureString2 = function()
	{
		var oPoint = new CPoint1();
		
		if (this.m_oGlyphString.GetLength() <= 0)
			return oPoint;

		var fTop = 0xFFFF, fBottom = -0xFFFF, fLeft = 0xFFFF, fRight = -0xFFFF;

		var oSizeTmp = this.m_oGlyphString.GetBBox2 ();

		oPoint.fX = oSizeTmp.fLeft;
		oPoint.fY = oSizeTmp.fTop;

		oPoint.fWidth  = Math.abs ((oSizeTmp.fRight - oSizeTmp.fLeft));
		oPoint.fHeight = Math.abs ((oSizeTmp.fTop - oSizeTmp.fBottom));
		
		return oPoint;
	}
	
	this.GetNextChar2 = function()
	{
		return this.m_oGlyphString.GetNext();
	}
	
	this.ReleaseFontFace = function()
	{
		if (null != this.fontFace)
			_FT_Done_Face(HEAP[this.fontFace]);
	}
	
	this.ReleaseFontFaceAndLibrary = function()
	{
		this.fontFace=HEAP[this.library];
		if(this.fontFace!=0){
			this.library=HEAP[this.fontFace];
			_FT_Done_Library(this.fontFace);
			FUNCTION_TABLE[HEAP[this.library+8]](this.library,this.library)
		}
		
		STACKTOP=this.firstStackTop;
	}
	
	this.CloseFreeType = function()
	{
		__shutdownRuntime__();
	}
	
	this.IsSuccess = function()
	{
		return (0 == this.error);
	}

    this.SetTextMatrix = function(fA, fB, fC, fD, fE, fF)
    {
        if (!this.m_pFont)
            return false;

        if (this.m_pFont.SetTextMatrix (fA, fB, fC, fD, 0, 0))
			this.m_oGlyphString.SetCTM (fA, fB, fC, fD, 0, 0);
		this.m_oGlyphString.SetTrans (fE, fF);

        return true;
    }
	
	this.SetStringGID = function(bStringGID)
	{
		this.m_bStringGID = bStringGID;

		if (!this.m_pFont)
			return;

		this.m_pFont.SetStringGID (this.m_bStringGID);
	}
};