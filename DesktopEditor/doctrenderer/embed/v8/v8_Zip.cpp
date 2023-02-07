#include "../ZipEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSZip
{
#define CURRENTWRAPPER CZipEmbed

	FUNCTION_WRAPPER_V8_1(_open, open)
	FUNCTION_WRAPPER_V8  (_create, create)
	FUNCTION_WRAPPER_V8  (_save, save)
	FUNCTION_WRAPPER_V8_1(_getFile, getFile)
	FUNCTION_WRAPPER_V8_2(_addFile, addFile)
	FUNCTION_WRAPPER_V8_1(_removeFile, removeFile)
	FUNCTION_WRAPPER_V8  (_close, close)
	FUNCTION_WRAPPER_V8  (_getPaths, getPaths)

	FUNCTION_WRAPPER_V8_2(_decodeImage, decodeImage)
	FUNCTION_WRAPPER_V8_6(_encodeImageData, encodeImageData)
	FUNCTION_WRAPPER_V8_2(_encodeImage, encodeImage)
	FUNCTION_WRAPPER_V8_1(_getImageType, getImageType)

	v8::Handle<v8::ObjectTemplate> CreateZipTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);

		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		// методы
		NSV8Objects::Template_Set(result, "open",       _open);
		NSV8Objects::Template_Set(result, "create",     _create);
		NSV8Objects::Template_Set(result, "save",       _save);
		NSV8Objects::Template_Set(result, "getFile",    _getFile);
		NSV8Objects::Template_Set(result, "addFile",    _addFile);
		NSV8Objects::Template_Set(result, "removeFile", _removeFile);
		NSV8Objects::Template_Set(result, "close",      _close);
		NSV8Objects::Template_Set(result, "getPaths",   _getPaths);

		NSV8Objects::Template_Set(result, "decodeImage",     _decodeImage);
		NSV8Objects::Template_Set(result, "encodeImageData", _encodeImageData);
		NSV8Objects::Template_Set(result, "encodeImage",     _encodeImage);
		NSV8Objects::Template_Set(result, "getImageType",    _getImageType);

		return handle_scope.Escape(result);
	}

	void CreateNativeZip(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		v8::Isolate* isolate = args.GetIsolate();
		v8::HandleScope scope(isolate);

		v8::Handle<v8::ObjectTemplate> ZipTemplate = NSZip::CreateZipTemplate(isolate);
		CZipEmbed* pZip = new CZipEmbed();

		v8::Local<v8::Object> obj = ZipTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
		obj->SetInternalField(0, v8::External::New(CV8Worker::GetCurrent(), pZip));

		NSJSBase::CJSEmbedObjectPrivate::CreateWeaker(obj);

		args.GetReturnValue().Set(obj);
	}
}

void CZipEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
	v8::Isolate* current = CV8Worker::GetCurrent();
	context->m_internal->m_global->Set(current, name.c_str(), v8::FunctionTemplate::New(current, NSZip::CreateNativeZip));
}
