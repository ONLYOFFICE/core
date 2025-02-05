// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../ZipEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSZipEmbed
{
#define CURRENTWRAPPER CZipEmbed

	FUNCTION_WRAPPER_V8_1(_open, open)
	FUNCTION_WRAPPER_V8_0(_create, create)
	FUNCTION_WRAPPER_V8_0(_save, save)
	FUNCTION_WRAPPER_V8_1(_getFile, getFile)
	FUNCTION_WRAPPER_V8_2(_addFile, addFile)
	FUNCTION_WRAPPER_V8_1(_removeFile, removeFile)
	FUNCTION_WRAPPER_V8_0(_close, close)
	FUNCTION_WRAPPER_V8_0(_getPaths, getPaths)
	FUNCTION_WRAPPER_V8_2(_decodeImage, decodeImage)
	FUNCTION_WRAPPER_V8_6(_encodeImageData, encodeImageData)
	FUNCTION_WRAPPER_V8_2(_encodeImage, encodeImage)
	FUNCTION_WRAPPER_V8_1(_getImageType, getImageType)
	FUNCTION_WRAPPER_V8_1(_getImageBuffer, getImageBuffer)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "open",	_open);
		NSV8Objects::Template_Set(result, "create",	_create);
		NSV8Objects::Template_Set(result, "save",	_save);
		NSV8Objects::Template_Set(result, "getFile",	_getFile);
		NSV8Objects::Template_Set(result, "addFile",	_addFile);
		NSV8Objects::Template_Set(result, "removeFile",	_removeFile);
		NSV8Objects::Template_Set(result, "close",	_close);
		NSV8Objects::Template_Set(result, "getPaths",	_getPaths);
		NSV8Objects::Template_Set(result, "decodeImage",	_decodeImage);
		NSV8Objects::Template_Set(result, "encodeImageData",	_encodeImageData);
		NSV8Objects::Template_Set(result, "encodeImage",	_encodeImage);
		NSV8Objects::Template_Set(result, "getImageType",	_getImageType);
		NSV8Objects::Template_Set(result, "getImageBuffer",	_getImageBuffer);

		return handle_scope.Escape(result);
	}
}

class CZipEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSZipEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CZipEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CZipEmbedAdapter();
	return m_pAdapter;
}

std::string CZipEmbed::getName() { return "CZipEmbed"; }

CJSEmbedObject* CZipEmbed::getCreator()
{
	return new CZipEmbed();
}
