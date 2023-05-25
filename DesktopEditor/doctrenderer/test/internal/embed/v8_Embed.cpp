// THIS FILE WILL BE GENERATED
// THIS IS TEST PREVIEW

#include "../Embed.h"

#include "js_embed.h"

namespace NSEmbed
{
	void FunctionSum(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CTestEmbed* _this = (CTestEmbed*)unwrap_native(args.Holder());
		v8::Local<v8::Value> arg0 = args[0];
		v8::Local<v8::Value> arg1 = args[1];
		JSSmart<CJSValue> ret = _this->FunctionSum(_Native2Value(&arg0), _Native2Value(&arg1));
		_ReturnJSValue(reinterpret_cast<const void*>(&args), ret);
	}

	void FunctionSquare(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CTestEmbed* _this = (CTestEmbed*)unwrap_native(args.Holder());
		v8::Local<v8::Value> arg0 = args[0];
		JSSmart<CJSValue> ret = _this->FunctionSquare(_Native2Value(&arg0));
		_ReturnJSValue(reinterpret_cast<const void*>(&args), ret);
	}

	void FunctionDel(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CTestEmbed* _this = (CTestEmbed*)unwrap_native(args.Holder());
		v8::Local<v8::Value> arg0 = args[0];
		v8::Local<v8::Value> arg1 = args[1];
		JSSmart<CJSValue> ret = _this->FunctionDel(_Native2Value(&arg0), _Native2Value(&arg1));
		_ReturnJSValue(reinterpret_cast<const void*>(&args), ret);
	}

	void FunctionGet(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		CTestEmbed* _this = (CTestEmbed*)unwrap_native(args.Holder());
		JSSmart<CJSValue> ret = _this->FunctionGet();
		_ReturnJSValue(reinterpret_cast<const void*>(&args), ret);
	}

	v8::Handle<v8::ObjectTemplate> CreateTemplate(v8::Isolate* isolate)
	{
		v8::EscapableHandleScope handle_scope(isolate);

		v8::Local<v8::ObjectTemplate> result = v8::ObjectTemplate::New(isolate);
		result->SetInternalFieldCount(1);

		// methods
		_TemplateSet(reinterpret_cast<void*>(&result), "FunctionSum", reinterpret_cast<void*>(FunctionSum));
		_TemplateSet(reinterpret_cast<void*>(&result), "FunctionSquare", reinterpret_cast<void*>(FunctionSquare));
		_TemplateSet(reinterpret_cast<void*>(&result), "FunctionDel", reinterpret_cast<void*>(FunctionDel));
		_TemplateSet(reinterpret_cast<void*>(&result), "FunctionGet", reinterpret_cast<void*>(FunctionGet));

		return handle_scope.Escape(result);
	}

	void CreateNative(const v8::FunctionCallbackInfo<v8::Value>& args)
	{
		_CreateNativeInternalField(new CTestEmbed(), reinterpret_cast<void*>(NSEmbed::CreateTemplate), reinterpret_cast<const void*>(&args));
	}
}

void CTestEmbed::CreateObjectInContext(const std::string& name, JSSmart<CJSContext> context)
{
	_InsertToGlobal(name, context, reinterpret_cast<void*>(NSEmbed::CreateNative));
}
