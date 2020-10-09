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
                             "," + std::to_string((int)((double)_c.A / 255.0)) + ")";
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
