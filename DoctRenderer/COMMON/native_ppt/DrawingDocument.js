var g_fontManager = new CFontManager();
g_fontManager.Initialize(true);

var TRACK_CIRCLE_RADIUS     = 5;
var TRACK_RECT_SIZE2        = 4;
var TRACK_RECT_SIZE         = 8;
var TRACK_DISTANCE_ROTATE   = 25;
var TRACK_ADJUSTMENT_SIZE   = 10;

var FOCUS_OBJECT_THUMBNAILS     = 0;
var FOCUS_OBJECT_MAIN           = 1;
var FOCUS_OBJECT_NOTES          = 2;

var COMMENT_WIDTH   = 18;
var COMMENT_HEIGHT  = 16;

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

/*function CTextMeasurer()
{
    this.m_oManager = new CFontManager();
    this.m_oFont = null;
    this.m_oLastFont    = new CFontSetup();
}

CTextMeasurer.prototype =
{
    Init : function()
    {
        this.m_oManager.Initialize();
    },

    SetFont : function(font)
    {
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

        window.g_font_infos[font.FontFamily.Index].LoadFont(window.g_font_loader, this.m_oManager, font.FontSize, oFontStyle, 72, 72);
    },

    SetTextPr: function(textPr)
    {
        this.m_oTextPr = textPr.Copy();
    },

    SetFontSlot: function(slot, fontSizeKoef)
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
    },

    GetFont : function()
    {
        return this.m_oFont;
    },

    Measure : function(text)
    {
        var Width  = 0;
        var Height = 0;

        var Temp = this.m_oManager.MeasureChar( text.charCodeAt(0) );

        Width  = Temp.fAdvanceX * 25.4 / 72;
        Height = 0;//Temp.fHeight;

        return { Width : Width, Height : Height };
    },

    GetAscender : function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Ascender   = this.m_oManager.m_lAscender;

        return Ascender * this.m_oFont.FontSize / UnitsPerEm * g_dKoef_pt_to_mm;
    },
    GetDescender : function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Descender  = this.m_oManager.m_lDescender;

        return Descender * this.m_oFont.FontSize / UnitsPerEm * g_dKoef_pt_to_mm;
    },
    GetHeight : function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Height     = this.m_oManager.m_lLineHeight;

        return Height * this.m_oFont.FontSize / UnitsPerEm * g_dKoef_pt_to_mm;
    }
};    */

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
        var Ascender   = ( 0 !== this.m_oManager.m_lLineHeight ) ? 1.2 * this.m_oManager.m_lUnits_Per_Em * this.m_oManager.m_lAscender / this.m_oManager.m_lLineHeight : this.m_oManager.m_lAscender;

        return Ascender * this.m_oLastFont.SetUpSize / UnitsPerEm * g_dKoef_pt_to_mm;
    }
    this.GetDescender = function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Descender  = ( 0 !== this.m_oManager.m_lLineHeight ) ? 1.2 * this.m_oManager.m_lUnits_Per_Em * this.m_oManager.m_lDescender / this.m_oManager.m_lLineHeight : this.m_oManager.m_lDescender;

        return Descender * this.m_oLastFont.SetUpSize / UnitsPerEm * g_dKoef_pt_to_mm;
    }
    this.GetHeight = function()
    {
        var UnitsPerEm = this.m_oManager.m_lUnits_Per_Em;
        var Height     = ( 0 !== this.m_oManager.m_lLineHeight ) ? 1.2 * this.m_oManager.m_lUnits_Per_Em : this.m_oManager.m_lLineHeight;

        return Height * this.m_oLastFont.SetUpSize / UnitsPerEm * g_dKoef_pt_to_mm;
    }
}

var g_oTextMeasurer = new CTextMeasurer();
g_oTextMeasurer.Init();

function CTableOutlineDr()
{
    this.TableOutline = null;
    this.Counter = 0;
    this.bIsNoTable = true;
    this.bIsTracked = false;

    this.CurPos = null;
    this.TrackTablePos = 0; // 0 - left_top, 1 - right_top, 2 - right_bottom, 3 - left_bottom
    this.TrackOffsetX = 0;
    this.TrackOffsetY = 0;

    this.InlinePos = null;

    this.IsChangeSmall = true;
    this.ChangeSmallPoint = null;

    this.checkMouseDown = function(pos, word_control)
    {
        if (null == this.TableOutline)
            return false;

        var _table_track = this.TableOutline;
        var _d = 13 * g_dKoef_pix_to_mm * 100 / word_control.m_nZoomValue;

        this.IsChangeSmall = true;
        this.ChangeSmallPoint = pos;

        switch (this.TrackTablePos)
        {
            case 1:
            {
                var _x = _table_track.X + _table_track.W;
                var _b = _table_track.Y;
                var _y = _b - _d;
                var _r = _x + _d;

                if ((pos.X > _x) && (pos.X < _r) && (pos.Y > _y) && (pos.Y < _b))
                {
                    this.TrackOffsetX = pos.X - _x;
                    this.TrackOffsetY = pos.Y - _b;
                    return true;
                }
                return false;
            }
            case 2:
            {
                var _x = _table_track.X + _table_track.W;
                var _y = _table_track.Y + _table_track.H;
                var _r = _x + _d;
                var _b = _y + _d;

                if ((pos.X > _x) && (pos.X < _r) && (pos.Y > _y) && (pos.Y < _b))
                {
                    this.TrackOffsetX = pos.X - _x;
                    this.TrackOffsetY = pos.Y - _y;
                    return true;
                }
                return false;
            }
            case 3:
            {
                var _r = _table_track.X;
                var _x = _r - _d;
                var _y = _table_track.Y + _table_track.H;
                var _b = _y + _d;

                if ((pos.X > _x) && (pos.X < _r) && (pos.Y > _y) && (pos.Y < _b))
                {
                    this.TrackOffsetX = pos.X - _r;
                    this.TrackOffsetY = pos.Y - _y;
                    return true;
                }
                return false;
            }
            case 0:
            default:
            {
                var _r = _table_track.X;
                var _b = _table_track.Y;
                var _x = _r - _d;
                var _y = _b - _d;

                if ((pos.X > _x) && (pos.X < _r) && (pos.Y > _y) && (pos.Y < _b))
                {
                    this.TrackOffsetX = pos.X - _r;
                    this.TrackOffsetY = pos.Y - _b;
                    return true;
                }
                return false;
            }
        }
        
        return false;
    }

    this.checkMouseUp = function(X, Y, word_control)
    {
        this.bIsTracked = false;

        if (null == this.TableOutline || (true === this.IsChangeSmall) || word_control.m_oApi.isViewMode)
            return false;

        var _d = 13 * g_dKoef_pix_to_mm * 100 / word_control.m_nZoomValue;

        var _outline = this.TableOutline;
        var _table = _outline.Table;

        if (!_table.Is_Inline())
        {
            word_control.m_oLogicDocument.Create_NewHistoryPoint();
            switch (this.TrackTablePos)
            {
                case 1:
                {
                    var _w_pix = this.TableOutline.W * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                    var pos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X - _w_pix, Y);
                    _table.Move(pos.X - this.TrackOffsetX, pos.Y - this.TrackOffsetY, pos.Page);
                    _outline.X = pos.X - this.TrackOffsetX;
                    _outline.Y = pos.Y - this.TrackOffsetY;
                    _outline.PageNum = pos.Page;
                    break;
                }
                case 2:
                {
                    var _w_pix = this.TableOutline.W * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                    var _h_pix = this.TableOutline.H * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                    var pos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X - _w_pix, Y - _h_pix);
                    _table.Move(pos.X - this.TrackOffsetX, pos.Y - this.TrackOffsetY, pos.Page);
                    _outline.X = pos.X - this.TrackOffsetX;
                    _outline.Y = pos.Y - this.TrackOffsetY;
                    _outline.PageNum = pos.Page;
                    break;
                }
                case 3:
                {
                    var _h_pix = this.TableOutline.H * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                    var pos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X, Y - _h_pix);
                    _table.Move(pos.X - this.TrackOffsetX, pos.Y - this.TrackOffsetY, pos.Page);
                    _outline.X = pos.X - this.TrackOffsetX;
                    _outline.Y = pos.Y - this.TrackOffsetY;
                    _outline.PageNum = pos.Page;
                    break;
                }
                case 0:
                default:
                {
                    var pos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X, Y);
                    _table.Move(pos.X - this.TrackOffsetX, pos.Y - this.TrackOffsetY, pos.Page);
                    _outline.X = pos.X - this.TrackOffsetX;
                    _outline.Y = pos.Y - this.TrackOffsetY;
                    _outline.PageNum = pos.Page;
                    break;
                }
            }
        }
        else
        {
            if (null != this.InlinePos)
            {
                // inline move
                word_control.m_oLogicDocument.Create_NewHistoryPoint();
                _table.Move(this.InlinePos.X, this.InlinePos.Y, this.InlinePos.Page);
            }
        }
        _table.Cursor_MoveToStartPos();
        _table.Document_SetThisElementCurrent();
    }

    this.checkMouseMove = function(X, Y, word_control)
    {
        if (null == this.TableOutline)
            return false;

        if (true === this.IsChangeSmall)
        {
            var _pos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X, Y);
            var _dist = 15 * g_dKoef_pix_to_mm * 100 / word_control.m_nZoomValue;
            if ((Math.abs(_pos.X - this.ChangeSmallPoint.X) < _dist) && (Math.abs(_pos.Y - this.ChangeSmallPoint.Y) < _dist) && (_pos.Page == this.ChangeSmallPoint.Page))
            {
                this.CurPos = { X: this.ChangeSmallPoint.X, Y : this.ChangeSmallPoint.Y, Page: this.ChangeSmallPoint.Page };

                switch (this.TrackTablePos)
                {
                    case 1:
                    {
                        this.CurPos.X -= this.TableOutline.W;
                        break;
                    }
                    case 2:
                    {
                        this.CurPos.X -= this.TableOutline.W;
                        this.CurPos.Y -= this.TableOutline.H;
                        break;
                    }
                    case 3:
                    {
                        this.CurPos.Y -= this.TableOutline.H;
                        break;
                    }
                    case 0:
                    default:
                    {
                        break;
                    }
                }

                this.CurPos.X -= this.TrackOffsetX;
                this.CurPos.Y -= this.TrackOffsetY;
                return;
            }
            this.IsChangeSmall = false;

            this.TableOutline.Table.Selection_Remove();
            editor.WordControl.m_oLogicDocument.Document_UpdateSelectionState();
        }

        var _d = 13 * g_dKoef_pix_to_mm * 100 / word_control.m_nZoomValue;
        switch (this.TrackTablePos)
        {
            case 1:
            {
                var _w_pix = this.TableOutline.W * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                this.CurPos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X - _w_pix, Y);
                break;
            }
            case 2:
            {
                var _w_pix = this.TableOutline.W * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                var _h_pix = this.TableOutline.H * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                this.CurPos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X - _w_pix, Y - _h_pix);
                break;
            }
            case 3:
            {
                var _h_pix = this.TableOutline.H * g_dKoef_mm_to_pix * word_control.m_nZoomValue / 100;
                this.CurPos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X, Y - _h_pix);
                break;
            }
            case 0:
            default:
            {
                this.CurPos = word_control.m_oDrawingDocument.ConvertCoordsFromCursor2(X, Y);
                break;
            }
        }

        this.CurPos.X -= this.TrackOffsetX;
        this.CurPos.Y -= this.TrackOffsetY;
    }

    this.CheckStartTrack = function(word_control)
    {
        var pos = word_control.m_oDrawingDocument.ConvertCoordsToCursor(this.TableOutline.X, this.TableOutline.Y, this.TableOutline.PageNum, true);

        var _x0 = word_control.m_oEditor.AbsolutePosition.L;
        var _y0 = word_control.m_oEditor.AbsolutePosition.T;

        if (pos.X < _x0 && pos.Y < _y0)
        {
            this.TrackTablePos = 2;
        }
        else if (pos.X < _x0)
        {
            this.TrackTablePos = 1;
        }
        else if (pos.Y < _y0)
        {
            this.TrackTablePos = 3;
        }
        else
        {
            this.TrackTablePos = 0;
        }
    }
}

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

function CDrawingDocument()
{
    this.IsLockObjectsEnable = false;

    this.cursorPaintFormat = "";    

    this.m_oWordControl     = null;
    this.m_oLogicDocument   = null;

    this.SlidesCount        = 0;
    this.IsEmptyPresentation = false;

    this.SlideCurrent       = -1;
    this.SlideCurrectRect   = new CDrawingPage();

    this.m_bIsSelection = false;
    this.m_bIsSearching = false;

    this.CurrentSearchNavi = null;

    var oThis = this;
    this.m_sLockedCursorType = "";
    this.TableOutlineDr = new CTableOutlineDr();

    this.HorVerAnchors = [];

    this.m_lCurrentRendererPage = -1;
    this.m_oDocRenderer = null;
    this.m_bOldShowMarks = false;

    this.UpdateTargetFromPaint = false;
    this.NeedTarget = true;
    this.TextMatrix = null;
    this.TargetShowFlag = false;
    this.TargetShowNeedFlag = false;

    this.CanvasHit = document.createElement('canvas');
    this.CanvasHit.width = 10;
    this.CanvasHit.height = 10;
    this.CanvasHitContext = this.CanvasHit.getContext('2d');

    this.TargetCursorColor = {R: 0, G: 0, B: 0};    

    this.AutoShapesTrack = null;
    this.TransitionSlide = new CTransitionAnimation(null);

    this.Start_CollaborationEditing = function()
    {
    }

    this.IsMobileVersion = function()
    {
    }

    this.SetCursorType = function(sType)
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
    }

    this.SetTargetColor = function(r, g, b)
    {
    }

    this.OnRecalculatePage = function(index, pageObject)
    {
    }

    this.OnEndRecalculate = function()
    {
        this.SlidesCount = this.m_oLogicDocument.Slides.length;
    }
	
	this.ChangePageAttack = function(pageIndex)
	{
	}

    this.RenderDocument = function(Renderer)
    {
    }

    this.ToRenderer = function()
    {
    }

    this.ToRenderer2 = function()
    {
    }

    this.isComleteRenderer = function()
    {
    }
    this.isComleteRenderer2 = function()
    {
    }
    this.ToRendererPart = function()
    {        
    }

    this.SendChangeDocumentToApi = function(bIsAttack)
    {
    }

    this.InitGuiCanvasTextProps = function(div_id)
    {        
    }

    this.DrawGuiCanvasTextProps = function(props)
    {        
    }

    this.DrawSelection = function(overlay)
    {       
    }
    this.DrawSearch = function(overlay)
    {
    }

    this.DrawSearchCur = function(overlay, place)
    {        
    }

    this.StopRenderingPage = function(pageIndex)
    {
		return;
    }

    this.ClearCachePages = function()
    {
    }

    this.FirePaint = function()
    {
    }

    this.StartTrackAutoshape = function()
    {
    }
    this.EndTrackAutoShape = function()
    {
    }

    this.ConvertCoordsFromCursor2 = function(x, y)
    {       
    }

    this.IsCursorInTableCur = function(x, y, page)
    {
        return false;
    }

    this.ConvertCoordsToCursorWR = function(x, y)
    {
    }

    this.ConvertCoordsToCursorWR_2 = function(x, y)
    {
    }

    this.ConvertCoordsToCursorWR_Comment = function(x, y)
    {
    }

    this.ConvertCoordsToCursor = function(x, y)
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

    this.CheckTargetDraw = function(x, y)
    {        
    }

    this.UpdateTarget = function(x, y, pageIndex)
    {        
    }

    this.SetTargetSize = function(size)
    {
    }
    this.DrawTarget = function()
    {
    }
    this.TargetShow = function()
    {
    }
    this.CheckTargetShow = function()
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
    this.AddPageSearch = function(findText, rects)
    {
    }

    this.StartSearch = function()
    {
    }
    this.EndSearch = function(bIsChange)
    {
    }
    this.AddPageSelection = function(pageIndex, x, y, width, height)
    {
    }
    this.SelectShow = function()
    {
    }

    this.Set_RulerState_Table = function(markup)
    {        
    }

    this.Set_RulerState_Paragraph = function(obj, margins)
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

    // вот оооочень важная функция. она выкидывает из кэша неиспользуемые шрифты
    this.CheckFontCache = function()
    {        
    }

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

    this.LockSlide = function(slideNum)
    {
    }

    this.UnLockSlide = function(slideNum)
    {
    }

    this.DrawTrackSelectShapes = function(x, y, w, h)
    {
    }

    this.DrawAdjustment = function(matrix, x, y)
    {
    }

    // cursor
    this.UpdateTargetTransform = function(matrix)
    {
        this.TextMatrix = matrix;
    }

    this.UpdateThumbnailsAttack = function()
    {
    }

    this.CheckGuiControlColors = function(bIsAttack)
    {        
    }

    this.SendControlColors = function(bIsAttack)
    {        
    }

    this.SendThemeColorScheme = function()
    {        
    }

    this.DrawImageTextureFillShape = function(url)
    {
    }

    this.DrawImageTextureFillSlide = function(url)
    {        
    }

    this.InitGuiCanvasShape = function(div_id)
    {        
    }

    this.InitGuiCanvasSlide = function(div_id)
    {        
    }

    this.CheckTableStyles = function(tableLook, gr_frame)
    {        
    }

    this.OnSelectEnd = function()
    {
    }

    this.GetCommentWidth = function(type)
    {
        var _index = 0;
        if ((type & 0x02) == 0x02)
            _index = 2;
        if ((type & 0x01) == 0x01)
            _index += 1;

        return g_comment_image_offsets[_index][2] * g_dKoef_pix_to_mm * 100 / this.m_oWordControl.m_nZoomValue;
    }

    this.GetCommentHeight = function(type)
    {
        var _index = 0;
        if ((type & 0x02) == 0x02)
            _index = 2;
        if ((type & 0x01) == 0x01)
            _index += 1;

        return g_comment_image_offsets[_index][3] * g_dKoef_pix_to_mm * 100 / this.m_oWordControl.m_nZoomValue;
    }

    this.DrawVerAnchor = function(xPos, bIsFromDrawings)
    {
    }

    this.DrawHorAnchor = function(yPos, bIsFromDrawings)
    {
    }

    this.DrawHorVerAnchor = function()
    {
    }
}

function CThPage()
{
    this.PageIndex = -1;

    this.left   = 0;
    this.top    = 0;
    this.right  = 0;
    this.bottom = 0;

    this.IsRecalc = false;
    this.cachedImage = null;

    this.IsSelected = false;
    this.IsFocused = false;
    this.IsLocked = false;

    this.Draw = function(context, xDst, yDst, wDst, hDst, contextW, contextH)
    {
    }
}

function CThumbnailsManager()
{
    this.m_oFontManager = new CFontManager();

    this.m_bIsScrollVisible = true;
    this.DigitWidths = new Array();

    this.backgroundColor = "#B0B0B0";
    this.overColor = "#D3D3D3";
    this.selectColor = "#FFD86B";
    this.selectoverColor = "#FFE065";

    this.SlideWidth = 297;
    this.SlideHeight = 210;

    this.SlidesCount = 0;

    this.m_dScrollY = 0;
    this.m_dScrollY_max = 0;

    this.m_bIsVisible = false;
    this.m_nCurrentPage = -1;
    this.m_bIsUpdate = false;
    this.m_arrPages = new Array();
    this.m_lDrawingFirst = -1;
    this.m_lDrawingEnd = -1;

    this.const_offsetX = 0;
    this.const_border_w = 4;

    this.bIsEmptyDrawed = false;


    this.FocusObjType = FOCUS_OBJECT_MAIN;
    this.LockMainObjType = false;

    this.SelectPageEnabled = true;

    this.m_oWordControl = null;
    var oThis = this;
    
    // инициализация шрифта
    this.SetFont = function(font)
    {
    }

    this.Init = function()
    {
    }

    this.CheckSizes = function()
    {        
    }

    this.verticalScroll = function(sender,scrollPositionY,maxY,isAtTop,isAtBottom)
    {
    }

    this.CalculatePlaces = function()
    {        
    }

    this.OnPaint = function()
    {        
    }

    this.OnUpdateOverlay = function()
    {        
    }

    this.FocusRectDraw = function(ctx, x, y, r, b)
    { 
    }

    this.onCheckUpdate = function()
    {        
    }

    this.SetFocusElement = function(type)
    {        
    }

    this.GetSelectedSlidesRange = function()
    {        
    }

    this.GetSelectedArray = function()
    {
    }

    this.CorrectShiftSelect = function(isTop, isEnd)
    {        
    }    
}

function DrawBackground(graphics, unifill, w, h)
{
    // первым делом рисуем белый рект!
    if (true)
    {
        // ну какой-то бэкграунд должен быть
        graphics.SetIntegerGrid(false);

        var _l = 0;
        var _t = 0;
        var _r = (0 + w);
        var _b = (0 + h);

        graphics._s();
        graphics._m(_l, _t);
        graphics._l(_r, _t);
        graphics._l(_r, _b);
        graphics._l(_l, _b);
        graphics._z();

        graphics.b_color1(255, 255, 255, 255);
        graphics.df();
        graphics._e();
    }

    if (unifill == null || unifill.fill == null)
        return;

    graphics.SetIntegerGrid(false);

    var _shape = new Object();

    _shape.brush = unifill;
    _shape.pen = null;
    _shape.TransformMatrix = new CMatrix();
    _shape.extX = w;
    _shape.extY = h;
    _shape.check_bounds = function(checker)
    {
        checker._s();
        checker._m(0, 0);
        checker._l(this.extX, 0);
        checker._l(this.extX, this.extY);
        checker._l(0, this.extY);
        checker._z();
        checker._e();
    }

    var shape_drawer = new CShapeDrawer();
    shape_drawer.fromShape2(_shape, graphics, null);
    shape_drawer.draw(null);
}

function CSlideDrawer()
{
    this.m_oWordControl     = null;
    this.CONST_MAX_SLIDE_CACHE_SIZE = 104857600; // 100 megabytes
    this.CONST_BORDER       = 10; // in px

    this.IsCached           = false;
    this.CachedCanvas       = null;
    this.CachedCanvasCtx    = null;

    this.BoundsChecker      = new CSlideBoundsChecker();

    this.bIsEmptyPresentation = false;
    this.IsRecalculateSlide = false;

    this.CheckSlide = function(slideNum)
    {
        this.bIsEmptyPresentation = false;
        if (-1 == slideNum)
            this.bIsEmptyPresentation = true;

        var dKoef = this.m_oWordControl.m_nZoomValue * g_dKoef_mm_to_pix / 100;
        if (this.m_oWordControl.bIsRetinaSupport)
            dKoef *= 2;

        var w_mm = this.m_oWordControl.m_oLogicDocument.Width;
        var h_mm = this.m_oWordControl.m_oLogicDocument.Height;
        var w_px = (w_mm * dKoef) >> 0;
        var h_px = (h_mm * dKoef) >> 0;

        this.BoundsChecker.init(w_px, h_px, w_mm, h_mm);
        this.BoundsChecker.transform(1,0,0,1,0,0);

        if (this.bIsEmptyPresentation)
        {
            this.BoundsChecker._s();
            this.BoundsChecker._m(0, 0);
            this.BoundsChecker._l(w_mm, 0);
            this.BoundsChecker._l(w_mm, h_mm);
            this.BoundsChecker._l(0, h_mm);
            this.BoundsChecker._z();

            return;
        }

        this.m_oWordControl.m_oLogicDocument.DrawPage(slideNum, this.BoundsChecker);        
    }

    this.DrawSlide = function(outputCtx, scrollX, scrollX_max, scrollY, scrollY_max, slideNum)
    {        
    }
}

window.g_comment_image = new Image();
window.g_comment_image.asc_complete = false;
window.g_comment_image.onload = function(){
    window.g_comment_image.asc_complete = true;
};
window.g_comment_image.src = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAR0AAAAlCAMAAABI6s09AAAABGdBTUEAALGPC/xhBQAAAMBQTFRF2Ypk03pP0nZJ1oNb68Ku03hL1X9U7MWy1X9V9d7T3JNv5bCW0nZJ9uXc////8dPE+vDq5rSb5a6U4aOF5K6T2Idg////1HxR6r6o03lN0nZJ6Lef+u7p8NDA0ndK////78++0nZJ8dPE////0nZJ1HtP2Ihh////03hM////////3JVy0nZJ0nZJ9+Xc0nZJ////3JVy3JVx////////AAAA3Zh38tbJ3ZZ08dXH4qaJ5a6U9N3R+/Tw////0nZJQeZHfgAAADZ0Uk5TZJaoUk+v3lzdEi9hDlIbKVN8eY54xTTnc/NKegRg9EJm8WQz7+3EFPYoSKborlkmCqeoJ00ATKvZ0wAAA3tJREFUaN7dmOdi2jAUhelukjaQkkGCGIHYbDAYNYOh93+r2mZIsq6WwVHD+RfukWx93OtjUsCxHoaE0fAB7yWvHFCcjB6JRI+jCc7kzMVfSEzD5zWj5yFdPuSXDLm9C7/nVOMyX5SvnDwRhZ4mWZz5+Dd0yJoToevTK7jNLxHzByryRYZcqemzK0fkbbWWaPVGRqxTqVH6tJ8/XbBfGPPVjVtlT/Tr9t/ToZ+l6bR2l2hxdITJQfLil6/syjqRwonwkDrrVKqePu15fy5XWfTr9s/eO+I0EvlYnRFuz7VCRHF1ZSdHavfOEIaEUHBZE/0XJbjTmuWfyf7Ze0ckqjgWeh86AVaoKPrlrVb6ztGx7h2RKLesRa8UUcUiHei0MJ2KePMVgY4+rQJj/7fzy0YZ6h2AzuacTYCOee8cRKcq0qmm78YgrZCNH/1w2zvHnSyTHOT9mjQsUjreK7vbq0d38fhVnqp3PFXvePnSMclB3q9Jw4DS+XNHFvHuq0X82d013SWqMGIrwjSia6B3dgPJrczhuWNC3Io7onQ6jfk0wrNazOJLNzp0l7iS2IWK0Duoo+gdbmUOz52j08GUTqQwwrOYhkAShjEesSKfRuVA5jRZJsTTO1fgMK8AdHA4+AvCiSsAHMU0KgfyP6JThelUITo4rIaS9yiwIp/GTXGW3NsUKEInUdGpAE+cd56s+EjS10xJRT6N8oHMQOdqzOjKFR17yadxgwcufsTnTjY80mlUFD/kcyeTOhmKXfWbW5d1KtW1nKyu5WR1D6WTRb76rd9nnUr5lnR8Szq+Czq1+/j6L0t698sXel/3tbRTJtZp8KT/5dWUz51Kmo5Xc0Gn3bxJRmaPZ8kMy02zLTrBseKcJnRabZ4Ol4VCGnp+q+2CTpD802m2x7Pc/k7ZqB8ATiqJ02CyEO/XTVa8vws6OLjtM3g4OP3bAHSKcHinCR3er6PTbwfYCZ1EvS2eBE5P69zB6R2agzZp6I7OFo8eDoNH7jTPQZs0dEgnOvRUfWQLp3kO2qShSzo4jA89nYdHcJrnoE0aOqUTHXpgBEfvNM9B1j9goQxEv1s60aHN4Oid5jnI+gcQHOp3TAeH4TGd5jm470gKB9jfNR1nOZjCA8I5NToWOcjhgeGcHB2LHGTwSOCcHh2LHNz7ZXBOkI5FDmr9J0jHIgd1/n8LiumvxDAoYwAAAABJRU5ErkJggg==";
var g_comment_image_offsets = [
    [5, 0, 16, 15],
    [31, 0, 16, 15],
    [57, 0, 19, 18],
    [86, 0, 19, 18],
    [115, 0, 32, 30],
    [157, 0, 32, 30],
    [199, 0, 38, 36],
    [247, 0, 38, 36]
];