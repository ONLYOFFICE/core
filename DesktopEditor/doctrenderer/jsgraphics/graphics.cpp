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

void CJSGraphics::_ds()
{
    // m_oContext.stroke();
}

void CJSGraphics::_df()
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
