// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../MemoryStreamEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCMemoryStreamEmbed <JSExport>
-(JSValue*) Copy : (JSValue*)stream : (JSValue*)pos : (JSValue*)len;
-(JSValue*) ClearNoAttack;
-(JSValue*) WriteByte : (JSValue*)value;
-(JSValue*) WriteBool : (JSValue*)value;
-(JSValue*) WriteLong : (JSValue*)value;
-(JSValue*) WriteDouble : (JSValue*)value;
-(JSValue*) WriteDouble2 : (JSValue*)value;
-(JSValue*) WriteStringA : (JSValue*)value;
-(JSValue*) WriteString : (JSValue*)value;
-(JSValue*) WriteString2 : (JSValue*)value;
@end

@interface CJSCMemoryStreamEmbed : NSObject<IJSCMemoryStreamEmbed, JSEmbedObjectProtocol>
{
@public
	CMemoryStreamEmbed* m_internal;
}
@end

@implementation CJSCMemoryStreamEmbed
EMBED_OBJECT_WRAPPER_METHODS(CMemoryStreamEmbed);

FUNCTION_WRAPPER_JS_3(Copy, Copy)
FUNCTION_WRAPPER_JS_0(ClearNoAttack, ClearNoAttack)
FUNCTION_WRAPPER_JS_1(WriteByte, WriteByte)
FUNCTION_WRAPPER_JS_1(WriteBool, WriteBool)
FUNCTION_WRAPPER_JS_1(WriteLong, WriteLong)
FUNCTION_WRAPPER_JS_1(WriteDouble, WriteDouble)
FUNCTION_WRAPPER_JS_1(WriteDouble2, WriteDouble2)
FUNCTION_WRAPPER_JS_1(WriteStringA, WriteStringA)
FUNCTION_WRAPPER_JS_1(WriteString, WriteString)
FUNCTION_WRAPPER_JS_1(WriteString2, WriteString2)
@end

class CMemoryStreamEmbedAdapter : public CJSEmbedObjectAdapterJSC
{
public:
	virtual id getExportedObject(CJSEmbedObject* pNative) override
	{
		return [[CJSCMemoryStreamEmbed alloc] init:(CMemoryStreamEmbed*)pNative];
	}
};

CJSEmbedObjectAdapterBase* CMemoryStreamEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CMemoryStreamEmbedAdapter();
	return m_pAdapter;
}

std::string CMemoryStreamEmbed::getName() { return "CMemoryStreamEmbed"; }

CJSEmbedObject* CMemoryStreamEmbed::getCreator()
{
	return new CMemoryStreamEmbed();
}
