// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../ZipEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCZipEmbed <JSExport>
-(JSValue*) open : (JSValue*)typedArray_or_Folder;
-(JSValue*) create;
-(JSValue*) save;
-(JSValue*) getFile : (JSValue*)filePath;
-(JSValue*) addFile : (JSValue*)filePath : (JSValue*)typedArray;
-(JSValue*) removeFile : (JSValue*)filePath;
-(JSValue*) close;
-(JSValue*) getPaths;
-(JSValue*) decodeImage : (JSValue*)typedArray : (JSValue*)isRgba;
-(JSValue*) encodeImageData : (JSValue*)typedArray : (JSValue*)w : (JSValue*)h : (JSValue*)stride : (JSValue*)format : (JSValue*)isRgba;
-(JSValue*) encodeImage : (JSValue*)typedArray : (JSValue*)format;
-(JSValue*) getImageType : (JSValue*)typedArray;
-(JSValue*) getImageBuffer : (JSValue*)path;
@end

@interface CJSCZipEmbed : NSObject<IJSCZipEmbed, JSEmbedObjectProtocol>
{
@public
	CZipEmbed* m_internal;
}
@end

@implementation CJSCZipEmbed
EMBED_OBJECT_WRAPPER_METHODS(CZipEmbed);

FUNCTION_WRAPPER_JS_1(open, open)
FUNCTION_WRAPPER_JS_0(create, create)
FUNCTION_WRAPPER_JS_0(save, save)
FUNCTION_WRAPPER_JS_1(getFile, getFile)
FUNCTION_WRAPPER_JS_2(addFile, addFile)
FUNCTION_WRAPPER_JS_1(removeFile, removeFile)
FUNCTION_WRAPPER_JS_0(close, close)
FUNCTION_WRAPPER_JS_0(getPaths, getPaths)
FUNCTION_WRAPPER_JS_2(decodeImage, decodeImage)
FUNCTION_WRAPPER_JS_6(encodeImageData, encodeImageData)
FUNCTION_WRAPPER_JS_2(encodeImage, encodeImage)
FUNCTION_WRAPPER_JS_1(getImageType, getImageType)
FUNCTION_WRAPPER_JS_1(getImageBuffer, getImageBuffer)
@end

class CZipEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCZipEmbed alloc] init:(CZipEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CZipEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CZipEmbedAdapter();
	return m_pAdapter;
}

std::string CZipEmbed::getName() { return "CZipEmbed"; }

CJSEmbedObject* CZipEmbed::getCreator()
{
	return new CZipEmbed();
}
