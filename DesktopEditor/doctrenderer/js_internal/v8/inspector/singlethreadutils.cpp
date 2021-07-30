#include "singlethreadutils.h"
#include <iostream>//logging
#include <fstream>//get file data
#include <sstream>//string stream in getContents

//#define V8_DEBUG_MULTITHREADED

#ifdef V8_DEBUG_MULTITHREADED

#define LOCK_AND_ENTER_ISOLATE(isolate_ptr) \
    v8::Locker aasdufasidhfajskfjadhfk_lock_isolate_by_current_thread(isolate_ptr); \
    v8::Isolate::Scope aasdufasidhfajskfjadhfk_enter_isolate_scope(isolate_ptr);

#define LOCK_AND_ENTER_ISOLATE_BY_CONTEXT(context) \
    v8::Isolate *asdfdsfduf_isolate_ptr = context->GetIsolate(); \
    LOCK_AND_ENTER_ISOLATE(asdfdsfduf_isolate_ptr)

#else

#define LOCK_AND_ENTER_ISOLATE(isolate_ptr)
#define LOCK_AND_ENTER_ISOLATE_BY_CONTEXT(context)

#endif

std::string NSJSBase::v8_debug::internal::getContents(std::istream &in)
{
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string NSJSBase::v8_debug::internal::getFileData(const std::string &filename)
{
    std::ifstream f(filename);
    return getContents(f);
}

v8_inspector::StringView NSJSBase::v8_debug::internal::strToView(const std::string &str) {
    const uint8_t *data = reinterpret_cast<const uint8_t*>(str.c_str());
    std::size_t len = str.length();
    return v8_inspector::StringView{data, len};
}

std::string NSJSBase::v8_debug::internal::viewToStr(v8::Isolate *isolate
                      , const v8_inspector::StringView &view)
{
    LOCK_AND_ENTER_ISOLATE(isolate);

    v8::Local<v8::String> v8str = viewTov8str(isolate, view);
    v8::String::Utf8Value utf8(v8str);
    return std::string(*utf8);
}

v8::Local<v8::Object> NSJSBase::v8_debug::internal::parseJsonImpl(v8::Local<v8::Context> context
                                   , v8::Local<v8::String> v8str)
{
    v8::Local<v8::Value> value = v8::JSON::Parse(context, v8str).ToLocalChecked();
    v8::MaybeLocal<v8::Object> obj = value->ToObject(context);
    if (obj.IsEmpty()) {
        v8::Local<v8::Object>();
    }
    return
                obj.ToLocalChecked()
            ;
}

v8::Local<v8::String> NSJSBase::v8_debug::internal::tov8str(v8::Isolate *isolate
                                                            , const std::string &str)
{
    LOCK_AND_ENTER_ISOLATE(isolate);

    return v8::String::NewFromUtf8(isolate
                                   , str.c_str()
                                   , v8::NewStringType::kNormal
                                   , str.length())
            .ToLocalChecked();
}

v8::Local<v8::Value> NSJSBase::v8_debug::internal::getJsonPropertyImpl(v8::Local<v8::Context> context
                                                          , v8::Local<v8::Object> jsonObject
                                                          , const std::string &property)
{
    LOCK_AND_ENTER_ISOLATE_BY_CONTEXT(context);

    v8::Local<v8::Value> name = v8::String::NewFromUtf8(
                context->GetIsolate()
                , property.c_str()
                , v8::NewStringType::kNormal
                , property.length())
            .ToLocalChecked();
    v8::MaybeLocal<v8::Value> maybeProperty = jsonObject->Get(context, name);
    if (maybeProperty.IsEmpty()) {
        return v8::Local<v8::Value>();
    }
    return maybeProperty.ToLocalChecked();
}

std::string NSJSBase::v8_debug::internal::getMethod(v8::Local<v8::Context> context
                                                    , const std::string &json)
{
    return getMethodImpl(context, parseJson(context, json));
}

std::string NSJSBase::v8_debug::internal::getMethod(v8::Local<v8::Context> context
                                                    , const v8_inspector::StringView &view)
{
    return getMethodImpl(context, parseJson(context, view));
}

std::string NSJSBase::v8_debug::internal::getMethodImpl(v8::Local<v8::Context> context
                                                        , v8::Local<v8::Object> jsonObj)
{
    if (jsonObj.IsEmpty()) {
        return std::string();
    }
    return asString(getJsonPropertyImpl(context, jsonObj, "method"));
}

v8::Local<v8::String> NSJSBase::v8_debug::internal::viewTov8str(v8::Isolate *isolate
                                  , const v8_inspector::StringView &view)
{
    LOCK_AND_ENTER_ISOLATE(isolate);

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

v8::Local<v8::Object> NSJSBase::v8_debug::internal::parseJson(v8::Local<v8::Context> context
                                   , const std::string &str)
{
    LOCK_AND_ENTER_ISOLATE_BY_CONTEXT(context);

    return parseJsonImpl(context, tov8str(context->GetIsolate(), str));
}

v8::Local<v8::Object> NSJSBase::v8_debug::internal::parseJson(v8::Local<v8::Context> context
                                    , const v8_inspector::StringView &view)
{
    LOCK_AND_ENTER_ISOLATE_BY_CONTEXT(context);

    return parseJsonImpl(context, viewTov8str(context->GetIsolate(), view));
}

void NSJSBase::v8_debug::internal::logWithPrefix(std::ostream &out
                                                 , const std::string &prefix
                                                 , const std::string &message)
{
    out << prefix;
    static constexpr std::size_t bigLen = 10000;
    if (message.size() > bigLen) {
        out << "message of size " << message.size();
    }
    else {
        out << message;
    }
    out << std::endl;
}

void NSJSBase::v8_debug::internal::logCdtMessage(std::ostream &out
                                                 , const std::string &message)
{
    logWithPrefix(out, "CDT: ", message);
}

void NSJSBase::v8_debug::internal::logOutgoingMessage(std::ostream &out
                                                      , const std::string &message)
{
    logWithPrefix(out, "responce: ", message);
}

v8::Local<v8::Value> NSJSBase::v8_debug::internal::getJsonProperty(
        v8::Local<v8::Context> context
        , const std::string &json
        , const std::string &property)
{
    v8::Local<v8::Object> jsonObject = parseJson(context, json);
    if (jsonObject.IsEmpty()) {
        return v8::Local<v8::Value>();
    }
    return getJsonPropertyImpl(context, jsonObject, property);
}

std::string NSJSBase::v8_debug::internal::asString(v8::Local<v8::Value> value)
{
    v8::String::Utf8Value utf8(value);
    return std::string(*utf8);
}
