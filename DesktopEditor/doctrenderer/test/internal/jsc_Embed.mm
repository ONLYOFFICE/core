#include "Embed.h"
#include "../../js_internal/jsc/jsc_base.h"

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

// all remaining methods will be implemented at runtime

@end
