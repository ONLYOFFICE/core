#include "./TextMeasurerEmbed.h"
#include "./PointerEmbed.h"
#include "./../../fontengine/TextShaper.h"
#include "./../../fontengine/TextHyphen.h"

#define RAW_POINTER(value) ((CPointerEmbedObject*)value->toObject()->getNative())->Data
#define POINTER_OBJECT(value) ((CPointerEmbedObject*)value->toObject()->getNative())

// в js не хотим следить, чтобы в каждом face была ссылка на library - т.е. чтобы
// сначала удалились все face, а потом library - поэтому делаем свой счетчик ссылок
// и следим за library сами. Т.е. используем FT_Library_Reference/FT_Library_UnReference


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

CTextMeasurerEmbed::CTextMeasurerEmbed()
{
	m_hyphen_engine = new NSHyphen::CEngine();
}
CTextMeasurerEmbed::~CTextMeasurerEmbed()
{
	NSHyphen::CEngine* tmp = (NSHyphen::CEngine*)m_hyphen_engine;
	delete tmp;
	m_hyphen_engine = NULL;
}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Malloc(JSSmart<CJSValue> typed_array_or_len)
{
	void* pData = NULL;
	size_t len = 0;

	if (typed_array_or_len->isNumber())
	{
		len = (size_t)typed_array_or_len->toInt32();
		if (0 != len)
			pData = malloc((size_t)len);
	}
	else
	{
		JSSmart<CJSTypedArray> typedArray = typed_array_or_len->toTypedArray();
		CJSDataBuffer pBuffer = typedArray->getData();
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
	CPointerEmbedObject* pointer = new CPointerEmbedObject(NSShaper::FT_Library_Init(), [](void* data) { NSShaper::FT_Library_UnReference(data); });
	return pointer->createObject();
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Set_TrueType_HintProp(JSSmart<CJSValue> library, JSSmart<CJSValue> tt_interpreter)
{
	return CJSContext::createInt(NSShaper::FT_Set_TrueType_HintProp(RAW_POINTER(library), tt_interpreter->toUInt32()));
}

JSSmart<CJSValue> CTextMeasurerEmbed::FT_Open_Face(JSSmart<CJSValue> library, JSSmart<CJSValue> memory, JSSmart<CJSValue> size, JSSmart<CJSValue> face_index)
{
	void* ftlibrary = RAW_POINTER(library);
	void* face = NSShaper::FT_Open_Face(ftlibrary, (unsigned char*)RAW_POINTER(memory), size->toUInt32(), face_index->toInt32());
	if (NULL != face)
		NSShaper::FT_Library_Reference(ftlibrary);
	CPointerEmbedObject* pointer = new CPointerEmbedObject(face, [](void* data) { NSShaper::FT_Done_Face_With_Library(data); });
	return pointer->createObject();
}
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Open_Face2(JSSmart<CJSValue> library, JSSmart<CJSValue> array, JSSmart<CJSValue> face_index)
{
	void* ftlibrary = RAW_POINTER(library);
	JSSmart<CJSTypedArray> typedArray = array->toTypedArray();
	CJSDataBuffer buffer = typedArray->getData();
	void* face = NSShaper::FT_Open_Face(ftlibrary, (unsigned char*)buffer.Data, (unsigned int)buffer.Len, face_index->toInt32());
	if (NULL != face)
		NSShaper::FT_Library_Reference(ftlibrary);
	CPointerEmbedObject* pointer = new CPointerEmbedObject(face, [](void* data) { NSShaper::FT_Done_Face_With_Library(data); });
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
JSSmart<CJSValue> CTextMeasurerEmbed::FT_Get_Glyph_Render_Buffer(JSSmart<CJSValue> face, JSSmart<CJSValue> size)
{
	void* Data = NSShaper::FT_Get_Glyph_Render_Buffer(RAW_POINTER(face));
	int nSize = size->toInt32();
	int nSizeMax = NSShaper::FT_Get_Glyph_Render_BufferSize(RAW_POINTER(face));
	if (nSize > nSizeMax)
		nSize = nSizeMax;
	return CJSContext::createUint8Array((unsigned char*)Data, nSize, true);
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

#ifdef SUPPORT_HARFBUZZ_SHAPER
JSSmart<CJSValue> CTextMeasurerEmbed::HB_LanguageFromString(JSSmart<CJSValue> language_bcp_47)
{
	void* Data = NSShaper::HB_LanguageFromString(language_bcp_47->toStringA());
	CPointerEmbedObject* pObject = new CPointerEmbedObject(Data, NSPointerObjectDeleters::EmptyDeleter);
	return pObject->createObject();
}

JSSmart<CJSValue> CTextMeasurerEmbed::HB_ShapeText(JSSmart<CJSValue> face, JSSmart<CJSValue> font, JSSmart<CJSValue> text,
												   JSSmart<CJSValue> nFeatures, JSSmart<CJSValue> nScript, JSSmart<CJSValue> nDirection, JSSmart<CJSValue> nLanguage)
{
	CPointerEmbedObject* pFont = POINTER_OBJECT(font);
	CExternalPointerJS result;

	JSSmart<CJSTypedArray> typedArray = text->toTypedArray();
	CJSDataBuffer buffer = typedArray->getData();

	char* pText = (char*)buffer.Data;

	NSShaper::HB_ShapeText(RAW_POINTER(face), pFont->Data, pText,
						   nFeatures->toUInt32(), nScript->toUInt32(), nDirection->toUInt32(), RAW_POINTER(nLanguage),
						   &result, true);

	if (buffer.IsExternalize)
		buffer.Free();

	if (NULL == result.Data)
		return CJSContext::createNull();

	return CJSContext::createUint8Array(result.Data, result.Len, false);
}

JSSmart<CJSValue> CTextMeasurerEmbed::HB_FontMalloc()
{
	CPointerEmbedObject* pObject = new CPointerEmbedObject(NULL, [](void* data) { NSShaper::HB_FontFree(data); });
	return pObject->createObject();
}

JSSmart<CJSValue> CTextMeasurerEmbed::HB_FontFree(JSSmart<CJSValue> font)
{
	CPointerEmbedObject* pFont = POINTER_OBJECT(font);
	pFont->Free();
	return CJSContext::createUndefined();
}
#endif

JSSmart<CJSValue> CTextMeasurerEmbed::Hyphen_SetCacheSize(JSSmart<CJSValue> size)
{
	((NSHyphen::CEngine*)m_hyphen_engine)->SetCacheSize(size->toInt32());
	return CJSContext::createUndefined();
}

inline int GetUtf8SymbolLen(const unsigned char& c)
{
	if (0x00 == (c & 0x80))
		return 1;
	else if (0x00 == (c & 0x20))
		return 2;
	else if (0x00 == (c & 0x10))
		return 3;
	else if (0x00 == (c & 0x0F))
		return 4;
	else if (0x00 == (c & 0x08))
		return 4;
	else if (0x00 == (c & 0x04))
		return 5;
	return 6;
}

JSSmart<CJSValue> CTextMeasurerEmbed::Hyphen_Word(JSSmart<CJSValue> lang, JSSmart<CJSValue> word)
{
	std::string sWord = word->toStringA();
	const char* curUnicode = sWord.c_str();
	char* result = ((NSHyphen::CEngine*)m_hyphen_engine)->Process(lang->toInt32(), curUnicode, (int)sWord.length());

	if (!result)
		return CJSContext::createNull();

	int count = 0;
	int pos = 0;

	while (result[pos] != 0)
	{
		if (1 == (result[pos] & 1))
			++count;
		++pos;
	}

	if (0 == count)
		return CJSContext::createNull();

	CJSArray* ret = CJSContext::createArray(count);

	pos = 0;
	count = 0;
	while (result[pos] != 0)
	{
		if (1 == (result[pos] & 1))
			ret->set(count++, CJSContext::createInt(pos + 1));
		pos++;
	}

	return ret;
}

JSSmart<CJSValue> CTextMeasurerEmbed::Hyphen_IsDictionaryExist(JSSmart<CJSValue> lang)
{
	return CJSContext::createBool(((NSHyphen::CEngine*)m_hyphen_engine)->IsDictionaryExist(lang->toInt32()));
}
