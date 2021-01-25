#include "../MemoryStreamEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCMemoryStream <JSExport>
-(JSValue*) Copy : (JSValue*)value : (JSValue*)pos : (JSValue*)len;
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

@interface CJSCMemoryStream : NSObject<IJSCMemoryStream, JSEmbedObjectProtocol>
{
@public
    CMemoryStreamEmbed* m_internal;
}
@end

@implementation CJSCMemoryStream

EMBED_OBJECT_WRAPPER_METHODS(CMemoryStreamEmbed)

FUNCTION_WRAPPER_JS_3(Copy, Copy)
FUNCTION_WRAPPER_JS(ClearNoAttack, ClearNoAttack)
FUNCTION_WRAPPER_JS_1(WriteByte, WriteByte)
FUNCTION_WRAPPER_JS_1(WriteBool, WriteBool)
FUNCTION_WRAPPER_JS_1(WriteLong, WriteLong)
FUNCTION_WRAPPER_JS_1(WriteDouble, WriteDouble)
FUNCTION_WRAPPER_JS_1(WriteDouble2, WriteDouble2)
FUNCTION_WRAPPER_JS_1(WriteStringA, WriteStringA)
FUNCTION_WRAPPER_JS_1(WriteString, WriteString)
FUNCTION_WRAPPER_JS_1(WriteString2, WriteString2)

@end

void CMemoryStreamEmbed::CreateObjectInContext(const std::string &name, JSSmart<CJSContext> context)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        return [[CJSCMemoryStream alloc] init];
    };
}
