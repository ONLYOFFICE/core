var g_fontManager = new CFontManager();
g_fontManager.Initialize(true);

function CTableMarkup(Table)
{
    this.Internal =
    {
        RowIndex  : 0,
        CellIndex : 0,
        PageNum   : 0
    };
    this.Table = Table;
    this.X = 0; // Смещение таблицы от начала страницы до первой колонки

    this.Cols    = new Array(); // массив ширин колонок
    this.Margins = new Array(); // массив левых и правых маргинов

    this.Rows    = new Array(); // массив позиций, высот строк(для данной страницы)
                                // Rows = [ { Y : , H :  }, ... ]

    this.CurCol = 0; // текущая колонка
    this.CurRow = 0; // текущая строка

    this.TransformX = 0;
    this.TransformY = 0;
}

CTableMarkup.prototype =
{
    CreateDublicate : function()
    {
        var obj = new CTableMarkup(this.Table);

        obj.Internal = { RowIndex : this.Internal.RowIndex, CellIndex : this.Internal.CellIndex, PageNum : this.Internal.PageNum };
        obj.X = this.X;

        var len = this.Cols.length;
        for (var i = 0; i < len; i++)
            obj.Cols[i] = this.Cols[i];

        len = this.Margins.length;
        for (var i = 0; i < len; i++)
            obj.Margins[i] = { Left : this.Margins[i].Left, Right : this.Margins[i].Right };

        len = this.Rows.length;
        for (var i = 0; i < len; i++)
            obj.Rows[i] = { Y : this.Rows[i].Y, H : this.Rows[i].H };

        obj.CurRow = this.CurRow;
        obj.CurCol = this.CurCol;

        return obj;
    },

    CorrectFrom : function()
    {
        this.X += this.TransformX;

        var _len = this.Rows.length;
        for (var i = 0; i < _len; i++)
        {
            this.Rows[i].Y += this.TransformY;
        }
    },

    CorrectTo : function()
    {
        this.X -= this.TransformX;

        var _len = this.Rows.length;
        for (var i = 0; i < _len; i++)
        {
            this.Rows[i].Y -= this.TransformY;
        }
    },

    Get_X : function()
    {
        return this.X;
    },

    Get_Y : function()
    {
        var _Y = 0;
        if (this.Rows.length > 0)
        {
            _Y = this.Rows[0].Y;
        }
        return _Y;
    }
};

function CTableOutline(Table, PageNum, X, Y, W, H)
{
    this.Table = Table;
    this.PageNum = PageNum;

    this.X = X;
    this.Y = Y;

    this.W = W;
    this.H = H;
}

function CTextMeasurer()
{
    this.m_oManager     = new CFontManager();

    this.m_oFont        = null;

    // RFonts
    this.m_oTextPr      = null;
    this.m_oLastFont    = new CFontSetup();

    this.Init = function()
    {
        this.m_oManager.Initialize();
    }

    this.SetFont = function(font)
    {
        if (!font)
            return;

        this.m_oFont = font;

        if (-1 == font.FontFamily.Index || undefined === font.FontFamily.Index || null == font.FontFamily.Index)
            font.FontFamily.Index = window.g_map_font_index[font.FontFamily.Name];

        if (font.FontFamily.Index == undefined || font.FontFamily.Index == -1)
            return;

        var bItalic = true === font.Italic;
        var bBold   = true === font.Bold;

        var oFontStyle = FontStyle.FontStyleRegular;
        if ( !bItalic && bBold )
            oFontStyle = FontStyle.FontStyleBold;
        else if ( bItalic && !bBold )
            oFontStyle = FontStyle.FontStyleItalic;
        else if ( bItalic && bBold )
            oFontStyle = FontStyle.FontStyleBoldItalic;

        var _lastSetUp = this.m_oLastFont;
        if (_lastSetUp.SetUpIndex != font.FontFamily.Index || _lastSetUp.SetUpSize != font.FontSize || _lastSetUp.SetUpStyle != oFontStyle)
        {
            _lastSetUp.SetUpIndex = font.FontFamily.Index;
            _lastSetUp.SetUpSize = font.FontSize;
            _lastSetUp.SetUpStyle = oFontStyle;

            window.g_font_infos[_lastSetUp.SetUpIndex].LoadFont(window.g_font_loader, this.m_oManager, _lastSetUp.SetUpSize, _lastSetUp.SetUpStyle, 72, 72);
        }
    }

    this.SetTextPr = function(textPr)
    {
        this.m_oTextPr = textPr.Copy();
    }

    this.SetFontSlot = function(slot, fontSizeKoef)
    {
        var _rfonts = this.m_oTextPr.RFonts;
        var _lastFont = this.m_oLastFont;

        switch (slot)
        {
            case fontslot_ASCII:
            {
                _lastFont.Name   = _rfonts.Ascii.Name;
                _lastFont.Index  = _rfonts.Ascii.Index;

                if (_lastFont.Index == -1 || _lastFont.Index === undefined)
                {
                    _lastFont.Index = window.g_map_font_index[_lastFont.Name];
                }

                _lastFont.Size = this.m_oTextPr.FontSize;
                _lastFont.Bold = this.m_oTextPr.Bold;
                _lastFont.Italic = this.m_oTextPr.Italic;

                break;
            }
            case fontslot_CS:
            {
                _lastFont.Name   = _rfonts.CS.Name;
                _lastFont.Index  = _rfonts.CS.Index;

                if (_lastFont.Index == -1 || _lastFont.Index === undefined)
                {
                    _lastFont.Index = window.g_map_font_index[_lastFont.Name];
                }

                _lastFont.Size = this.m_oTextPr.FontSizeCS;
                _lastFont.Bold = this.m_oTextPr.BoldCS;
                _lastFont.Italic = this.m_oTextPr.ItalicCS;

                break;
            }
            case fontslot_EastAsia:
            {
                _lastFont.Name   = _rfonts.EastAsia.Name;
                _lastFont.Index  = _rfonts.EastAsia.Index;

                if (_lastFont.Index == -1 || _lastFont.Index === undefined)
                {
                    _lastFont.Index = window.g_map_font_index[_lastFont.Name];
                }

                _lastFont.Size = this.m_oTextPr.FontSize;
                _lastFont.Bold = this.m_oTextPr.Bold;
                _lastFont.Italic = this.m_oTextPr.Italic;

                break;
            }
            case fontslot_HAnsi:
            default:
            {
                _lastFont.Name   = _rfonts.HAnsi.Name;
                _lastFont.Index  = _rfonts.HAnsi.Index;

                if (_lastFont.Index == -1 || _lastFont.Index === undefined)
                {
                    _lastFont.Index = window.g_map_font_index[_lastFont.Name];
                }

                _lastFont.Size = this.m_oTextPr.FontSize;
                _lastFont.Bold = this.m_oTextPr.Bold;
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

        if (_lastFont.Index != _lastFont.SetUpIndex || _lastFont.Size != _lastFont.SetUpSize || _style != _lastFont.SetUpStyle)
        {
            _lastFont.SetUpIndex = _lastFont.Index;
            _lastFont.SetUpSize = _lastFont.Size;
            _lastFont.SetUpStyle = _style;

            window.g_font_infos[_lastFont.SetUpIndex].LoadFont(window.g_font_loader, this.m_oManager, _lastFont.SetUpSize, _lastFont.SetUpStyle, 72, 72);
        }
    }

    this.GetTextPr = function()
    {
        return this.m_oTextPr;
    }

    this.GetFont = function()
    {
        return this.m_oFont;
    }

    this.Measure = function(text)
    {
        var Width  = 0;
        var Height = 0;

        var Temp = this.m_oManager.MeasureChar( text.charCodeAt(0) );
		
        Width  = Temp.fAdvanceX * 25.4 / 72;
        Height = 0;//Temp.fHeight;

        return { Width : Width, Height : Height };
    }
    this.Measure2 = function(text)
    {
        var Width  = 0;

        var Temp = this.m_oManager.MeasureChar( text.charCodeAt(0) );

        Width  = Temp.fAdvanceX * 25.4 / 72;

        return { Width : Width, Ascent : (Temp.oBBox.fMaxY * 25.4 / 72), Height : ((Temp.oBBox.fMaxY - Temp.oBBox.fMinY) * 25.4 / 72),
            WidthG: ((Temp.oBBox.fMaxX - Temp.oBBox.fMinX) * 25.4 / 72)};
    }

    this.MeasureCode = function(lUnicode)
    {
        var Width  = 0;
        var Height = 0;

        var Temp = this.m_oManager.MeasureChar( lUnicode );

        Width  = Temp.fAdvanceX * 25.4 / 72;
        Height = 0;//Temp.fHeight;

        return { Width : Width, Height : Height };
    }
    this.Measure2Code = function(lUnicode)
    {
        var Width  = 0;

        var Temp = this.m_oManager.MeasureChar( lUnicode );

        Width  = Temp.fAdvanceX * 25.4 / 72;

        return { Width : Width, Ascent : (Temp.oBBox.fMaxY * 25.4 / 72), Height : ((Temp.oBBox.fMaxY - Temp.oBBox.fMinY) * 25.4 / 72),
            WidthG: ((Temp.oBBox.fMaxX - Temp.oBBox.fMinX) * 25.4 / 72)};
    }

    this.GetAscender = function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Ascender   = this.m_oManager.m_lAscender;

        return Ascender * this.m_oLastFont.SetUpSize / UnitsPerEm * g_dKoef_pt_to_mm;
    }
    this.GetDescender = function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Descender  = this.m_oManager.m_lDescender;

        return Descender * this.m_oLastFont.SetUpSize / UnitsPerEm * g_dKoef_pt_to_mm;
    }
    this.GetHeight = function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Height     = this.m_oManager.m_lLineHeight;

        return Height * this.m_oLastFont.SetUpSize / UnitsPerEm * g_dKoef_pt_to_mm;
    }
}

var g_oTextMeasurer = new CTextMeasurer();
g_oTextMeasurer.Init();

function _rect()
{
    this.x = 0;
    this.y = 0;
    this.w = 0;
    this.h = 0;
}

function CDrawingPage()
{
    this.left   = 0;
    this.top    = 0;
    this.right  = 0;
    this.bottom = 0;

    this.cachedImage = null;
}

function CPage()
{
    this.width_mm       = 210;
    this.height_mm      = 297;

    this.margin_left    = 0;
    this.margin_top     = 0;
    this.margin_right   = 0;
    this.margin_bottom  = 0;

    this.pageIndex      = -1;
}

function CDrawingDocument()
{
    this.IsLockObjectsEnable = false;

    this.m_oWordControl     = null;
    this.m_oLogicDocument   = null;
 	this.m_oDocumentRenderer = null;

    this.m_arrPages         = new Array();
    this.m_lPagesCount      = 0;

    this.m_lDrawingFirst    = -1;
    this.m_lDrawingEnd      = -1;
    this.m_lCurrentPage     = -1;

    this.FrameRect = { IsActive : false, Rect : { X : 0, Y : 0, R : 0, B : 0 }, Frame : null,
        Track : { X : 0, Y : 0, L : 0, T : 0, R : 0, B : 0, PageIndex : 0, Type : -1 }, IsTracked : false, PageIndex : 0 };

    this.m_lCountCalculatePages = 0;

    this.Start_CollaborationEditing = function()
    {
    }
    this.SetCursorType = function(sType, Data)
    {
    }
    this.LockCursorType = function(sType)
    {
    }
    this.LockCursorTypeCur = function()
    {
    }
    this.UnlockCursorType = function()
    {
    }

    this.OnStartRecalculate = function(pageCount)
    {
        this.m_lCountCalculatePages = pageCount;
    }

    this.OnRepaintPage = function(index)
    {
    }

    this.OnRecalculatePage = function(index, pageObject)
    {
        this.m_lCountCalculatePages = index + 1;

        if (undefined === this.m_arrPages[index])
            this.m_arrPages[index] = new CPage();

        var page = this.m_arrPages[index];

        page.width_mm  = pageObject.Width;
        page.height_mm = pageObject.Height;

        page.margin_left    = pageObject.Margins.Left;
        page.margin_top     = pageObject.Margins.Top;
        page.margin_right   = pageObject.Margins.Right;
        page.margin_bottom  = pageObject.Margins.Bottom;

        page.pageIndex = index;
    }

    this.OnEndRecalculate = function(isFull, isBreak)
    {
        if (undefined != isBreak)
        {
            this.m_lCountCalculatePages = this.m_lPagesCount;
        }

        if (isFull)
        {
            this.m_lPagesCount = this.m_lCountCalculatePages;
        }
    }
	
	this.ChangePageAttack = function(pageIndex)
	{
	}    

    this.RenderDocument = function(Renderer)
    {
        for (var i = 0; i < this.m_lPagesCount; i++)
        {
            var page = this.m_arrPages[i];
            Renderer.BeginPage(page.width_mm, page.height_mm);
            this.m_oLogicDocument.DrawPage(i, Renderer);
            Renderer.EndPage();
        }
    }

    this.ToRenderer = function()
    {
        var Renderer = new CDocumentRenderer();
        Renderer.VectorMemoryForPrint = new CMemory();
        var old_marks = this.m_oWordControl.m_oApi.ShowParaMarks;
        this.m_oWordControl.m_oApi.ShowParaMarks = false;
        this.RenderDocument(Renderer);
        this.m_oWordControl.m_oApi.ShowParaMarks = old_marks;
        var ret = Renderer.Memory.GetBase64Memory();
        return ret;
    }
    
    this.ClearCachePages = function()
    {       
    }

    this.FirePaint = function()
    {
    }    

    this.InitViewer = function()
    {
    }

    this.TargetStart = function()
    {
    }
    this.TargetEnd = function()
    {
    }
    this.UpdateTargetNoAttack = function()
    {
    }

    this.GetTargetStyle = function()
    {
    }

    this.SetTargetColor = function(r, g, b)
    {
    }

    this.UpdateTargetTransform = function(matrix)
    {
    }

    this.UpdateTarget = function(x, y, pageIndex)
    {        
    }    
    
    this.SetTargetSize = function(size)
    {       
    }

    this.TargetShow = function()
    {
    }
    
    this.StartTrackImage = function(obj, x, y, w, h, type, pagenum)
    {
    }
    this.StartTrackTable = function(obj, transform)
    {
    }
    this.EndTrackTable = function(pointer, bIsAttack)
    {
    }
    this.CheckTrackTable = function()
    {
    }
    
    this.SetCurrentPage = function(PageIndex)
    {
    }

    this.SelectEnabled = function(bIsEnabled)
    {
    }
	this.SelectClear = function()
    {
    }
    this.SearchClear = function()
    {
    }
    this.AddPageSearch = function(findText, rects, type)
    {        
    }

    this.StartSearchTransform = function(transform)
    {
    }

    this.EndSearchTransform = function()
    {
    }

    this.StartSearch = function()
    {
    }
    this.EndSearch = function(bIsChange)
    {
    }

    this.AddPageSelection = function(pageIndex, x, y, w, h)
    {        
    }

    this.AddPageSelection2 = function(pageIndex, x, y, width, height)
    {       
    }

    this.SelectShow = function()
    {
    }

    this.Set_RulerState_Table = function(markup, transform)
    {        
    }

    this.Set_RulerState_Paragraph = function(margins)
    {
    }

    this.Set_RulerState_HdrFtr = function(bHeader, Y0, Y1)
    {        
    }

    this.Update_ParaTab = function(Default_Tab, ParaTabs)
    {
    }

    this.UpdateTableRuler = function(isCols, index, position)
    {        
    }
    this.GetDotsPerMM = function(value)
    {
        return value * this.m_oWordControl.m_nZoomValue * g_dKoef_mm_to_pix / 100;
    }

    this.GetMMPerDot = function(value)
    {
        return value / this.GetDotsPerMM( 1 );
    }
    this.GetVisibleMMHeight = function()
    {
    }    

    // при загрузке документа - нужно понять какие шрифты используются
    this.CheckFontNeeds = function()
    {
        var map_keys = this.m_oWordControl.m_oLogicDocument.Document_Get_AllFontNames();
        var dstfonts = new Array();
        for (var i in map_keys)
        {
            dstfonts[dstfonts.length] = new CFont(i, 0, "", 0, null);
        }
        this.m_oWordControl.m_oLogicDocument.Fonts = dstfonts;
        return;        
    }

    // вот здесь весь трекинг
    this.DrawTrack = function(type, matrix, left, top, width, height, isLine, canRotate)
    {
    }

    this.DrawTrackSelectShapes = function(x, y, w, h)
    {
    }

    this.DrawAdjustment = function(matrix, x, y)
    {
    }

    this.LockTrackPageNum = function(nPageNum)
    {
    }
    this.UnlockTrackPageNum = function()
    {
    }

    this.IsMobileVersion = function()
    {
        return false;
    }

    this.OnSelectEnd = function()
    {
    }

    this.DrawVerAnchor = function(pageNum, xPos, bIsFromDrawings)
    {
    }

    this.DrawHorAnchor = function(pageNum, yPos, bIsFromDrawings)
    {
    }

    this.DrawHorVerAnchor = function()
    {
    }

    // track text (inline)
    this.StartTrackText = function()
    {
    }
    this.EndTrackText = function()
    {
    }
}