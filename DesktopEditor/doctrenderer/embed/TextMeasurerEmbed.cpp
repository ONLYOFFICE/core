#include "./TextMeasurerEmbed.h"
#include "./../../fontengine/TextShaper.h"

#define RAW_POINTER(value) ((CPointerEmbedObject*)value->toObject()->getNative())->Data

class CExternalPointerJS : public NSShaper::CExternalPointer
{
public:
    CExternalPointerJS() : NSShaper::CExternalPointer() {}
    virtual ~CExternalPointerJS() {}

public:
    virtual void Alloc(const unsigned int& len)
    {
        Len = len;
        Data = NSAllocator::Alloc((size_t)Len);
    }
    virtual void Free()
    {
        if (Data)
            NSAllocator::Free(Data, (size_t)Len);
    }
};

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
    CPointerEmbedObject* pointer = new CPointerEmbedObject(NSShaper::FT_Library_Init(), [](void* data) { NSShaper::FT_Library_Destroy(data); });
    return pointer->createObject();
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Set_TrueType_HintProp(JSSmart<CJSValue> library, JSSmart<CJSValue> tt_interpreter)
{
    return CJSContext::createInt(NSShaper::FT_Set_TrueType_HintProp(RAW_POINTER(library), tt_interpreter->toUInt32()));
}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Open_Face(JSSmart<CJSValue> library, JSSmart<CJSValue> memory, JSSmart<CJSValue> size, JSSmart<CJSValue> face_index)
{
    void* face = NSShaper::FT_Open_Face(RAW_POINTER(library), (unsigned char*)RAW_POINTER(memory), size->toUInt32(), face_index->toInt32());
    CPointerEmbedObject* pointer = new CPointerEmbedObject(face, [](void* data) { NSShaper::FT_Done_Face(data); });
    return pointer->createObject();
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_GetFaceInfo(JSSmart<CJSValue> face)
{
    CExternalPointerJS result;
    if (!NSShaper::FT_GetFaceInfo(RAW_POINTER(face), &result))
        return CJSContext::createNull();

    return CJSContext::createUint8Array(result.Data, result.Len, false);
}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Load_Glyph(JSSmart<CJSValue> face, JSSmart<CJSValue> gid, JSSmart<CJSValue> mode)
{
    return CJSContext::createInt(NSShaper::FT_Load_Glyph(RAW_POINTER(face), gid->toUInt32(), mode->toInt32()));
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Get_Glyph_Measure_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> is_vector)
{
    CExternalPointerJS result;
    if (!NSShaper::FT_Get_Glyph_Measure_Params(RAW_POINTER(face), is_vector->toBool(), &result))
        return CJSContext::createNull();

    return CJSContext::createUint8Array(result.Data, result.Len, false);
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Get_Glyph_Render_Params(JSSmart<CJSValue> face, JSSmart<CJSValue> render_mode)
{
    CExternalPointerJS result;
    if (!NSShaper::FT_Get_Glyph_Render_Params(RAW_POINTER(face), render_mode->toInt32(), &result))
        return CJSContext::createNull();

    return CJSContext::createUint8Array(result.Data, result.Len, false);
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Get_Glyph_Render_Buffer(JSSmart<CJSValue> face)
{
    void* Data = NSShaper::FT_Get_Glyph_Render_Buffer(RAW_POINTER(face));
    CPointerEmbedObject* pObject = new CPointerEmbedObject(Data, NSPointerObjectDeleters::EmptyDeleter);
    return pObject->createObject();
}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Set_Transform(JSSmart<CJSValue> face, JSSmart<CJSValue> xx, JSSmart<CJSValue> yx, JSSmart<CJSValue> xy, JSSmart<CJSValue> yy)
{
    NSShaper::FT_Set_Transform(RAW_POINTER(face), xx->toInt32(), yx->toInt32(), xy->toInt32(), yy->toInt32());
    return CJSContext::createUndefined();
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Set_Char_Size(JSSmart<CJSValue> face, JSSmart<CJSValue> char_width, JSSmart<CJSValue> char_height, JSSmart<CJSValue> hres, JSSmart<CJSValue> vres)
{
    return CJSContext::createInt(NSShaper::FT_Set_Char_Size(RAW_POINTER(face), char_width->toInt32(), char_height->toInt32(), hres->toUInt32(), vres->toUInt32()));
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_SetCMapForCharCode(JSSmart<CJSValue> face, JSSmart<CJSValue> unicode)
{
    return CJSContext::createUInt(NSShaper::FT_SetCMapForCharCode(RAW_POINTER(face), unicode->toUInt32()));
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_GetKerningX(JSSmart<CJSValue> face, JSSmart<CJSValue> gid1, JSSmart<CJSValue> gid2)
{
    return CJSContext::createInt(NSShaper::FT_GetKerningX(RAW_POINTER(face), gid1->toUInt32(), gid2->toUInt32()));
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_GetFaceMaxAdvanceX(JSSmart<CJSValue> face)
{
    return CJSContext::createInt(NSShaper::FT_GetFaceMaxAdvanceX(RAW_POINTER(face)));
}
