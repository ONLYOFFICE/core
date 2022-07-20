#include "../TextMeasurerEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSMeasurer
{
    #define CURRENTWRAPPER CTextMeasurerEmbed

    FUNCTION_WRAPPER_V8_1(_FT_Malloc, FT_Malloc)
    FUNCTION_WRAPPER_V8_1(_FT_Free, FT_Free)

    FUNCTION_WRAPPER_V8  (_FT_Init, FT_Init)
    FUNCTION_WRAPPER_V8_2(_FT_Set_TrueType_HintProp, FT_Set_TrueType_HintProp)

    FUNCTION_WRAPPER_V8_4(_FT_Open_Face, FT_Open_Face)
    FUNCTION_WRAPPER_V8_3(_FT_Open_Face2, FT_Open_Face2)
    FUNCTION_WRAPPER_V8_1(_FT_GetFaceInfo, FT_GetFaceInfo)

    FUNCTION_WRAPPER_V8_3(_FT_Load_Glyph, FT_Load_Glyph)
    FUNCTION_WRAPPER_V8_2(_FT_Get_Glyph_Measure_Params, FT_Get_Glyph_Measure_Params)
    FUNCTION_WRAPPER_V8_2(_FT_Get_Glyph_Render_Params, FT_Get_Glyph_Render_Params)
    FUNCTION_WRAPPER_V8_2(_FT_Get_Glyph_Render_Buffer, FT_Get_Glyph_Render_Buffer)

    FUNCTION_WRAPPER_V8_5(_FT_Set_Transform, FT_Set_Transform)
    FUNCTION_WRAPPER_V8_5(_FT_Set_Char_Size, FT_Set_Char_Size)
    FUNCTION_WRAPPER_V8_2(_FT_SetCMapForCharCode, FT_SetCMapForCharCode)
    FUNCTION_WRAPPER_V8_3(_FT_GetKerningX, FT_GetKerningX)
    FUNCTION_WRAPPER_V8_1(_FT_GetFaceMaxAdvanceX, FT_GetFaceMaxAdvanceX)

#ifdef SUPPORT_HARFBUZZ_SHAPER
    FUNCTION_WRAPPER_V8_1(_HB_LanguageFromString, HB_LanguageFromString)
    FUNCTION_WRAPPER_V8_7(_HB_ShapeText, HB_ShapeText)
    FUNCTION_WRAPPER_V8  (_HB_FontMalloc, HB_FontMalloc)
    FUNCTION_WRAPPER_V8_1(_HB_FontFree, HB_FontFree)
#endif

    v8::Handle<v8::ObjectTemplate> CreateMeasurerTemplate(v8::Isolate* isolate)
    {
        v8::EscapableHandleScope handle_scope(isolate);

        v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
        result->SetInternalFieldCount(1);

        // методы
        NSV8Objects::Template_Set(result, "FT_Malloc",                  _FT_Malloc);
        NSV8Objects::Template_Set(result, "FT_Free",                    _FT_Free);

        NSV8Objects::Template_Set(result, "FT_Init",                    _FT_Init);
        NSV8Objects::Template_Set(result, "FT_Set_TrueType_HintProp",   _FT_Set_TrueType_HintProp);

        NSV8Objects::Template_Set(result, "FT_Open_Face",               _FT_Open_Face);
        NSV8Objects::Template_Set(result, "FT_Open_Face2",              _FT_Open_Face2);
        NSV8Objects::Template_Set(result, "FT_GetFaceInfo",             _FT_GetFaceInfo);

        NSV8Objects::Template_Set(result, "FT_Load_Glyph",              _FT_Load_Glyph);
        NSV8Objects::Template_Set(result, "FT_Get_Glyph_Measure_Params",_FT_Get_Glyph_Measure_Params);
        NSV8Objects::Template_Set(result, "FT_Get_Glyph_Render_Params", _FT_Get_Glyph_Render_Params);
        NSV8Objects::Template_Set(result, "FT_Get_Glyph_Render_Buffer", _FT_Get_Glyph_Render_Buffer);

        NSV8Objects::Template_Set(result, "FT_Set_Transform",           _FT_Set_Transform);
        NSV8Objects::Template_Set(result, "FT_Set_Char_Size",           _FT_Set_Char_Size);
        NSV8Objects::Template_Set(result, "FT_SetCMapForCharCode",      _FT_SetCMapForCharCode);
        NSV8Objects::Template_Set(result, "FT_GetKerningX",             _FT_GetKerningX);
        NSV8Objects::Template_Set(result, "FT_GetFaceMaxAdvanceX",      _FT_GetFaceMaxAdvanceX);

#ifdef SUPPORT_HARFBUZZ_SHAPER
        NSV8Objects::Template_Set(result, "HB_LanguageFromString",      _HB_LanguageFromString);
        NSV8Objects::Template_Set(result, "HB_ShapeText",               _HB_ShapeText);
        NSV8Objects::Template_Set(result, "HB_FontMalloc",              _HB_FontMalloc);
        NSV8Objects::Template_Set(result, "HB_FontFree",                _HB_FontFree);
#endif

        return handle_scope.Escape(result);
    }

    void CreateNativeMeasurer(const v8::FunctionCallbackInfo<v8::Value>& args)
    {
        v8::Isolate* isolate = args.GetIsolate();
        v8::HandleScope scope(isolate);

        v8::Handle<v8::ObjectTemplate> MeasurerTemplate = CreateMeasurerTemplate(isolate);
        CTextMeasurerEmbed* pMeasurer = new CTextMeasurerEmbed();

        v8::Local<v8::Object> obj = MeasurerTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
        obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), pMeasurer));

        NSJSBase::CJSEmbedObjectPrivate::CreateWeaker(obj);

        args.GetReturnValue().Set(obj);
    }
}

void CTextMeasurerEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
    v8::Isolate* current = CV8Worker::GetCurrent();
    context->m_internal->m_global->Set(current, name.c_str(), v8::FunctionTemplate::New(current, NSMeasurer::CreateNativeMeasurer));
}
