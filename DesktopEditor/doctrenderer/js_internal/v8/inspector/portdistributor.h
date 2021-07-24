#ifndef CPORTDISTRIBUTOR_H
#define CPORTDISTRIBUTOR_H

#include <atomic>//std::atomic

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CPortDistributor
{
    static constexpr uint16_t m_iStartPort = 8080;
    static std::atomic<uint16_t> m_iCount;

public:
    static uint16_t getPort();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CPORTDISTRIBUTOR_H
