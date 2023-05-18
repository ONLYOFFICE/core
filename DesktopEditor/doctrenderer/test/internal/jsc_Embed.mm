#include "Embed.h"
#import "../../js_internal/jsc/jsc_base.h"

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

-(id) init
{
	self = [super init];
	if (self)
		m_internal = new CTestEmbed();
	return self;
}
-(void) dealloc
{
	RELEASEOBJECT(m_internal);
}
- (void*) getNative
{
	return m_internal;
}
//EMBED_OBJECT_WRAPPER_METHODS(CTestEmbed)

FUNCTION_WRAPPER_JS_2(FunctionSum, FunctionSum)
FUNCTION_WRAPPER_JS_1(FunctionSquare, FunctionSquare)
FUNCTION_WRAPPER_JS_2(FunctionDel, FunctionDel)
FUNCTION_WRAPPER_JS(FunctionGet, FunctionGet)

@end
