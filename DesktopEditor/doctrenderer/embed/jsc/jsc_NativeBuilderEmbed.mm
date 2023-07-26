// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../NativeBuilderEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCBuilderEmbed <JSExport>
-(JSValue*) OpenFile : (JSValue*)sPath : (JSValue*)sParams;
-(JSValue*) CreateFile : (JSValue*)type;
-(JSValue*) SetTmpFolder : (JSValue*)path;
-(JSValue*) SaveFile : (JSValue*)type : (JSValue*)path : (JSValue*)params;
-(JSValue*) CloseFile;
-(JSValue*) OpenTmpFile : (JSValue*)path : (JSValue*)params;
@end

@interface CJSCBuilderEmbed : NSObject<IJSCBuilderEmbed, JSEmbedObjectProtocol>
{
@public
	CBuilderEmbed* m_internal;
}
@end

@implementation CJSCBuilderEmbed
EMBED_OBJECT_WRAPPER_METHODS(CBuilderEmbed);

FUNCTION_WRAPPER_JS_2(OpenFile, OpenFile)
FUNCTION_WRAPPER_JS_1(CreateFile, CreateFile)
FUNCTION_WRAPPER_JS_1(SetTmpFolder, SetTmpFolder)
FUNCTION_WRAPPER_JS_3(SaveFile, SaveFile)
FUNCTION_WRAPPER_JS_0(CloseFile, CloseFile)
FUNCTION_WRAPPER_JS_2(OpenTmpFile, OpenTmpFile)
@end

class CBuilderEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCBuilderEmbed alloc] init:(CBuilderEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CBuilderEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CBuilderEmbedAdapter();
	return m_pAdapter;
}

std::string CBuilderEmbed::getName() { return "CBuilderEmbed"; }

CJSEmbedObject* CBuilderEmbed::getCreator()
{
	return new CBuilderEmbed();
}
