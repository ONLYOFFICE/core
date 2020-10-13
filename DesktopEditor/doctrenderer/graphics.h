#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include <string>

#include "../common/Types.h"
#include "v8.h"
#include "libplatform/libplatform.h"

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

namespace NSGraphics
{
    class CGraphics
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
        CGraphics()
        {
            m_oContext = nullptr;
            m_dWidthMM = 0.0;
            m_dHeightMM = 0.0;
            m_lWidthPix = 0.0;
            m_lHeightPix = 0.0;
            m_dDpiX = 96.0;
            m_dDpiY = 96.0;
            m_bIsBreak = false;

            m_oPen = nullptr; // new AscCommon.CPen();
            m_bPenColorInit = false;
            m_oBrush = nullptr; // new AscCommon.CBrush();
            m_bBrushColorInit = false;

            m_oFontManager = nullptr;

            m_oCoordTransform = nullptr; // new CMatrixL();
            m_oBaseTransform = nullptr; // new CMatrixL();
            m_oTransform = nullptr; // new CMatrixL();
            m_oFullTransform = nullptr; // new CMatrixL();
            m_oInvertFullTransform = nullptr; // new CMatrixL();

            ArrayPoints = nullptr;

            m_oTextPr = nullptr;
            m_oGrFonts = nullptr; // new AscCommon.CGrRFonts();
            m_oLastFont = nullptr; // new AscCommon.CFontSetup();

            m_bIntegerGrid = true;

            ClipManager = nullptr; // new AscCommon.CClipManager();
            // ClipManager.BaseObject = this;

            TextureFillTransformScaleX = 1;
            TextureFillTransformScaleY = 1;
            IsThumbnail = false;

            IsDemonstrationMode = false;

            GrState = nullptr; // new AscCommon.CGrState();
            // GrState.Parent = this;

            globalAlpha = 1;

            TextClipRect = nullptr;
            IsClipContext = false;

            IsUseFonts2 = false;
            m_oFontManager2 = nullptr;
            m_oLastFont2 = nullptr;

            ClearMode = false;
            IsRetina = false;

            dash_no_smart = nullptr;
        }
        ~CGraphics()
        {
            if(m_oContext)                 delete m_oContext;
            if(m_oPen)                     delete m_oPen;
            if(m_oBrush)                   delete m_oBrush;
            if(m_oFontManager)             delete m_oFontManager;
            if(m_oCoordTransform)          delete m_oCoordTransform;
            if(m_oBaseTransform)           delete m_oBaseTransform;
            if(m_oTransform)               delete m_oTransform;
            if(m_oFullTransform)           delete m_oFullTransform;
            if(m_oInvertFullTransform)     delete m_oInvertFullTransform;
            if(ArrayPoints)                delete ArrayPoints;
            if(m_oTextPr)                  delete m_oTextPr;
            if(m_oGrFonts)                 delete m_oGrFonts;
            if(m_oLastFont)                delete m_oLastFont;
            if(LastFontOriginInfo.Replace) delete LastFontOriginInfo.Replace;
            if(ClipManager)                delete ClipManager;
            if(GrState)                    delete GrState;
            if(TextClipRect)               delete TextClipRect;
            if(m_oFontManager2)            delete m_oFontManager2;
            if(m_oLastFont2)               delete m_oLastFont2;
            if(dash_no_smart)              delete dash_no_smart;
        }

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

        v8::Local<v8::Value>* g_m_oContext()             { return m_oContext;             }
        v8::Local<v8::Value>* g_m_oPen()                 { return m_oPen;                 }
        v8::Local<v8::Value>* g_m_oBrush()               { return m_oBrush;               }
        v8::Local<v8::Value>* g_m_oFontManager()         { return m_oFontManager;         }
        v8::Local<v8::Value>* g_m_oCoordTransform()      { return m_oCoordTransform;      }
        v8::Local<v8::Value>* g_m_oBaseTransform()       { return m_oBaseTransform;       }
        v8::Local<v8::Value>* g_m_oTransform()           { return m_oTransform;           }
        v8::Local<v8::Value>* g_m_oFullTransform()       { return m_oFullTransform;       }
        v8::Local<v8::Value>* g_m_oInvertFullTransform() { return m_oInvertFullTransform; }
        v8::Local<v8::Value>* g_ArrayPoints()            { return ArrayPoints;            }
        v8::Local<v8::Value>* g_m_oTextPr()              { return m_oTextPr;              }
        v8::Local<v8::Value>* g_m_oGrFonts()             { return m_oGrFonts;             }
        v8::Local<v8::Value>* g_m_oLastFont()            { return m_oLastFont;            }
        v8::Local<v8::Value>* g_ClipManager()            { return ClipManager;            }
        v8::Local<v8::Value>* g_GrState()                { return GrState;                }
        v8::Local<v8::Value>* g_TextClipRect()           { return TextClipRect;           }
        v8::Local<v8::Value>* g_m_oFontManager2()        { return m_oFontManager2;        }
        v8::Local<v8::Value>* g_m_oLastFont2()           { return m_oLastFont2;           }
        v8::Local<v8::Value>* g_dash_no_smart()          { return dash_no_smart;          }
        double g_m_dWidthMM()        { return m_dWidthMM;          }
        double g_m_dHeightMM()       { return m_dHeightMM;         }
        double g_m_lWidthPix()       { return m_lWidthPix;         }
        double g_m_lHeightPix()      { return m_lHeightPix;        }
        double g_m_dDpiX()           { return m_dDpiX;             }
        double g_m_dDpiY()           { return m_dDpiY;             }
        bool g_m_bIsBreak()          { return m_bIsBreak;          }
        bool g_m_bPenColorInit()     { return m_bPenColorInit;     }
        bool g_m_bBrushColorInit()   { return m_bBrushColorInit;   }
        bool g_m_bIntegerGrid()      { return m_bIntegerGrid;      }
        bool g_IsThumbnail()         { return IsThumbnail;         }
        bool g_IsDemonstrationMode() { return IsDemonstrationMode; }
        bool g_IsClipContext()       { return IsClipContext;       }
        bool g_IsUseFonts2()         { return IsUseFonts2;         }
        bool g_ClearMode()           { return ClearMode;           }
        bool g_IsRetina()            { return IsRetina;            }
        CFont               g_m_oCurFont()         { return m_oCurFont;         }
        CLastFontOriginInfo g_LastFontOriginInfo() { return LastFontOriginInfo; }
        int g_TextureFillTransformScaleX() { return TextureFillTransformScaleX; }
        int g_TextureFillTransformScaleY() { return TextureFillTransformScaleY; }
        int g_globalAlpha()                { return globalAlpha;                }

        void s_m_oContext            (v8::Local<v8::Value>* a) { m_oContext             = a; }
        void s_m_oPen                (v8::Local<v8::Value>* a) { m_oPen                 = a; }
        void s_m_oBrush              (v8::Local<v8::Value>* a) { m_oBrush               = a; }
        void s_m_oFontManager        (v8::Local<v8::Value>* a) { m_oFontManager         = a; }
        void s_m_oCoordTransform     (v8::Local<v8::Value>* a) { m_oCoordTransform      = a; }
        void s_m_oBaseTransform      (v8::Local<v8::Value>* a) { m_oBaseTransform       = a; }
        void s_m_oTransform          (v8::Local<v8::Value>* a) { m_oTransform           = a; }
        void s_m_oFullTransform      (v8::Local<v8::Value>* a) { m_oFullTransform       = a; }
        void s_m_oInvertFullTransform(v8::Local<v8::Value>* a) { m_oInvertFullTransform = a; }
        void s_ArrayPoints           (v8::Local<v8::Value>* a) { ArrayPoints            = a; }
        void s_m_oTextPr             (v8::Local<v8::Value>* a) { m_oTextPr              = a; }
        void s_m_oGrFonts            (v8::Local<v8::Value>* a) { m_oGrFonts             = a; }
        void s_m_oLastFont           (v8::Local<v8::Value>* a) { m_oLastFont            = a; }
        void s_ClipManager           (v8::Local<v8::Value>* a) { ClipManager            = a; }
        void s_GrState               (v8::Local<v8::Value>* a) { GrState                = a; }
        void s_TextClipRect          (v8::Local<v8::Value>* a) { TextClipRect           = a; }
        void s_m_oFontManager2       (v8::Local<v8::Value>* a) { m_oFontManager2        = a; }
        void s_m_oLastFont2          (v8::Local<v8::Value>* a) { m_oLastFont2           = a; }
        void s_dash_no_smart         (v8::Local<v8::Value>* a) { dash_no_smart          = a; }
        void s_m_dWidthMM       (double a) { m_dWidthMM          = a; }
        void s_m_dHeightMM      (double a) { m_dHeightMM         = a; }
        void s_m_lWidthPix      (double a) { m_lWidthPix         = a; }
        void s_m_lHeightPix     (double a) { m_lHeightPix        = a; }
        void s_m_dDpiX          (double a) { m_dDpiX             = a; }
        void s_m_dDpiY          (double a) { m_dDpiY             = a; }
        void s_m_bIsBreak         (bool a) { m_bIsBreak          = a; }
        void s_m_bPenColorInit    (bool a) { m_bPenColorInit     = a; }
        void s_m_bBrushColorInit  (bool a) { m_bBrushColorInit   = a; }
        void s_m_bIntegerGrid     (bool a) { m_bIntegerGrid      = a; }
        void s_IsThumbnail        (bool a) { IsThumbnail         = a; }
        void s_IsDemonstrationMode(bool a) { IsDemonstrationMode = a; }
        void s_IsClipContext      (bool a) { IsClipContext       = a; }
        void s_IsUseFonts2        (bool a) { IsUseFonts2         = a; }
        void s_ClearMode          (bool a) { ClearMode           = a; }
        void s_IsRetina           (bool a) { IsRetina            = a; }
        void s_m_oCurFont        (CFont a)               { m_oCurFont         = a; }
        void s_LastFontOriginInfo(CLastFontOriginInfo a) { LastFontOriginInfo = a; }
        void s_TextureFillTransformScaleX(int a) { TextureFillTransformScaleX = a; }
        void s_TextureFillTransformScaleY(int a) { TextureFillTransformScaleY = a; }
        void s_globalAlpha               (int a) { globalAlpha                = a; }
    };
}

#endif // CGRAPHICS_H
