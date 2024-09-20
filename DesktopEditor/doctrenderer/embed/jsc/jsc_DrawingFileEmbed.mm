// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../DrawingFileEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCDrawingFileEmbed <JSExport>
-(JSValue*) OpenFile : (JSValue*)sFile : (JSValue*)sPassword;
-(JSValue*) CloseFile;
-(JSValue*) GetType;
-(JSValue*) GetErrorCode;
-(JSValue*) GetInfo;
-(JSValue*) GetPixmap : (JSValue*)nPageIndex : (JSValue*)nRasterW : (JSValue*)nRasterH : (JSValue*)nBackgroundColor;
-(JSValue*) DestroyPixmap : (JSValue*)typedArray;
-(JSValue*) GetLinks : (JSValue*)nPageIndex;
-(JSValue*) GetStructure;
-(JSValue*) GetInteractiveFormsInfo;
-(JSValue*) GetInteractiveFormsFonts : (JSValue*)nTypeFonts;
-(JSValue*) GetInteractiveFormsAP : (JSValue*)nRasterW : (JSValue*)nRasterH : (JSValue*)nBackgroundColor : (JSValue*)nPageIndex : (JSValue*)nWidget : (JSValue*)nView : (JSValue*)nButtonView;
-(JSValue*) GetButtonIcons : (JSValue*)nBackgroundColor : (JSValue*)nPageIndex : (JSValue*)bBase64 : (JSValue*)nButtonWidget : (JSValue*)nIconView;
-(JSValue*) GetAnnotationsInfo : (JSValue*)nPageIndex;
-(JSValue*) GetAnnotationsAP : (JSValue*)nRasterW : (JSValue*)nRasterH : (JSValue*)nBackgroundColor : (JSValue*)nPageIndex : (JSValue*)nAnnot : (JSValue*)nView;
-(JSValue*) GetGlyphs : (JSValue*)nPageIndex;
-(JSValue*) DestroyTextInfo;
-(JSValue*) IsNeedCMap;
-(JSValue*) ScanPage : (JSValue*)nPageIndex : (JSValue*)mode;
-(JSValue*) GetImageBase64 : (JSValue*)rId;
-(JSValue*) FreeWasmData : (JSValue*)typedArray;
@end

@interface CJSCDrawingFileEmbed : NSObject<IJSCDrawingFileEmbed, JSEmbedObjectProtocol>
{
@public
	CDrawingFileEmbed* m_internal;
}
@end

@implementation CJSCDrawingFileEmbed
EMBED_OBJECT_WRAPPER_METHODS(CDrawingFileEmbed);

FUNCTION_WRAPPER_JS_2(OpenFile, OpenFile)
FUNCTION_WRAPPER_JS_0(CloseFile, CloseFile)
FUNCTION_WRAPPER_JS_0(GetType, GetType)
FUNCTION_WRAPPER_JS_0(GetErrorCode, GetErrorCode)
FUNCTION_WRAPPER_JS_0(GetInfo, GetInfo)
FUNCTION_WRAPPER_JS_4(GetPixmap, GetPixmap)
FUNCTION_WRAPPER_JS_1(DestroyPixmap, DestroyPixmap)
FUNCTION_WRAPPER_JS_1(GetLinks, GetLinks)
FUNCTION_WRAPPER_JS_0(GetStructure, GetStructure)
FUNCTION_WRAPPER_JS_0(GetInteractiveFormsInfo, GetInteractiveFormsInfo)
FUNCTION_WRAPPER_JS_1(GetInteractiveFormsFonts, GetInteractiveFormsFonts)
FUNCTION_WRAPPER_JS_7(GetInteractiveFormsAP, GetInteractiveFormsAP)
FUNCTION_WRAPPER_JS_5(GetButtonIcons, GetButtonIcons)
FUNCTION_WRAPPER_JS_1(GetAnnotationsInfo, GetAnnotationsInfo)
FUNCTION_WRAPPER_JS_6(GetAnnotationsAP, GetAnnotationsAP)
FUNCTION_WRAPPER_JS_1(GetGlyphs, GetGlyphs)
FUNCTION_WRAPPER_JS_0(DestroyTextInfo, DestroyTextInfo)
FUNCTION_WRAPPER_JS_0(IsNeedCMap, IsNeedCMap)
FUNCTION_WRAPPER_JS_2(ScanPage, ScanPage)
FUNCTION_WRAPPER_JS_1(GetImageBase64, GetImageBase64)
FUNCTION_WRAPPER_JS_1(FreeWasmData, FreeWasmData)
@end

class CDrawingFileEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCDrawingFileEmbed alloc] init:(CDrawingFileEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CDrawingFileEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CDrawingFileEmbedAdapter();
	return m_pAdapter;
}

std::string CDrawingFileEmbed::getName() { return "CDrawingFileEmbed"; }

CJSEmbedObject* CDrawingFileEmbed::getCreator()
{
	return new CDrawingFileEmbed();
}
