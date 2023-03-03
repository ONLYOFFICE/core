#include "utils.h"

#include <iostream>
#include <fstream>

std::string convertToString(v8::Isolate* pIsolate, const v8_inspector::StringView oStringView)
{
	int length = static_cast<int>(oStringView.length());
	v8::Local<v8::String> message =
		(
			oStringView.is8Bit()
			? v8::String::NewFromOneByte(pIsolate, reinterpret_cast<const uint8_t*>(oStringView.characters8()), v8::NewStringType::kNormal, length).ToLocalChecked()
			: v8::String::NewFromTwoByte(pIsolate, reinterpret_cast<const uint16_t*>(oStringView.characters16()), v8::NewStringType::kNormal, length).ToLocalChecked()
		);
	v8::String::Utf8Value result(pIsolate, message);
	return *result;
}

v8_inspector::StringView convertToStringView(const std::string& str)
{
	const uint8_t* pStringView = reinterpret_cast<const uint8_t*>(str.c_str());
	return { pStringView, str.length() };
}

v8::Local<v8::Object> parseJson(const v8::Local<v8::Context>& context, const std::string& sJson)
{
	v8::MaybeLocal<v8::Value> jsonValue = v8::JSON::Parse(context, v8::String::NewFromUtf8(context->GetIsolate(), &sJson[0], v8::NewStringType::kNormal).ToLocalChecked());
	if (jsonValue.IsEmpty())
	{
		return v8::Local<v8::Object>();
	}
	return jsonValue.ToLocalChecked()->ToObject(context).ToLocalChecked();
}

std::string getPropertyFromJson(v8::Isolate* pIsolate, const v8::Local<v8::Object>& jsonObject, const std::string& sPropertyName)
{
	v8::Local<v8::Value> property = jsonObject->Get(pIsolate->GetCurrentContext(), v8::String::NewFromUtf8(pIsolate, sPropertyName.c_str(), v8::NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
	v8::String::Utf8Value utf8Value(pIsolate, property);
	return *utf8Value;
}

std::string readFileContent(const std::string& sFilename)
{
	std::ifstream oFile(sFilename);
	if (!oFile.is_open())
	{
		std:: cerr << "Could not open the file: " << sFilename << std::endl;
		return "";
	}
	std::string sContent((std::istreambuf_iterator<char>(oFile)), std::istreambuf_iterator<char>());
	return sContent;
}
