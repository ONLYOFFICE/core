#ifndef UTILS_H
#define UTILS_H

#include <v8-inspector.h>
#include <string>

//dealing with file data
std::string getContents(std::istream &in);
std::string getFileData(const std::string &filename);

//logging
void logWithPrefix(std::ostream &out, const std::string &prefix, const std::string &message);
void logCdtMessage(std::ostream &out, const std::string &message);
void logOutgoingMessage(std::ostream &out, const std::string &message);

//for sending and acquiring frontend messages
v8_inspector::StringView strToView(const std::string &str);
std::string viewToStr(v8::Isolate *isolate
                      , const v8_inspector::StringView &view);

//
v8::Local<v8::String> tov8str(v8::Isolate *isolate, const std::string &str);
//
v8::Local<v8::String> viewTov8str(v8::Isolate *isolate
                                  , const v8_inspector::StringView &view);


//to check "Runtime.runIfWaitingForDebugger" method to run debugging script
v8::Local<v8::Object> parseJsonImpl(v8::Local<v8::Context> context
        , v8::Local<v8::String> v8str
        );
std::string getJsonProperty(
        v8::Local<v8::Context> context
        , v8::Local<v8::Object> jsonObject
        , const std::string &property
        );

std::string getMethodImpl(v8::Local<v8::Context> context
                          , v8::Local<v8::Object> jsonObj);


//
std::string getMethod(
        v8::Local<v8::Context> context
        , const std::string &json
        );
std::string getMethod(
        v8::Local<v8::Context> context
        , const v8_inspector::StringView &view
        );


//
v8::Local<v8::Object> parseJson(
        v8::Local<v8::Context> context
        , const std::string &str
        );
v8::Local<v8::Object> parseJson(
        v8::Local<v8::Context> context
        , const v8_inspector::StringView &view
        );

//
v8::Local<v8::Script> makeTrialScript(v8::Local<v8::Context> context);

//
v8::Local<v8::Script> getFileScript(v8::Local<v8::Context> context
                                    , const std::string &filename);

#endif // UTILS_H
