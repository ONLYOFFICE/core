(function(window, undefined) {

function CDocMetaSelection()
{
    this.Page1 = 0;
    this.Line1 = 0;
    this.Glyph1 = 0;

    this.Page2 = 0;
    this.Line2 = 0;
    this.Glyph2 = 0;

    this.IsSelection = false;
}

function CSpan()
{
    this.fontName = 0;
    this.fontSize = 0;

    this.colorR = 0;
    this.colorG = 0;
    this.colorB = 0;

    this.inner = "";

    this.CreateDublicate = function()
    {
        var ret = new CSpan();

        ret.fontName = this.fontName;
        ret.fontSize = this.fontSize;

        ret.colorR = this.colorR;
        ret.colorG = this.colorG;
        ret.colorB = this.colorB;

        ret.inner = this.inner;

        return ret;
    }
}

function CDocMeta()
{
	this.Pages = null;
	this.stream = null;
	this.Selection = new CDocMetaSelection();
}

CDocMeta.prototype =
{
	getStreamPage : function(pageNum)
    {
        return this.stream;
    },
	
	GetNearestPos : function(pageNum, x, y)
    {
        var page = this.Pages[pageNum];
        var s = this.getStreamPage(pageNum);
        s.Seek(page.start);

        // textline parameters
        var _line = -1;
        var _glyph = -1;
        var _minDist = 0xFFFFFF;

        // textline parameters
        var _lineX = 0;
        var _lineY = 0;
        var _lineEx = 0;
        var _lineEy = 0;
        var _lineAscent = 0;
        var _lineDescent = 0;
        var _lineWidth = 0;
        var _lineGidExist = false;
        var _linePrevCharX = 0;
        var _lineCharCount = 0;
        var _lineLastGlyphWidth = 0;
        var _arrayGlyphOffsets = [];

        var _numLine = -1;

        var _lenGls = 0;
        var tmp = 0;

        while (s.pos < page.end)
        {
            var command = s.GetUChar();

            switch (command)
            {
                
                case 80:
                {
                    if (0 != _lineCharCount)
                        _linePrevCharX += s.GetDouble2();

                    _arrayGlyphOffsets[_lineCharCount] = _linePrevCharX;

                    _lineCharCount++;

                    if (_lineGidExist)
                        s.Skip(4);
                    else
                        s.Skip(2);

                    if (0 == _lineWidth)
                        _lineLastGlyphWidth = s.GetDouble2();
                    else
                        s.Skip(2);

                    break;
                }
                
                case 160:
                {
                    // textline
                    _linePrevCharX = 0;
                    _lineCharCount = 0;
                    _lineWidth = 0;

                    _arrayGlyphOffsets.splice(0, _arrayGlyphOffsets.length);

                    ++_numLine;

                    var mask = s.GetUChar();
                    _lineX = s.GetDouble();
                    _lineY = s.GetDouble();

                    if ((mask & 0x01) != 0)
                    {
                        _lineEx = 1;
                        _lineEy = 0;
                    }
                    else
                    {
                        _lineEx = s.GetDouble();
                        _lineEy = s.GetDouble();
                    }

                    _lineAscent = s.GetDouble();
                    _lineDescent = s.GetDouble();

                    if ((mask & 0x04) != 0)
                        _lineWidth = s.GetDouble();

                    if ((mask & 0x02) != 0)
                        _lineGidExist = true;
                    else
                        _lineGidExist = false;

                    break;
                }
                case 162:
                {
                    // textline end

                    // все подсчитано
                    if (0 == _lineWidth)
                        _lineWidth = _linePrevCharX + _lineLastGlyphWidth;

                    // в принципе код один и тот же. Но почти всегда линии горизонтальные.
                    // а для горизонтальной линии все можно пооптимизировать
                    if (_lineEx == 1 && _lineEy == 0)
                    {
                        var _distX = x - _lineX;
                        if (y >= (_lineY - _lineAscent) && y <= (_lineY + _lineDescent) && _distX >= 0 && _distX <= _lineWidth)
                        {
                            // попали внутрь линии. Теперь нужно найти глиф
                            _line = _numLine;

                            _lenGls = _arrayGlyphOffsets.length;
                            for (_glyph = 0; _glyph < _lenGls; _glyph++)
                            {
                                if (_arrayGlyphOffsets[_glyph] > _distX)
                                    break;
                            }
                            if (_glyph > 0)
                                --_glyph;

                            return { Line : _line, Glyph : _glyph };
                        }

                        if (_distX >= 0 && _distX <= _lineWidth)
                            tmp = Math.abs(y - _lineY);
                        else if (_distX < 0)
                        {
                            tmp = Math.sqrt((x - _lineX) * (x - _lineX) + (y - _lineY) * (y - _lineY));
                        }
                        else
                        {
                            var _xx1 = _lineX + _lineWidth;
                            tmp = Math.sqrt((x - _xx1) * (x - _xx1) + (y - _lineY) * (y - _lineY));
                        }

                        if (tmp < _minDist)
                        {
                            _minDist = tmp;
                            _line = _numLine;

                            if (_distX < 0)
                                _glyph = -2;
                            else if (_distX > _lineWidth)
                            {
                                _glyph = -1;
                            }
                            else
                            {
                                _lenGls = _arrayGlyphOffsets.length;
                                for (_glyph = 0; _glyph < _lenGls; _glyph++)
                                {
                                    if (_arrayGlyphOffsets[_glyph] > _distX)
                                        break;
                                }

                                if (_glyph > 0)
                                    _glyph--;
                            }
                        }

                        // Ничего не надо делать, уже найдена более "ближняя" линия
                    }
                    else
                    {
                        // определяем точки descent линии
                        var ortX = -_lineEy;
                        var ortY = _lineEx;

                        var _dx = _lineX + ortX * _lineDescent;
                        var _dy = _lineY + ortY * _lineDescent;

                        // теперь проекции (со знаком) на линию descent
                        var h = -((x - _dx) * ortX + (y - _dy) * ortY);
                        var w = (x - _dx) * _lineEx + (y - _dy) * _lineEy;

                        if (w >= 0 && w <= _lineWidth && h >= 0 && h <= (_lineDescent + _lineAscent))
                        {
                            // попали внутрь линии. Теперь нужно найти глиф
                            _line = _numLine;

                            _lenGls = _arrayGlyphOffsets.length;
                            for (_glyph = 0; _glyph < _lenGls; _glyph++)
                            {
                                if (_arrayGlyphOffsets[_glyph] > w)
                                    break;
                            }

                            if (_glyph > 0)
                                _glyph--;

                            return { Line : _line, Glyph : _glyph };
                        }

                        if (w >= 0 && w <= _lineWidth)
                            tmp = Math.abs(h - _lineDescent);
                        else if (w < 0)
                        {
                            tmp = Math.sqrt((x - _lineX) * (x - _lineX) + (y - _lineY) * (y - _lineY));
                        }
                        else
                        {
                            var _tmpX = _lineX + _lineWidth * _lineEx;
                            var _tmpY = _lineY + _lineWidth * _lineEy;
                            tmp = Math.sqrt((x - _tmpX) * (x - _tmpX) + (y - _tmpY) * (y - _tmpY));
                        }

                        //tmp = Math.abs(h - _lineDescent);
                        if (tmp < _minDist)
                        {
                            _minDist = tmp;
                            _line = _numLine;

                            if (w < 0)
                                _glyph = -2;
                            else if (w > _lineWidth)
                                _glyph = -1;
                            else
                            {
                                _lenGls = _arrayGlyphOffsets.length;
                                for (_glyph = 0; _glyph < _lenGls; _glyph++)
                                {
                                    if (_arrayGlyphOffsets[_glyph] > w)
                                        break;
                                }

                                if (_glyph > 0)
                                    _glyph--;
                            }
                        }

                        // Ничего не надо делать, уже найдена более "ближняя" линия
                    }

                    break;
                }
                
            }
        }

        return { Line : _line, Glyph : _glyph };
    },

	OnMouseDown : function(page, x, y)
    {
        var ret = this.GetNearestPos(page, x, y);

        var sel = this.Selection;
        sel.Page1 = page;
        sel.Line1 = ret.Line;
        sel.Glyph1 = ret.Glyph;

        sel.Page2 = page;
        sel.Line2 = ret.Line;
        sel.Glyph2 = ret.Glyph;

        sel.IsSelection = true;
        this.OnUpdateSelection();
    },
	
	OnMouseMove : function(page, x, y)
    {
        if (false === this.Selection.IsSelection)
            return;

        var ret = this.GetNearestPos(page, x, y);

        var sel = this.Selection;
        sel.Page2 = page;
        sel.Line2 = ret.Line;
        sel.Glyph2 = ret.Glyph;

        this.OnUpdateSelection();
    },
	
	OnMouseUp : function()
    {
        this.Selection.IsSelection = false;
    },
	
	OnUpdateSelection : function()
    {
        editor.WordControl.m_oOverlayApi.Show();
        editor.WordControl.OnUpdateOverlay();
    },
}
})(window, undefined);