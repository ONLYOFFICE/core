// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../HashEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCHashEmbed <JSExport>
-(JSValue*) hash : (JSValue*)data : (JSValue*)size : (JSValue*)alg;
-(JSValue*) hash2 : (JSValue*)password : (JSValue*)salt : (JSValue*)spinCount : (JSValue*)alg;
@end

@interface CJSCHashEmbed : NSObject<IJSCHashEmbed, JSEmbedObjectProtocol>
{
@public
	CHashEmbed* m_internal;
}
@end

@implementation CJSCHashEmbed
EMBED_OBJECT_WRAPPER_METHODS(CHashEmbed);

FUNCTION_WRAPPER_JS_3(hash, hash)
FUNCTION_WRAPPER_JS_4(hash2, hash2)
@end

class CHashEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCHashEmbed alloc] init:(CHashEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CHashEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CHashEmbedAdapter();
	return m_pAdapter;
}

std::string CHashEmbed::getName() { return "CHashEmbed"; }

CJSEmbedObject* CHashEmbed::getCreator()
{
	return new CHashEmbed();
}
