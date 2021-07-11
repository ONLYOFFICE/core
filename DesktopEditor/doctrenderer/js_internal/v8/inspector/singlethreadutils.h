#ifndef SINGLETHREADUTILS_H
#define SINGLETHREADUTILS_H

#include <v8-inspector.h>
#include <string>

namespace NSJSBase {
namespace v8_debug {
namespace internal {

    //getting file data
    std::string getContents(std::istream &in);
    std::string getFileData(const std::string &filename);

    //logging
    void logWithPrefix(std::ostream &out, const std::string &prefix, const std::string &message);
    void logCdtMessage(std::ostream &out, const std::string &message);
    void logOutgoingMessage(std::ostream &out, const std::string &message);

    //conversion between different strings
    v8_inspector::StringView strToView(const std::string &str);
    std::string viewToStr(v8::Isolate *isolate
                          , const v8_inspector::StringView &view);
    v8::Local<v8::String> tov8str(v8::Isolate *isolate
                                  , const std::string &str);
    v8::Local<v8::String> viewTov8str(v8::Isolate *isolate
                                      , const v8_inspector::StringView &view);


    //get value of property |property| from json object
    std::string getJsonProperty(
            v8::Local<v8::Context> context
            , v8::Local<v8::Object> jsonObject
            , const std::string &property
            );

    //extract field "method" from json string
    std::string getMethodImpl(v8::Local<v8::Context> context
                              , v8::Local<v8::Object> jsonObj);
    std::string getMethod(
            v8::Local<v8::Context> context
            , const std::string &json
            );
    std::string getMethod(
            v8::Local<v8::Context> context
            , const v8_inspector::StringView &view
            );


    //parse string and get json object
    v8::Local<v8::Object> parseJsonImpl(v8::Local<v8::Context> context
            , v8::Local<v8::String> v8str
            );
    v8::Local<v8::Object> parseJson(
            v8::Local<v8::Context> context
            , const std::string &str
            );
    v8::Local<v8::Object> parseJson(
            v8::Local<v8::Context> context
            , const v8_inspector::StringView &view
            );
}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase


#endif // SINGLETHREADUTILS_H
