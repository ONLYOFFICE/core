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
    // private methods
    void private_FillGlyph(const v8::Local<v8::Value>& pGlyph, const v8::Local<v8::Value>& _bounds);
    void private_FillGlyphC(const v8::Local<v8::Value>& pGlyph, double cropX, double cropW);
    void private_FillGlyph2(const v8::Local<v8::Value>& pGlyph);
    void SetIntegerGrid(bool param);
    bool GetIntegerGrid();
    void DrawStringASCII(const std::string& name, int size, bool bold, bool italic, const v8::Local<v8::Value>& text, double x, double y, bool bIsHeader);
    void DrawStringASCII2(const std::string& name, int size, bool bold, bool italic, const v8::Local<v8::Value>& text, double x, double y, bool bIsHeader);
    void DrawHeaderEdit(double yPos, const v8::Local<v8::Value>& lock_type, int sectionNum, bool bIsRepeat, const v8::Local<v8::Value>& type);
    void DrawFooterEdit(double yPos, const v8::Local<v8::Value>& lock_type, int sectionNum, bool bIsRepeat, const v8::Local<v8::Value>& type);
    void DrawLockParagraph(const v8::Local<v8::Value>& lock_type, double x, double y1, double y2);
    void DrawLockObjectRect(const v8::Local<v8::Value>& lock_type, double x, double y, double w, double h);
    void DrawEmptyTableLine(double x1, double y1, double x2, double y2);
    void DrawSpellingLine(double y0, double x0, double x1, double w);
    // smart methods for horizontal / vertical lines
    void drawHorLine(int align, double y, double x, double r, int penW);
    void drawHorLine2(int align, double y, double x, double r, int penW);
    void drawVerLine(int align, double x, double y, double b, int penW);
    // мега крутые функции для таблиц
    void drawHorLineExt(int align, double y, double x, double r, int penW, double leftMW, double rightMW);
    void rect(double x, double y, double w, double h);
    void TableRect(double x, double y, double w, double h);
    // функции клиппирования
    void AddClipRect(double x, double y, double w, double h);
    void RemoveClipRect() {}
    void SetClip(const v8::Local<v8::Value>& r);
    void RemoveClip();
    void drawCollaborativeChanges(double x, double y, double w, double h, const v8::Local<v8::Value>& Color);
    void drawMailMergeField(double x, double y, double w, double h);
    void drawSearchResult(double x, double y, double w, double h);
    void drawFlowAnchor(double x, double y);
    void SavePen();
    void RestorePen();
    void SaveBrush();
    void RestoreBrush();
    void SavePenBrush();
    void RestorePenBrush();
    void SaveGrState();
    void RestoreGrState();
    void StartClipPath() {}
    void EndClipPath();
    void StartCheckTableDraw();
    void EndCheckTableDraw(bool bIsRestore);
    void SetTextClipRect(double _l, double _t, double _r, double _b);
    void AddSmartRect(double x, double y, double w, double h, int pen_w);
    void CheckUseFonts2(const v8::Local<v8::Value>& _transform);
    void UncheckUseFonts2();
    void Drawing_StartCheckBounds(double x, double y, double w, double h) {}
    void Drawing_EndCheckBounds() {}
    void DrawPresentationComment(const v8::Local<v8::Value>& type, double x, double y, double w, double h);
    void DrawPolygon(const v8::Local<v8::Value>& oPath, int lineWidth, double shift);
    void DrawFootnoteRect(double x, double y, double w, double h);

    double m_dDpiX_get() { return m_dDpiX; }
    int globalAlpha_get() { return globalAlpha; }
};

#endif // CJSGRAPHICS_H
