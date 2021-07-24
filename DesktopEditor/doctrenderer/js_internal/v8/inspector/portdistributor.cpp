#include "portdistributor.h"

std::atomic<uint16_t> NSJSBase::v8_debug::internal::CPortDistributor::m_iCount{0};

uint16_t NSJSBase::v8_debug::internal::CPortDistributor::getPort()
{
    return m_iStartPort + m_iCount++;
}
