#include "../GraphicsEmbed.h"
#include "../../js_internal/v8/v8_base.h"

#define PROPERTY_GET(NAME, NAME_EMBED, TYPE)                                                               \
    void NAME(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)                \
    {                                                                                                      \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());                             \
        info.GetReturnValue().Set(TYPE::New(v8::Isolate::GetCurrent(), _this->m_pInternal->NAME_EMBED())); \
    }

#define PROPERTY_GET_OBJECT(NAME, NAME_EMBED)                                                \
    void NAME(v8::Local<v8::String> _name, const v8::PropertyCallbackInfo<v8::Value>& info)  \
    {                                                                                        \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());               \
        v8::Local<v8::Value>* v = _this->m_pInternal->NAME_EMBED();                          \
        if(v) info.GetReturnValue().Set(*v);                                                 \
        else  info.GetReturnValue().Set(v8::Undefined(v8::Isolate::GetCurrent()));           \
    }

#define PROPERTY_SET_DOUBLE(NAME, NAME_EMBED)                                                                      \
    void NAME(v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) \
    {                                                                                                              \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());                                     \
        JSSmart<NSJSBase::CJSValue> v = js_value(value);                                                           \
        _this->m_pInternal->NAME_EMBED(v->toDouble());                                                             \
        info.GetReturnValue().Set(value);                                                                          \
    }

#define PROPERTY_SET_INT(NAME, NAME_EMBED)                                                                         \
    void NAME(v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) \
    {                                                                                                              \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());                                     \
        JSSmart<NSJSBase::CJSValue> v = js_value(value);                                                           \
        _this->m_pInternal->NAME_EMBED(v->toInt32());                                                              \
        info.GetReturnValue().Set(value);                                                                          \
    }

#define PROPERTY_SET_BOOL(NAME, NAME_EMBED)                                                                        \
    void NAME(v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) \
    {                                                                                                              \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());                                     \
        JSSmart<NSJSBase::CJSValue> v = js_value(value);                                                           \
        _this->m_pInternal->NAME_EMBED(v->toBool());                                                               \
        info.GetReturnValue().Set(value);                                                                          \
    }

#define PROPERTY_SET_OBJECT(NAME, NAME_EMBED)                                                                      \
    void NAME(v8::Local<v8::String> _name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) \
    {                                                                                                              \
        CURRENTWRAPPER* _this = (CURRENTWRAPPER*)unwrap_native(info.Holder());                                     \
        _this->m_pInternal->NAME_EMBED(&value);                                                                    \
        info.GetReturnValue().Set(value);                                                                          \
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
    PROPERTY_GET(_g_m_dWidthMM,   g_m_dWidthMM,   v8::Number)
    PROPERTY_GET(_g_m_dHeightMM,  g_m_dHeightMM,  v8::Number)
    PROPERTY_GET(_g_m_lWidthPix,  g_m_lWidthPix,  v8::Number)
    PROPERTY_GET(_g_m_lHeightPix, g_m_lHeightPix, v8::Number)
    PROPERTY_GET(_g_m_dDpiX,      g_m_dDpiX,      v8::Number)
    PROPERTY_GET(_g_m_dDpiY,      g_m_dDpiY,      v8::Number)
    PROPERTY_GET(_g_m_bIsBreak,          g_m_bIsBreak,          v8::Boolean)
    PROPERTY_GET(_g_m_bPenColorInit,     g_m_bPenColorInit,     v8::Boolean)
    PROPERTY_GET(_g_m_bBrushColorInit,   g_m_bBrushColorInit,   v8::Boolean)
    PROPERTY_GET(_g_m_bIntegerGrid,      g_m_bIntegerGrid,      v8::Boolean)
    PROPERTY_GET(_g_IsThumbnail,         g_IsThumbnail,         v8::Boolean)
    PROPERTY_GET(_g_IsDemonstrationMode, g_IsDemonstrationMode, v8::Boolean)
    PROPERTY_GET(_g_IsClipContext,       g_IsClipContext,       v8::Boolean)
    PROPERTY_GET(_g_IsUseFonts2,         g_IsUseFonts2,         v8::Boolean)
    PROPERTY_GET(_g_ClearMode,           g_ClearMode,           v8::Boolean)
    PROPERTY_GET(_g_IsRetina,            g_IsRetina,            v8::Boolean)
    // PROPERTY_GET_OBJECT(_g_m_oCurFont,          g_m_oCurFont)
    // PROPERTY_GET_OBJECT(_g_LastFontOriginInfo,  g_LastFontOriginInfo)
    PROPERTY_GET(_g_TextureFillTransformScaleX, g_TextureFillTransformScaleX, v8::Int32)
    PROPERTY_GET(_g_TextureFillTransformScaleY, g_TextureFillTransformScaleY, v8::Int32)
    PROPERTY_GET(_g_globalAlpha,                g_globalAlpha,                v8::Int32)

    // PROPERTY SET
    PROPERTY_SET_OBJECT(_s_m_oContext,             s_m_oContext)
    PROPERTY_SET_OBJECT(_s_m_oPen,                 s_m_oPen)
    PROPERTY_SET_OBJECT(_s_m_oBrush,               s_m_oBrush)
    PROPERTY_SET_OBJECT(_s_m_oFontManager,         s_m_oFontManager)
    PROPERTY_SET_OBJECT(_s_m_oCoordTransform,      s_m_oCoordTransform)
    PROPERTY_SET_OBJECT(_s_m_oBaseTransform,       s_m_oBaseTransform)
    PROPERTY_SET_OBJECT(_s_m_oTransform,           s_m_oTransform)
    PROPERTY_SET_OBJECT(_s_m_oFullTransform,       s_m_oFullTransform)
    PROPERTY_SET_OBJECT(_s_m_oInvertFullTransform, s_m_oInvertFullTransform)
    PROPERTY_SET_OBJECT(_s_ArrayPoints,            s_ArrayPoints)
    PROPERTY_SET_OBJECT(_s_m_oTextPr,              s_m_oTextPr)
    PROPERTY_SET_OBJECT(_s_m_oGrFonts,             s_m_oGrFonts)
    PROPERTY_SET_OBJECT(_s_m_oLastFont,            s_m_oLastFont)
    PROPERTY_SET_OBJECT(_s_ClipManager,            s_ClipManager)
    PROPERTY_SET_OBJECT(_s_GrState,                s_GrState)
    PROPERTY_SET_OBJECT(_s_TextClipRect,           s_TextClipRect)
    PROPERTY_SET_OBJECT(_s_m_oFontManager2,        s_m_oFontManager2)
    PROPERTY_SET_OBJECT(_s_m_oLastFont2,           s_m_oLastFont2)
    PROPERTY_SET_OBJECT(_s_dash_no_smart,          s_dash_no_smart)
    PROPERTY_SET_DOUBLE(_s_m_dWidthMM,   s_m_dWidthMM)
    PROPERTY_SET_DOUBLE(_s_m_dHeightMM,  s_m_dHeightMM)
    PROPERTY_SET_DOUBLE(_s_m_lWidthPix,  s_m_lWidthPix)
    PROPERTY_SET_DOUBLE(_s_m_lHeightPix, s_m_lHeightPix)
    PROPERTY_SET_DOUBLE(_s_m_dDpiX,      s_m_dDpiX)
    PROPERTY_SET_DOUBLE(_s_m_dDpiY,      s_m_dDpiY)
    PROPERTY_SET_BOOL(_s_m_bIsBreak,          s_m_bIsBreak)
    PROPERTY_SET_BOOL(_s_m_bPenColorInit,     s_m_bPenColorInit)
    PROPERTY_SET_BOOL(_s_m_bBrushColorInit,   s_m_bBrushColorInit)
    PROPERTY_SET_BOOL(_s_m_bIntegerGrid,      s_m_bIntegerGrid)
    PROPERTY_SET_BOOL(_s_IsThumbnail,         s_IsThumbnail)
    PROPERTY_SET_BOOL(_s_IsDemonstrationMode, s_IsDemonstrationMode)
    PROPERTY_SET_BOOL(_s_IsClipContext,       s_IsClipContext)
    PROPERTY_SET_BOOL(_s_IsUseFonts2,         s_IsUseFonts2)
    PROPERTY_SET_BOOL(_s_ClearMode,           s_ClearMode)
    PROPERTY_SET_BOOL(_s_IsRetina,            s_IsRetina)
    // PROPERTY_SET_OBJECT(_s_m_oCurFont,          s_m_oCurFont)
    // PROPERTY_SET_OBJECT(_s_LastFontOriginInfo,  s_LastFontOriginInfo)
    PROPERTY_SET_INT(_s_TextureFillTransformScaleX, s_TextureFillTransformScaleX)
    PROPERTY_SET_INT(_s_TextureFillTransformScaleY, s_TextureFillTransformScaleY)
    PROPERTY_SET_INT(_s_globalAlpha,                s_globalAlpha)

    v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate)
    {
        v8::EscapableHandleScope handle_scope(isolate);

        v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New();
        result->SetInternalFieldCount(1);

        v8::Isolate* current = v8::Isolate::GetCurrent();

        // свойства
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oContext"),                 _g_m_oContext,                 _s_m_oContext);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dWidthMM"),                 _g_m_dWidthMM,                 _s_m_dWidthMM);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dHeightMM"),                _g_m_dHeightMM,                _s_m_dHeightMM);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_lWidthPix"),                _g_m_lWidthPix,                _s_m_lWidthPix);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_lHeightPix"),               _g_m_lHeightPix,               _s_m_lHeightPix);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiX"),                    _g_m_dDpiX,                    _s_m_dDpiX);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_dDpiY"),                    _g_m_dDpiY,                    _s_m_dDpiY);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bIsBreak"),                 _g_m_bIsBreak,                 _s_m_bIsBreak);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oPen"),                     _g_m_oPen,                     _s_m_oPen);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bPenColorInit"),            _g_m_bPenColorInit,            _s_m_bPenColorInit);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oBrush"),                   _g_m_oBrush,                   _s_m_oBrush);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bBrushColorInit"),          _g_m_bBrushColorInit,          _s_m_bBrushColorInit);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFontManager"),             _g_m_oFontManager,             _s_m_oFontManager);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oCoordTransform"),          _g_m_oCoordTransform,          _s_m_oCoordTransform);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oBaseTransform"),           _g_m_oBaseTransform,           _s_m_oBaseTransform);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFullTransform"),           _g_m_oFullTransform,           _s_m_oFullTransform);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oInvertFullTransform"),     _g_m_oInvertFullTransform,     _s_m_oInvertFullTransform);
        result->SetAccessor(v8::String::NewFromUtf8(current, "ArrayPoints"),                _g_ArrayPoints,                _s_ArrayPoints);
        // result->SetAccessor(v8::String::NewFromUtf8(current, "m_oCurFont"),                 _g_m_oCurFont,                 _s_m_oCurFont);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oTextPr"),                  _g_m_oTextPr,                  _s_m_oTextPr);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oGrFonts"),                 _g_m_oGrFonts,                 _s_m_oGrFonts);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oLastFont"),                _g_m_oLastFont,                _s_m_oLastFont);
        // result->SetAccessor(v8::String::NewFromUtf8(current, "LastFontOriginInfo"),         _g_LastFontOriginInfo,         _s_LastFontOriginInfo);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_bIntegerGrid"),             _g_m_bIntegerGrid,             _s_m_bIntegerGrid);
        result->SetAccessor(v8::String::NewFromUtf8(current, "ClipManager"),                _g_ClipManager,                _s_ClipManager);
        result->SetAccessor(v8::String::NewFromUtf8(current, "TextureFillTransformScaleX"), _g_TextureFillTransformScaleX, _s_TextureFillTransformScaleX);
        result->SetAccessor(v8::String::NewFromUtf8(current, "TextureFillTransformScaleY"), _g_TextureFillTransformScaleY, _s_TextureFillTransformScaleY);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsThumbnail"),                _g_IsThumbnail,                _s_IsThumbnail);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsDemonstrationMode"),        _g_IsDemonstrationMode,        _s_IsDemonstrationMode);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsDemonstrationMode"),        _g_IsDemonstrationMode,        _s_IsDemonstrationMode);
        result->SetAccessor(v8::String::NewFromUtf8(current, "GrState"),                    _g_GrState,                    _s_GrState);
        result->SetAccessor(v8::String::NewFromUtf8(current, "globalAlpha"),                _g_globalAlpha,                _s_globalAlpha);
        result->SetAccessor(v8::String::NewFromUtf8(current, "TextClipRect"),               _g_TextClipRect,               _s_TextClipRect);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsClipContext"),              _g_IsClipContext,              _s_IsClipContext);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsUseFonts2"),                _g_IsUseFonts2,                _s_IsUseFonts2);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oFontManager2"),            _g_m_oFontManager2,            _s_m_oFontManager2);
        result->SetAccessor(v8::String::NewFromUtf8(current, "m_oLastFont2"),               _g_m_oLastFont2,               _s_m_oLastFont2);
        result->SetAccessor(v8::String::NewFromUtf8(current, "ClearMode"),                  _g_ClearMode,                  _s_ClearMode);
        result->SetAccessor(v8::String::NewFromUtf8(current, "IsRetina"),                   _g_IsRetina,                   _s_IsRetina);
        result->SetAccessor(v8::String::NewFromUtf8(current, "dash_no_smart"),              _g_dash_no_smart,              _s_dash_no_smart);

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
}

void CGraphicsEmbed::CreateNativeGraphics(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);

    v8::Handle<v8::ObjectTemplate> GraphicsTemplate = NSGraphics::CreateGraphicsTemplate(isolate);
    CGraphicsEmbed* pGraphics = new CGraphicsEmbed();

    v8::Local<v8::Object> obj = GraphicsTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), pGraphics));

    args.GetReturnValue().Set(obj);
}

void CGraphicsEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
    v8::Isolate* current = CV8Worker::GetCurrent();
    context->m_internal->m_global->Set(current, name.c_str(), v8::FunctionTemplate::New(current, CreateNativeGraphics));
}
