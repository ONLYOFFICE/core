#ifndef CGRAPHICS_H
#define CGRAPHICS_H

#include <string>
#include <vector>

#include "../common/Types.h"
#include "../graphics/GraphicsRenderer.h"

#include "v8.h"
#include "libplatform/libplatform.h"

/*
#define GRAPHICS_NATIVE_COMMAND_EndDraw                  1
#define GRAPHICS_NATIVE_COMMAND_put_GlobalAlpha          2
#define GRAPHICS_NATIVE_COMMAND_Start_GlobalAlpha        3
#define GRAPHICS_NATIVE_COMMAND_End_GlobalAlpha          4
#define GRAPHICS_NATIVE_COMMAND_p_color                  5
#define GRAPHICS_NATIVE_COMMAND_p_width                  6
#define GRAPHICS_NATIVE_COMMAND_p_dash                   7
#define GRAPHICS_NATIVE_COMMAND_b_color1                 8
#define GRAPHICS_NATIVE_COMMAND_b_color2                 9
#define GRAPHICS_NATIVE_COMMAND_transform                10
#define GRAPHICS_NATIVE_COMMAND_CalculateFullTransform   11
#define GRAPHICS_NATIVE_COMMAND__s                       12
#define GRAPHICS_NATIVE_COMMAND__e                       13
#define GRAPHICS_NATIVE_COMMAND__z                       14
#define GRAPHICS_NATIVE_COMMAND__m                       15
#define GRAPHICS_NATIVE_COMMAND__l                       16
#define GRAPHICS_NATIVE_COMMAND__c                       17
#define GRAPHICS_NATIVE_COMMAND__c2                      18
#define GRAPHICS_NATIVE_COMMAND_ds                       19
#define GRAPHICS_NATIVE_COMMAND_df                       20
#define GRAPHICS_NATIVE_COMMAND_save                     21
#define GRAPHICS_NATIVE_COMMAND_restore                  22
#define GRAPHICS_NATIVE_COMMAND_clip                     23
#define GRAPHICS_NATIVE_COMMAND_reset                    24
#define GRAPHICS_NATIVE_COMMAND_transform3               25
#define GRAPHICS_NATIVE_COMMAND_FreeFont                 26
#define GRAPHICS_NATIVE_COMMAND_ClearLastFont            27
#define GRAPHICS_NATIVE_COMMAND_drawImage2               28
#define GRAPHICS_NATIVE_COMMAND_drawImage                29
#define GRAPHICS_NATIVE_COMMAND_GetFont                  30
#define GRAPHICS_NATIVE_COMMAND_font                     31
#define GRAPHICS_NATIVE_COMMAND_SetFont                  32
#define GRAPHICS_NATIVE_COMMAND_SetTextPr                33
#define GRAPHICS_NATIVE_COMMAND_SetFontSlot              34
#define GRAPHICS_NATIVE_COMMAND_GetTextPr                35
#define GRAPHICS_NATIVE_COMMAND_FillText                 36
#define GRAPHICS_NATIVE_COMMAND_t                        37
#define GRAPHICS_NATIVE_COMMAND_FillText2                38
#define GRAPHICS_NATIVE_COMMAND_t2                       39
#define GRAPHICS_NATIVE_COMMAND_FillTextCode             40
#define GRAPHICS_NATIVE_COMMAND_tg                       41
#define GRAPHICS_NATIVE_COMMAND_charspace                42
#define GRAPHICS_NATIVE_COMMAND_private_FillGlyph        43
#define GRAPHICS_NATIVE_COMMAND_private_FillGlyphC       44
#define GRAPHICS_NATIVE_COMMAND_private_FillGlyph2       45
#define GRAPHICS_NATIVE_COMMAND_SetIntegerGrid           46
#define GRAPHICS_NATIVE_COMMAND_GetIntegerGrid           47
#define GRAPHICS_NATIVE_COMMAND_DrawStringASCII          48
#define GRAPHICS_NATIVE_COMMAND_DrawStringASCII2         49
#define GRAPHICS_NATIVE_COMMAND_DrawHeaderEdit           50
#define GRAPHICS_NATIVE_COMMAND_DrawFooterEdit           51
#define GRAPHICS_NATIVE_COMMAND_DrawLockParagraph        52
#define GRAPHICS_NATIVE_COMMAND_DrawLockObjectRect       53
#define GRAPHICS_NATIVE_COMMAND_DrawEmptyTableLine       54
#define GRAPHICS_NATIVE_COMMAND_DrawSpellingLine         55
#define GRAPHICS_NATIVE_COMMAND_drawHorLine              56
#define GRAPHICS_NATIVE_COMMAND_drawHorLine2             57
#define GRAPHICS_NATIVE_COMMAND_drawVerLine              58
#define GRAPHICS_NATIVE_COMMAND_drawHorLineExt           59
#define GRAPHICS_NATIVE_COMMAND_rect                     60
#define GRAPHICS_NATIVE_COMMAND_TableRect                61
#define GRAPHICS_NATIVE_COMMAND_AddClipRect              62
#define GRAPHICS_NATIVE_COMMAND_RemoveClipRect           63
#define GRAPHICS_NATIVE_COMMAND_SetClip                  64
#define GRAPHICS_NATIVE_COMMAND_RemoveClip               65
#define GRAPHICS_NATIVE_COMMAND_drawCollaborativeChanges 66
#define GRAPHICS_NATIVE_COMMAND_drawMailMergeField       67
#define GRAPHICS_NATIVE_COMMAND_drawSearchResult         68
#define GRAPHICS_NATIVE_COMMAND_drawFlowAnchor           69
#define GRAPHICS_NATIVE_COMMAND_SavePen                  70
#define GRAPHICS_NATIVE_COMMAND_RestorePen               71
#define GRAPHICS_NATIVE_COMMAND_SaveBrush                72
#define GRAPHICS_NATIVE_COMMAND_RestoreBrush             73
#define GRAPHICS_NATIVE_COMMAND_SavePenBrush             74
#define GRAPHICS_NATIVE_COMMAND_RestorePenBrush          75
#define GRAPHICS_NATIVE_COMMAND_SaveGrState              76
#define GRAPHICS_NATIVE_COMMAND_RestoreGrState           77
#define GRAPHICS_NATIVE_COMMAND_StartClipPath            78
#define GRAPHICS_NATIVE_COMMAND_EndClipPath              79
#define GRAPHICS_NATIVE_COMMAND_StartCheckTableDraw      80
#define GRAPHICS_NATIVE_COMMAND_EndCheckTableDraw        81
#define GRAPHICS_NATIVE_COMMAND_SetTextClipRect          82
#define GRAPHICS_NATIVE_COMMAND_AddSmartRect             83
#define GRAPHICS_NATIVE_COMMAND_CheckUseFonts2           84
#define GRAPHICS_NATIVE_COMMAND_UncheckUseFonts2         85
#define GRAPHICS_NATIVE_COMMAND_Drawing_StartCheckBounds 86
#define GRAPHICS_NATIVE_COMMAND_Drawing_EndCheckBounds   87
#define GRAPHICS_NATIVE_COMMAND_DrawPresentationComment  88
#define GRAPHICS_NATIVE_COMMAND_DrawPolygon              89
#define GRAPHICS_NATIVE_COMMAND_DrawFootnoteRect         90
*/

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
        CGraphicsRenderer* pRenderer;
        std::wstring m_sFontsDirectory;
        std::wstring m_sImagesDirectory;
        std::wstring m_sThemesDirectory;

    private:
        v8::Local<v8::Value>* m_oContext;
        double m_dWidthMM;
        double m_dHeightMM;
        double m_lWidthPix;
        double m_lHeightPix;
        double m_dDpiX;
        double m_dDpiY;
        bool m_bIsBreak;

        NSStructures::CPen* m_oPen;
        bool m_bPenColorInit;
        NSStructures::CBrush* m_oBrush;
        bool m_bBrushColorInit;

        v8::Local<v8::Value>* m_oFontManager;

        Aggplus::CMatrix* m_oCoordTransform;
        Aggplus::CMatrix* m_oBaseTransform;
        Aggplus::CMatrix* m_oTransform;
        Aggplus::CMatrix* m_oFullTransform;
        Aggplus::CMatrix* m_oInvertFullTransform;

        std::vector<std::pair<double, double>> ArrayPoints;

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

        double globalAlpha;

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

        void init(double width_px, double height_px, double width_mm, double height_mm);
        void EndDraw() {}
        void put_GlobalAlpha(bool enable, double globalAlpha);
        void Start_GlobalAlpha() {}
        void End_GlobalAlpha();
        // pen methods
        void p_color(int r, int g, int b, int a);
        void p_width(double w);
        void p_dash(size_t length, double* dash);
        // brush methods
        void b_color1(int r, int g, int b, int a);
        void b_color2(int r, int g, int b, int a);
        void transform(double sx, double shy, double shx, double sy, double tx, double ty);
        void CalculateFullTransform(bool isInvertNeed) {}
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
        void save() {}
        void restore();
        void clip();
        void reset();
        void transform3(double sx, double shy, double shx, double sy, double tx, double ty, bool isNeedInvert);
        void FreeFont() {}
        void ClearLastFont() {}
        // images
        void drawImage(const std::wstring& img, double x, double y, double w, double h, BYTE alpha);
        // text
        void GetFont() {}
        void font(int font_id, int font_size) {}
        void SetFont(const std::wstring& path, int face, double size, int style);
        void SetTextPr() {}
        void GetTextPr() {}
        void FillText(double x, double y, int text);
        void t() {}
        void FillText2(double x, double y, int text, double cropX, double cropW);
        void t2() {}
        void FillTextCode(double x, double y, int lUnicode);
        void tg(int text, double x, double y);
        void charspace() {}
        // private methods
        void private_FillGlyph()  {}
        void private_FillGlyphC() {}
        void private_FillGlyph2() {}
        void SetIntegerGrid(bool param);
        bool GetIntegerGrid() {}
        void DrawStringASCII() {}
        void DrawStringASCII2() {}
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

        /*
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
        */
    };
}

#endif // CGRAPHICS_H
