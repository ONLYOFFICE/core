#include "../NativeControlEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSNativeControl
{
#define CURRENTWRAPPER CNativeControlEmbed

	FUNCTION_WRAPPER_V8(_GetFilePath, GetFilePath)
	FUNCTION_WRAPPER_V8_1(_SetFilePath, SetFilePath)
	FUNCTION_WRAPPER_V8(_GetFileId, GetFileId)
	FUNCTION_WRAPPER_V8_1(_SetFileId, SetFileId)
	FUNCTION_WRAPPER_V8_1(_GetFileArrayBuffer, GetFileBinary)
	FUNCTION_WRAPPER_V8_1(_GetFontArrayBuffer, GetFontBinary)
	FUNCTION_WRAPPER_V8(_GetFontsDirectory, GetFontsDirectory)
	FUNCTION_WRAPPER_V8_1(_GetFileString, GetFileString)
	FUNCTION_WRAPPER_V8(_GetEditorType, GetEditorType)
	FUNCTION_WRAPPER_V8(_CheckNextChange, CheckNextChange)
	FUNCTION_WRAPPER_V8(_GetChangesCount, GetCountChanges)
	FUNCTION_WRAPPER_V8_1(_GetChangesFile, GetChangesFile)
	FUNCTION_WRAPPER_V8_1(_Save_AllocNative, Save_AllocNative)
	FUNCTION_WRAPPER_V8_2(_Save_ReAllocNative, Save_ReAllocNative)
	FUNCTION_WRAPPER_V8_2(_Save_End, Save_End)
	FUNCTION_WRAPPER_V8_1(_AddImageInChanges, AddImageInChanges)
	FUNCTION_WRAPPER_V8_1(_ConsoleLog, ConsoleLog)
	FUNCTION_WRAPPER_V8_3(_SaveChanges, SaveChanges)
	FUNCTION_WRAPPER_V8_1(_zipOpenFile, zipOpenFile)
	FUNCTION_WRAPPER_V8_1(_zipOpenFileBase64, zipOpenFileBase64)
	FUNCTION_WRAPPER_V8_1(_zipGetFileAsString, zipGetFileAsString)
	FUNCTION_WRAPPER_V8_1(_zipGetFileAsBinary, zipGetFileAsBinary)
	FUNCTION_WRAPPER_V8(_zipCloseFile, zipCloseFile)
	FUNCTION_WRAPPER_V8_1(_GetImageUrl, GetImageUrl)
	FUNCTION_WRAPPER_V8_1(_GetImageOriginalSize, GetImageOriginalSize)
	FUNCTION_WRAPPER_V8(_GetImagesPath, GetImagesPath)

	// Без SaveChanges
	v8::Handle<v8::ObjectTemplate> CreateNativeControlTemplateBuilder(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);

		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(V8IsolateOneArg);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "SetFilePath",        _SetFilePath);
		NSV8Objects::Template_Set(result, "GetFilePath",        _GetFilePath);
		NSV8Objects::Template_Set(result, "SetFileId",          _SetFileId);
		NSV8Objects::Template_Set(result, "GetFileId",          _GetFileId);
		NSV8Objects::Template_Set(result, "GetFileBinary",      _GetFileArrayBuffer);
		NSV8Objects::Template_Set(result, "GetFontBinary",      _GetFontArrayBuffer);
		NSV8Objects::Template_Set(result, "GetFontsDirectory",  _GetFontsDirectory);
		NSV8Objects::Template_Set(result, "GetFileString",      _GetFileString);
		NSV8Objects::Template_Set(result, "GetEditorType",      _GetEditorType);
		NSV8Objects::Template_Set(result, "CheckNextChange",    _CheckNextChange);
		NSV8Objects::Template_Set(result, "GetCountChanges",    _GetChangesCount);
		NSV8Objects::Template_Set(result, "GetChangesFile",     _GetChangesFile);
		NSV8Objects::Template_Set(result, "Save_End",           _Save_End);
		NSV8Objects::Template_Set(result, "AddImageInChanges",  _AddImageInChanges);
		NSV8Objects::Template_Set(result, "ConsoleLog",         _ConsoleLog);
		NSV8Objects::Template_Set(result, "ZipOpen",            _zipOpenFile);
		NSV8Objects::Template_Set(result, "ZipOpenBase64",      _zipOpenFileBase64);
		NSV8Objects::Template_Set(result, "ZipFileAsString",    _zipGetFileAsString);
		NSV8Objects::Template_Set(result, "ZipFileAsBinary",    _zipGetFileAsBinary);
		NSV8Objects::Template_Set(result, "ZipClose",           _zipCloseFile);
		NSV8Objects::Template_Set(result, "getImageUrl",        _GetImageUrl);
		NSV8Objects::Template_Set(result, "getImagesDirectory", _GetImagesPath);
		NSV8Objects::Template_Set(result, "GetImageOriginalSize", _GetImageOriginalSize);

		return handle_scope.Escape(result);
	}

	// Без SaveChanges
	void CreateNativeObjectBuilder(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CreateNativeInternalField(new CNativeControlEmbed(), CreateNativeControlTemplateBuilder, args, NSJSBase::iadtSingletonNative);
	}
}

void CNativeControlEmbed::CreateObjectBuilderInContext(const std::string& name, JSSmart<CJSContext> context)
{
	InsertToGlobal(name, context, NSNativeControl::CreateNativeObjectBuilder);
}
