#ifndef CSERVERHOLDER_H
#define CSERVERHOLDER_H

#include <memory>//std::unique_ptr

namespace NSJSBase {
namespace v8_debug {
namespace internal {

class CSingleConnectionServer;

class CServerHolder
{
public:
    using shared_flag_t = bool;

private:
    std::unique_ptr<CSingleConnectionServer> m_pServer{nullptr};
    shared_flag_t m_bInUse{false};
    shared_flag_t m_bServerReady{false};

    void printLaunchHint(uint16_t port) const;

public:
    CServerHolder(const CServerHolder&) = delete;
    CServerHolder(CServerHolder&&) = delete;
    CServerHolder& operator=(const CServerHolder&) = delete;
    CServerHolder& operator=(CServerHolder&&) = delete;

    struct CUseData {
        CSingleConnectionServer *pServer;
        shared_flag_t &inUse;
        shared_flag_t &ready;
    };

    CServerHolder(uint16_t port);
    CUseData getServer();
    bool free() const;
    shared_flag_t& getServerReadyFlag();
    bool isServerReady();
    ~CServerHolder();
};

}//namespace internal
}//namespace v8_debug
}//namespace NSJSBase

#endif // CSERVERHOLDER_H
