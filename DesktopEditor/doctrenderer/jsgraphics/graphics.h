#ifndef CJSGRAPHICS_H
#define CJSGRAPHICS_H

#include <string>

#include "../../common/Types.h"
#include "../../../Common/3dParty/v8/v8/include/v8.h"
#include "../../../Common/3dParty/v8/v8/include/libplatform/libplatform.h"

struct CFont
{
    std::string Name;
    int FontSize;
    bool Bold;
    bool Italic;

    CFont() : Name(""), FontSize(10), Bold(false), Italic(false) {}
};

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
    v8::Local<v8::Value> m_oBrush;
    bool m_bBrushColorInit;

    v8::Local<v8::Value>* m_oFontManager;

    v8::Local<v8::Value> m_oCoordTransform;
    v8::Local<v8::Value> m_oTransform;
    v8::Local<v8::Value> m_oFullTransform;
    v8::Local<v8::Value> m_oInvertFullTransform;

    v8::Local<v8::Value>* ArrayPoints;

    CFont m_oCurFont;

    v8::Local<v8::Value>* m_oTextPr;
    v8::Local<v8::Value> m_oGrFonts;
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
    // brush methods
    void b_color1(int r, int g, int b, int a);
    void b_color2(int r, int g, int b, int a);
    void transform(double sx, double shy, double shx, double sy, double tx, double ty);
    void CalculateFullTransform(bool isInvertNeed);
    // path commands
    void _s();
    void _e();
    void _z();
    void _m(double x, double y);
    void _l(double x, double y);
    void _c(double x1, double y1, double x2, double y2, double x3, double y3);
    void _c2(double x1, double y1, double x2, double y2);
    void ds();
    void df();
    // canvas state
    void save();
    void restore();
    void clip();
    void reset();
    void transform3(const v8::Local<v8::Value>& m, bool isNeedInvert);
    void FreeFont();
    void ClearLastFont();
    // images
    void drawImage2(const v8::Local<v8::Value>& img, double x, double y, double w, double h, int alpha, const v8::Local<v8::Value>& srcRect);
    void drawImage(const v8::Local<v8::Value>& img, double x, double y, double w, double h, int alpha, const v8::Local<v8::Value>& srcRect, const v8::Local<v8::Value>& nativeImage);
    // text
    CFont GetFont();
    void font(unsigned int font_id, int font_size);
    void SetFont(const v8::Local<v8::Value>& font);
    void SetTextPr(v8::Local<v8::Value>* textPr, v8::Local<v8::Value>* theme);
    void SetFontSlot(const v8::Local<v8::Value>& slot, double fontSizeKoef);
    v8::Local<v8::Value> GetTextPr();
    void FillText(double x, double y, const v8::Local<v8::Value>& text);
    void t(const v8::Local<v8::Value>& text, double x, double y, bool isBounds);
    void FillText2(double x, double y, const v8::Local<v8::Value>& text, double cropX, double cropW);
    void t2(const v8::Local<v8::Value>& text, double x, double y, double cropX, double cropW);
    void FillTextCode(double x, double y, const v8::Local<v8::Value>& lUnicode);
    void tg(const v8::Local<v8::Value>& text, double x, double y);
    void charspace(const v8::Local<v8::Value>& space) {}

    double m_dDpiX_get() { return m_dDpiX; }
    int globalAlpha_get() { return globalAlpha; }
};

#endif // CJSGRAPHICS_H
