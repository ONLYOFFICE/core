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

	void CreateNativeHash(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CreateNativeInternalField(new CHashEmbed(), NSHash::CreateHashTemplate, args);
	}
}

void CHashEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
	InsertToGlobal(name, context, NSHash::CreateNativeHash);
}
