// THIS FILE WILL BE GENERATED
// THIS IS TEST PREVIEW

/*
#include "../Embed.h"

// TODO: should it be "js_embed.h" or this file should be generated right in js_internal/embed/ ???
#include "v8/v8_base.h"

namespace NSEmbed
{
	void _FunctionSum(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CTestEmbed* _this = (CTestEmbed*)unwrap_native(args.Holder());
		JSSmart<CJSValue> ret = _this->FunctionSum(js_value(args[0]), js_value(args[1]));
		js_return(args, ret);
	}

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);

		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		// methods
		NSV8Objects::Template_Set(result, "FunctionSum", _FunctionSum);

		return handle_scope.Escape(result);
	}

	void CreateNative(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CreateNativeInternalField(new CTestEmbed(), NSEmbed::CreateTemplate, args);
	}
}

void CTestEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
	InsertToGlobal(name, context, NSEmbed::CreateNative);
}
*/
