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

struct CLastFontOriginInfo
{
    std::string Name;
    v8::Local<v8::Value>* Replace;

    CLastFontOriginInfo() : Name(""), Replace(nullptr) {}
};

class CJSGraphics
{
private:
    v8::Local<v8::Value>* m_oContext;
    double m_dWidthMM;
    double m_dHeightMM;
    double m_lWidthPix;
    double m_lHeightPix;
    double m_dDpiX;
    double m_dDpiY;
    bool m_bIsBreak;

    v8::Local<v8::Value>* m_oPen;
    bool m_bPenColorInit;
    v8::Local<v8::Value>* m_oBrush;
    bool m_bBrushColorInit;

    v8::Local<v8::Value>* m_oFontManager;

    v8::Local<v8::Value>* m_oCoordTransform;
    v8::Local<v8::Value>* m_oBaseTransform;
    v8::Local<v8::Value>* m_oTransform;
    v8::Local<v8::Value>* m_oFullTransform;
    v8::Local<v8::Value>* m_oInvertFullTransform;

    v8::Local<v8::Value>* ArrayPoints;

    CFont m_oCurFont;

    v8::Local<v8::Value>* m_oTextPr;
    v8::Local<v8::Value>* m_oGrFonts;
    v8::Local<v8::Value>* m_oLastFont;

    CLastFontOriginInfo LastFontOriginInfo;

    bool m_bIntegerGrid;

    v8::Local<v8::Value>* ClipManager;

    int TextureFillTransformScaleX;
    int TextureFillTransformScaleY;
    bool IsThumbnail;

    bool IsDemonstrationMode;

    v8::Local<v8::Value>* GrState;

    int globalAlpha;

    v8::Local<v8::Value>* TextClipRect;
    bool IsClipContext;

    bool IsUseFonts2;
    v8::Local<v8::Value>* m_oFontManager2;
    v8::Local<v8::Value>* m_oLastFont2;

    bool ClearMode;
    bool IsRetina;

    v8::Local<v8::Value>* dash_no_smart;
public:
    CJSGraphics();
    ~CJSGraphics();

    void init(v8::Local<v8::Value>* context, double width_px, double height_px, double width_mm, double height_mm);
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
    void SetTextPr(const v8::Local<v8::Value>& textPr, const v8::Local<v8::Value>& theme);
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

    v8::Local<v8::Value>* m_oContext_get()             { return m_oContext;             }
    v8::Local<v8::Value>* m_oPen_get()                 { return m_oPen;                 }
    v8::Local<v8::Value>* m_oBrush_get()               { return m_oBrush;               }
    v8::Local<v8::Value>* m_oFontManager_get()         { return m_oFontManager;         }
    v8::Local<v8::Value>* m_oCoordTransform_get()      { return m_oCoordTransform;      }
    v8::Local<v8::Value>* m_oBaseTransform_get()       { return m_oBaseTransform;       }
    v8::Local<v8::Value>* m_oTransform_get()           { return m_oTransform;           }
    v8::Local<v8::Value>* m_oFullTransform_get()       { return m_oFullTransform;       }
    v8::Local<v8::Value>* m_oInvertFullTransform_get() { return m_oInvertFullTransform; }
    v8::Local<v8::Value>* ArrayPoints_get()            { return ArrayPoints;            }
    v8::Local<v8::Value>* m_oTextPr_get()              { return m_oTextPr;              }
    v8::Local<v8::Value>* m_oGrFonts_get()             { return m_oGrFonts;             }
    v8::Local<v8::Value>* m_oLastFont_get()            { return m_oLastFont;            }
    v8::Local<v8::Value>* ClipManager_get()            { return ClipManager;            }
    v8::Local<v8::Value>* GrState_get()                { return GrState;                }
    v8::Local<v8::Value>* TextClipRect_get()           { return TextClipRect;           }
    v8::Local<v8::Value>* m_oFontManager2_get()        { return m_oFontManager2;        }
    v8::Local<v8::Value>* m_oLastFont2_get()           { return m_oLastFont2;           }
    v8::Local<v8::Value>* dash_no_smart_get()          { return dash_no_smart;          }
    double m_dWidthMM_get()        { return m_dWidthMM;          }
    double m_dHeightMM_get()       { return m_dHeightMM;         }
    double m_lWidthPix_get()       { return m_lWidthPix;         }
    double m_lHeightPix_get()      { return m_lHeightPix;        }
    double m_dDpiX_get()           { return m_dDpiX;             }
    double m_dDpiY_get()           { return m_dDpiY;             }
    bool m_bIsBreak_get()          { return m_bIsBreak;          }
    bool m_bPenColorInit_get()     { return m_bPenColorInit;     }
    bool m_bBrushColorInit_get()   { return m_bBrushColorInit;   }
    bool m_bIntegerGrid_get()      { return m_bIntegerGrid;      }
    bool IsThumbnail_get()         { return IsThumbnail;         }
    bool IsDemonstrationMode_get() { return IsDemonstrationMode; }
    bool IsClipContext_get()       { return IsClipContext;       }
    bool IsUseFonts2_get()         { return IsUseFonts2;         }
    bool ClearMode_get()           { return ClearMode;           }
    bool IsRetina_get()            { return IsRetina;            }
    CFont               m_oCurFont_get()         { return m_oCurFont;         }
    CLastFontOriginInfo LastFontOriginInfo_get() { return LastFontOriginInfo; }
    int TextureFillTransformScaleX_get() { return TextureFillTransformScaleX; }
    int TextureFillTransformScaleY_get() { return TextureFillTransformScaleY; }
    int globalAlpha_get()                { return globalAlpha;                }

    void m_oContext_set            (v8::Local<v8::Value>* a) { m_oContext             = a; }
    void m_oPen_set                (v8::Local<v8::Value>* a) { m_oPen                 = a; }
    void m_oBrush_set              (v8::Local<v8::Value>* a) { m_oBrush               = a; }
    void m_oFontManager_set        (v8::Local<v8::Value>* a) { m_oFontManager         = a; }
    void m_oCoordTransform_set     (v8::Local<v8::Value>* a) { m_oCoordTransform      = a; }
    void m_oBaseTransform_set      (v8::Local<v8::Value>* a) { m_oBaseTransform       = a; }
    void m_oTransform_set          (v8::Local<v8::Value>* a) { m_oTransform           = a; }
    void m_oFullTransform_set      (v8::Local<v8::Value>* a) { m_oFullTransform       = a; }
    void m_oInvertFullTransform_set(v8::Local<v8::Value>* a) { m_oInvertFullTransform = a; }
    void ArrayPoints_set           (v8::Local<v8::Value>* a) { ArrayPoints            = a; }
    void m_oTextPr_set             (v8::Local<v8::Value>* a) { m_oTextPr              = a; }
    void m_oGrFonts_set            (v8::Local<v8::Value>* a) { m_oGrFonts             = a; }
    void m_oLastFont_set           (v8::Local<v8::Value>* a) { m_oLastFont            = a; }
    void ClipManager_set           (v8::Local<v8::Value>* a) { ClipManager            = a; }
    void GrState_set               (v8::Local<v8::Value>* a) { GrState                = a; }
    void TextClipRect_set          (v8::Local<v8::Value>* a) { TextClipRect           = a; }
    void m_oFontManager2_set       (v8::Local<v8::Value>* a) { m_oFontManager2        = a; }
    void m_oLastFont2_set          (v8::Local<v8::Value>* a) { m_oLastFont2           = a; }
    void dash_no_smart_set         (v8::Local<v8::Value>* a) { dash_no_smart          = a; }
    void m_dWidthMM_set       (double a) { m_dWidthMM          = a; }
    void m_dHeightMM_set      (double a) { m_dHeightMM         = a; }
    void m_lWidthPix_set      (double a) { m_lWidthPix         = a; }
    void m_lHeightPix_set     (double a) { m_lHeightPix        = a; }
    void m_dDpiX_set          (double a) { m_dDpiX             = a; }
    void m_dDpiY_set          (double a) { m_dDpiY             = a; }
    void m_bIsBreak_set         (bool a) { m_bIsBreak          = a; }
    void m_bPenColorInit_set    (bool a) { m_bPenColorInit     = a; }
    void m_bBrushColorInit_set  (bool a) { m_bBrushColorInit   = a; }
    void m_bIntegerGrid_set     (bool a) { m_bIntegerGrid      = a; }
    void IsThumbnail_set        (bool a) { IsThumbnail         = a; }
    void IsDemonstrationMode_set(bool a) { IsDemonstrationMode = a; }
    void IsClipContext_set      (bool a) { IsClipContext       = a; }
    void IsUseFonts2_set        (bool a) { IsUseFonts2         = a; }
    void ClearMode_set          (bool a) { ClearMode           = a; }
    void IsRetina_set           (bool a) { IsRetina            = a; }
    void m_oCurFont_set        (CFont a)               { m_oCurFont         = a; }
    void LastFontOriginInfo_set(CLastFontOriginInfo a) { LastFontOriginInfo = a; }
    void TextureFillTransformScaleX_set(int a) { TextureFillTransformScaleX = a; }
    void TextureFillTransformScaleY_set(int a) { TextureFillTransformScaleY = a; }
    void globalAlpha_set               (int a) { globalAlpha                = a; }
};

#endif // CJSGRAPHICS_H
