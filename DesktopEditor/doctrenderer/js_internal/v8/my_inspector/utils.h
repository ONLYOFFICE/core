#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <v8.h>
#include <v8-inspector.h>

static inline std::string convertToString(v8::Isolate* isolate, const v8_inspector::StringView stringView) {
  int length = static_cast<int>(stringView.length());
  v8::Local<v8::String> message = (
		stringView.is8Bit()
		  ? v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t*>(stringView.characters8()), v8::NewStringType::kNormal, length)
		  : v8::String::NewFromTwoByte(isolate, reinterpret_cast<const uint16_t*>(stringView.characters16()), v8::NewStringType::kNormal, length)
	  ).ToLocalChecked();
  v8::String::Utf8Value result(isolate, message);
  return *result;
}

static inline v8_inspector::StringView convertToStringView(const std::string &str) {
  auto* stringView = reinterpret_cast<const uint8_t*>(str.c_str());
  return { stringView, str.length() };
}

static inline v8::Local<v8::Object> parseJson(const v8::Local<v8::Context> &context, const std::string &json) {
	v8::MaybeLocal<v8::Value> value_ = v8::JSON::Parse(context, v8::String::NewFromUtf8(context->GetIsolate(), &json[0], v8::NewStringType::kNormal).ToLocalChecked());
	if (value_.IsEmpty()) {
		return v8::Local<v8::Object>();
	}
	return value_.ToLocalChecked()->ToObject(context).ToLocalChecked();
}

static inline std::string getPropertyFromJson(v8::Isolate* isolate, const v8::Local<v8::Object> &jsonObject, const std::string &propertyName) {
	v8::Local<v8::Value> property = jsonObject->Get(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate, propertyName.c_str(), v8::NewStringType::kNormal).ToLocalChecked()).ToLocalChecked();
	v8::String::Utf8Value utf8Value(isolate, property);
	return *utf8Value;
}

static std::string readFileContent(const std::string &filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std:: cerr << "Could not open the file: " << filename << std::endl;
		return "";
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

static std::string getExceptionMessage(v8::Isolate* isolate, const v8::Local<v8::Value> &exception) {
	v8::Local<v8::Object> error = v8::Local<v8::Object>::Cast(exception);
	v8::String::Utf8Value exceptionMessage(isolate, error);
	return *exceptionMessage;
}

static bool compileScript(const v8::Local<v8::Context>& context, const v8::Local<v8::String> &source, const std::string &filePath, v8::Local<v8::Script> &script, const v8::TryCatch &tryCatch) {
	const auto isolate_ = context->GetIsolate();
	v8::ScriptOrigin scriptOrigin = v8::ScriptOrigin(
			v8::String::NewFromUtf8(
					isolate_,
					("file://" + filePath).c_str(),
					v8::NewStringType::kNormal
			).ToLocalChecked()
	);
	v8::MaybeLocal<v8::Script> script_ = v8::Script::Compile(context, source, &scriptOrigin);
	if (!script_.IsEmpty()) {
		script = script_.ToLocalChecked();
	}
	return !tryCatch.HasCaught();
}

static bool executeScript(const v8::Local<v8::Context>& context, const v8::Local<v8::Script> &script, const v8::TryCatch &tryCatch) {
	script->Run(context).ToLocalChecked();
	return !tryCatch.HasCaught();
}

static bool runScript(const v8::Local<v8::Context>& context, const std::string &filePath) {
	std::string fileContent = readFileContent(filePath);

	const auto& isolate_ = context->GetIsolate();
	v8::Local<v8::Script> script;
	v8::TryCatch tryCatch(isolate_);
	v8::Local<v8::String> source = v8::String::NewFromUtf8(
			isolate_,
			fileContent.c_str(),
			v8::NewStringType::kNormal
	).ToLocalChecked();

	if (!compileScript(context, source, filePath, script, tryCatch)) {
		std::cerr << "could not compile the script" << std::endl;
		std::cerr << "Exception: " << getExceptionMessage(isolate_, tryCatch.Exception()) << std::endl;
		return false;
	}
	if (!executeScript(context, script, tryCatch)) {
		std::cerr << "could not execute the script" << std::endl;
		std::cerr << "Exception: " << getExceptionMessage(isolate_, tryCatch.Exception()) << std::endl;
		return false;
	}

	return true;
}

#endif // UTILS_H
