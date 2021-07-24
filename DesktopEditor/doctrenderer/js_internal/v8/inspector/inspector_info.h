#ifndef CINSPECTORINFO_H
#define CINSPECTORINFO_H

#include <stdint.h>//uint16_t
#include <string>//std::string

namespace NSJSBase {
namespace v8_debug {
namespace internal {

struct CInspectorInfo
{
    bool log;
    int contextGroupId;
    std::string contextName;

    static int getContextId();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CINSPECTORINFO_H
