#include "../HashEmbed.h"
#include "../../js_internal/v8/v8_base.h"

namespace NSHash
{
#define CURRENTWRAPPER CHashEmbed

	FUNCTION_WRAPPER_V8_3(_hash, hash)
	FUNCTION_WRAPPER_V8_4(_hash2, hash2)

	v8::Handle<v8::ObjectTemplate> CreateHashTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);

		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		// methods
		NSV8Objects::Template_Set(result, "hash",	_hash);
		NSV8Objects::Template_Set(result, "hash2",	_hash2);

		return handle_scope.Escape(result);
	}
}

std::string CHashEmbed::getName() { return "CHashEmbed"; }

CJSEmbedObject* CHashEmbed::getCreator()
{
	return new CHashEmbed();
}

void* CHashEmbed::GetDataForEmbedObject(void* data)
{
	v8::Isolate* isolate = reinterpret_cast<v8::Isolate*>(data);
	v8::Local<v8::ObjectTemplate>* internalTemplate = new v8::Local<v8::ObjectTemplate>(NSHash::CreateHashTemplate(isolate));
	return reinterpret_cast<void*>(internalTemplate);
}

std::vector<std::string> CHashEmbed::getMethodNames()
{
	return std::vector<std::string>();
}

void CHashEmbed::initFunctions()
{
}
