#ifndef _BUILD_NATIVE_TEXT_MEASURER_EMBED_H_
#define _BUILD_NATIVE_TEXT_MEASURER_EMBED_H_

#include "../js_internal/js_base.h"
#include "./PointerEmbed.h"

#if defined(__ANDROID__) || defined(_IOS)
#define SUPPORT_HARFBUZZ_SHAPER
#endif

using namespace NSJSBase;
class CTextMeasurerEmbed : public CJSEmbedObject
{
public:
    CTextMeasurerEmbed()
    {
    }
    ~CTextMeasurerEmbed()
    {
    }

public:
    JSSmart<CJSValue> FT_Malloc(JSSmart<CJSValue> typed_array_or_len);
    JSSmart<CJSValue> FT_Free(JSSmart<CJSValue> pointer);

    JSSmart<CJSValue> FT_Init();
    JSSmart<CJSValue> FT_Set_TrueType_HintProp(JSSmart<CJSValue> library, JSSmart<CJSValue> tt_interpreter);

    JSSmart<CJSValue> FT_Open_Face(JSSmart<CJSValue> library, JSSmart<CJSValue> memory, JSSmart<CJSValue> size, JSSmart<CJSValue> face_index);
    JSSmart<CJSValue> FT_Open_Face2(JSSmart<CJSValue> library, JSSmart<CJSValue> array, JSSmart<CJSValue> face_index);
    JSSmart<CJSValue> FT_GetFaceInfo(JSSmart<CJSValue> face);

    JSSmart<CJSValue> FT_Load_Glyph(JSSmart<CJSValue> face, JSSmart<CJSValue> gid, JSSmart<CJSValue> mode);
    JSSmart<CJSValue> FT_Get_Glyph_Measure_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> is_vector);
    JSSmart<CJSValue> FT_Get_Glyph_Render_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> render_mode);
    JSSmart<CJSValue> FT_Get_Glyph_Render_Buffer(JSSmart<CJSValue> face, JSSmart<CJSValue> size);

    JSSmart<CJSValue> FT_Set_Transform(JSSmart<CJSValue> face, JSSmart<CJSValue> xx, JSSmart<CJSValue> yx, JSSmart<CJSValue> xy, JSSmart<CJSValue> yy);
    JSSmart<CJSValue> FT_Set_Char_Size(JSSmart<CJSValue> face, JSSmart<CJSValue> char_width, JSSmart<CJSValue> char_height, JSSmart<CJSValue> hres, JSSmart<CJSValue> vres);
    JSSmart<CJSValue> FT_SetCMapForCharCode(JSSmart<CJSValue> face, JSSmart<CJSValue> unicode);
    JSSmart<CJSValue> FT_GetKerningX(JSSmart<CJSValue> face, JSSmart<CJSValue> gid1, JSSmart<CJSValue> gid2);
    JSSmart<CJSValue> FT_GetFaceMaxAdvanceX(JSSmart<CJSValue> face);

#ifdef SUPPORT_HARFBUZZ_SHAPER
    JSSmart<CJSValue> HB_LanguageFromString(JSSmart<CJSValue> language_bcp_47);

    JSSmart<CJSValue> HB_ShapeText(JSSmart<CJSValue> face, JSSmart<CJSValue> font, JSSmart<CJSValue> text,
                                   JSSmart<CJSValue> nFeatures, JSSmart<CJSValue> nScript, JSSmart<CJSValue> nDirection, JSSmart<CJSValue> nLanguage);

    JSSmart<CJSValue> HB_FontMalloc();
    JSSmart<CJSValue> HB_FontFree(JSSmart<CJSValue> font);
#endif

    static void CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context);
};

#endif // _BUILD_NATIVE_TEXT_MEASURER_EMBED_H_
