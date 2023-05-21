#include "Embed.h"
#import "js_embed.h"

@protocol IJSCTestEmbed <JSExport>
-(JSValue*) FunctionSum : (JSValue*)param1 : (JSValue*)param1;
-(JSValue*) FunctionSquare : (JSValue*)param;
-(JSValue*) FunctionDel : (JSValue*)param1 : (JSValue*)param1;
-(JSValue*) FunctionGet;
@end

@interface CJSCTestEmbed : NSObject<IJSCTestEmbed, JSEmbedObjectProtocol>
{
@public
	CTestEmbed* m_internal;
}
@end

@implementation CJSCTestEmbed

EMBED_OBJECT_WRAPPER_METHODS(CTestEmbed);

#if 0
-(JSValue*) FunctionSum:(JSValue*)p1 : (JSValue*)p2
{
	JSSmart<CJSValue> ret = m_internal->FunctionSum(NSJSBase::Native2Value((__bridge void*)p1), NSJSBase::Native2Value((__bridge void*)p2));
	return (__bridge JSValue*)NSJSBase::Value2Native(ret);
}
...
#endif

// all remaining methods will be implemented at runtime

@end
