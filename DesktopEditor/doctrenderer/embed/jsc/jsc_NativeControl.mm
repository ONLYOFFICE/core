#include "../NativeControlEmbed.h"
#include "../../js_internal/jsc/jsc_base.h"

@protocol IJSCNativeControl <JSExport>
-(JSValue*) GetFilePath;
-(JSValue*) SetFilePath : (JSValue*)path;
-(JSValue*) GetFileId;
-(JSValue*) SetFileId : (JSValue*)fileid;
-(JSValue*) GetFileBinary : (JSValue*)file;
-(JSValue*) GetFontBinary : (JSValue*)file;
-(JSValue*) GetFontsDirectory;
-(JSValue*) GetFileString : (JSValue*)file;
-(JSValue*) GetEditorType;
-(JSValue*) CheckNextChange;
-(JSValue*) GetCountChanges;
-(JSValue*) GetChangesFile : (JSValue*)index;
-(JSValue*) Save_AllocNative : (JSValue*)len;
-(JSValue*) Save_ReAllocNative : (JSValue*)pos : (JSValue*)len;
-(JSValue*) Save_End : (JSValue*)pos : (JSValue*)len;
-(JSValue*) AddImageInChanges : (JSValue*)img;
-(JSValue*) ConsoleLog : (JSValue*)message;
-(JSValue*) SaveChanges : (JSValue*)param : (JSValue*)delete_index : (JSValue*)count;
-(JSValue*) zipOpenFile : (JSValue*)name;
-(JSValue*) zipOpenFileBase64 : (JSValue*)name;
-(JSValue*) zipGetFileAsString : (JSValue*)name;
-(JSValue*) zipGetFileAsBinary : (JSValue*)name;
-(JSValue*) zipCloseFile;
-(JSValue*) GetImageUrl : (JSValue*)url;
-(JSValue*) GetImagesPath;

@end

@interface CJSCNativeControl : NSObject<IJSCNativeControl, JSEmbedObjectProtocol>
{
@public
    CNativeControlEmbed* m_internal;
}
@end

@implementation CJSCNativeControl

EMBED_OBJECT_WRAPPER_METHODS(CNativeControlEmbed)

FUNCTION_WRAPPER_JS(GetFilePath, GetFilePath)
FUNCTION_WRAPPER_JS_1(SetFilePath, SetFilePath)
FUNCTION_WRAPPER_JS(GetFileId, GetFileId)
FUNCTION_WRAPPER_JS_1(SetFileId, SetFileId)
FUNCTION_WRAPPER_JS_1(GetFileBinary, GetFileBinary)
FUNCTION_WRAPPER_JS_1(GetFontBinary, GetFontBinary);
FUNCTION_WRAPPER_JS(GetFontsDirectory, GetFontsDirectory)
FUNCTION_WRAPPER_JS_1(GetFileString, GetFileString)
FUNCTION_WRAPPER_JS(GetEditorType, GetEditorType)
FUNCTION_WRAPPER_JS(CheckNextChange, CheckNextChange)
FUNCTION_WRAPPER_JS(GetCountChanges, GetCountChanges)
FUNCTION_WRAPPER_JS_1(GetChangesFile, GetChangesFile)
FUNCTION_WRAPPER_JS_1(Save_AllocNative, Save_AllocNative)
FUNCTION_WRAPPER_JS_2(Save_ReAllocNative, Save_ReAllocNative)
FUNCTION_WRAPPER_JS_2(Save_End, Save_End)
FUNCTION_WRAPPER_JS_1(AddImageInChanges, AddImageInChanges)
FUNCTION_WRAPPER_JS_1(ConsoleLog, ConsoleLog)
FUNCTION_WRAPPER_JS_3(SaveChanges, SaveChanges)
FUNCTION_WRAPPER_JS_1(zipOpenFile, zipOpenFile)
FUNCTION_WRAPPER_JS_1(zipOpenFileBase64, zipOpenFileBase64)
FUNCTION_WRAPPER_JS_1(zipGetFileAsString, zipGetFileAsString)
FUNCTION_WRAPPER_JS_1(zipGetFileAsBinary, zipGetFileAsBinary)
FUNCTION_WRAPPER_JS(zipCloseFile, zipCloseFile);
FUNCTION_WRAPPER_JS_1(GetImageUrl, GetImageUrl);
FUNCTION_WRAPPER_JS(GetImagesPath, GetImagesPath)

@end

void CNativeControlEmbed::CreateObjectInContext(const std::string &name, JSSmart<CJSContext> context)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        return [[CJSCNativeControl alloc] init];
    };
}
void CNativeControlEmbed::CreateObjectBuilderInContext(const std::string &name, JSSmart<CJSContext> context)
{
    context->m_internal->context[[NSString stringWithAString:name]] = ^(){
        return [[CJSCNativeControl alloc] init];
    };
}
