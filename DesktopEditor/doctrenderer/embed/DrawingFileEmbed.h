#ifndef _BUILD_DRAWING_EMBED_H_
#define _BUILD_DRAWING_EMBED_H_

#include "../js_internal/js_base.h"
#include "../../graphics/pro/officedrawingfile.h"

class CDrawingFile;

using namespace NSJSBase;
class JS_DECL CDrawingFileEmbed : public CJSEmbedObject
{
public:
	CDrawingFile* m_pFile;

public:
	CDrawingFileEmbed();
	~CDrawingFileEmbed();

	virtual void* getObject() override { return (void*)m_pFile; }

public:
	JSSmart<CJSValue> OpenFile(JSSmart<CJSValue> sFile, JSSmart<CJSValue> sPassword);
	JSSmart<CJSValue> CloseFile();

	JSSmart<CJSValue> GetType();
	JSSmart<CJSValue> GetErrorCode();

	JSSmart<CJSValue> GetInfo();

	JSSmart<CJSValue> GetPixmap(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor);
	JSSmart<CJSValue> DestroyPixmap(JSSmart<CJSValue> typedArray);

	JSSmart<CJSValue> GetLinks(JSSmart<CJSValue> nPageIndex);
	JSSmart<CJSValue> GetStructure();
	JSSmart<CJSValue> GetInteractiveFormsInfo();
	JSSmart<CJSValue> GetInteractiveFormsFonts(JSSmart<CJSValue> nTypeFonts);

	JSSmart<CJSValue> GetInteractiveFormsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nWidget, JSSmart<CJSValue> nView, JSSmart<CJSValue> nButtonView);
	JSSmart<CJSValue> GetButtonIcons(JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> bBase64, JSSmart<CJSValue> nButtonWidget, JSSmart<CJSValue> nIconView);
	JSSmart<CJSValue> GetAnnotationsInfo(JSSmart<CJSValue> nPageIndex);
	JSSmart<CJSValue> GetAnnotationsAP(JSSmart<CJSValue> nRasterW, JSSmart<CJSValue> nRasterH, JSSmart<CJSValue> nBackgroundColor, JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> nAnnot, JSSmart<CJSValue> nView);

	JSSmart<CJSValue> GetFontBinary(JSSmart<CJSValue> Id);
	JSSmart<CJSValue> GetGlyphs(JSSmart<CJSValue> nPageIndex);
	JSSmart<CJSValue> DestroyTextInfo();

	JSSmart<CJSValue> IsNeedCMap();
	JSSmart<CJSValue> ScanPage(JSSmart<CJSValue> nPageIndex, JSSmart<CJSValue> mode);

	JSSmart<CJSValue> GetImageBase64(JSSmart<CJSValue> rId);

	JSSmart<CJSValue> FreeWasmData(JSSmart<CJSValue> typedArray);

	JSSmart<CJSValue> SplitPages(JSSmart<CJSValue> arrPageIndexes, JSSmart<CJSValue> data);
	JSSmart<CJSValue> MergePages(JSSmart<CJSValue> data, JSSmart<CJSValue> nMaxID, JSSmart<CJSValue> sPrefixForm);
	JSSmart<CJSValue> UnmergePages();

	DECLARE_EMBED_METHODS
};

bool EmbedDrawingFile(JSSmart<NSJSBase::CJSContext>& context, IOfficeDrawingFile* pFile);

#endif // _BUILD_NATIVE_ZIP_EMBED_H_
