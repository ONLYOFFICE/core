// THIS FILE WAS GENERATED AUTOMATICALLY. DO NOT CHANGE IT!
// IF YOU NEED TO UPDATE THIS CODE, JUST RERUN PYTHON SCRIPT WITH "--internal" OPTION.

#include "../HashEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSHashEmbed
{
#define CURRENTWRAPPER CHashEmbed

	FUNCTION_WRAPPER_V8_3(_hash, hash)
	FUNCTION_WRAPPER_V8_4(_hash2, hash2)

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		NSV8Objects::Template_Set(result, "hash",	_hash);
		NSV8Objects::Template_Set(result, "hash2",	_hash2);

		return handle_scope.Escape(result);
	}
}

class CHashEmbedAdapter : public CJSEmbedObjectAdapterV8Template
{
public:
	virtual v8::Local<v8::ObjectTemplate> getTemplate(v8::Isolate* isolate) override
	{
		v8::EscapableHandleScope handle_scope(isolate);
		v8::Local<v8::ObjectTemplate> templ = NSHashEmbed::CreateTemplate(isolate);
		return handle_scope.Escape(templ);
	}
};

CJSEmbedObjectAdapterBase* CHashEmbed::getAdapter()
{
	if (m_pAdapter == nullptr)
		m_pAdapter = new CHashEmbedAdapter();
	return m_pAdapter;
}

std::string CHashEmbed::getName() { return "CHashEmbed"; }

CJSEmbedObject* CHashEmbed::getCreator()
{
	return new CHashEmbed();
}
