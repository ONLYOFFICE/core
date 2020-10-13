#include "../GraphicsEmbed.h"
#include "../../v8/v8_base.h"

#define PROPERTY_GET(NAME, NAME_EMBED, TYPE)                                                                \
    void NAME(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)                 \
    {                                                                                                       \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());                              \
        info.GetReturnValue().Set(TYPE::New(v8::Isolate::GetCurrent(), _this->->m_pInternal->NAME_EMBED())) \
    }

#define PROPERTY_GET_OBJECT(NAME, NAME_EMBED)                                                \
    void NAME(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)  \
    {                                                                                        \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());               \
        v8::Local<v8::Value>* v = _this->m_pInternal->NAME_EMBED();                          \
        if(v) info.GetReturnValue().Set(*v);                                                 \
        else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));           \
    }

namespace NSGraphics
{
    #define CURRENTWRAPPER CGraphicsEmbed

    // FUNCTION
    FUNCTION_WRAPPER_V8_5(_init,              init)
    FUNCTION_WRAPPER_V8  (_EndDraw,           EndDraw)
    FUNCTION_WRAPPER_V8_2(_put_GlobalAlpha,   put_GlobalAlpha)
    FUNCTION_WRAPPER_V8  (_Start_GlobalAlpha, Start_GlobalAlpha)
    FUNCTION_WRAPPER_V8  (_End_GlobalAlpha,   End_GlobalAlpha)
    // pen methods
    FUNCTION_WRAPPER_V8_4(_p_color, p_color)
    FUNCTION_WRAPPER_V8_1(_p_width, p_width)
    FUNCTION_WRAPPER_V8_1(_p_dash,  p_dash)
    // brush methods
    FUNCTION_WRAPPER_V8_4(_b_color1,               b_color1)
    FUNCTION_WRAPPER_V8_4(_b_color2,               b_color2)
    FUNCTION_WRAPPER_V8_6(_transform,              transform)
    FUNCTION_WRAPPER_V8_1(_CalculateFullTransform, CalculateFullTransform)
    // path commands
    FUNCTION_WRAPPER_V8  (__s,  _s)
    FUNCTION_WRAPPER_V8  (__e,  _e)
    FUNCTION_WRAPPER_V8  (__z,  _z)
    FUNCTION_WRAPPER_V8_2(__m,  _m)
    FUNCTION_WRAPPER_V8_2(__l,  _l)
    FUNCTION_WRAPPER_V8_6(__c,  _c)
    FUNCTION_WRAPPER_V8_4(__c2, _c2)
    FUNCTION_WRAPPER_V8  (_ds,  ds)
    FUNCTION_WRAPPER_V8  (_df,  df)
    // canvas state
    FUNCTION_WRAPPER_V8  (_save,          save)
    FUNCTION_WRAPPER_V8  (_restore,       restore)
    FUNCTION_WRAPPER_V8  (_clip,          clip)
    FUNCTION_WRAPPER_V8  (_reset,         reset)
    FUNCTION_WRAPPER_V8_2(_transform3,    transform3)
    FUNCTION_WRAPPER_V8  (_FreeFont,      FreeFont)
    FUNCTION_WRAPPER_V8  (_ClearLastFont, ClearLastFont)
    // images
    FUNCTION_WRAPPER_V8_7(_drawImage2, drawImage2)
    FUNCTION_WRAPPER_V8_8(_drawImage,  drawImage)
    // text
    FUNCTION_WRAPPER_V8  (_GetFont,      GetFont)
    FUNCTION_WRAPPER_V8_2(_font,         font)
    FUNCTION_WRAPPER_V8_1(_SetFont,      SetFont)
    FUNCTION_WRAPPER_V8_2(_SetTextPr,    SetTextPr)
    FUNCTION_WRAPPER_V8_2(_SetFontSlot,  SetFontSlot)
    FUNCTION_WRAPPER_V8  (_GetTextPr,    GetTextPr)
    FUNCTION_WRAPPER_V8_3(_FillText,     FillText)
    FUNCTION_WRAPPER_V8_4(_t,            t)
    FUNCTION_WRAPPER_V8_5(_FillText2,    FillText2)
    FUNCTION_WRAPPER_V8_5(_t2,           t2)
    FUNCTION_WRAPPER_V8_3(_FillTextCode, FillTextCode)
    FUNCTION_WRAPPER_V8_3(_tg,           tg)
    FUNCTION_WRAPPER_V8_1(_charspace,    charspace)
    // private methods
    FUNCTION_WRAPPER_V8_2(_private_FillGlyph,  private_FillGlyph)
    FUNCTION_WRAPPER_V8_3(_private_FillGlyphC, private_FillGlyphC)
    FUNCTION_WRAPPER_V8_1(_private_FillGlyph2, private_FillGlyph2)
    FUNCTION_WRAPPER_V8_1(_SetIntegerGrid,     SetIntegerGrid)
    FUNCTION_WRAPPER_V8  (_GetIntegerGrid,     GetIntegerGrid)
    FUNCTION_WRAPPER_V8_8(_DrawStringASCII,    DrawStringASCII)
    FUNCTION_WRAPPER_V8_8(_DrawStringASCII2,   DrawStringASCII2)
    FUNCTION_WRAPPER_V8_5(_DrawHeaderEdit,     DrawHeaderEdit)
    FUNCTION_WRAPPER_V8_5(_DrawFooterEdit,     DrawFooterEdit)
    FUNCTION_WRAPPER_V8_4(_DrawLockParagraph,  DrawLockParagraph)
    FUNCTION_WRAPPER_V8_5(_DrawLockObjectRect, DrawLockObjectRect)
    FUNCTION_WRAPPER_V8_4(_DrawEmptyTableLine, DrawEmptyTableLine)
    FUNCTION_WRAPPER_V8_4(_DrawSpellingLine,   DrawSpellingLine)
    // smart methods for horizontal / vertical lines
    FUNCTION_WRAPPER_V8_5(_drawHorLine,  drawHorLine)
    FUNCTION_WRAPPER_V8_5(_drawHorLine2, drawHorLine2)
    FUNCTION_WRAPPER_V8_5(_drawVerLine,  drawVerLine)
    // мега крутые функции для таблиц
    FUNCTION_WRAPPER_V8_7(_drawHorLineExt, drawHorLineExt)
    FUNCTION_WRAPPER_V8_4(_rect,           rect)
    FUNCTION_WRAPPER_V8_4(_TableRect,      TableRect)
    // функции клиппирования
    FUNCTION_WRAPPER_V8_4(_AddClipRect,              AddClipRect)
    FUNCTION_WRAPPER_V8  (_RemoveClipRect,           RemoveClipRect)
    FUNCTION_WRAPPER_V8_1(_SetClip,                  SetClip)
    FUNCTION_WRAPPER_V8  (_RemoveClip,               RemoveClip)
    FUNCTION_WRAPPER_V8_5(_drawCollaborativeChanges, drawCollaborativeChanges)
    FUNCTION_WRAPPER_V8_4(_drawMailMergeField,       drawMailMergeField)
    FUNCTION_WRAPPER_V8_4(_drawSearchResult,         drawSearchResult)
    FUNCTION_WRAPPER_V8_2(_drawFlowAnchor,           drawFlowAnchor)
    FUNCTION_WRAPPER_V8  (_SavePen,                  SavePen)
    FUNCTION_WRAPPER_V8  (_RestorePen,               RestorePen)
    FUNCTION_WRAPPER_V8  (_SaveBrush,                SaveBrush)
    FUNCTION_WRAPPER_V8  (_RestoreBrush,             RestoreBrush)
    FUNCTION_WRAPPER_V8  (_SavePenBrush,             SavePenBrush)
    FUNCTION_WRAPPER_V8  (_RestorePenBrush,          RestorePenBrush)
    FUNCTION_WRAPPER_V8  (_SaveGrState,              SaveGrState)
    FUNCTION_WRAPPER_V8  (_RestoreGrState,           RestoreGrState)
    FUNCTION_WRAPPER_V8  (_StartClipPath,            StartClipPath)
    FUNCTION_WRAPPER_V8  (_EndClipPath,              EndClipPath)
    FUNCTION_WRAPPER_V8  (_StartCheckTableDraw,      StartCheckTableDraw)
    FUNCTION_WRAPPER_V8_1(_EndCheckTableDraw,        EndCheckTableDraw)
    FUNCTION_WRAPPER_V8_4(_SetTextClipRect,          SetTextClipRect)
    FUNCTION_WRAPPER_V8_5(_AddSmartRect,             AddSmartRect)
    FUNCTION_WRAPPER_V8_1(_CheckUseFonts2,           CheckUseFonts2)
    FUNCTION_WRAPPER_V8  (_UncheckUseFonts2,         UncheckUseFonts2)
    FUNCTION_WRAPPER_V8_4(_Drawing_StartCheckBounds, Drawing_StartCheckBounds)
    FUNCTION_WRAPPER_V8  (_Drawing_EndCheckBounds,   Drawing_EndCheckBounds)
    FUNCTION_WRAPPER_V8_5(_DrawPresentationComment,  DrawPresentationComment)
    FUNCTION_WRAPPER_V8_3(_DrawPolygon,              DrawPolygon)
    FUNCTION_WRAPPER_V8_4(_DrawFootnoteRect,         DrawFootnoteRect)

    // PROPERTY GET
    PROPERTY_GET_OBJECT(_g_m_oContext,             g_m_oContext)
    PROPERTY_GET_OBJECT(_g_m_oPen,                 g_m_oPen)
    PROPERTY_GET_OBJECT(_g_m_oBrush,               g_m_oBrush)
    PROPERTY_GET_OBJECT(_g_m_oFontManager,         g_m_oFontManager)
    PROPERTY_GET_OBJECT(_g_m_oCoordTransform,      g_m_oCoordTransform)
    PROPERTY_GET_OBJECT(_g_m_oBaseTransform,       g_m_oBaseTransform)
    PROPERTY_GET_OBJECT(_g_m_oTransform,           g_m_oTransform)
    PROPERTY_GET_OBJECT(_g_m_oFullTransform,       g_m_oFullTransform)
    PROPERTY_GET_OBJECT(_g_m_oInvertFullTransform, g_m_oInvertFullTransform)
    PROPERTY_GET_OBJECT(_g_ArrayPoints,            g_ArrayPoints)
    PROPERTY_GET_OBJECT(_g_m_oTextPr,              g_m_oTextPr)
    PROPERTY_GET_OBJECT(_g_m_oGrFonts,             g_m_oGrFonts)
    PROPERTY_GET_OBJECT(_g_m_oLastFont,            g_m_oLastFont)
    PROPERTY_GET_OBJECT(_g_ClipManager,            g_ClipManager)
    PROPERTY_GET_OBJECT(_g_GrState,                g_GrState)
    PROPERTY_GET_OBJECT(_g_TextClipRect,           g_TextClipRect)
    PROPERTY_GET_OBJECT(_g_m_oFontManager2,        g_m_oFontManager2)
    PROPERTY_GET_OBJECT(_g_m_oLastFont2,           g_m_oLastFont2)
    PROPERTY_GET_OBJECT(_g_dash_no_smart,          g_dash_no_smart)
    /*
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
    */

    v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate)
    {
        v8::EscapableHandleScope handle_scope(isolate);

        v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
        result->SetInternalFieldCount(1);

        v8::Isolate* current = v8::Isolate::GetCurrent();

        // свойства
        /*
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oContext"),                 m_oContext_get_w, m_oContext_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dWidthMM"),                 m_dWidthMM_get_w, m_dWidthMM_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dHeightMM"),                m_dHeightMM_get_w, m_dHeightMM_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_lWidthPix"),                m_lWidthPix_get_w, m_lWidthPix_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_lHeightPix"),               m_lHeightPix_get_w, m_lHeightPix_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiX"),                    m_dDpiX_get_w, m_dDpiX_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiY"),                    m_dDpiY_get_w, m_dDpiY_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bIsBreak"),                 m_bIsBreak_get_w, m_bIsBreak_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oPen"),                     m_oPen_get_w, m_oPen_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bPenColorInit"),            m_bPenColorInit_get_w, m_bPenColorInit_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oBrush"),                   m_oBrush_get_w, m_oBrush_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bBrushColorInit"),          m_bBrushColorInit_get_w, m_bBrushColorInit_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFontManager"),             m_oFontManager_get_w, m_oFontManager_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oCoordTransform"),          m_oCoordTransform_get_w, m_oCoordTransform_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oBaseTransform"),           m_oBaseTransform_get_w, m_oBaseTransform_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFullTransform"),           m_oFullTransform_get_w, m_oFullTransform_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oInvertFullTransform"),     m_oInvertFullTransform_get_w, m_oInvertFullTransform_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "ArrayPoints"),                ArrayPoints_get_w, ArrayPoints_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oCurFont"),                 m_oCurFont_get_w, m_oCurFont_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oTextPr"),                  m_oTextPr_get_w, m_oTextPr_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oGrFonts"),                 m_oGrFonts_get_w, m_oGrFonts_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oLastFont"),                m_oLastFont_get_w, m_oLastFont_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "LastFontOriginInfo"),         LastFontOriginInfo_get_w, LastFontOriginInfo_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bIntegerGrid"),             m_bIntegerGrid_get_w, m_bIntegerGrid_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "ClipManager"),                ClipManager_get_w, ClipManager_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "TextureFillTransformScaleX"), TextureFillTransformScaleX_get_w, TextureFillTransformScaleX_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "TextureFillTransformScaleY"), TextureFillTransformScaleY_get_w, TextureFillTransformScaleY_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsThumbnail"),                IsThumbnail_get_w, IsThumbnail_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsDemonstrationMode"),        IsDemonstrationMode_get_w, IsDemonstrationMode_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsDemonstrationMode"),        IsDemonstrationMode_get_w, IsDemonstrationMode_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "GrState"),                    GrState_get_w, GrState_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "globalAlpha"),                globalAlpha_get_w, globalAlpha_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "TextClipRect"),               TextClipRect_get_w, TextClipRect_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsClipContext"),              IsClipContext_get_w, IsClipContext_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsUseFonts2"),                IsUseFonts2_get_w, IsUseFonts2_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFontManager2"),            m_oFontManager2_get_w, m_oFontManager2_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oLastFont2"),               m_oLastFont2_get_w, m_oLastFont2_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "ClearMode"),                  ClearMode_get_w, ClearMode_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsRetina"),                   IsRetina_get_w, IsRetina_set_w);
        result->SetAccessor(v8::String::NewFromUtf8(current, "dash_no_smart"),              dash_no_smart_get_w, dash_no_smart_set_w);
        */

        // методы
        NSV8Objects::Template_Set(result, "init",                     _init);
        NSV8Objects::Template_Set(result, "EndDraw",                  _EndDraw);
        NSV8Objects::Template_Set(result, "put_GlobalAlpha",          _put_GlobalAlpha);
        NSV8Objects::Template_Set(result, "Start_GlobalAlpha",        _Start_GlobalAlpha);
        NSV8Objects::Template_Set(result, "End_GlobalAlpha",          _End_GlobalAlpha);
        NSV8Objects::Template_Set(result, "p_color",                  _p_color);
        NSV8Objects::Template_Set(result, "p_width",                  _p_width);
        NSV8Objects::Template_Set(result, "p_dash",                   _p_dash);
        NSV8Objects::Template_Set(result, "b_color1",                 _b_color1);
        NSV8Objects::Template_Set(result, "b_color2",                 _b_color2);
        NSV8Objects::Template_Set(result, "transform",                _transform);
        NSV8Objects::Template_Set(result, "CalculateFullTransform",   _CalculateFullTransform);
        NSV8Objects::Template_Set(result, "_s",                       __s);
        NSV8Objects::Template_Set(result, "_e",                       __e);
        NSV8Objects::Template_Set(result, "_z",                       __z);
        NSV8Objects::Template_Set(result, "_m",                       __m);
        NSV8Objects::Template_Set(result, "_l",                       __l);
        NSV8Objects::Template_Set(result, "_c",                       __c);
        NSV8Objects::Template_Set(result, "_c2",                      __c2);
        NSV8Objects::Template_Set(result, "ds",                       _ds);
        NSV8Objects::Template_Set(result, "df",                       _df);
        NSV8Objects::Template_Set(result, "save",                     _save);
        NSV8Objects::Template_Set(result, "restore",                  _restore);
        NSV8Objects::Template_Set(result, "clip",                     _clip);
        NSV8Objects::Template_Set(result, "reset",                    _reset);
        NSV8Objects::Template_Set(result, "transform3",               _transform3);
        NSV8Objects::Template_Set(result, "FreeFont",                 _FreeFont);
        NSV8Objects::Template_Set(result, "ClearLastFont",            _ClearLastFont);
        NSV8Objects::Template_Set(result, "drawImage2",               _drawImage2);
        NSV8Objects::Template_Set(result, "drawImage",                _drawImage);
        NSV8Objects::Template_Set(result, "GetFont",                  _GetFont);
        NSV8Objects::Template_Set(result, "font",                     _font);
        NSV8Objects::Template_Set(result, "SetFont",                  _SetFont);
        NSV8Objects::Template_Set(result, "SetTextPr",                _SetTextPr);
        NSV8Objects::Template_Set(result, "SetFontSlot",              _SetFontSlot);
        NSV8Objects::Template_Set(result, "GetTextPr",                _GetTextPr);
        NSV8Objects::Template_Set(result, "FillText",                 _FillText);
        NSV8Objects::Template_Set(result, "t",                        _t);
        NSV8Objects::Template_Set(result, "FillText2",                _FillText2);
        NSV8Objects::Template_Set(result, "t2",                       _t2);
        NSV8Objects::Template_Set(result, "FillTextCode",             _FillTextCode);
        NSV8Objects::Template_Set(result, "tg",                       _tg);
        NSV8Objects::Template_Set(result, "charspace",                _charspace);
        NSV8Objects::Template_Set(result, "private_FillGlyph",        _private_FillGlyph);
        NSV8Objects::Template_Set(result, "private_FillGlyphC",       _private_FillGlyphC);
        NSV8Objects::Template_Set(result, "private_FillGlyph2",       _private_FillGlyph2);
        NSV8Objects::Template_Set(result, "SetIntegerGrid",           _SetIntegerGrid);
        NSV8Objects::Template_Set(result, "GetIntegerGrid",           _GetIntegerGrid);
        NSV8Objects::Template_Set(result, "DrawStringASCII",          _DrawStringASCII);
        NSV8Objects::Template_Set(result, "DrawStringASCII2",         _DrawStringASCII2);
        NSV8Objects::Template_Set(result, "DrawHeaderEdit",           _DrawHeaderEdit);
        NSV8Objects::Template_Set(result, "DrawFooterEdit",           _DrawFooterEdit);
        NSV8Objects::Template_Set(result, "DrawLockParagraph",        _DrawLockParagraph);
        NSV8Objects::Template_Set(result, "DrawLockObjectRect",       _DrawLockObjectRect);
        NSV8Objects::Template_Set(result, "DrawEmptyTableLine",       _DrawEmptyTableLine);
        NSV8Objects::Template_Set(result, "DrawSpellingLine",         _DrawSpellingLine);
        NSV8Objects::Template_Set(result, "drawHorLine",              _drawHorLine);
        NSV8Objects::Template_Set(result, "drawHorLine2",             _drawHorLine2);
        NSV8Objects::Template_Set(result, "drawVerLine",              _drawVerLine);
        NSV8Objects::Template_Set(result, "drawHorLineExt",           _drawHorLineExt);
        NSV8Objects::Template_Set(result, "rect",                     _rect);
        NSV8Objects::Template_Set(result, "TableRect",                _TableRect);
        NSV8Objects::Template_Set(result, "AddClipRect",              _AddClipRect);
        NSV8Objects::Template_Set(result, "RemoveClipRect",           _RemoveClipRect);
        NSV8Objects::Template_Set(result, "SetClip",                  _SetClip);
        NSV8Objects::Template_Set(result, "RemoveClip",               _RemoveClip);
        NSV8Objects::Template_Set(result, "drawCollaborativeChanges", _drawCollaborativeChanges);
        NSV8Objects::Template_Set(result, "drawMailMergeField",       _drawMailMergeField);
        NSV8Objects::Template_Set(result, "drawSearchResult",         _drawSearchResult);
        NSV8Objects::Template_Set(result, "drawFlowAnchor",           _drawFlowAnchor);
        NSV8Objects::Template_Set(result, "SavePen",                  _SavePen);
        NSV8Objects::Template_Set(result, "RestorePen",               _RestorePen);
        NSV8Objects::Template_Set(result, "SaveBrush",                _SaveBrush);
        NSV8Objects::Template_Set(result, "RestoreBrush",             _RestoreBrush);
        NSV8Objects::Template_Set(result, "SavePenBrush",             _SavePenBrush);
        NSV8Objects::Template_Set(result, "RestorePenBrush",          _RestorePenBrush);
        NSV8Objects::Template_Set(result, "SaveGrState",              _SaveGrState);
        NSV8Objects::Template_Set(result, "RestoreGrState",           _RestoreGrState);
        NSV8Objects::Template_Set(result, "StartClipPath",            _StartClipPath);
        NSV8Objects::Template_Set(result, "EndClipPath",              _EndClipPath);
        NSV8Objects::Template_Set(result, "StartCheckTableDraw",      _StartCheckTableDraw);
        NSV8Objects::Template_Set(result, "EndCheckTableDraw",        _EndCheckTableDraw);
        NSV8Objects::Template_Set(result, "SetTextClipRect",          _SetTextClipRect);
        NSV8Objects::Template_Set(result, "AddSmartRect",             _AddSmartRect);
        NSV8Objects::Template_Set(result, "CheckUseFonts2",           _CheckUseFonts2);
        NSV8Objects::Template_Set(result, "UncheckUseFonts2",         _UncheckUseFonts2);
        NSV8Objects::Template_Set(result, "Drawing_StartCheckBounds", _Drawing_StartCheckBounds);
        NSV8Objects::Template_Set(result, "Drawing_EndCheckBounds",   _Drawing_EndCheckBounds);
        NSV8Objects::Template_Set(result, "DrawPresentationComment",  _DrawPresentationComment);
        NSV8Objects::Template_Set(result, "DrawPolygon",              _DrawPolygon);
        NSV8Objects::Template_Set(result, "DrawFootnoteRect",         _DrawFootnoteRect);

        return handle_scope.Escape(result);
    }

    void CreateNativeGraphics(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        v8::Handle<v8::ObjectTemplate> GraphicsTemplate = NSGraphics::CreateGraphicsTemplate(isolate);
        CGraphicsEmbed* pGraphics = new CGraphicsEmbed();

        v8::Local<v8::Object> obj = GraphicsTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
        obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), pGraphics));

        args.GetReturnValue().Set(obj);
    }
}

void CGraphicsEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
    v8::Isolate* current = CV8Worker::GetCurrent();
    context->m_internal->m_global->Set(current, name.c_str(), v8::FunctionTemplate::New(current, NSGraphics::CreateNativeGraphics));
}

/*
void init_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    v8::Local<v8::Value> args0 = args[0];
    pGraphics->init(&args0, to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void EndDraw_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->EndDraw();
}
void put_GlobalAlpha_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->put_GlobalAlpha(to_bool(args[0]), to_int(args[1]));
}
void Start_GlobalAlpha_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->Start_GlobalAlpha();
}
void End_GlobalAlpha_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->End_GlobalAlpha();
}
void p_color_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_color(to_int(args[0]), to_int(args[1]), to_int(args[2]), to_int(args[3]));
}
void p_width_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_width(to_int(args[0]));
}
void p_dash_w                  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->p_dash(args[0]);
}
void b_color1_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->b_color1(to_int(args[0]), to_int(args[1]), to_int(args[2]), to_int(args[3]));
}
void b_color2_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->b_color2(to_int(args[0]), to_int(args[1]), to_int(args[2]), to_int(args[3]));
}
void transform_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 6)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->transform(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_double(args[5]));
}
void CalculateFullTransform_w  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->CalculateFullTransform(to_bool(args[0]));
}
void _s_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_s();
}
void _e_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_e();
}
void _z_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_z();
}
void _m_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_m(to_double(args[0]), to_double(args[1]));
}
void _l_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_l(to_double(args[0]), to_double(args[1]));
}
void _c_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 6)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_c(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_double(args[5]));
}
void _c2_w                     (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->_c2(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void ds_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->ds();
}
void df_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->df();
}
void save_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->save();
}
void restore_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->restore();
}
void clip_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->clip();
}
void reset_w                   (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->reset();
}
void transform3_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->transform3(args[0], to_bool(args[1]));
}
void FreeFont_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FreeFont();
}
void ClearLastFont_w           (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->ClearLastFont();
}
void drawImage2_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 7)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawImage2(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_int(args[5]), args[6]);
}
void drawImage_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 8)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawImage(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]), to_int(args[5]), args[6], args[7]);
}
void GetFont_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->GetFont();
}
void font_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->font(to_uint(args[0]), to_int(args[1]));
}
void SetFont_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetFont(args[0]);
}
void SetTextPr_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetTextPr(args[0], args[1]);
}
void SetFontSlot_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetFontSlot(args[0], to_double(args[1]));
}
void GetTextPr_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->GetTextPr();
}
void FillText_w                (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FillText(to_double(args[0]), to_double(args[1]), args[2]);
}
void t_w                       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->t(args[0], to_double(args[1]), to_double(args[2]), to_bool(args[3]));
}
void FillText2_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FillText2(to_double(args[0]), to_double(args[1]), args[2], to_double(args[3]), to_double(args[4]));
}
void t2_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->t2(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void FillTextCode_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->FillTextCode(to_double(args[0]), to_double(args[1]), args[2]);
}
void tg_w                      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->tg(args[0], to_double(args[1]), to_double(args[2]));
}
void charspace_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->charspace(args[0]);
}
void private_FillGlyph_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->private_FillGlyph(args[0], args[1]);
}
void private_FillGlyphC_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 3)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->private_FillGlyphC(args[0], to_double(args[1]), to_double(args[2]));
}
void private_FillGlyph2_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->private_FillGlyph2(args[0]);
}
void SetIntegerGrid_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetIntegerGrid(to_bool(args[0]));
}
void GetIntegerGrid_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->GetIntegerGrid();
}
void DrawStringASCII_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 8)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawStringASCII(to_string(args[0]), to_int(args[1]), to_bool(args[2]), to_bool(args[3]), args[4], to_double(args[5]), to_double(args[6]), to_bool(args[7]));
}
void DrawStringASCII2_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 8)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawStringASCII2(to_string(args[0]), to_int(args[1]), to_bool(args[2]), to_bool(args[3]), args[4], to_double(args[5]), to_double(args[6]), to_bool(args[7]));
}
void DrawHeaderEdit_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawHeaderEdit(to_double(args[0]), args[1], to_int(args[2]), to_bool(args[3]), args[4]);
}
void DrawFooterEdit_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawFooterEdit(to_double(args[0]), args[1], to_int(args[2]), to_bool(args[3]), args[4]);
}
void DrawLockParagraph_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawLockParagraph(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void DrawLockObjectRect_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawLockObjectRect(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void DrawEmptyTableLine_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawEmptyTableLine(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void DrawSpellingLine_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawSpellingLine(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void drawHorLine_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawHorLine(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]));
}
void drawHorLine2_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawHorLine2(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]));
}
void drawVerLine_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawVerLine(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]));
}
void drawHorLineExt_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 7)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawHorLineExt(to_int(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), to_int(args[4]), to_double(args[5]), to_double(args[6]));
}
void rect_w                    (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->rect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void TableRect_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->TableRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void AddClipRect_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->AddClipRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void RemoveClipRect_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RemoveClipRect();
}
void SetClip_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetClip(args[0]);
}
void RemoveClip_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RemoveClip();
}
void drawCollaborativeChanges_w(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawCollaborativeChanges(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]), args[4]);
}
void drawMailMergeField_w      (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawMailMergeField(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void drawSearchResult_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawSearchResult(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void drawFlowAnchor_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 2)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->drawFlowAnchor(to_double(args[0]), to_double(args[1]));
}
void SavePen_w                 (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SavePen();
}
void RestorePen_w              (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestorePen();
}
void SaveBrush_w               (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SaveBrush();
}
void RestoreBrush_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestoreBrush();
}
void SavePenBrush_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SavePenBrush();
}
void RestorePenBrush_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestorePenBrush();
}
void SaveGrState_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SaveGrState();
}
void RestoreGrState_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->RestoreGrState();
}
void StartClipPath_w           (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->StartClipPath();
}
void EndClipPath_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->EndClipPath();
}
void StartCheckTableDraw_w     (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->StartCheckTableDraw();
}
void EndCheckTableDraw_w       (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->EndCheckTableDraw(to_bool(args[0]));
}
void SetTextClipRect_w         (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->SetTextClipRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void AddSmartRect_w            (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->AddSmartRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]),  to_int(args[4]));
}
void CheckUseFonts2_w          (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 1)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->CheckUseFonts2(args[0]);
}
void UncheckUseFonts2_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->UncheckUseFonts2();
}
void Drawing_StartCheckBounds_w(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->Drawing_StartCheckBounds(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
void Drawing_EndCheckBounds_w  (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->Drawing_EndCheckBounds();
}
void DrawPresentationComment_w (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 5)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawPresentationComment(args[0], to_double(args[1]), to_double(args[2]), to_double(args[3]), to_double(args[4]));
}
void DrawPolygon_w             (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawPolygon(args[0], to_int(args[1]), to_double(args[2]));
}
void DrawFootnoteRect_w        (const v8::FunctionCallbackInfo<v8::Value>& args)
{
    args.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
    if (args.Length() < 4)
        return;
    CJSGraphics* pGraphics = unwrap_Graphics(args.This());
    pGraphics->DrawFootnoteRect(to_double(args[0]), to_double(args[1]), to_double(args[2]), to_double(args[3]));
}
*/
/*
void m_oContext_get_w                (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oContext_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_dWidthMM_get_w                (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_dWidthMM_get()));
}
void m_dHeightMM_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_dHeightMM_get()));
}
void m_lWidthPix_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_lWidthPix_get()));
}
void m_lHeightPix_get_w              (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_lHeightPix_get()));
}
void m_dDpiX_get_w                   (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_dDpiX_get()));
}
void m_dDpiY_get_w                   (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_dDpiY_get()));
}
void m_bIsBreak_get_w                (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->m_bIsBreak_get()));
}
void m_oPen_get_w                    (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oPen_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_bPenColorInit_get_w           (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->m_bPenColorInit_get()));
}
void m_oBrush_get_w                  (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oBrush_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_bBrushColorInit_get_w         (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->m_bBrushColorInit_get()));
}
void m_oFontManager_get_w            (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oFontManager_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oCoordTransform_get_w         (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oCoordTransform_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oBaseTransform_get_w          (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oBaseTransform_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oTransform_get_w              (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oTransform_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oFullTransform_get_w          (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oFullTransform_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oInvertFullTransform_get_w    (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oInvertFullTransform_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void ArrayPoints_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->ArrayPoints_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oCurFont_get_w                (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    // info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->m_oCurFont_get()));
}
void m_oTextPr_get_w                 (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oTextPr_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oGrFonts_get_w                (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oGrFonts_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oLastFont_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oLastFont_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void LastFontOriginInfo_get_w        (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    // info.GetReturnValue().Set(v8::Number::New(v8::Isolate::GetCurrent(), pGraphics->LastFontOriginInfo_get()));
}
void m_bIntegerGrid_get_w            (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->m_bIntegerGrid_get()));
}
void ClipManager_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->ClipManager_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void TextureFillTransformScaleX_get_w(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), pGraphics->TextureFillTransformScaleX_get()));
}
void TextureFillTransformScaleY_get_w(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), pGraphics->TextureFillTransformScaleY_get()));
}
void IsThumbnail_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->IsThumbnail_get()));
}
void IsDemonstrationMode_get_w       (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->IsDemonstrationMode_get()));
}
void GrState_get_w                   (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->GrState_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void globalAlpha_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Int32::New(v8::Isolate::GetCurrent(), pGraphics->globalAlpha_get()));
}
void TextClipRect_get_w              (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->TextClipRect_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void IsClipContext_get_w             (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->IsClipContext_get()));
}
void IsUseFonts2_get_w               (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->IsUseFonts2_get()));
}
void m_oFontManager2_get_w           (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oFontManager2_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void m_oLastFont2_get_w              (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->m_oLastFont2_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}
void ClearMode_get_w                 (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->ClearMode_get()));
}
void IsRetina_get_w                  (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    info.GetReturnValue().Set(v8::Boolean::New(v8::Isolate::GetCurrent(), pGraphics->IsRetina_get()));
}
void dash_no_smart_get_w             (v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    v8::Local<v8::Value>* v = pGraphics->dash_no_smart_get();
    if(v) info.GetReturnValue().Set(*v);
    else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));
}

void m_oContext_set_w                (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oContext_set(&value);
    info.GetReturnValue().Set(value);
}
void m_dWidthMM_set_w                (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_dWidthMM_set(to_double(value));
    info.GetReturnValue().Set(value);
}
void m_dHeightMM_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_dHeightMM_set(to_double(value));
    info.GetReturnValue().Set(value);
}
void m_lWidthPix_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_lWidthPix_set(to_double(value));
    info.GetReturnValue().Set(value);
}
void m_lHeightPix_set_w              (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_lHeightPix_set(to_double(value));
    info.GetReturnValue().Set(value);
}
void m_dDpiX_set_w                   (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_dDpiX_set(to_double(value));
    info.GetReturnValue().Set(value);
}
void m_dDpiY_set_w                   (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_dDpiY_set(to_double(value));
    info.GetReturnValue().Set(value);
}
void m_bIsBreak_set_w                (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_bIsBreak_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void m_oPen_set_w                    (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oPen_set(&value);
    info.GetReturnValue().Set(value);
}
void m_bPenColorInit_set_w           (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_bPenColorInit_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void m_oBrush_set_w                  (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oBrush_set(&value);
    info.GetReturnValue().Set(value);
}
void m_bBrushColorInit_set_w         (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_bBrushColorInit_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void m_oFontManager_set_w            (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oFontManager_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oCoordTransform_set_w         (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oCoordTransform_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oBaseTransform_set_w          (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oBaseTransform_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oTransform_set_w              (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oTransform_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oFullTransform_set_w          (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oFullTransform_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oInvertFullTransform_set_w    (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oInvertFullTransform_set(&value);
    info.GetReturnValue().Set(value);
}
void ArrayPoints_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->ArrayPoints_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oCurFont_set_w                (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    // CFont* pFont = unwrap_Font(value);
    // pGraphics->m_oCurFont_set(*pFont);
    info.GetReturnValue().Set(value);
}
void m_oTextPr_set_w                 (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oTextPr_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oGrFonts_set_w                (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oGrFonts_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oLastFont_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oLastFont_set(&value);
    info.GetReturnValue().Set(value);
}
void LastFontOriginInfo_set_w        (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    // CLastFontOriginInfo* pLastFontOriginInfo = unwrap_LastFontOriginInfo(value);
    // pGraphics->LastFontOriginInfo_set(*pLastFontOriginInfo);
    info.GetReturnValue().Set(value);
}
void m_bIntegerGrid_set_w            (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_bIntegerGrid_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void ClipManager_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->ClipManager_set(&value);
    info.GetReturnValue().Set(value);
}
void TextureFillTransformScaleX_set_w(v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->TextureFillTransformScaleX_set(to_int(value));
    info.GetReturnValue().Set(value);
}
void TextureFillTransformScaleY_set_w(v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->TextureFillTransformScaleY_set(to_int(value));
    info.GetReturnValue().Set(value);
}
void IsThumbnail_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->IsThumbnail_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void IsDemonstrationMode_set_w       (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->IsDemonstrationMode_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void GrState_set_w                   (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->GrState_set(&value);
    info.GetReturnValue().Set(value);
}
void globalAlpha_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->globalAlpha_set(to_int(value));
    info.GetReturnValue().Set(value);
}
void TextClipRect_set_w              (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->TextClipRect_set(&value);
    info.GetReturnValue().Set(value);
}
void IsClipContext_set_w             (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->IsClipContext_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void IsUseFonts2_set_w               (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->IsUseFonts2_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void m_oFontManager2_set_w           (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oFontManager2_set(&value);
    info.GetReturnValue().Set(value);
}
void m_oLastFont2_set_w              (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->m_oLastFont2_set(&value);
    info.GetReturnValue().Set(value);
}
void ClearMode_set_w                 (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->ClearMode_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void IsRetina_set_w                  (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->IsRetina_set(to_bool(value));
    info.GetReturnValue().Set(value);
}
void dash_no_smart_set_w             (v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    CJSGraphics* pGraphics = unwrap_Graphics(info.Holder());
    pGraphics->dash_no_smart_set(&value);
    info.GetReturnValue().Set(value);
}

v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate)
{
    v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    v8::Isolate* current = v8::Isolate::GetCurrent();

    // свойства
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oContext"),                 m_oContext_get_w, m_oContext_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_dWidthMM"),                 m_dWidthMM_get_w, m_dWidthMM_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_dHeightMM"),                m_dHeightMM_get_w, m_dHeightMM_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_lWidthPix"),                m_lWidthPix_get_w, m_lWidthPix_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_lHeightPix"),               m_lHeightPix_get_w, m_lHeightPix_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiX"),                    m_dDpiX_get_w, m_dDpiX_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiY"),                    m_dDpiY_get_w, m_dDpiY_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_bIsBreak"),                 m_bIsBreak_get_w, m_bIsBreak_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oPen"),                     m_oPen_get_w, m_oPen_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_bPenColorInit"),            m_bPenColorInit_get_w, m_bPenColorInit_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oBrush"),                   m_oBrush_get_w, m_oBrush_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_bBrushColorInit"),          m_bBrushColorInit_get_w, m_bBrushColorInit_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFontManager"),             m_oFontManager_get_w, m_oFontManager_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oCoordTransform"),          m_oCoordTransform_get_w, m_oCoordTransform_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oBaseTransform"),           m_oBaseTransform_get_w, m_oBaseTransform_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFullTransform"),           m_oFullTransform_get_w, m_oFullTransform_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oInvertFullTransform"),     m_oInvertFullTransform_get_w, m_oInvertFullTransform_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "ArrayPoints"),                ArrayPoints_get_w, ArrayPoints_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oCurFont"),                 m_oCurFont_get_w, m_oCurFont_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oTextPr"),                  m_oTextPr_get_w, m_oTextPr_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oGrFonts"),                 m_oGrFonts_get_w, m_oGrFonts_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oLastFont"),                m_oLastFont_get_w, m_oLastFont_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "LastFontOriginInfo"),         LastFontOriginInfo_get_w, LastFontOriginInfo_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_bIntegerGrid"),             m_bIntegerGrid_get_w, m_bIntegerGrid_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "ClipManager"),                ClipManager_get_w, ClipManager_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "TextureFillTransformScaleX"), TextureFillTransformScaleX_get_w, TextureFillTransformScaleX_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "TextureFillTransformScaleY"), TextureFillTransformScaleY_get_w, TextureFillTransformScaleY_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "IsThumbnail"),                IsThumbnail_get_w, IsThumbnail_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "IsDemonstrationMode"),        IsDemonstrationMode_get_w, IsDemonstrationMode_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "IsDemonstrationMode"),        IsDemonstrationMode_get_w, IsDemonstrationMode_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "GrState"),                    GrState_get_w, GrState_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "globalAlpha"),                globalAlpha_get_w, globalAlpha_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "TextClipRect"),               TextClipRect_get_w, TextClipRect_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "IsClipContext"),              IsClipContext_get_w, IsClipContext_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "IsUseFonts2"),                IsUseFonts2_get_w, IsUseFonts2_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFontManager2"),            m_oFontManager2_get_w, m_oFontManager2_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "m_oLastFont2"),               m_oLastFont2_get_w, m_oLastFont2_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "ClearMode"),                  ClearMode_get_w, ClearMode_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "IsRetina"),                   IsRetina_get_w, IsRetina_set_w);
    result->SetAccessor(v8::String::NewFromUtf8(current, "dash_no_smart"),              dash_no_smart_get_w, dash_no_smart_set_w);

    // методы
    result->Set(current, "init",                     v8::FunctionTemplate::New(current, init_w));
    result->Set(current, "EndDraw",                  v8::FunctionTemplate::New(current, EndDraw_w));
    result->Set(current, "put_GlobalAlpha",          v8::FunctionTemplate::New(current, put_GlobalAlpha_w));
    result->Set(current, "Start_GlobalAlpha",        v8::FunctionTemplate::New(current, Start_GlobalAlpha_w));
    result->Set(current, "End_GlobalAlpha",          v8::FunctionTemplate::New(current, End_GlobalAlpha_w));
    result->Set(current, "p_color",                  v8::FunctionTemplate::New(current, p_color_w));
    result->Set(current, "p_width",                  v8::FunctionTemplate::New(current, p_width_w));
    result->Set(current, "p_dash",                   v8::FunctionTemplate::New(current, p_dash_w));
    result->Set(current, "b_color1",                 v8::FunctionTemplate::New(current, b_color1_w));
    result->Set(current, "b_color2",                 v8::FunctionTemplate::New(current, b_color2_w));
    result->Set(current, "transform",                v8::FunctionTemplate::New(current, transform_w));
    result->Set(current, "CalculateFullTransform",   v8::FunctionTemplate::New(current, CalculateFullTransform_w));
    result->Set(current, "_s",                       v8::FunctionTemplate::New(current, _s_w));
    result->Set(current, "_e",                       v8::FunctionTemplate::New(current, _e_w));
    result->Set(current, "_z",                       v8::FunctionTemplate::New(current, _z_w));
    result->Set(current, "_m",                       v8::FunctionTemplate::New(current, _m_w));
    result->Set(current, "_l",                       v8::FunctionTemplate::New(current, _l_w));
    result->Set(current, "_c",                       v8::FunctionTemplate::New(current, _c_w));
    result->Set(current, "_c2",                      v8::FunctionTemplate::New(current, _c2_w));
    result->Set(current, "ds",                       v8::FunctionTemplate::New(current, ds_w));
    result->Set(current, "df",                       v8::FunctionTemplate::New(current, df_w));
    result->Set(current, "save",                     v8::FunctionTemplate::New(current, save_w));
    result->Set(current, "restore",                  v8::FunctionTemplate::New(current, restore_w));
    result->Set(current, "clip",                     v8::FunctionTemplate::New(current, clip_w));
    result->Set(current, "reset",                    v8::FunctionTemplate::New(current, reset_w));
    result->Set(current, "transform3",               v8::FunctionTemplate::New(current, transform3_w));
    result->Set(current, "FreeFont",                 v8::FunctionTemplate::New(current, FreeFont_w));
    result->Set(current, "ClearLastFont",            v8::FunctionTemplate::New(current, ClearLastFont_w));
    result->Set(current, "drawImage2",               v8::FunctionTemplate::New(current, drawImage2_w));
    result->Set(current, "drawImage",                v8::FunctionTemplate::New(current, drawImage_w));
    result->Set(current, "GetFont",                  v8::FunctionTemplate::New(current, GetFont_w));
    result->Set(current, "font",                     v8::FunctionTemplate::New(current, font_w));
    result->Set(current, "SetFont",                  v8::FunctionTemplate::New(current, SetFont_w));
    result->Set(current, "SetTextPr",                v8::FunctionTemplate::New(current, SetTextPr_w));
    result->Set(current, "SetFontSlot",              v8::FunctionTemplate::New(current, SetFontSlot_w));
    result->Set(current, "GetTextPr",                v8::FunctionTemplate::New(current, GetTextPr_w));
    result->Set(current, "FillText",                 v8::FunctionTemplate::New(current, FillText_w));
    result->Set(current, "t",                        v8::FunctionTemplate::New(current, t_w));
    result->Set(current, "FillText2",                v8::FunctionTemplate::New(current, FillText2_w));
    result->Set(current, "t2",                       v8::FunctionTemplate::New(current, t2_w));
    result->Set(current, "FillTextCode",             v8::FunctionTemplate::New(current, FillTextCode_w));
    result->Set(current, "tg",                       v8::FunctionTemplate::New(current, tg_w));
    result->Set(current, "charspace",                v8::FunctionTemplate::New(current, charspace_w));
    result->Set(current, "private_FillGlyph",        v8::FunctionTemplate::New(current, private_FillGlyph_w));
    result->Set(current, "private_FillGlyphC",       v8::FunctionTemplate::New(current, private_FillGlyphC_w));
    result->Set(current, "private_FillGlyph2",       v8::FunctionTemplate::New(current, private_FillGlyph2_w));
    result->Set(current, "SetIntegerGrid",           v8::FunctionTemplate::New(current, SetIntegerGrid_w));
    result->Set(current, "GetIntegerGrid",           v8::FunctionTemplate::New(current, GetIntegerGrid_w));
    result->Set(current, "DrawStringASCII",          v8::FunctionTemplate::New(current, DrawStringASCII_w));
    result->Set(current, "DrawStringASCII2",         v8::FunctionTemplate::New(current, DrawStringASCII2_w));
    result->Set(current, "DrawHeaderEdit",           v8::FunctionTemplate::New(current, DrawHeaderEdit_w));
    result->Set(current, "DrawFooterEdit",           v8::FunctionTemplate::New(current, DrawFooterEdit_w));
    result->Set(current, "DrawLockParagraph",        v8::FunctionTemplate::New(current, DrawLockParagraph_w));
    result->Set(current, "DrawLockObjectRect",       v8::FunctionTemplate::New(current, DrawLockObjectRect_w));
    result->Set(current, "DrawEmptyTableLine",       v8::FunctionTemplate::New(current, DrawEmptyTableLine_w));
    result->Set(current, "DrawSpellingLine",         v8::FunctionTemplate::New(current, DrawSpellingLine_w));
    result->Set(current, "drawHorLine",              v8::FunctionTemplate::New(current, drawHorLine_w));
    result->Set(current, "drawHorLine2",             v8::FunctionTemplate::New(current, drawHorLine2_w));
    result->Set(current, "drawVerLine",              v8::FunctionTemplate::New(current, drawVerLine_w));
    result->Set(current, "drawHorLineExt",           v8::FunctionTemplate::New(current, drawHorLineExt_w));
    result->Set(current, "rect",                     v8::FunctionTemplate::New(current, rect_w));
    result->Set(current, "TableRect",                v8::FunctionTemplate::New(current, TableRect_w));
    result->Set(current, "AddClipRect",              v8::FunctionTemplate::New(current, AddClipRect_w));
    result->Set(current, "RemoveClipRect",           v8::FunctionTemplate::New(current, RemoveClipRect_w));
    result->Set(current, "SetClip",                  v8::FunctionTemplate::New(current, SetClip_w));
    result->Set(current, "RemoveClip",               v8::FunctionTemplate::New(current, RemoveClip_w));
    result->Set(current, "drawCollaborativeChanges", v8::FunctionTemplate::New(current, drawCollaborativeChanges_w));
    result->Set(current, "drawMailMergeField",       v8::FunctionTemplate::New(current, drawMailMergeField_w));
    result->Set(current, "drawSearchResult",         v8::FunctionTemplate::New(current, drawSearchResult_w));
    result->Set(current, "drawFlowAnchor",           v8::FunctionTemplate::New(current, drawFlowAnchor_w));
    result->Set(current, "SavePen",                  v8::FunctionTemplate::New(current, SavePen_w));
    result->Set(current, "RestorePen",               v8::FunctionTemplate::New(current, RestorePen_w));
    result->Set(current, "SaveBrush",                v8::FunctionTemplate::New(current, SaveBrush_w));
    result->Set(current, "RestoreBrush",             v8::FunctionTemplate::New(current, RestoreBrush_w));
    result->Set(current, "SavePenBrush",             v8::FunctionTemplate::New(current, SavePenBrush_w));
    result->Set(current, "RestorePenBrush",          v8::FunctionTemplate::New(current, RestorePenBrush_w));
    result->Set(current, "SaveGrState",              v8::FunctionTemplate::New(current, SaveGrState_w));
    result->Set(current, "RestoreGrState",           v8::FunctionTemplate::New(current, RestoreGrState_w));
    result->Set(current, "StartClipPath",            v8::FunctionTemplate::New(current, StartClipPath_w));
    result->Set(current, "EndClipPath",              v8::FunctionTemplate::New(current, EndClipPath_w));
    result->Set(current, "StartCheckTableDraw",      v8::FunctionTemplate::New(current, StartCheckTableDraw_w));
    result->Set(current, "EndCheckTableDraw",        v8::FunctionTemplate::New(current, EndCheckTableDraw_w));
    result->Set(current, "SetTextClipRect",          v8::FunctionTemplate::New(current, SetTextClipRect_w));
    result->Set(current, "AddSmartRect",             v8::FunctionTemplate::New(current, AddSmartRect_w));
    result->Set(current, "CheckUseFonts2",           v8::FunctionTemplate::New(current, CheckUseFonts2_w));
    result->Set(current, "UncheckUseFonts2",         v8::FunctionTemplate::New(current, UncheckUseFonts2_w));
    result->Set(current, "Drawing_StartCheckBounds", v8::FunctionTemplate::New(current, Drawing_StartCheckBounds_w));
    result->Set(current, "Drawing_EndCheckBounds",   v8::FunctionTemplate::New(current, Drawing_EndCheckBounds_w));
    result->Set(current, "DrawPresentationComment",  v8::FunctionTemplate::New(current, DrawPresentationComment_w));
    result->Set(current, "DrawPolygon",              v8::FunctionTemplate::New(current, DrawPolygon_w));
    result->Set(current, "DrawFootnoteRect",         v8::FunctionTemplate::New(current, DrawFootnoteRect_w));

    return result;
}
void CreateGraphics(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Handle<v8::ObjectTemplate> oUser = CreateGraphicsTemplate(isolate);
    CJSGraphics* pGraphics = new CJSGraphics();

    v8::Local<v8::Object> obj = oUser->NewInstance();
    obj->SetInternalField(0, v8::External::New(v8::Isolate::GetCurrent(), pGraphics));

    args.GetReturnValue().Set(obj);
}
*/
