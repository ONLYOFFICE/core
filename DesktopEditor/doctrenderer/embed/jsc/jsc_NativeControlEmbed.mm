// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../NativeControlEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCNativeControlEmbed <JSExport>
-(JSValue*) SetFilePath : (JSValue*)path;
-(JSValue*) GetFilePath;
-(JSValue*) SetFileId : (JSValue*)fileId;
-(JSValue*) GetFileId;
-(JSValue*) GetFileBinary : (JSValue*)file;
-(JSValue*) GetFontBinary : (JSValue*)file;
-(JSValue*) GetFontsDirectory;
-(JSValue*) GetFileString : (JSValue*)file;
-(JSValue*) GetEditorType;
-(JSValue*) CheckNextChange;
-(JSValue*) GetCountChanges;
-(JSValue*) GetChangesFile : (JSValue*)index;
-(JSValue*) Save_End : (JSValue*)pos : (JSValue*)len;
-(JSValue*) AddImageInChanges : (JSValue*)img;
-(JSValue*) ConsoleLog : (JSValue*)message;
-(JSValue*) ZipOpen : (JSValue*)name;
-(JSValue*) ZipOpenBase64 : (JSValue*)name;
-(JSValue*) ZipFileAsString : (JSValue*)name;
-(JSValue*) ZipFileAsBinary : (JSValue*)name;
-(JSValue*) ZipClose;
-(JSValue*) GetImageUrl : (JSValue*)sUrl;
-(JSValue*) GetImagesPath;
-(JSValue*) GetImageOriginalSize : (JSValue*)sUrl;
@end

@interface CJSCNativeControlEmbed : NSObject<IJSCNativeControlEmbed, JSEmbedObjectProtocol>
{
@public
	CNativeControlEmbed* m_internal;
}
@end

@implementation CJSCNativeControlEmbed
EMBED_OBJECT_WRAPPER_METHODS(CNativeControlEmbed);

FUNCTION_WRAPPER_JS_1(SetFilePath, SetFilePath)
FUNCTION_WRAPPER_JS_0(GetFilePath, GetFilePath)
FUNCTION_WRAPPER_JS_1(SetFileId, SetFileId)
FUNCTION_WRAPPER_JS_0(GetFileId, GetFileId)
FUNCTION_WRAPPER_JS_1(GetFileBinary, GetFileBinary)
FUNCTION_WRAPPER_JS_1(GetFontBinary, GetFontBinary)
FUNCTION_WRAPPER_JS_0(GetFontsDirectory, GetFontsDirectory)
FUNCTION_WRAPPER_JS_1(GetFileString, GetFileString)
FUNCTION_WRAPPER_JS_0(GetEditorType, GetEditorType)
FUNCTION_WRAPPER_JS_0(CheckNextChange, CheckNextChange)
FUNCTION_WRAPPER_JS_0(GetCountChanges, GetCountChanges)
FUNCTION_WRAPPER_JS_1(GetChangesFile, GetChangesFile)
FUNCTION_WRAPPER_JS_2(Save_End, Save_End)
FUNCTION_WRAPPER_JS_1(AddImageInChanges, AddImageInChanges)
FUNCTION_WRAPPER_JS_1(ConsoleLog, ConsoleLog)
FUNCTION_WRAPPER_JS_1(ZipOpen, ZipOpen)
FUNCTION_WRAPPER_JS_1(ZipOpenBase64, ZipOpenBase64)
FUNCTION_WRAPPER_JS_1(ZipFileAsString, ZipFileAsString)
FUNCTION_WRAPPER_JS_1(ZipFileAsBinary, ZipFileAsBinary)
FUNCTION_WRAPPER_JS_0(ZipClose, ZipClose)
FUNCTION_WRAPPER_JS_1(GetImageUrl, GetImageUrl)
FUNCTION_WRAPPER_JS_0(GetImagesPath, GetImagesPath)
FUNCTION_WRAPPER_JS_1(GetImageOriginalSize, GetImageOriginalSize)
@end

class CNativeControlEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCNativeControlEmbed alloc] init:(CNativeControlEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CNativeControlEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CNativeControlEmbedAdapter();
	return m_pAdapter;
}

std::string CNativeControlEmbed::getName() { return "CNativeControlEmbed"; }

CJSEmbedObject* CNativeControlEmbed::getCreator()
{
	return new CNativeControlEmbed();
}
