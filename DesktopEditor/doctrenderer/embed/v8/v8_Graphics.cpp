#include "../GraphicsEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSGraphics
{
    #define CURRENTWRAPPER CGraphicsEmbed

    // FUNCTION
    FUNCTION_WRAPPER_V8_5(_init,              init)
    FUNCTION_WRAPPER_V8  (_destroy,           Destroy)
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
    FUNCTION_WRAPPER_V8  (_FreeFont,      FreeFont)
    FUNCTION_WRAPPER_V8  (_ClearLastFont, ClearLastFont)
    // images
    FUNCTION_WRAPPER_V8_7(_drawImage2, drawImage2)
    FUNCTION_WRAPPER_V8_8(_drawImage,  drawImage)
    // text
    FUNCTION_WRAPPER_V8  (_GetFont,      GetFont)
    FUNCTION_WRAPPER_V8_2(_font,         font)
    FUNCTION_WRAPPER_V8_4(_SetFont,      SetFont)
    FUNCTION_WRAPPER_V8  (_GetTextPr,    GetTextPr)
    FUNCTION_WRAPPER_V8_3(_FillText,     FillText)
    FUNCTION_WRAPPER_V8_3(_t,            t)
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
    FUNCTION_WRAPPER_V8_3(_DrawStringASCII,    DrawStringASCII)
    FUNCTION_WRAPPER_V8_3(_DrawStringASCII2,   DrawStringASCII2)
    FUNCTION_WRAPPER_V8_5(_DrawHeaderEdit,     DrawHeaderEdit)
    FUNCTION_WRAPPER_V8_5(_DrawFooterEdit,     DrawFooterEdit)
    FUNCTION_WRAPPER_V8_3(_DrawLockParagraph,  DrawLockParagraph)
    FUNCTION_WRAPPER_V8_4(_DrawLockObjectRect, DrawLockObjectRect)
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
    FUNCTION_WRAPPER_V8_4(_SetClip,                  SetClip)
    FUNCTION_WRAPPER_V8  (_RemoveClip,               RemoveClip)
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
    FUNCTION_WRAPPER_V8_4(_SetTextClipRect,          SetTextClipRect)
    FUNCTION_WRAPPER_V8_5(_AddSmartRect,             AddSmartRect)
    FUNCTION_WRAPPER_V8_1(_CheckUseFonts2,           CheckUseFonts2)
    FUNCTION_WRAPPER_V8  (_UncheckUseFonts2,         UncheckUseFonts2)
    FUNCTION_WRAPPER_V8_4(_Drawing_StartCheckBounds, Drawing_StartCheckBounds)
    FUNCTION_WRAPPER_V8  (_Drawing_EndCheckBounds,   Drawing_EndCheckBounds)
    FUNCTION_WRAPPER_V8_5(_DrawPresentationComment,  DrawPresentationComment)
    FUNCTION_WRAPPER_V8_3(_DrawPolygon,              DrawPolygon)
    FUNCTION_WRAPPER_V8_4(_DrawFootnoteRect,         DrawFootnoteRect)
    // new methods
    FUNCTION_WRAPPER_V8_1(_toDataURL,             toDataURL)
    FUNCTION_WRAPPER_V8  (_GetPenColor,           GetPenColor)
    FUNCTION_WRAPPER_V8  (_GetBrushColor,         GetBrushColor)
    FUNCTION_WRAPPER_V8_2(_put_brushTexture,      put_brushTexture)
    FUNCTION_WRAPPER_V8_1(_put_brushTextureMode,  put_brushTextureMode)
    FUNCTION_WRAPPER_V8_1(_put_BrushTextureAlpha, put_BrushTextureAlpha)
    FUNCTION_WRAPPER_V8_8(_put_BrushGradient,     put_BrushGradient)
    FUNCTION_WRAPPER_V8_2(_TransformPointX,       TransformPointX)
    FUNCTION_WRAPPER_V8_2(_TransformPointY,       TransformPointY)
    FUNCTION_WRAPPER_V8_1(_put_LineJoin,          put_LineJoin)
    FUNCTION_WRAPPER_V8  (_get_LineJoin,          get_LineJoin)
    FUNCTION_WRAPPER_V8_4(_put_TextureBounds,     put_TextureBounds)
    FUNCTION_WRAPPER_V8  (_GetlineWidth,          GetlineWidth)
    FUNCTION_WRAPPER_V8_1(_DrawPath,              DrawPath)
    FUNCTION_WRAPPER_V8_2(_CoordTransformOffset,  CoordTransformOffset)
    FUNCTION_WRAPPER_V8  (_GetTransform,          GetTransform)

    v8::Handle<v8::ObjectTemplate> CreateGraphicsTemplate(v8::Isolate* isolate)
    {
        v8::EscapableHandleScope handle_scope(isolate);

        v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
        result->SetInternalFieldCount(1);

        v8::Isolate* current = v8::Isolate::GetCurrent();

        // методы
        NSV8Objects::Template_Set(result, "create",                   _init);
        NSV8Objects::Template_Set(result, "Destroy",                  _destroy);
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
        NSV8Objects::Template_Set(result, "FreeFont",                 _FreeFont);
        NSV8Objects::Template_Set(result, "ClearLastFont",            _ClearLastFont);
        NSV8Objects::Template_Set(result, "drawImage2",               _drawImage2);
        NSV8Objects::Template_Set(result, "drawImage",                _drawImage);
        NSV8Objects::Template_Set(result, "GetFont",                  _GetFont);
        NSV8Objects::Template_Set(result, "font",                     _font);
        NSV8Objects::Template_Set(result, "SetFont",                  _SetFont);
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
        NSV8Objects::Template_Set(result, "SetTextClipRect",          _SetTextClipRect);
        NSV8Objects::Template_Set(result, "AddSmartRect",             _AddSmartRect);
        NSV8Objects::Template_Set(result, "CheckUseFonts2",           _CheckUseFonts2);
        NSV8Objects::Template_Set(result, "UncheckUseFonts2",         _UncheckUseFonts2);
        NSV8Objects::Template_Set(result, "Drawing_StartCheckBounds", _Drawing_StartCheckBounds);
        NSV8Objects::Template_Set(result, "Drawing_EndCheckBounds",   _Drawing_EndCheckBounds);
        NSV8Objects::Template_Set(result, "DrawPresentationComment",  _DrawPresentationComment);
        NSV8Objects::Template_Set(result, "DrawPolygon",              _DrawPolygon);
        NSV8Objects::Template_Set(result, "DrawFootnoteRect",         _DrawFootnoteRect);
        NSV8Objects::Template_Set(result, "toDataURL",                _toDataURL);
        NSV8Objects::Template_Set(result, "GetPenColor",              _GetPenColor);
        NSV8Objects::Template_Set(result, "GetBrushColor",            _GetBrushColor);
        NSV8Objects::Template_Set(result, "put_brushTexture",         _put_brushTexture);
        NSV8Objects::Template_Set(result, "put_brushTextureMode",     _put_brushTextureMode);
        NSV8Objects::Template_Set(result, "put_BrushTextureAlpha",    _put_BrushTextureAlpha);
        NSV8Objects::Template_Set(result, "put_BrushGradient",        _put_BrushGradient);
        NSV8Objects::Template_Set(result, "TransformPointX",          _TransformPointX);
        NSV8Objects::Template_Set(result, "TransformPointY",          _TransformPointY);
        NSV8Objects::Template_Set(result, "put_LineJoin",             _put_LineJoin);
        NSV8Objects::Template_Set(result, "get_LineJoin",             _get_LineJoin);
        NSV8Objects::Template_Set(result, "put_TextureBounds",        _put_TextureBounds);
        NSV8Objects::Template_Set(result, "GetlineWidth",             _GetlineWidth);
        NSV8Objects::Template_Set(result, "DrawPath",                 _DrawPath);
        NSV8Objects::Template_Set(result, "CoordTransformOffset",     _CoordTransformOffset);
        NSV8Objects::Template_Set(result, "GetTransform",             _GetTransform);

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

		NSJSBase::CJSEmbedObjectPrivate::CreateWeaker(obj);

        args.GetReturnValue().Set(obj);
    }
}

void CGraphicsEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
    v8::Isolate* current = CV8Worker::GetCurrent();
    context->m_internal->m_global->Set(current, name.c_str(), v8::FunctionTemplate::New(current, NSGraphics::CreateNativeGraphics));
}
