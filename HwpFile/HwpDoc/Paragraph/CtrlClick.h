#ifndef CTRLCLICK_H
#define CTRLCLICK_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
class CCtrlClick : public CCtrl
{
	int m_nSize;
	std::string m_sClickHereStr;
public:
	CCtrlClick(const std::string& sCtrlId, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLCLICK_H
