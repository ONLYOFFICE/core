#include "./TextMeasurerEmbed.h"

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Malloc(JSSmart<CJSValue> typed_array_or_len)
{
    void* pData = NULL;
    size_t len = 0;
    if (typed_array_or_len->isNumber())
    {
        len = (size_t)typed_array_or_len->toInt32();
        pData = malloc((size_t)len);
    }
    else
    {
        CJSDataBuffer pBuffer = typed_array_or_len->toTypedArray()->getData();
        pData = malloc(pBuffer.Len);
        memcpy(pData, pBuffer.Data, pBuffer.Len);
        if (pBuffer.IsExternalize)
            pBuffer.Free();
    }

    CPointerEmbedObject* pObject = new CPointerEmbedObject(pData, NSPointerObjectDeleters::FreeDeleter);
    return pObject->createObject();
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Free(JSSmart<CJSValue> pointer)
{
    CPointerEmbedObject* pEmbed = (CPointerEmbedObject*)pointer->toObject()->getNative();
    pEmbed->Free();
    return CJSContext::createUndefined();
}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Init()
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Set_TrueType_HintProp(JSSmart<CJSValue> library, JSSmart<CJSValue> tt_interpreter)
{

}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Open_Face(JSSmart<CJSValue> library, JSSmart<CJSValue> memory, JSSmart<CJSValue> size, JSSmart<CJSValue> face_index)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_GetFaceInfo(JSSmart<CJSValue> face)
{

}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Load_Glyph(JSSmart<CJSValue> face, JSSmart<CJSValue> gid, JSSmart<CJSValue> mode)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Get_Glyph_Measure_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> is_vector)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Get_Glyph_Render_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> render_mode)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Get_Glyph_Render_Buffer(JSSmart<CJSValue> face)
{

}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Set_Transform(JSSmart<CJSValue> face, JSSmart<CJSValue> xx, JSSmart<CJSValue> yx, JSSmart<CJSValue> xy, JSSmart<CJSValue> yy)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Set_Char_Size(JSSmart<CJSValue> face, JSSmart<CJSValue> char_width, JSSmart<CJSValue> char_height, JSSmart<CJSValue> hres, JSSmart<CJSValue> vres)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_SetCMapForCharCode(JSSmart<CJSValue> face, JSSmart<CJSValue> unicode)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_GetKerningX(JSSmart<CJSValue> face, JSSmart<CJSValue> gid1, JSSmart<CJSValue> gid2)
{

}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_GetFaceMaxAdvanceX(JSSmart<CJSValue> face)
{

}
