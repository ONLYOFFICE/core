#include "graphics.h"

#include <string>
#include <cmath>

CJSGraphics::CJSGraphics()
{

}

void CJSGraphics::init(const v8::Local<v8::Value>& context, double width_px, double height_px, double width_mm, double height_mm)
{
    m_oContext   = context;
    m_lHeightPix = height_px; // height_px >> 0;
    m_lWidthPix  = width_px; // width_px >> 0;
    m_dWidthMM   = width_mm;
    m_dHeightMM  = height_mm;
    m_dDpiX      = 25.4 * m_lWidthPix / m_dWidthMM;
    m_dDpiY      = 25.4 * m_lHeightPix / m_dHeightMM;

    /*
    m_oCoordTransform.sx = m_dDpiX / 25.4;
    m_oCoordTransform.sy = m_dDpiY / 25.4;

    TextureFillTransformScaleX = 1 / m_oCoordTransform.sx;
    TextureFillTransformScaleY = 1 / m_oCoordTransform.sy;

    m_oLastFont.Clear();
    m_oContext.save();
    */

    m_bPenColorInit = false;
    m_bBrushColorInit = false;
}

void CJSGraphics::put_GlobalAlpha(bool enable, int alpha)
{
    if(!enable)
    {
        globalAlpha = 1;
        // m_oContext.globalAlpha = 1;
    }
    else
    {
        globalAlpha = alpha;
        // m_oContext.globalAlpha = alpha;
    }
}

void CJSGraphics::End_GlobalAlpha()
{
    if (!m_bIntegerGrid)
    {
        // m_oContext.setTransform(1,0,0,1,0,0);
    }

    // ДРУГОЙ МЕТОД КЛАССА
    // b_color1(255, 255, 255, 140);

    // m_oContext.fillRect(0, 0, m_lWidthPix, m_lHeightPix);
    // m_oContext.beginPath();

    if (!m_bIntegerGrid)
    {
        /*
        m_oContext.setTransform(m_oFullTransform.sx, m_oFullTransform.shy, m_oFullTransform.shx,
                                m_oFullTransform.sy, m_oFullTransform.tx, m_oFullTransform.ty);
        */
    }
}

void CJSGraphics::p_color(int r, int g, int b, int a)
{
    /*
    var _c = m_oPen.Color;
    if (m_bPenColorInit && _c.R == r && _c.G == g && _c.B == b && _c.A == a)
        return;

    m_bPenColorInit = true;
    _c.R = r;
    _c.G = g;
    _c.B = b;
    _c.A = a;

    m_oContext.strokeStyle = "rgba(" + std::to_string(_c.R) + "," + std::to_string(_c.G) + "," + std::to_string(_c.B) +
                             "," + std::to_string(static_cast<int>(static_cast<double>(_c.A) / 255.0)) + ")";
    */
}

void CJSGraphics::p_width(int w)
{
    // m_oPen.LineWidth = static_cast<int>(static_cast<double>(w) / 1000.0);

    if (!m_bIntegerGrid)
    {
        /*
        if (0 != m_oPen.LineWidth)
        {
            m_oContext.lineWidth = m_oPen.LineWidth;
        }
        else
        {
            double _x1 = m_oFullTransform.TransformPointX(0, 0);
            double _y1 = m_oFullTransform.TransformPointY(0, 0);
            double _x2 = m_oFullTransform.TransformPointX(1, 1);
            double _y2 = m_oFullTransform.TransformPointY(1, 1);

            double _koef = sqrt(((_x2 - _x1) * (_x2 - _x1) + (_y2 - _y1) * (_y2 - _y1)) / 2);
            m_oContext.lineWidth = static_cast<int>(1.0 / _koef);
        }
        */
    }
    else
    {
        /*
        if (0 != m_oPen.LineWidth)
        {
            v8::Local<v8::Value> _m = m_oFullTransform;
            double x = _m.sx + _m.shx;
            double y = _m.sy + _m.shy;

            double koef = sqrt((x * x + y * y) / 2);
            m_oContext.lineWidth = static_cast<int>(m_oPen.LineWidth * koef);
        }
        else
        {
            m_oContext.lineWidth = 1;
        }
        */
    }
}

void CJSGraphics::p_dash(const v8::Local<v8::Value>& params)
{
    /*
    if (!m_oContext.setLineDash)
        return;

    dash_no_smart = params ? params.slice() : null;
    m_oContext.setLineDash(params ? params : []);
    */
}

void CJSGraphics::b_color1(int r, int g, int b, int a)
{
    /*
    var _c = m_oBrush.Color1;
    if (m_bBrushColorInit && _c.R == r && _c.G == g && _c.B == b && _c.A == a)
        return;

    m_bBrushColorInit = true;

    _c.R = r;
    _c.G = g;
    _c.B = b;
    _c.A = a;

    m_oContext.fillStyle = "rgba(" + std::to_string(_c.R) + "," + std::to_string(_c.G) + "," + std::to_string(_c.B) +
                           "," + std::to_string(static_cast<int>(static_cast<double>(_c.A) / 255.0)) + ")";
    */
}

void CJSGraphics::b_color2(int r, int g, int b, int a)
{
    /*
    var _c = m_oBrush.Color2;
    _c.R = r;
    _c.G = g;
    _c.B = b;
    _c.A = a;
    */
}

void CJSGraphics::transform(double sx, double shy, double shx, double sy, double tx, double ty)
{
    /*
    v8::Local<v8::Value> _t = m_oTransform;
    _t.sx    = sx;
    _t.shx   = shx;
    _t.shy   = shy;
    _t.sy    = sy;
    _t.tx    = tx;
    _t.ty    = ty;

    CalculateFullTransform();
    if (!m_bIntegerGrid)
    {
        v8::Local<v8::Value> _ft = m_oFullTransform;
        m_oContext.setTransform(_ft.sx, _ft.shy, _ft.shx, _ft.sy, _ft.tx, s_ft.ty);
    }

    if (NULL != m_oFontManager)
        m_oFontManager.SetTextMatrix(_t.sx, _t.shy, _t.shx, _t.sy, _t.tx, _t.ty);
    */
}

void CJSGraphics::CalculateFullTransform(bool isInvertNeed)
{
    /*
    v8::Local<v8::Value> _ft = m_oFullTransform;
    v8::Local<v8::Value> _t  = m_oTransform;
    _ft.sx  = _t.sx;
    _ft.shx = _t.shx;
    _ft.shy = _t.shy;
    _ft.sy  = _t.sy;
    _ft.tx  = _t.tx;
    _ft.ty  = _t.ty;
    global_MatrixTransformer.MultiplyAppend(_ft, m_oCoordTransform);

    v8::Local<v8::Value> _it = m_oInvertFullTransform;
    _it.sx = _ft.sx;
    _it.shx = _ft.shx;
    _it.shy = _ft.shy;
    _it.sy = _ft.sy;
    _it.tx = _ft.tx;
    _it.ty = _ft.ty;

    if (!isInvertNeed)
    {
        global_MatrixTransformer.MultiplyAppendInvert(_it, _t);
    }
    */
}

void CJSGraphics::_s()
{
    // m_oContext.beginPath();
}

void CJSGraphics::_e()
{
    // m_oContext.beginPath();
}

void CJSGraphics::_z()
{
    // m_oContext.closePath();
}

void CJSGraphics::_m(double x, double y)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.moveTo(x, y);

        if (ArrayPoints != NULL)
            ArrayPoints[ArrayPoints.length] = {x: x, y: y};
    }
    else
    {
        var _x = (m_oFullTransform.TransformPointX(x, y)) >> 0;
        var _y = (m_oFullTransform.TransformPointY(x, y)) >> 0;
        m_oContext.moveTo(_x + 0.5, _y + 0.5);
    }
    */
}

void CJSGraphics::_l(double x, double y)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.lineTo(x, y);

        if (ArrayPoints != NULL)
            ArrayPoints[ArrayPoints.length] = {x: x, y: y};
    }
    else
    {
        var _x = (m_oFullTransform.TransformPointX(x, y)) >> 0;
        var _y = (m_oFullTransform.TransformPointY(x, y)) >> 0;
        m_oContext.lineTo(_x + 0.5, _y + 0.5);
    }
    */
}

void CJSGraphics::_c(double x1, double y1, double x2, double y2, double x3, double y3)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.bezierCurveTo(x1, y1, x2 ,y2, x3, y3);

        if (ArrayPoints != NULL)
        {
            ArrayPoints[ArrayPoints.length] = {x: x1, y: y1};
            ArrayPoints[ArrayPoints.length] = {x: x2, y: y2};
            ArrayPoints[ArrayPoints.length] = {x: x3, y: y3};
        }
    }
    else
    {
        var _x1 = (m_oFullTransform.TransformPointX(x1, y1)) >> 0;
        var _y1 = (m_oFullTransform.TransformPointY(x1, y1)) >> 0;

        var _x2 = (m_oFullTransform.TransformPointX(x2, y2)) >> 0;
        var _y2 = (m_oFullTransform.TransformPointY(x2, y2)) >> 0;

        var _x3 = (m_oFullTransform.TransformPointX(x3, y3)) >> 0;
        var _y3 = (m_oFullTransform.TransformPointY(x3, y3)) >> 0;
        m_oContext.bezierCurveTo(_x1 + 0.5, _y1 + 0.5, _x2 + 0.5, _y2 + 0.5, _x3 + 0.5, _y3 + 0.5);
    }
    */
}

void CJSGraphics::_c2(double x1, double y1, double x2, double y2)
{
    /*
    if (!m_bIntegerGrid)
    {
        m_oContext.quadraticCurveTo(x1, y1, x2, y2);

        if (ArrayPoints != NULL)
        {
            ArrayPoints[ArrayPoints.length] = {x: x1, y: y1};
            ArrayPoints[ArrayPoints.length] = {x: x2, y: y2};
        }
    }
    else
    {
        var _x1 = (m_oFullTransform.TransformPointX(x1, y1)) >> 0;
        var _y1 = (m_oFullTransform.TransformPointY(x1, y1)) >> 0;

        var _x2 = (m_oFullTransform.TransformPointX(x2, y2)) >> 0;
        var _y2 = (m_oFullTransform.TransformPointY(x2, y2)) >> 0;

        m_oContext.quadraticCurveTo(_x1 + 0.5, _y1 + 0.5, _x2 + 0.5, _y2 + 0.5);
    }
    */
}

void CJSGraphics::ds()
{
    // m_oContext.stroke();
}

void CJSGraphics::df()
{
    // m_oContext.fill();
}

void CJSGraphics::save()
{
    // m_oContext.save();
}

void CJSGraphics::restore()
{
    // m_oContext.restore();

    m_bPenColorInit = false;
    m_bBrushColorInit = false;
}

void CJSGraphics::clip()
{
    // m_oContext.clip();
}

void CJSGraphics::reset()
{
    // m_oTransform.Reset();
    CalculateFullTransform(false);

    /*
    if (!m_bIntegerGrid)
        m_oContext.setTransform(m_oCoordTransform.sx, 0, 0, m_oCoordTransform.sy, 0, 0);
    */
}

void CJSGraphics::transform3(const v8::Local<v8::Value>& m, bool isNeedInvert)
{
    /*
    v8::Local<v8::Value> _t = m_oTransform;
    _t.sx  = m.sx;
    _t.shx = m.shx;
    _t.shy = m.shy;
    _t.sy  = m.sy;
    _t.tx  = m.tx;
    _t.ty  = m.ty;
    CalculateFullTransform(isNeedInvert);

    if (!m_bIntegerGrid)
    {
        var _ft = m_oFullTransform;
        m_oContext.setTransform(_ft.sx, _ft.shy, _ft.shx, _ft.sy, _ft.tx, _ft.ty);
    }
    else
        SetIntegerGrid(false);
    */
}

void CJSGraphics::FreeFont()
{
    // m_oFontManager.m_pFont = null;
}

void CJSGraphics::ClearLastFont()
{
    /*
    m_oLastFont  = new AscCommon.CFontSetup();
    m_oLastFont2 = NULL;
    */
}

void CJSGraphics::drawImage2(const v8::Local<v8::Value>& img, double x, double y, double w, double h, int alpha, const v8::Local<v8::Value>& srcRect)
{
    /*
    if (srcRect)
    {
        // test on need draw:
        if (srcRect.l >= 100 || srcRect.t >= 100)
            return;
        if (srcRect.r <= 0 || srcRect.b <= 0)
            return;
    }

    var isA = (undefined !== alpha && null != alpha && 255 != alpha);
    var _oldGA = 0;
    if (isA)
    {
        _oldGA = this.m_oContext.globalAlpha;
        this.m_oContext.globalAlpha = alpha / 255;
    }

    if (false === this.m_bIntegerGrid)
    {
        if (!srcRect)
        {
            // тут нужно проверить, можно ли нарисовать точно. т.е. может картинка ровно такая, какая нужна.
            if (!global_MatrixTransformer.IsIdentity2(this.m_oTransform))
            {
                this.m_oContext.drawImage(img,x,y,w,h);
            }
            else
            {
                var xx = this.m_oFullTransform.TransformPointX(x, y);
                var yy = this.m_oFullTransform.TransformPointY(x, y);
                var rr = this.m_oFullTransform.TransformPointX(x + w, y + h);
                var bb = this.m_oFullTransform.TransformPointY(x + w, y + h);
                var ww = rr - xx;
                var hh = bb - yy;

                if (Math.abs(img.width - ww) < 2 && Math.abs(img.height - hh) < 2)
                {
                    // рисуем точно
                    this.m_oContext.setTransform(1, 0, 0, 1, 0, 0);
                    this.m_oContext.drawImage(img, xx >> 0, yy >> 0);

                    var _ft = this.m_oFullTransform;
                    this.m_oContext.setTransform(_ft.sx,_ft.shy,_ft.shx,_ft.sy,_ft.tx,_ft.ty);

                }
                else
                {
                    this.m_oContext.drawImage(img,x,y,w,h);
                }
            }
        }
        else
        {
            var _w = img.width;
            var _h = img.height;
            if (_w > 0 && _h > 0)
            {
                var _sx = 0;
                var _sy = 0;
                var _sr = _w;
                var _sb = _h;

                var _l = srcRect.l;
                var _t = srcRect.t;
                var _r = 100 - srcRect.r;
                var _b = 100 - srcRect.b;

                _sx += _l * _w / 100;
                _sr -= _r * _w / 100;
                _sy += _t * _h / 100;
                _sb -= _b * _h / 100;

                var naturalW = _w;
                naturalW -= _sx;
                naturalW += (_sr - _w);

                var naturalH = _h;
                naturalH -= _sy;
                naturalH += (_sb - _h);

                var tmpW = w;
                var tmpH = h;
                if (_sx < 0)
                {
                    x += (-_sx * tmpW / naturalW);
                    w -= (-_sx * tmpW / naturalW);
                    _sx = 0;
                }
                if (_sy < 0)
                {
                    y += (-_sy * tmpH / naturalH);
                    h -= (-_sy * tmpH / naturalH);
                    _sy = 0;
                }
                if (_sr > _w)
                {
                    w -= ((_sr - _w) * tmpW / naturalW);
                    _sr = _w;
                }
                if (_sb > _h)
                {
                    h -= ((_sb - _h) * tmpH / naturalH);
                    _sb = _h;
                }

                if (_sx >= _sr || _sx >= _w || _sr <= 0 || w <= 0)
                    return;
                if (_sy >= _sb || _sy >= _h || _sb <= 0 || h <= 0)
                    return;

                this.m_oContext.drawImage(img,_sx,_sy,_sr-_sx,_sb-_sy,x,y,w,h);
            }
            else
            {
                this.m_oContext.drawImage(img,x,y,w,h);
            }
        }
    }
    else
    {
        var _x1 = (this.m_oFullTransform.TransformPointX(x,y)) >> 0;
        var _y1 = (this.m_oFullTransform.TransformPointY(x,y)) >> 0;
        var _x2 = (this.m_oFullTransform.TransformPointX(x+w,y+h)) >> 0;
        var _y2 = (this.m_oFullTransform.TransformPointY(x+w,y+h)) >> 0;

        x = _x1;
        y = _y1;
        w = _x2 - _x1;
        h = _y2 - _y1;

        if (!srcRect)
        {
            // тут нужно проверить, можно ли нарисовать точно. т.е. может картинка ровно такая, какая нужна.
            if (!global_MatrixTransformer.IsIdentity2(this.m_oTransform))
            {
                this.m_oContext.drawImage(img,_x1,_y1,w,h);
            }
            else
            {
                if (Math.abs(img.width - w) < 2 && Math.abs(img.height - h) < 2)
                {
                    // рисуем точно
                    this.m_oContext.drawImage(img, x, y);
                }
                else
                {
                    this.m_oContext.drawImage(img,_x1,_y1,w,h);
                }
            }
        }
        else
        {
            var _w = img.width;
            var _h = img.height;
            if (_w > 0 && _h > 0)
            {
                var __w = w;
                var __h = h;
                var _delW = Math.max(0, -srcRect.l) + Math.max(0, srcRect.r - 100) + 100;
                var _delH = Math.max(0, -srcRect.t) + Math.max(0, srcRect.b - 100) + 100;

                var _sx = 0;
                if (srcRect.l > 0 && srcRect.l < 100)
                    _sx = Math.min((_w * srcRect.l / 100) >> 0, _w - 1);
                else if (srcRect.l < 0)
                {
                    var _off = ((-srcRect.l / _delW) * __w);
                    x += _off;
                    w -= _off;
                }
                var _sy = 0;
                if (srcRect.t > 0 && srcRect.t < 100)
                    _sy = Math.min((_h * srcRect.t / 100) >> 0, _h - 1);
                else if (srcRect.t < 0)
                {
                    var _off = ((-srcRect.t / _delH) * __h);
                    y += _off;
                    h -= _off;
                }
                var _sr = _w;
                if (srcRect.r > 0 && srcRect.r < 100)
                    _sr = Math.max(Math.min((_w * srcRect.r / 100) >> 0, _w - 1), _sx);
                else if (srcRect.r > 100)
                {
                    var _off = ((srcRect.r - 100) / _delW) * __w;
                    w -= _off;
                }
                var _sb = _h;
                if (srcRect.b > 0 && srcRect.b < 100)
                    _sb = Math.max(Math.min((_h * srcRect.b / 100) >> 0, _h - 1), _sy);
                else if (srcRect.b > 100)
                {
                    var _off = ((srcRect.b - 100) / _delH) * __h;
                    h -= _off;
                }

                if ((_sr-_sx) > 0 && (_sb-_sy) > 0 && w > 0 && h > 0)
                    this.m_oContext.drawImage(img,_sx,_sy,_sr-_sx,_sb-_sy,x,y,w,h);
            }
            else
            {
                this.m_oContext.drawImage(img,x,y,w,h);
            }
        }
    }

    if (isA)
    {
        this.m_oContext.globalAlpha = _oldGA;
    }
    */
}

void CJSGraphics::drawImage(const v8::Local<v8::Value>& img, double x, double y, double w, double h, int alpha, const v8::Local<v8::Value>& srcRect, const v8::Local<v8::Value>& nativeImage)
{
    /*
    if (nativeImage)
    {
        this.drawImage2(nativeImage,x,y,w,h,alpha,srcRect);
        return;
    }

    var _img = editor.ImageLoader.map_image_index[img];
    if (_img != undefined && _img.Status == AscFonts.ImageLoadStatus.Loading)
    {
        // TODO: IMAGE_LOADING
    }
    else if (_img != undefined && _img.Image != null)
    {
        this.drawImage2(_img.Image,x,y,w,h,alpha,srcRect);
    }
    else
    {
        var _x = x;
        var _y = y;
        var _r = x+w;
        var _b = y+h;

        var ctx = this.m_oContext;
        var old_p = ctx.lineWidth;

        var bIsNoIntGrid = false;

        if (this.m_bIntegerGrid)
        {
            _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
            _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;
            _r = (this.m_oFullTransform.TransformPointX(x+w,y+h) >> 0) + 0.5;
            _b = (this.m_oFullTransform.TransformPointY(x+w,y+h) >> 0) + 0.5;

            ctx.lineWidth = 1;
        }
        else
        {
            if (global_MatrixTransformer.IsIdentity2(this.m_oTransform))
            {
                bIsNoIntGrid = true;

                this.SetIntegerGrid(true);
                _x = (this.m_oFullTransform.TransformPointX(x,y) >> 0) + 0.5;
                _y = (this.m_oFullTransform.TransformPointY(x,y) >> 0) + 0.5;
                _r = (this.m_oFullTransform.TransformPointX(x+w,y+h) >> 0) + 0.5;
                _b = (this.m_oFullTransform.TransformPointY(x+w,y+h) >> 0) + 0.5;

                ctx.lineWidth = 1;
            }
            else
            {
                ctx.lineWidth = 1 / this.m_oCoordTransform.sx;
            }
        }

        //ctx.strokeStyle = "#FF0000";
        ctx.strokeStyle = "#F98C76";

        ctx.beginPath();
        ctx.moveTo(_x,_y);
        ctx.lineTo(_r,_b);
        ctx.moveTo(_r,_y);
        ctx.lineTo(_x,_b);
        ctx.stroke();

        ctx.beginPath();
        ctx.moveTo(_x,_y);
        ctx.lineTo(_r,_y);
        ctx.lineTo(_r,_b);
        ctx.lineTo(_x,_b);
        ctx.closePath();

        ctx.stroke();
        ctx.beginPath();

        if (bIsNoIntGrid)
            this.SetIntegerGrid(false);

        ctx.lineWidth = old_p;
        ctx.strokeStyle = "rgba(" + this.m_oPen.Color.R + "," + this.m_oPen.Color.G + "," +
                this.m_oPen.Color.B + "," + (this.m_oPen.Color.A / 255) + ")";
    }
    */
}

CFont CJSGraphics::GetFont()
{
    return m_oCurFont;
}

void CJSGraphics::font(unsigned int font_id, int font_size)
{
    /*
    AscFonts.g_font_infos[AscFonts.g_map_font_index[font_id]].LoadFont(editor.FontLoader, this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager,
    Math.max(font_size, 1), 0, this.m_dDpiX, this.m_dDpiY, this.m_oTransform);
    */
}

void CJSGraphics::SetFont(const v8::Local<v8::Value>& font)
{
    /*
    if (null == font)
        return;

    this.m_oCurFont.Name        = font.FontFamily.Name;
    this.m_oCurFont.FontSize    = font.FontSize;
    this.m_oCurFont.Bold        = font.Bold;
    this.m_oCurFont.Italic      = font.Italic;

    var bItalic = true === font.Italic;
    var bBold   = true === font.Bold;

    var oFontStyle = FontStyle.FontStyleRegular;
    if ( !bItalic && bBold )
        oFontStyle = FontStyle.FontStyleBold;
    else if ( bItalic && !bBold )
        oFontStyle = FontStyle.FontStyleItalic;
    else if ( bItalic && bBold )
        oFontStyle = FontStyle.FontStyleBoldItalic;

    var _last_font = this.IsUseFonts2 ? this.m_oLastFont2 : this.m_oLastFont;
    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    _last_font.SetUpName = font.FontFamily.Name;
    _last_font.SetUpSize = font.FontSize;
    _last_font.SetUpStyle = oFontStyle;

    g_fontApplication.LoadFont(_last_font.SetUpName, AscCommon.g_font_loader, _font_manager, font.FontSize, oFontStyle, this.m_dDpiX, this.m_dDpiY, this.m_oTransform, this.LastFontOriginInfo);

    var _mD = _last_font.SetUpMatrix;
    var _mS = this.m_oTransform;

    _mD.sx = _mS.sx;
    _mD.sy = _mS.sy;
    _mD.shx = _mS.shx;
    _mD.shy = _mS.shy;
    _mD.tx = _mS.tx;
    _mD.ty = _mS.ty;
    */
}

void CJSGraphics::SetTextPr(v8::Local<v8::Value>* textPr, v8::Local<v8::Value>* theme)
{
    /*
    m_oTextPr = textPr;
    if (theme)
        m_oGrFonts.checkFromTheme(theme.themeElements.fontScheme, m_oTextPr.RFonts);
    else
        m_oGrFonts = m_oTextPr.RFonts;
    */
}

void CJSGraphics::SetFontSlot(const v8::Local<v8::Value>& slot, double fontSizeKoef)
{
    /*
    var _rfonts = this.m_oGrFonts;
    var _lastFont = this.IsUseFonts2 ? this.m_oLastFont2 : this.m_oLastFont;

    switch (slot)
    {
    case fontslot_ASCII:
    {
        _lastFont.Name   = _rfonts.Ascii.Name;
        _lastFont.Size = this.m_oTextPr.FontSize;
        _lastFont.Bold = this.m_oTextPr.Bold;
        _lastFont.Italic = this.m_oTextPr.Italic;

        break;
    }
    case fontslot_CS:
    {
        _lastFont.Name   = _rfonts.CS.Name;
        _lastFont.Size = this.m_oTextPr.FontSizeCS;
        _lastFont.Bold = this.m_oTextPr.BoldCS;
        _lastFont.Italic = this.m_oTextPr.ItalicCS;

        break;
    }
    case fontslot_EastAsia:
    {
        _lastFont.Name   = _rfonts.EastAsia.Name;
        _lastFont.Size = this.m_oTextPr.FontSize;
        _lastFont.Bold = this.m_oTextPr.Bold;
        _lastFont.Italic = this.m_oTextPr.Italic;

        break;
    }
    case fontslot_HAnsi:
    default:
    {
        _lastFont.Name   = _rfonts.HAnsi.Name;
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

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    if (_lastFont.Name != _lastFont.SetUpName || _lastFont.Size != _lastFont.SetUpSize || _style != _lastFont.SetUpStyle || !_font_manager.m_pFont)
    {
        _lastFont.SetUpName = _lastFont.Name;
        _lastFont.SetUpSize = _lastFont.Size;
        _lastFont.SetUpStyle = _style;

        g_fontApplication.LoadFont(_lastFont.SetUpName, AscCommon.g_font_loader, _font_manager, _lastFont.SetUpSize, _lastFont.SetUpStyle, this.m_dDpiX, this.m_dDpiY, this.m_oTransform, this.LastFontOriginInfo);

        var _mD = _lastFont.SetUpMatrix;
        var _mS = this.m_oTransform;
        _mD.sx = _mS.sx;
        _mD.sy = _mS.sy;
        _mD.shx = _mS.shx;
        _mD.shy = _mS.shy;
        _mD.tx = _mS.tx;
        _mD.ty = _mS.ty;
    }
    else
    {
        var _mD = _lastFont.SetUpMatrix;
        var _mS = this.m_oTransform;

        if (_mD.sx != _mS.sx || _mD.sy != _mS.sy || _mD.shx != _mS.shx || _mD.shy != _mS.shy || _mD.tx != _mS.tx || _mD.ty != _mS.ty)
        {
            _mD.sx = _mS.sx;
            _mD.sy = _mS.sy;
            _mD.shx = _mS.shx;
            _mD.shy = _mS.shy;
            _mD.tx = _mS.tx;
            _mD.ty = _mS.ty;

            _font_manager.SetTextMatrix(_mD.sx,_mD.shy,_mD.shx,_mD.sy,_mD.tx,_mD.ty);
        }
    }
    */
}

v8::Local<v8::Value> CJSGraphics::GetTextPr()
{
    return *m_oTextPr;
}

void CJSGraphics::FillText(double x, double y, const v8::Local<v8::Value>& text)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        var _code = text.charCodeAt(0);

        if (null != this.LastFontOriginInfo.Replace)
            _code = g_fontApplication.GetReplaceGlyph(_code, this.LastFontOriginInfo.Replace);

        _font_manager.LoadString4C(_code,_x,_y);
    }
    catch(err)
    {
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        this.private_FillGlyph(pGlyph);
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CJSGraphics::t(const v8::Local<v8::Value>& text, double x, double y, bool isBounds)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        _font_manager.LoadString2(text,_x,_y);
    }
    catch(err)
    {
    }

    this.m_oContext.setTransform(1,0,0,1,0,0);
    var _bounds = isBounds ? {x:100000, y:100000, r:-100000, b:-100000} : null;
    while (true)
    {
        var pGlyph = _font_manager.GetNextChar2();
        if (null == pGlyph)
            break;

        if (null != pGlyph.oBitmap)
        {
            this.private_FillGlyph(pGlyph, _bounds);
        }
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }

    return _bounds;
    */
}

void CJSGraphics::FillText2(double x, double y, const v8::Local<v8::Value>& text, double cropX, double cropW)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        var _code = text.charCodeAt(0);

        if (null != this.LastFontOriginInfo.Replace)
            _code = g_fontApplication.GetReplaceGlyph(_code, this.LastFontOriginInfo.Replace);

        _font_manager.LoadString4C(_code,_x,_y);
    }
    catch(err)
    {
    }

    this.m_oContext.setTransform(1,0,0,1,0,0);
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        this.private_FillGlyphC(pGlyph,cropX,cropW);
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CJSGraphics::t2(const v8::Local<v8::Value>& text, double x, double y, double cropX, double cropW)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        _font_manager.LoadString2(text,_x,_y);
    }
    catch(err)
    {
    }

    this.m_oContext.setTransform(1,0,0,1,0,0);
    while (true)
    {
        var pGlyph = _font_manager.GetNextChar2();
        if (null == pGlyph)
            break;

        if (null != pGlyph.oBitmap)
        {
            this.private_FillGlyphC(pGlyph,cropX,cropW);
        }
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CJSGraphics::FillTextCode(double x, double y, const v8::Local<v8::Value>& lUnicode)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        if (null != this.LastFontOriginInfo.Replace)
            lUnicode = g_fontApplication.GetReplaceGlyph(lUnicode, this.LastFontOriginInfo.Replace);

        _font_manager.LoadString4C(lUnicode,_x,_y);
    }
    catch(err)
    {
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        this.private_FillGlyph(pGlyph);
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}

void CJSGraphics::tg(const v8::Local<v8::Value>& text, double x, double y)
{
    /*
    if (this.m_bIsBreak)
        return;

    var _x = this.m_oInvertFullTransform.TransformPointX(x,y);
    var _y = this.m_oInvertFullTransform.TransformPointY(x,y);

    var _font_manager = this.IsUseFonts2 ? this.m_oFontManager2 : this.m_oFontManager;

    try
    {
        _font_manager.LoadString3C(text,_x,_y);
    }
    catch(err)
    {
    }

    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(1,0,0,1,0,0);
    }
    var pGlyph = _font_manager.m_oGlyphString.m_pGlyphsBuffer[0];
    if (null == pGlyph)
        return;

    if (null != pGlyph.oBitmap)
    {
        var _a = this.m_oBrush.Color1.A;
        if (255 != _a)
            this.m_oContext.globalAlpha = _a / 255;
        this.private_FillGlyph(pGlyph);

        if (255 != _a)
            this.m_oContext.globalAlpha = 1.0;
    }
    if (false === this.m_bIntegerGrid)
    {
        this.m_oContext.setTransform(this.m_oFullTransform.sx,this.m_oFullTransform.shy,this.m_oFullTransform.shx,
                                     this.m_oFullTransform.sy,this.m_oFullTransform.tx,this.m_oFullTransform.ty);
    }
    */
}
