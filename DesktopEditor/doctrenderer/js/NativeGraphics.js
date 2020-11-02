
function CNativeGraphics()
{
    this.Native = CreateNativeGraphics();

    this.m_oContext   = null;
    this.m_dWidthMM   = 0;
    this.m_dHeightMM  = 0;
    this.m_lWidthPix  = 0;
    this.m_lHeightPix = 0;
    this.m_dDpiX      = 96.0;
    this.m_dDpiY      = 96.0;
    this.m_bIsBreak   = false;

    //this.m_oPen            = new AscCommon.CPen();
    this.m_bPenColorInit   = false;
    //this.m_oBrush          = new AscCommon.CBrush();
    this.m_bBrushColorInit = false;

    this.m_oFontManager = null;

    //this.m_oCoordTransform      = new AscCommon.CMatrixL();
    //this.m_oBaseTransform       = new AscCommon.CMatrixL();
    //this.m_oTransform           = new AscCommon.CMatrixL();
    //this.m_oFullTransform       = new AscCommon.CMatrixL();
    //this.m_oInvertFullTransform = new AscCommon.CMatrixL();

    this.ArrayPoints = null;

    this.m_oCurFont =
    {
        Name     : "",
        FontSize : 10,
        Bold     : false,
        Italic   : false
    };

    // RFonts
    this.m_oTextPr      = null;
    //this.m_oGrFonts     = new AscCommon.CGrRFonts();
    //this.m_oLastFont    = new AscCommon.CFontSetup();

    this.LastFontOriginInfo = { Name : "", Replace : null };

    this.m_bIntegerGrid = true;

    //this.ClipManager = new AscCommon.CClipManager();
    //this.ClipManager.BaseObject = this;

    this.TextureFillTransformScaleX = 1;
    this.TextureFillTransformScaleY = 1;
    this.IsThumbnail = false;

    this.IsDemonstrationMode = false;

    //this.GrState = new AscCommon.CGrState();
    //this.GrState.Parent = this;

    this.TextClipRect  = null;
    this.IsClipContext = false;

    this.IsUseFonts2     = false;
    this.m_oFontManager2 = null;
    this.m_oLastFont2    = null;

    this.ClearMode = false;
    this.IsRetina  = false;

    this.dash_no_smart = null;
}

CNativeGraphics.prototype =
{
    init : function(context, width_px, height_px, width_mm, height_mm)
    {
        this.Native["init"](width_px, height_px, width_mm, height_mm);
    },
    EndDraw : function()
    {
        // this.Native["EndDraw"]();
    },
    put_GlobalAlpha : function(enable, alpha)
    {
        this.Native["put_GlobalAlpha"](enable, alpha);
    },
    Start_GlobalAlpha : function()
    {
        // this.Native["Start_GlobalAlpha"]();
    },
    End_GlobalAlpha : function()
    {
        this.Native["End_GlobalAlpha"]();
    },
    // pen methods
    p_color : function(r, g, b, a)
    {
        this.Native["p_color"](r, g, b, a);
    },
    p_width : function(w)
    {
        this.Native["p_width"](w);
    },
    p_dash : function(params)
    {
        this.Native["p_dash"](params ? params : []);
    },
    // brush methods
    b_color1 : function(r, g, b, a)
    {
        this.Native["b_color1"](r, g, b, a);
    },
    b_color2 : function(r, g, b, a)
    {
        this.Native["b_color2"](r, g, b, a);
    },
    transform : function(sx, shy, shx, sy, tx, ty)
    {
        this.Native["transform"](sx, shy, shx, sy, tx, ty);
    },
    CalculateFullTransform : function(isInvertNeed)
    {
        this.Native["CalculateFullTransform"](isInvertNeed);
    },
    // path commands
    _s : function()
    {
        this.Native["_s"]();
    },
    _e : function()
    {
        this.Native["_e"]();
    },
    _z : function()
    {
        this.Native["_z"]();
    },
    _m : function(x, y)
    {
        this.Native["_m"](x, y);
    },
    _l : function(x, y)
    {
        this.Native["_l"](x, y);
    },
    _c : function(x1, y1, x2, y2, x3, y3)
    {
        this.Native["_c"](x1, y1, x2, y2, x3, y3);
    },
    _c2 : function(x1, y1, x2, y2)
    {
        this.Native["_c2"](x1, y1, x2, y2);
    },
    ds : function()
    {
        this.Native["ds"]();
    },
    df : function()
    {
        this.Native["df"]();
    },
    // canvas state
    save : function()
    {
        this.Native["save"]();
    },
    restore : function()
    {
        this.Native["restore"]();
    },
    clip : function()
    {
        this.Native["clip"]();
    },
    reset : function()
    {
        this.Native["reset"]();
    },
    transform3 : function(m, isNeedInvert)
    {
        this.Native["transform3"](m.sx, m.shy, m.shx, m.sy, m.tx, m.ty, isNeedInvert);
    },
    FreeFont : function()
    {
        this.Native["FreeFont"]();
    },
    ClearLastFont : function()
    {
        this.Native["ClearLastFont"]();
    },
    // images
    drawImage2 : function(img, x, y, w, h, alpha, srcRect)
    {
        this.Native["drawImage2"](img, x, y, w, h, alpha, srcRect);
    },
    drawImage : function(img, x, y, w, h, alpha, srcRect, nativeImage)
    {
        this.Native["drawImage"](img, x, y, w, h, alpha, srcRect, nativeImage);
    },
    // text
    GetFont : function()
    {
        return this.Native["GetFont"]();
    },
    font : function(font_id, font_size)
    {
        this.Native["font"](font_id, font_size);
    },
    SetFont : function(font)
    {
        if (null == font)
            return;

        var flag = 0;
        if (font.Bold)    flag |= 0x01;
        if (font.Italic)  flag |= 0x02;
        if (font.Bold)    flag |= 0x04;
        if (font.Italic)  flag |= 0x08;

        this.Native["SetFont"](font.FontFamily.Name, font.FontFamily.Index, font.FontSize, flag);
    },
    SetTextPr : function(textPr, theme)
    {
        this.m_oTextPr = textPr;
        if (theme)
            this.m_oGrFonts.checkFromTheme(theme.themeElements.fontScheme, this.m_oTextPr.RFonts);
        else
            this.m_oGrFonts = this.m_oTextPr.RFonts;
        // this.Native["SetTextPr"](textPr, theme);
    },
    SetFontSlot : function(slot, fontSizeKoef)
    {
        var _rfonts   = this.m_oGrFonts;
        var _lastFont = this.IsUseFonts2 ? this.m_oLastFont2 : this.m_oLastFont;

        switch (slot)
        {
            case fontslot_ASCII:
            {
                _lastFont.Name   = _rfonts.Ascii.Name;
                _lastFont.Size   = this.m_oTextPr.FontSize;
                _lastFont.Bold   = this.m_oTextPr.Bold;
                _lastFont.Italic = this.m_oTextPr.Italic;

                break;
            }
            case fontslot_CS:
            {
                _lastFont.Name   = _rfonts.CS.Name;
                _lastFont.Size   = this.m_oTextPr.FontSizeCS;
                _lastFont.Bold   = this.m_oTextPr.BoldCS;
                _lastFont.Italic = this.m_oTextPr.ItalicCS;

                break;
            }
            case fontslot_EastAsia:
            {
                _lastFont.Name   = _rfonts.EastAsia.Name;
                _lastFont.Size   = this.m_oTextPr.FontSize;
                _lastFont.Bold   = this.m_oTextPr.Bold;
                _lastFont.Italic = this.m_oTextPr.Italic;

                break;
            }
            case fontslot_HAnsi:
            default:
            {
                _lastFont.Name   = _rfonts.HAnsi.Name;
                _lastFont.Size   = this.m_oTextPr.FontSize;
                _lastFont.Bold   = this.m_oTextPr.Bold;
                _lastFont.Italic = this.m_oTextPr.Italic;

                break;
            }
        }

        if (undefined !== fontSizeKoef)
            _lastFont.Size *= fontSizeKoef;

        var _style = 0;
        if (_lastFont.Italic)
            _style += 2;
        if (_lastFont.Bold)
            _style += 1;

        var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

        if (_lastFont.Name != _lastFont.SetUpName || _lastFont.Size != _lastFont.SetUpSize || _style != _lastFont.SetUpStyle || !_font_manager.m_pFont)
        {
            _lastFont.SetUpName  = _lastFont.Name;
            _lastFont.SetUpSize  = _lastFont.Size;
            _lastFont.SetUpStyle = _style;

            var _fontinfo = AscFonts.g_fontApplication.GetFontInfo(_lastFont.SetUpName, _lastFont.SetUpStyle, this.LastFontOriginInfo);

            // подбираем шрифт по стилю
            var bNeedBold   = false;
            var bNeedItalic = false;
            var index       = -1;
            var faceIndex   = 0;
            var bSrcItalic  = false;
            var bSrcBold    = false;
        
            switch (_lastFont.SetUpStyle)
            {
                case AscFonts.FontStyle.FontStyleBoldItalic:
                {
                    bSrcItalic  = true;
                    bSrcBold    = true;
        
                    bNeedBold   = true;
                    bNeedItalic = true;
                    if (-1 != _fontinfo.indexBI)
                    {
                        index = _fontinfo.indexBI;
                        faceIndex = _fontinfo.faceIndexBI;
                        bNeedBold   = false;
                        bNeedItalic = false;
                    }
                    else if (-1 != _fontinfo.indexB)
                    {
                        index = _fontinfo.indexB;
                        faceIndex = _fontinfo.faceIndexB;
                        bNeedBold = false;
                    }
                    else if (-1 != _fontinfo.indexI)
                    {
                        index = _fontinfo.indexI;
                        faceIndex = _fontinfo.faceIndexI;
                        bNeedItalic = false;
                    }
                    else
                    {
                        index = _fontinfo.indexR;
                        faceIndex = _fontinfo.faceIndexR;
                    }
                    break;
                }
                case AscFonts.FontStyle.FontStyleBold:
                {
                    bSrcBold    = true;
        
                    bNeedBold   = true;
                    bNeedItalic = false;
                    if (-1 != _fontinfo.indexB)
                    {
                        index = _fontinfo.indexB;
                        faceIndex = _fontinfo.faceIndexB;
                        bNeedBold = false;
                    }
                    else if (-1 != _fontinfo.indexR)
                    {
                        index = _fontinfo.indexR;
                        faceIndex = _fontinfo.faceIndexR;
                    }
                    else if (-1 != _fontinfo.indexBI)
                    {
                        index = _fontinfo.indexBI;
                        faceIndex = _fontinfo.faceIndexBI;
                        bNeedBold = false;
                    }
                    else
                    {
                        index = _fontinfo.indexI;
                        faceIndex = _fontinfo.faceIndexI;
                    }
                    break;
                }
                case AscFonts.FontStyle.FontStyleItalic:
                {
                    bSrcItalic  = true;
        
                    bNeedBold   = false;
                    bNeedItalic = true;
                    if (-1 != _fontinfo.indexI)
                    {
                        index = _fontinfo.indexI;
                        faceIndex = _fontinfo.faceIndexI;
                        bNeedItalic = false;
                    }
                    else if (-1 != _fontinfo.indexR)
                    {
                        index = _fontinfo.indexR;
                        faceIndex = _fontinfo.faceIndexR;
                    }
                    else if (-1 != _fontinfo.indexBI)
                    {
                        index = _fontinfo.indexBI;
                        faceIndex = _fontinfo.faceIndexBI;
                        bNeedItalic = false;
                    }
                    else
                    {
                        index = _fontinfo.indexB;
                        faceIndex = _fontinfo.faceIndexB;
                    }
                    break;
                }
                case AscFonts.FontStyle.FontStyleRegular:
                {
                    bNeedBold   = false;
                    bNeedItalic = false;
                    if (-1 != _fontinfo.indexR)
                    {
                        index = _fontinfo.indexR;
                        faceIndex = _fontinfo.faceIndexR;
                    }
                    else if (-1 != _fontinfo.indexI)
                    {
                        index = _fontinfo.indexI;
                        faceIndex = _fontinfo.faceIndexI;
                    }
                    else if (-1 != _fontinfo.indexB)
                    {
                        index = _fontinfo.indexB;
                        faceIndex = _fontinfo.faceIndexB;
                    }
                    else
                    {
                        index = _fontinfo.indexBI;
                        faceIndex = _fontinfo.faceIndexBI;
                    }
                }
            }
        
            var _info = 
            {
                Path        : AscFonts.g_font_files[index].Id,
                FaceIndex   : faceIndex,
                NeedBold    : bNeedBold,
                NeedItalic  : bNeedItalic,
                SrcBold     : bSrcBold,
                SrcItalic   : bSrcItalic
            };

            var flag = 0;
            if (_info.NeedBold)     flag |= 0x01;
            if (_info.NeedItalic)   flag |= 0x02;
            if (_info.SrcBold)      flag |= 0x04;
            if (_info.SrcItalic)    flag |= 0x08;

            this.Native["SetFont"](_info.Path, _info.FaceIndex, _lastFont.SetUpSize, flag);
        }
    },
    GetTextPr : function()
    {
        // this.Native["GetTextPr"]();
        return this.m_oTextPr;
    },
    FillText : function(x, y, text)
    {
        var _code = text.charCodeAt(0);
        this.Native["FillText"](x, y, _code);
    },
    t : function(text, x, y, isBounds)
    {
		var _arr = [];
        var _len = text.length;
        for (var i = 0; i < _len; i++)
            _arr.push(text.charCodeAt(i));

        this.Native["t"](x, y, _arr);
    },
    FillText2 : function(x, y, text, cropX, cropW)
    {
        var _code = text.charCodeAt(0);
        if (null != this.LastFontOriginInfo.Replace)
            _code = AscFonts.g_fontApplication.GetReplaceGlyph(_code, this.LastFontOriginInfo.Replace);

        this.Native["FillText2"](x, y, _code, cropX, cropW);
    },
    t2 : function(text, x, y, cropX, cropW)
    {
        var _arr = [];
        var _len = text.length;
        for (var i = 0; i < _len; i++)
            _arr.push(text.charCodeAt(i));

        this.Native["t2"](x, y, _arr, cropX, cropW);
    },
    FillTextCode : function(x, y, lUnicode)
    {
        if (null != this.LastFontOriginInfo.Replace)
            lUnicode = AscFonts.g_fontApplication.GetReplaceGlyph(lUnicode, this.LastFontOriginInfo.Replace);

        this.Native["FillTextCode"](x, y, lUnicode);
    },
    tg : function(text, x, y)
    {
        this.Native["tg"](text, x, y);
    },
    charspace : function(space)
    {
        // this.Native["charspace"](space);
    },
    // private methods
    private_FillGlyph : function(pGlyph, _bounds)
    {
        // new scheme
        var nW = pGlyph.oBitmap.nWidth;
        var nH = pGlyph.oBitmap.nHeight;

        if (0 == nW || 0 == nH)
            return;

        var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

		var nX = (_font_manager.m_oGlyphString.m_fX >> 0) + (pGlyph.fX + pGlyph.oBitmap.nX) >> 0;
		var nY = (_font_manager.m_oGlyphString.m_fY >> 0) + (pGlyph.fY - pGlyph.oBitmap.nY) >> 0;

        pGlyph.oBitmap.oGlyphData.checkColor(this.m_oBrush.Color1.R, this.m_oBrush.Color1.G, this.m_oBrush.Color1.B, nW, nH);

        if (null == this.TextClipRect)
            pGlyph.oBitmap.draw(this.m_oContext, nX, nY, this.TextClipRect);
        else
            pGlyph.oBitmap.drawCropInRect(this.m_oContext, nX, nY, this.TextClipRect);

        if (_bounds)
        {
            var _r = nX + pGlyph.oBitmap.nWidth;
            var _b = nY + pGlyph.oBitmap.nHeight;
            if (_bounds.x > nX)
                _bounds.x = nX;
			if (_bounds.y > nY)
				_bounds.y = nY;
			if (_bounds.r < _r)
				_bounds.r = _r;
			if (_bounds.b < _b)
				_bounds.b = _b;
        }

        this.Native["private_FillGlyph"](pGlyph, _bounds);
    },
    private_FillGlyphC : function(pGlyph, cropX, cropW)
    {
        // new scheme
        var nW = pGlyph.oBitmap.nWidth;
        var nH = pGlyph.oBitmap.nHeight;

        if (0 == nW || 0 == nH)
            return;

        var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

        var nX = (_font_manager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX) >> 0;
        var nY = (_font_manager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY) >> 0;

        var d_koef = this.m_dDpiX / 25.4;

        var cX = Math.max((cropX * d_koef) >> 0, 0);
        var cW = Math.min((cropW * d_koef) >> 0, nW);
        if (cW <= 0)
            cW = 1;

        pGlyph.oBitmap.oGlyphData.checkColor(this.m_oBrush.Color1.R, this.m_oBrush.Color1.G, this.m_oBrush.Color1.B, nW, nH);
        pGlyph.oBitmap.drawCrop(this.m_oContext, nX, nY, cW, nH, cX);

        this.Native["private_FillGlyphC"](pGlyph, cropX, cropW);
    },
    private_FillGlyph2 : function(pGlyph)
    {
        var i = 0;
        var j = 0;

        var nW = pGlyph.oBitmap.nWidth;
        var nH = pGlyph.oBitmap.nHeight;

        if (0 == nW || 0 == nH)
            return;

        var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

        var nX = (_font_manager.m_oGlyphString.m_fX + pGlyph.fX + pGlyph.oBitmap.nX) >> 0;
        var nY = (_font_manager.m_oGlyphString.m_fY + pGlyph.fY - pGlyph.oBitmap.nY) >> 0;

        var imageData = this.m_oContext.getImageData(nX, nY, nW, nH);
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

                    pPixels[indx]     = ((_r - r) * weight + (r << 8)) >>> 8;
                    pPixels[indx + 1] = ((_g - g) * weight + (g << 8)) >>> 8;
                    pPixels[indx + 2] = ((_b - b) * weight + (b << 8)) >>> 8;
                    pPixels[indx + 3] = (weight + a) - ((weight * a + 256) >>> 8);
                }

                indx += 4;
            }
        }
        this.m_oContext.putImageData(imageData, nX, nY);

        this.Native["private_FillGlyph2"](pGlyph);
    },
    SetIntegerGrid : function(param)
    {
        this.m_bIntegerGrid = param;
        this.Native["SetIntegerGrid"](param);
    },
    GetIntegerGrid : function()
    {
        // this.Native["GetIntegerGrid"]();
        return this.m_bIntegerGrid;
    },
    DrawStringASCII : function(name, size, bold, italic, text, x, y, bIsHeader)
    {
        var _textProp = {
            RFonts   : { Ascii : { Name : name, Index : -1 } },
            FontSize : (((size * 2 * 96 / this.m_dDpiY) + 0.5) >> 0) / 2,
            Bold     : false,
            Italic   : false
        };

        this.m_oTextPr = _textProp;
        this.m_oGrFonts.Ascii.Name = this.m_oTextPr.RFonts.Ascii.Name;
        this.m_oGrFonts.Ascii.Index = -1;

        this.SetFontSlot(fontslot_ASCII, 1);

        this.m_oFontManager.LoadString2(text, 0, 0);
        var measure = this.m_oFontManager.MeasureString2();

        var _ctx = this.m_oContext;
        _ctx.beginPath();
        _ctx.fillStyle = "#E1E1E1";
        _ctx.strokeStyle = GlobalSkin.RulerOutline;
        this.m_bBrushColorInit = false;
        this.m_bPenColorInit   = false;

        var _xPxOffset = 10;
        var _yPxOffset = 5;
        if (AscCommon.AscBrowser.isRetina)
        {
            _xPxOffset = (_xPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
            _yPxOffset = (_yPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        }

        var __x = this.m_oFullTransform.TransformPointX(x, y) >> 0;
        var __y = this.m_oFullTransform.TransformPointY(x, y) >> 0;
        var __w = (measure.fWidth >> 0) + 2 * _xPxOffset;
        var __h = (Math.abs(measure.fY) >> 0) + 2 * _yPxOffset;

        if (!bIsHeader)
            __y -= __h;

        if (!AscCommon.AscBrowser.isRetina)
            _ctx.rect(__x + 0.5, __y + 0.5, __w, __h);
        else
            _ctx.rect(__x, __y, __w, __h);

        _ctx.fill();
        _ctx.stroke();
        _ctx.beginPath();

        this.b_color1(68, 68, 68, 255);

        var _koef_px_to_mm = 25.4 / this.m_dDpiY;

        this.t(text, x + _xPxOffset * _koef_px_to_mm, y + ((bIsHeader ? __h : 0) - _yPxOffset) * _koef_px_to_mm);

        this.Native["DrawStringASCII"](name, size, bold, italic, text, x, y, bIsHeader);
    },
    DrawStringASCII2 : function(name, size, bold, italic, text, x, y, bIsHeader)
    {
        var _textProp = {
            RFonts   : { Ascii : { Name : name, Index : -1 } },
            FontSize : (((size * 2 * 96 / this.m_dDpiY) + 0.5) >> 0) / 2,
            Bold     : false,
            Italic   : false
        };

        this.m_oTextPr = _textProp;
        this.m_oGrFonts.Ascii.Name = this.m_oTextPr.RFonts.Ascii.Name;
        this.m_oGrFonts.Ascii.Index = -1;

        this.SetFontSlot(fontslot_ASCII, 1);

        this.m_oFontManager.LoadString2(text, 0, 0);
        var measure = this.m_oFontManager.MeasureString2();

        var _ctx = this.m_oContext;
        _ctx.beginPath();
        _ctx.fillStyle = "#E1E1E1";
        _ctx.strokeStyle = GlobalSkin.RulerOutline;
        this.m_bBrushColorInit = false;
        this.m_bPenColorInit   = false;

        var _xPxOffset = 10;
        var _yPxOffset = 5;
        if (AscCommon.AscBrowser.isRetina)
        {
			_xPxOffset = (_xPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
			_yPxOffset = (_yPxOffset * AscCommon.AscBrowser.retinaPixelRatio) >> 0;
        }

        var __x = this.m_oFullTransform.TransformPointX(this.m_dWidthMM - x, y) >> 0;
        var __y = this.m_oFullTransform.TransformPointY(this.m_dWidthMM - x, y) >> 0;
        var __w = (measure.fWidth >> 0) + 2 * _xPxOffset;
        var __h = (Math.abs(measure.fY) >> 0) + 2 * _yPxOffset;
        __x -= __w;

        if (!bIsHeader)
            __y -= __h;

        if (!AscCommon.AscBrowser.isRetina)
            _ctx.rect(__x + 0.5, __y + 0.5, __w, __h);
        else
            _ctx.rect(__x, __y, __w, __h);

        _ctx.fill();
        _ctx.stroke();
        _ctx.beginPath();

        this.b_color1(68, 68, 68, 255);

        var _koef_px_to_mm = 25.4 / this.m_dDpiY;

        var xPos = this.m_dWidthMM - x - (__w - _xPxOffset) * _koef_px_to_mm;

        this.t(text, xPos, y + ((bIsHeader ? __h : 0) - _yPxOffset) * _koef_px_to_mm);

        this.Native["DrawStringASCII2"](name, size, bold, italic, text, x, y, bIsHeader);
    },
    DrawHeaderEdit : function(yPos, lock_type, sectionNum, bIsRepeat, type)
    {
        this.Native["DrawHeaderEdit"](yPos, lock_type, sectionNum, bIsRepeat, type);
    },
    DrawFooterEdit : function(yPos, lock_type, sectionNum, bIsRepeat, type)
    {
        this.Native["DrawFooterEdit"](yPos, lock_type, sectionNum, bIsRepeat, type);
    },
    DrawLockParagraph : function(lock_type, x, y1, y2)
    {
        this.Native["DrawLockParagraph"](lock_type, x, y1, y2);
    },
    DrawLockObjectRect : function(lock_type, x, y, w, h)
    {
        if (this.IsThumbnail || lock_type == AscCommon.locktype_None || this.IsDemonstrationMode)
            return;

        this.Native["DrawLockObjectRect"](lock_type, x, y, w, h);
    },
    DrawEmptyTableLine : function(x1, y1, x2, y2)
    {
        this.Native["DrawEmptyTableLine"](x1, y1, x2, y2);
    },
    DrawSpellingLine : function(y0, x0, x1, w)
    {
        this.Native["DrawSpellingLine"](y0, x0, x1, w);
    },
    // smart methods for horizontal / vertical lines
    drawHorLine : function(align, y, x, r, penW)
    {
        this.Native["drawHorLine"](align, y, x, r, penW);
    },
    drawHorLine2 : function(align, y, x, r, penW)
    {
        this.Native["drawHorLine2"](align, y, x, r, penW);
    },
    drawVerLine : function(align, x, y, b, penW)
    {
        this.Native["drawVerLine"](align, x, y, b, penW);
    },
    // мега крутые функции для таблиц
    drawHorLineExt : function(align, y, x, r, penW, leftMW, rightMW)
    {
        this.Native["drawHorLineExt"](align, y, x, r, penW, leftMW, rightMW);
    },
    rect : function(x, y, w, h)
    {
        this.Native["rect"](x, y, w, h);
    },
    TableRect : function(x, y, w, h)
    {
        this.Native["TableRect"](x, y, w, h);
    },
    // функции клиппирования
    AddClipRect : function(x, y, w, h)
    {
        this.Native["AddClipRect"](x, y, w, h);
    },
    RemoveClipRect : function()
    {
        // this.Native["RemoveClipRect"]();
    },
    SetClip : function(r)
    {
        this.Native["SetClip"](r.x, r.y, r.w, r.h);
    },
    RemoveClip : function()
    {
        this.Native["RemoveClip"]();
    },
    drawCollaborativeChanges : function(x, y, w, h, Color)
    {
        this.Native["drawCollaborativeChanges"](x, y, w, h, Color.r, Color.g, Color.b, 255);
    },
    drawMailMergeField : function(x, y, w, h)
    {
        this.Native["drawMailMergeField"](x, y, w, h);
    },
    drawSearchResult : function(x, y, w, h)
    {
        this.Native["drawSearchResult"](x, y, w, h);
    },
    drawFlowAnchor : function(x, y)
    {
        if (!AscCommon.g_flow_anchor || !AscCommon.g_flow_anchor.asc_complete || (!editor || !editor.ShowParaMarks))
            return;

        if (false === this.m_bIntegerGrid)
        {
            this.m_oContext.setTransform(1,0,0,1,0,0);
        }

        var _x = this.m_oFullTransform.TransformPointX(x,y) >> 0;
        var _y = this.m_oFullTransform.TransformPointY(x,y) >> 0;

        this.m_oContext.drawImage(AscCommon.g_flow_anchor, _x, _y);

        if (false === this.m_bIntegerGrid)
        {
            this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
        }

        this.Native["drawFlowAnchor"](x, y);
    },
    SavePen : function()
    {
        this.Native["SavePen"]();
    },
    RestorePen : function()
    {
        this.Native["RestorePen"]();
    },
    SaveBrush : function()
    {
        this.Native["SaveBrush"]();
    },
    RestoreBrush : function()
    {
        this.Native["RestoreBrush"]();
    },
    SavePenBrush : function()
    {
        this.Native["SavePenBrush"]();
    },
    RestorePenBrush : function()
    {
        this.Native["RestorePenBrush"]();
    },
    SaveGrState : function()
    {
        this.Native["SaveGrState"]();
    },
    RestoreGrState : function()
    {
        this.Native["RestoreGrState"]();
    },
    StartClipPath : function()
    {
        this.Native["StartClipPath"]();
    },
    EndClipPath : function()
    {
        this.Native["EndClipPath"]();
    },
    StartCheckTableDraw : function()
    {
        return this.Native["StartCheckTableDraw"]();
    },
    EndCheckTableDraw : function(bIsRestore)
    {
        return this.Native["EndCheckTableDraw"](bIsRestore);
    },
    SetTextClipRect : function(_l, _t, _r, _b)
    {
        return this.Native["SetTextClipRect"](_l, _t, _r, _b);
    },
    AddSmartRect : function(x, y, w, h, pen_w)
    {
        return this.Native["AddSmartRect"](x, y, w, h, pen_w);
    },
    CheckUseFonts2 : function(_transform)
    {
        if (!global_MatrixTransformer.IsIdentity2(_transform))
        {
            if (!AscCommon.g_fontManager2)
            {
                AscCommon.g_fontManager2 = new AscFonts.CFontManager();
                AscCommon.g_fontManager2.Initialize(true);
            }

            this.m_oFontManager2 = AscCommon.g_fontManager2;

            if (null == this.m_oLastFont2)
                this.m_oLastFont2 = new AscCommon.CFontSetup();

            this.IsUseFonts2 = true;
        }

        this.Native["CheckUseFonts2"](_transform);
    },
    UncheckUseFonts2 : function()
    {
        this.IsUseFonts2 = false;

        this.Native["UncheckUseFonts2"]();
    },
    Drawing_StartCheckBounds : function(x, y, w, h)
    {
        this.Native["Drawing_StartCheckBounds"](x, y, w, h);
    },
    Drawing_EndCheckBounds : function()
    {
        this.Native["Drawing_EndCheckBounds"]();
    },
    DrawPresentationComment : function(type, x, y, w, h)
    {
        this.Native["DrawPresentationComment"](type, x, y, w, h);
    },
    DrawPolygon : function(oPath, lineWidth, shift)
    {
        this.m_oContext.lineWidth = lineWidth;
        this.m_oContext.beginPath();

        var Points    = oPath.Points;

        var nCount = Points.length;
        // берем предпоследнюю точку, т.к. последняя совпадает с первой
        var PrevX = Points[nCount - 2].X, PrevY = Points[nCount - 2].Y;
        var _x = Points[nCount - 2].X,
            _y = Points[nCount - 2].Y;
        var StartX, StartY ;

        for (var nIndex = 0; nIndex < nCount; nIndex++)
        {
            if(PrevX > Points[nIndex].X)
            {
                _y = Points[nIndex].Y - shift;
            }
            else if(PrevX < Points[nIndex].X)
            {
                _y  = Points[nIndex].Y + shift;
            }

            if(PrevY < Points[nIndex].Y)
            {
                _x = Points[nIndex].X - shift;
            }
            else if(PrevY > Points[nIndex].Y)
            {
                _x = Points[nIndex].X + shift;
            }

            PrevX = Points[nIndex].X;
            PrevY = Points[nIndex].Y;

            if(nIndex > 0)
            {
                if (1 == nIndex)
                {
                    StartX = _x;
                    StartY = _y;
                    this._m(_x, _y);
                }
                else
                {
                    this._l(_x, _y);
                }
            }
        }

        this._l(StartX, StartY);
        this.m_oContext.closePath();
        this.m_oContext.stroke();
        this.m_oContext.beginPath();

        this.Native["DrawPolygon"](oPath, lineWidth, shift);
    },
    DrawFootnoteRect : function(x, y, w, h)
    {
        var _old = this.m_bIntegerGrid;
        if (!_old)
            this.SetIntegerGrid(true);

        this.p_dash([1, 1]);

        this._s();

        var l = x;
        var t = y;
        var r = x + w;
        var b = y + h;

        this.drawHorLineExt(c_oAscLineDrawingRule.Top, t, l, r, 0, 0, 0);
        this.drawVerLine(c_oAscLineDrawingRule.Right, l, t, b, 0);
        this.drawVerLine(c_oAscLineDrawingRule.Left, r, t, b, 0);
        this.drawHorLineExt(c_oAscLineDrawingRule.Top, b, l, r, 0, 0, 0);

        this.ds();
        this._s();

        this.p_dash(null);

        if (!_old)
            this.SetIntegerGrid(false);

        this.Native["DrawFootnoteRect"](x, y, w, h);
    },
    // new methods
    toDataURL : function(type)
    {
        return this.Native["toDataURL"](type);
    }
};
