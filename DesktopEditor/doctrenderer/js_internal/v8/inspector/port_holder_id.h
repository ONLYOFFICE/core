#ifndef PORT_HOLDER_ID_H
#define PORT_HOLDER_ID_H

#include "thread_id.h"//GetCurrentThreadId
#include <cstddef>//std::size_t

namespace NSJSBase {
namespace v8_debug {
namespace internal {

using InspectorCountType = std::size_t;
struct CPortHolderId {
    ASC_THREAD_ID threadId;
    InspectorCountType count;
};
struct CPHIdLess {
    inline constexpr bool operator() (const CPortHolderId &lhs, const CPortHolderId &rhs) const {
        return
                (lhs.threadId == rhs.threadId)
                ?
                    lhs.count < rhs.count
                  :
                    lhs.threadId < rhs.threadId
                    ;
    }
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // PORT_HOLDER_ID_H
