#include "../HashEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCHash <JSExport>
-(JSValue*) hash : (JSValue*)data : (JSValue*)size : (JSValue*)alg;
-(JSValue*) hash2 : (JSValue*)password : (JSValue*)salt : (JSValue*)spinCount : (JSValue*)alg;

@end

@interface CJSCHash : NSObject<IJSCHash, JSEmbedObjectProtocol>
{
@public
	CHashEmbed* m_internal;
}
@end

@implementation CJSCHash

EMBED_OBJECT_WRAPPER_METHODS(CHashEmbed)

FUNCTION_WRAPPER_JS_3(hash, hash)
FUNCTION_WRAPPER_JS_4(hash2, hash2)

@end

void CHashEmbed::CreateObjectInContext(const std::string &name, JSSmart<CJSContext> context)
{
	context->m_internal->context[[NSString stringWithAString:name]] = ^(){
		return [[CJSCHash alloc] init];
	};
}
