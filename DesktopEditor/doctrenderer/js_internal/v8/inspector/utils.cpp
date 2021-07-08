#include "utils.h"
#include <iostream>
#include <fstream>//get file data
#include <sstream>//string stream in getContents

std::string getContents(std::istream &in)
{
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string getFileData(const std::string &filename)
{
    std::ifstream f(filename);
    return getContents(f);
}

v8_inspector::StringView strToView(const std::string &str) {
    const uint8_t *data = reinterpret_cast<const uint8_t*>(str.c_str());
    std::size_t len = str.length();
    return v8_inspector::StringView{data, len};
}

std::string viewToStr(v8::Isolate *isolate
                      , const v8_inspector::StringView &view)
{
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::String> v8str = viewTov8str(isolate, view);
    v8::String::Utf8Value utf8(v8str);
    return std::string(*utf8);
}

v8::Local<v8::Object> parseJsonImpl(v8::Local<v8::Context> context
                                   , v8::Local<v8::String> v8str)
{
    v8::Isolate *isolate = context->GetIsolate();
    v8::Isolate::Scope isolateScope(isolate);
    v8::EscapableHandleScope handleScope(isolate);

    v8::Local<v8::Value> value = v8::JSON::Parse(context, v8str).ToLocalChecked();
    v8::Local<v8::Object> obj = value->ToObject(context).ToLocalChecked();
    return
            handleScope.Escape(obj)
            ;
}

v8::Local<v8::String> tov8str(v8::Isolate *isolate, const std::string &str)
{
    return v8::String::NewFromUtf8(isolate
                                   , str.c_str()
                                   , v8::NewStringType::kNormal
                                   , str.length())
            .ToLocalChecked();
}

std::string getJsonProperty(v8::Local<v8::Context> context, v8::Local<v8::Object> jsonObject, const std::string &property)
{
    v8::Isolate *isolate = context->GetIsolate();
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::Value> name = v8::String::NewFromUtf8(
                context->GetIsolate()
                , property.c_str()
                , v8::NewStringType::kNormal
                , property.length())
            .ToLocalChecked();
    v8::Local<v8::Value> prop = jsonObject->Get(context, name).ToLocalChecked();
    v8::String::Utf8Value utf8(prop);
    return std::string(*utf8);
}

std::string getMethod(v8::Local<v8::Context> context, const std::string &json)
{
    return getMethodImpl(context, parseJson(context, json));
}

std::string getMethod(v8::Local<v8::Context> context, const v8_inspector::StringView &view)
{
    return getMethodImpl(context, parseJson(context, view));
}

std::string getMethodImpl(v8::Local<v8::Context> context, v8::Local<v8::Object> jsonObj)
{
    if (jsonObj.IsEmpty()) {
        return std::string();
    }
    return getJsonProperty(context, jsonObj, "method");
}

v8::Local<v8::String> viewTov8str(v8::Isolate *isolate
                                  , const v8_inspector::StringView &view)
{
    return
            (
            view.is8Bit()
            ?
            v8::String::NewFromOneByte
                (
                isolate
                , view.characters8()
                , v8::NewStringType::kNormal
                , view.length()
                )
            :
                v8::String::NewFromTwoByte
                (
                    isolate
                    , view.characters16()
                    , v8::NewStringType::kNormal
                    , view.length()
                    )
                    ).ToLocalChecked();
}

v8::Local<v8::Object> parseJson(v8::Local<v8::Context> context
                                   , const std::string &str)
{
    return parseJsonImpl(context, tov8str(context->GetIsolate(), str));
}

v8::Local<v8::Object> parseJson(v8::Local<v8::Context> context
                                    , const v8_inspector::StringView &view)
{
    return parseJsonImpl(context, viewTov8str(context->GetIsolate(), view));
}




v8::Local<v8::Script> makeTrialScript(v8::Local<v8::Context> context)
{
    v8::Isolate *isolate = context->GetIsolate();
    v8::Isolate::Scope isolateScope(isolate);
    v8::EscapableHandleScope handleScope(isolate);

    std::cout << "called make trial script" << std::endl;
    const char rawString[] = "function a()\
                                    {\
                                    return 2 + 3;\
                                    }\
                                    a();";
    v8::Local<v8::String> string =
            v8::String::NewFromUtf8(
                context->GetIsolate()
                , rawString
                , v8::NewStringType::kNormal
                ).ToLocalChecked();

    v8::Local<v8::Script> script = v8::Script::Compile(context, string).ToLocalChecked();

    return
            handleScope.Escape(script)
            ;
}

v8::Local<v8::Script> getFileScript(v8::Local<v8::Context> context
                                    , const std::string &filename)
{
    //
    std::string raw = getFileData(filename);
    std::cout << "script: " << raw << std::endl;

    //
    v8::Isolate *isolate = context->GetIsolate();
    v8::Local<v8::String> source = tov8str(isolate, raw);

    //
    v8::Local<v8::String> v8filename = tov8str(isolate, "file:///" + filename);

    //
    v8::ScriptOrigin origin(v8filename);

    //
    v8::Local<v8::Script> script = v8::Script::Compile(context, source, &origin).ToLocalChecked();
    return script;
}
