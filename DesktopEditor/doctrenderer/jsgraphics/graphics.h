#ifndef CJSGRAPHICS_H
#define CJSGRAPHICS_H

#include "../../common/Types.h"
#include "../../../Common/3dParty/v8/v8/include/v8.h"
#include "../../../Common/3dParty/v8/v8/include/libplatform/libplatform.h"

class CJSGraphics
{
private:
    v8::Local<v8::Value> m_oContext;
    double m_dWidthMM;
    double m_dHeightMM;
    double m_lWidthPix;
    double m_lHeightPix;
    double m_dDpiX;
    double m_dDpiY;

    v8::Local<v8::Value> m_oPen;
    bool m_bPenColorInit;
    bool m_bBrushColorInit;

    v8::Local<v8::Value> m_oCoordTransform;
    v8::Local<v8::Value> m_oFullTransform;

    v8::Local<v8::Value> m_oLastFont;

    bool m_bIntegerGrid;

    int TextureFillTransformScaleX;
    int TextureFillTransformScaleY;

    int globalAlpha = 1;

    v8::Local<v8::Value> dash_no_smart;
public:
    CJSGraphics();

    void init(const v8::Local<v8::Value>& context, double width_px, double height_px, double width_mm, double height_mm);
    void EndDraw() {}
    void put_GlobalAlpha(bool enable, int globalAlpha);
    void Start_GlobalAlpha() {}
    void End_GlobalAlpha();
    // pen methods
    void p_color(int r, int g, int b, int a);
    void p_width(int w);
    void p_dash(const v8::Local<v8::Value>& params);

    double m_dDpiX_get() { return m_dDpiX; }
    int globalAlpha_get() { return globalAlpha; }
};

#endif // CJSGRAPHICS_H
