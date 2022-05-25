#include "../ZipEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCZip <JSExport>
-(JSValue*) open : (JSValue*)file;
-(JSValue*) create;
-(JSValue*) save;
-(JSValue*) getFile : (JSValue*)path;
-(JSValue*) addFile : (JSValue*)path : (JSValue*)data;
-(JSValue*) removeFile : (JSValue*)path;
-(JSValue*) close;
@end

@interface CJSCZip : NSObject<IJSCZip, JSEmbedObjectProtocol>
{
@public
    CZipEmbed* m_internal;
}
@end

@implementation CJSCZip

EMBED_OBJECT_WRAPPER_METHODS(CJSCZip)

FUNCTION_WRAPPER_JS_1(open, open)
FUNCTION_WRAPPER_JS(create, create)
FUNCTION_WRAPPER_JS(save, save)
FUNCTION_WRAPPER_JS_1(getFile, getFile)
FUNCTION_WRAPPER_JS_2(addFile, addFile)
FUNCTION_WRAPPER_JS_1(removeFile, removeFile)
FUNCTION_WRAPPER_JS(close, close)

@end

void CGraphicsEmbed::CreateObjectInContext(const std::string &name, JSSmart<CJSContext> context)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        return [[CJSCZip alloc] init];
    };
}
