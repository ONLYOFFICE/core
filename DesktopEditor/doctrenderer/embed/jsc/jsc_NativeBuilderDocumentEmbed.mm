// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../NativeBuilderDocumentEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCBuilderDocumentEmbed <JSExport>
-(JSValue*) IsValid;
-(JSValue*) GetBinary;
-(JSValue*) GetFolder;
-(JSValue*) Close;
-(JSValue*) GetImageMap;
@end

@interface CJSCBuilderDocumentEmbed : NSObject<IJSCBuilderDocumentEmbed, JSEmbedObjectProtocol>
{
@public
	CBuilderDocumentEmbed* m_internal;
}
@end

@implementation CJSCBuilderDocumentEmbed
EMBED_OBJECT_WRAPPER_METHODS(CBuilderDocumentEmbed);

FUNCTION_WRAPPER_JS_0(IsValid, IsValid)
FUNCTION_WRAPPER_JS_0(GetBinary, GetBinary)
FUNCTION_WRAPPER_JS_0(GetFolder, GetFolder)
FUNCTION_WRAPPER_JS_0(Close, Close)
FUNCTION_WRAPPER_JS_0(GetImageMap, GetImageMap)
@end

class CBuilderDocumentEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCBuilderDocumentEmbed alloc] init:(CBuilderDocumentEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CBuilderDocumentEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CBuilderDocumentEmbedAdapter();
	return m_pAdapter;
}

std::string CBuilderDocumentEmbed::getName() { return "CBuilderDocumentEmbed"; }

CJSEmbedObject* CBuilderDocumentEmbed::getCreator()
{
	return new CBuilderDocumentEmbed();
}
