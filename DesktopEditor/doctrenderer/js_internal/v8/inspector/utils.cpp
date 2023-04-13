#include "utils.h"

#include "../v8_base.h"

namespace NSJSBase
{

	std::string convertToString(v8::Isolate* pIsolate, const v8_inspector::StringView oStringView)
	{
		int length = static_cast<int>(oStringView.length());
		v8::Local<v8::String> message =
			(
				oStringView.is8Bit()
				? v8::String::NewFromOneByte(pIsolate, reinterpret_cast<const uint8_t*>(oStringView.characters8()), v8::NewStringType::kNormal, length).ToLocalChecked()
				: v8::String::NewFromTwoByte(pIsolate, reinterpret_cast<const uint16_t*>(oStringView.characters16()), v8::NewStringType::kNormal, length).ToLocalChecked()
			);
		v8::String::Utf8Value result(V8IsolateFirstArg message);
		return *result;
	}

	v8_inspector::StringView convertToStringView(const std::string& str)
	{
		const uint8_t* pStringView = reinterpret_cast<const uint8_t*>(str.c_str());
		return { pStringView, str.length() };
	}

	v8::Local<v8::Object> parseJson(const v8::Local<v8::Context>& context, const std::string& sJson)
	{
		v8::MaybeLocal<v8::Value> jsonValue = v8::JSON::Parse(context, CreateV8String(context->GetIsolate(), sJson));
		if (jsonValue.IsEmpty())
		{
			return v8::Local<v8::Object>();
		}
		return jsonValue.ToLocalChecked()->ToObject(context).ToLocalChecked();
	}

	std::string getPropertyFromJson(v8::Isolate* pIsolate, const v8::Local<v8::Object>& jsonObject, const std::string& sPropertyName)
	{
		v8::Local<v8::Value> property = jsonObject->Get(pIsolate->GetCurrentContext(), CreateV8String(pIsolate, sPropertyName)).ToLocalChecked();
		v8::String::Utf8Value utf8Value(V8IsolateFirstArg property);
		return *utf8Value;
	}

}
