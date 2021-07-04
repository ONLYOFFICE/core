#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include "v8-inspector.h"//V8InspectorClient
#include "v8.h"//context

std::string getContents(std::istream &in);
std::string getFileData(const std::string &filename);

std::unique_ptr<v8_inspector::V8InspectorClient> makeInspectorClient(
        v8::Local<v8::Context> context
        , const std::string &contextName
        , uint16_t port
        );


void makeV8Stuff(const char *execLocation);

#endif // COMMON_H
