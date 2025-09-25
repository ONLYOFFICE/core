// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../TextMeasurerEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCTextMeasurerEmbed <JSExport>
#ifdef SUPPORT_HARFBUZZ_SHAPER
-(JSValue*) HB_LanguageFromString : (JSValue*)language_bcp_47;
-(JSValue*) HB_ShapeText : (JSValue*)face : (JSValue*)font : (JSValue*)text : (JSValue*)nFeatures : (JSValue*)nScript : (JSValue*)nDirection : (JSValue*)nLanguage;
-(JSValue*) HB_FontMalloc;
-(JSValue*) HB_FontFree : (JSValue*)font;
#endif
-(JSValue*) FT_Malloc : (JSValue*)typed_array_or_len;
-(JSValue*) FT_Free : (JSValue*)pointer;
-(JSValue*) FT_Init;
-(JSValue*) FT_Set_TrueType_HintProp : (JSValue*)library : (JSValue*)tt_interpreter;
-(JSValue*) FT_Open_Face : (JSValue*)library : (JSValue*)memory : (JSValue*)size : (JSValue*)face_index;
-(JSValue*) FT_Open_Face2 : (JSValue*)library : (JSValue*)array : (JSValue*)face_index;
-(JSValue*) FT_GetFaceInfo : (JSValue*)face;
-(JSValue*) FT_Load_Glyph : (JSValue*)face : (JSValue*)gid : (JSValue*)mode;
-(JSValue*) FT_Get_Glyph_Measure_Params : (JSValue*)face : (JSValue*)is_vector;
-(JSValue*) FT_Get_Glyph_Render_Params : (JSValue*)face : (JSValue*)render_mode;
-(JSValue*) FT_Get_Glyph_Render_Buffer : (JSValue*)face : (JSValue*)size;
-(JSValue*) FT_Set_Transform : (JSValue*)face : (JSValue*)xx : (JSValue*)yx : (JSValue*)xy : (JSValue*)yy;
-(JSValue*) FT_Set_Char_Size : (JSValue*)face : (JSValue*)char_width : (JSValue*)char_height : (JSValue*)hres : (JSValue*)vres;
-(JSValue*) FT_SetCMapForCharCode : (JSValue*)face : (JSValue*)unicode;
-(JSValue*) FT_GetKerningX : (JSValue*)face : (JSValue*)gid1 : (JSValue*)gid2;
-(JSValue*) FT_GetFaceMaxAdvanceX : (JSValue*)face;
-(JSValue*) Hyphen_SetCacheSize : (JSValue*)size;
-(JSValue*) Hyphen_Word : (JSValue*)lang : (JSValue*)word;
-(JSValue*) Hyphen_IsDictionaryExist : (JSValue*)lang;
@end

@interface CJSCTextMeasurerEmbed : NSObject<IJSCTextMeasurerEmbed, JSEmbedObjectProtocol>
{
@public
	CTextMeasurerEmbed* m_internal;
}
@end

@implementation CJSCTextMeasurerEmbed
EMBED_OBJECT_WRAPPER_METHODS(CTextMeasurerEmbed);

#ifdef SUPPORT_HARFBUZZ_SHAPER
FUNCTION_WRAPPER_JS_1(HB_LanguageFromString, HB_LanguageFromString)
FUNCTION_WRAPPER_JS_7(HB_ShapeText, HB_ShapeText)
FUNCTION_WRAPPER_JS_0(HB_FontMalloc, HB_FontMalloc)
FUNCTION_WRAPPER_JS_1(HB_FontFree, HB_FontFree)
#endif
FUNCTION_WRAPPER_JS_1(FT_Malloc, FT_Malloc)
FUNCTION_WRAPPER_JS_1(FT_Free, FT_Free)
FUNCTION_WRAPPER_JS_0(FT_Init, FT_Init)
FUNCTION_WRAPPER_JS_2(FT_Set_TrueType_HintProp, FT_Set_TrueType_HintProp)
FUNCTION_WRAPPER_JS_4(FT_Open_Face, FT_Open_Face)
FUNCTION_WRAPPER_JS_3(FT_Open_Face2, FT_Open_Face2)
FUNCTION_WRAPPER_JS_1(FT_GetFaceInfo, FT_GetFaceInfo)
FUNCTION_WRAPPER_JS_3(FT_Load_Glyph, FT_Load_Glyph)
FUNCTION_WRAPPER_JS_2(FT_Get_Glyph_Measure_Params, FT_Get_Glyph_Measure_Params)
FUNCTION_WRAPPER_JS_2(FT_Get_Glyph_Render_Params, FT_Get_Glyph_Render_Params)
FUNCTION_WRAPPER_JS_2(FT_Get_Glyph_Render_Buffer, FT_Get_Glyph_Render_Buffer)
FUNCTION_WRAPPER_JS_5(FT_Set_Transform, FT_Set_Transform)
FUNCTION_WRAPPER_JS_5(FT_Set_Char_Size, FT_Set_Char_Size)
FUNCTION_WRAPPER_JS_2(FT_SetCMapForCharCode, FT_SetCMapForCharCode)
FUNCTION_WRAPPER_JS_3(FT_GetKerningX, FT_GetKerningX)
FUNCTION_WRAPPER_JS_1(FT_GetFaceMaxAdvanceX, FT_GetFaceMaxAdvanceX)
FUNCTION_WRAPPER_JS_1(Hyphen_SetCacheSize, Hyphen_SetCacheSize)
FUNCTION_WRAPPER_JS_2(Hyphen_Word, Hyphen_Word)
FUNCTION_WRAPPER_JS_1(Hyphen_IsDictionaryExist, Hyphen_IsDictionaryExist)
@end

class CTextMeasurerEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCTextMeasurerEmbed alloc] init:(CTextMeasurerEmbed*)pNative];
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
