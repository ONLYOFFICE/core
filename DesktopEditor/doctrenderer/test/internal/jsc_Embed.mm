#include "Embed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCTestEmbed <JSExport>
//-(JSValue*) FunctionSum : (JSValue*)param1 : (JSValue*)param1;
//-(JSValue*) FunctionSquare : (JSValue*)param;
//-(JSValue*) FunctionDel : (JSValue*)param1 : (JSValue*)param1;
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

-(id) init:(NSJSBase::CJSEmbedObject*)pNativeObj
{
	self = [super init];
	if (self)
		m_internal = (CTestEmbed*)pNativeObj;
	return self;
}

// this methods should be implemeted in runtime

//FUNCTION_WRAPPER_JS_2(FunctionSum, FunctionSum)
//FUNCTION_WRAPPER_JS_1(FunctionSquare, FunctionSquare)
//FUNCTION_WRAPPER_JS_2(FunctionDel, FunctionDel)
//FUNCTION_WRAPPER_JS(FunctionGet, FunctionGet)

@end
