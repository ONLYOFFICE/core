#ifndef UTILS_H
#define UTILS_H

#include <v8.h>
#include <v8-inspector.h>

// convert v8_inspector::StringView to std::string
std::string convertToString(v8::Isolate* pIsolate, const v8_inspector::StringView oStringView);
// convert std::string to v8_inspector::StringView
v8_inspector::StringView convertToStringView(const std::string& str);
// get JSON object from JSON-string
v8::Local<v8::Object> parseJson(const v8::Local<v8::Context>& context, const std::string& sJson);
// get specified property value from JSON object
std::string getPropertyFromJson(v8::Isolate* pIsolate, const v8::Local<v8::Object>& jsonObject, const std::string& sPropertyName);
// get content of file as a string
std::string readFileContent(const std::string& sFilename);

#endif // UTILS_H
