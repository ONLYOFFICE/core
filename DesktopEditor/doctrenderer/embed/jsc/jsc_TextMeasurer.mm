#include "../TextMeasurerEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCTextMeasurer <JSExport>
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
-(JSValue*) FT_Set_Char_Size : (JSValue*)face : (JSValue*)cw : (JSValue*)ch : (JSValue*)hres : (JSValue*)vres;
-(JSValue*) FT_SetCMapForCharCode : (JSValue*)face : (JSValue*)unicode;
-(JSValue*) FT_GetKerningX : (JSValue*)face : (JSValue*)gid1 : (JSValue*)gid2;
-(JSValue*) FT_GetFaceMaxAdvanceX : (JSValue*)face;

#ifdef SUPPORT_HARFBUZZ_SHAPER
-(JSValue*) HB_LanguageFromString : (JSValue*)lang;
-(JSValue*) HB_ShapeText : (JSValue*) face : (JSValue*) font : (JSValue*) text : (JSValue*) nFeatures : (JSValue*) nScript : (JSValue*) nDirection : (JSValue*) nLanguage;
-(JSValue*) HB_FontMalloc;
-(JSValue*) HB_FontFree : (JSValue*)font;
#endif

@end

@interface CJSCTextMeasurer : NSObject<IJSCTextMeasurer, JSEmbedObjectProtocol>
{
@public
    CTextMeasurerEmbed* m_internal;
}
@end

@implementation CJSCTextMeasurer

EMBED_OBJECT_WRAPPER_METHODS(CTextMeasurerEmbed)

FUNCTION_WRAPPER_JS_1(FT_Malloc, FT_Malloc)
FUNCTION_WRAPPER_JS_1(FT_Free, FT_Free)

FUNCTION_WRAPPER_JS(FT_Init, FT_Init)
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

#ifdef SUPPORT_HARFBUZZ_SHAPER
FUNCTION_WRAPPER_JS_1(HB_LanguageFromString, HB_LanguageFromString)
FUNCTION_WRAPPER_JS_7(HB_ShapeText, HB_ShapeText)
FUNCTION_WRAPPER_JS(HB_FontMalloc, HB_FontMalloc)
FUNCTION_WRAPPER_JS_1(HB_FontFree, HB_FontFree)
#endif

@end

void CTextMeasurerEmbed::CreateObjectInContext(const std::string &name, JSSmart<CJSContext> context)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        return [[CJSCTextMeasurer alloc] init];
    };
}
