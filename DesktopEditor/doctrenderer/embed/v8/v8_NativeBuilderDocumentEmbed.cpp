// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../NativeBuilderDocumentEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSNativeBuilderDocumentEmbed
{
#define CURRENTWRAPPER CBuilderDocumentEmbed

	FUNCTION_WRAPPER_V8_0(_IsValid, IsValid)
	FUNCTION_WRAPPER_V8_0(_GetBinary, GetBinary)
	FUNCTION_WRAPPER_V8_0(_GetFolder, GetFolder)
	FUNCTION_WRAPPER_V8_0(_Close, Close)
	FUNCTION_WRAPPER_V8_0(_GetImageMap, GetImageMap)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "IsValid",	_IsValid);
		NSV8Objects::Template_Set(result, "GetBinary",	_GetBinary);
		NSV8Objects::Template_Set(result, "GetFolder",	_GetFolder);
		NSV8Objects::Template_Set(result, "Close",	_Close);
		NSV8Objects::Template_Set(result, "GetImageMap",	_GetImageMap);

		return handle_scope.Escape(result);
	}
}

class CBuilderDocumentEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSNativeBuilderDocumentEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CBuilderDocumentEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CBuilderDocumentEmbedAdapter();
	return m_pAdapter;
}

std::string CBuilderDocumentEmbed::getName() { return "CBuilderDocumentEmbed"; }

CJSEmbedObject* CBuilderDocumentEmbed::getCreator()
{
	return new CBuilderDocumentEmbed();
}
