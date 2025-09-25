// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../TextMeasurerEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSTextMeasurerEmbed
{
#define CURRENTWRAPPER CTextMeasurerEmbed

#ifdef SUPPORT_HARFBUZZ_SHAPER
	FUNCTION_WRAPPER_V8_1(_HB_LanguageFromString, HB_LanguageFromString)
	FUNCTION_WRAPPER_V8_7(_HB_ShapeText, HB_ShapeText)
	FUNCTION_WRAPPER_V8_0(_HB_FontMalloc, HB_FontMalloc)
	FUNCTION_WRAPPER_V8_1(_HB_FontFree, HB_FontFree)
#endif
	FUNCTION_WRAPPER_V8_1(_FT_Malloc, FT_Malloc)
	FUNCTION_WRAPPER_V8_1(_FT_Free, FT_Free)
	FUNCTION_WRAPPER_V8_0(_FT_Init, FT_Init)
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
	FUNCTION_WRAPPER_V8_1(_Hyphen_SetCacheSize, Hyphen_SetCacheSize)
	FUNCTION_WRAPPER_V8_2(_Hyphen_Word, Hyphen_Word)
	FUNCTION_WRAPPER_V8_1(_Hyphen_IsDictionaryExist, Hyphen_IsDictionaryExist)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

#ifdef SUPPORT_HARFBUZZ_SHAPER
		NSV8Objects::Template_Set(result, "HB_LanguageFromString",	_HB_LanguageFromString);
		NSV8Objects::Template_Set(result, "HB_ShapeText",	_HB_ShapeText);
		NSV8Objects::Template_Set(result, "HB_FontMalloc",	_HB_FontMalloc);
		NSV8Objects::Template_Set(result, "HB_FontFree",	_HB_FontFree);
#endif
		NSV8Objects::Template_Set(result, "FT_Malloc",	_FT_Malloc);
		NSV8Objects::Template_Set(result, "FT_Free",	_FT_Free);
		NSV8Objects::Template_Set(result, "FT_Init",	_FT_Init);
		NSV8Objects::Template_Set(result, "FT_Set_TrueType_HintProp",	_FT_Set_TrueType_HintProp);
		NSV8Objects::Template_Set(result, "FT_Open_Face",	_FT_Open_Face);
		NSV8Objects::Template_Set(result, "FT_Open_Face2",	_FT_Open_Face2);
		NSV8Objects::Template_Set(result, "FT_GetFaceInfo",	_FT_GetFaceInfo);
		NSV8Objects::Template_Set(result, "FT_Load_Glyph",	_FT_Load_Glyph);
		NSV8Objects::Template_Set(result, "FT_Get_Glyph_Measure_Params",	_FT_Get_Glyph_Measure_Params);
		NSV8Objects::Template_Set(result, "FT_Get_Glyph_Render_Params",	_FT_Get_Glyph_Render_Params);
		NSV8Objects::Template_Set(result, "FT_Get_Glyph_Render_Buffer",	_FT_Get_Glyph_Render_Buffer);
		NSV8Objects::Template_Set(result, "FT_Set_Transform",	_FT_Set_Transform);
		NSV8Objects::Template_Set(result, "FT_Set_Char_Size",	_FT_Set_Char_Size);
		NSV8Objects::Template_Set(result, "FT_SetCMapForCharCode",	_FT_SetCMapForCharCode);
		NSV8Objects::Template_Set(result, "FT_GetKerningX",	_FT_GetKerningX);
		NSV8Objects::Template_Set(result, "FT_GetFaceMaxAdvanceX",	_FT_GetFaceMaxAdvanceX);
		NSV8Objects::Template_Set(result, "Hyphen_SetCacheSize",	_Hyphen_SetCacheSize);
		NSV8Objects::Template_Set(result, "Hyphen_Word",	_Hyphen_Word);
		NSV8Objects::Template_Set(result, "Hyphen_IsDictionaryExist",	_Hyphen_IsDictionaryExist);

		return handle_scope.Escape(result);
	}
}

class CTextMeasurerEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSTextMeasurerEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CTextMeasurerEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CTextMeasurerEmbedAdapter();
	return m_pAdapter;
}

std::string CTextMeasurerEmbed::getName() { return "CTextMeasurerEmbed"; }

CJSEmbedObject* CTextMeasurerEmbed::getCreator()
{
	return new CTextMeasurerEmbed();
}
