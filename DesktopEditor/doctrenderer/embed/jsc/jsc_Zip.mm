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
-(JSValue*) getPaths;

-(JSValue*) decodeImage : (JSValue*)typedArray : (JSValue*)isRgba;
-(JSValue*) encodeImageData : (JSValue*)typedArray : (JSValue*)w : (JSValue*)h : (JSValue*)stride : (JSValue*)format : (JSValue*)isRgba;
-(JSValue*) encodeImage : (JSValue*)typedArray : (JSValue*)format;
-(JSValue*) getImageType : (JSValue*)typedArray;

@end

@interface CJSCZip : NSObject<IJSCZip, JSEmbedObjectProtocol>
{
@public
    CZipEmbed* m_internal;
}
@end

@implementation CJSCZip

EMBED_OBJECT_WRAPPER_METHODS(CZipEmbed)

FUNCTION_WRAPPER_JS_1(open, open)
FUNCTION_WRAPPER_JS(create, create)
FUNCTION_WRAPPER_JS(save, save)
FUNCTION_WRAPPER_JS_1(getFile, getFile)
FUNCTION_WRAPPER_JS_2(addFile, addFile)
FUNCTION_WRAPPER_JS_1(removeFile, removeFile)
FUNCTION_WRAPPER_JS(close, close)
FUNCTION_WRAPPER_JS(getPaths, getPaths)

FUNCTION_WRAPPER_JS_2(decodeImage, decodeImage)
FUNCTION_WRAPPER_JS_6(encodeImageData, encodeImageData)
FUNCTION_WRAPPER_JS_2(encodeImage, encodeImage)
FUNCTION_WRAPPER_JS_1(getImageType, getImageType)

@end

void CZipEmbed::CreateObjectInContext(const std::string &name, JSSmart<CJSContext> context)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        return [[CJSCZip alloc] init];
    };
}
