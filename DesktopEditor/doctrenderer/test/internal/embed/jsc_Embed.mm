// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT.

#include "../Embed.h"
#import "js_embed.h"

@protocol IJSCTestEmbed <JSExport>
-(JSValue*) FunctionSum : (JSValue*)param1 : (JSValue*)param2;
-(JSValue*) FunctionSquare : (JSValue*)param;
-(JSValue*) FunctionDel : (JSValue*)param1 : (JSValue*)param2;
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

-(JSValue*) FunctionSum : (JSValue*)param1 : (JSValue*)param2
{
	JSSmart<CJSValue> ret = m_internal->FunctionSum(NSJSBase::Native2Value((__bridge void*)param1), NSJSBase::Native2Value((__bridge void*)param2));
	return (__bridge JSValue*)NSJSBase::Value2Native(ret);
}

-(JSValue*) FunctionSquare : (JSValue*)param
{
	JSSmart<CJSValue> ret = m_internal->FunctionSquare(NSJSBase::Native2Value((__bridge void*)param));
	return (__bridge JSValue*)NSJSBase::Value2Native(ret);
}

-(JSValue*) FunctionDel : (JSValue*)param1 : (JSValue*)param2
{
	JSSmart<CJSValue> ret = m_internal->FunctionDel(NSJSBase::Native2Value((__bridge void*)param1), NSJSBase::Native2Value((__bridge void*)param2));
	return (__bridge JSValue*)NSJSBase::Value2Native(ret);
}

-(JSValue*) FunctionGet
{
	JSSmart<CJSValue> ret = m_internal->FunctionGet();
	return (__bridge JSValue*)NSJSBase::Value2Native(ret);
}

@end
