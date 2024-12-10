#ifndef CTRLCLICK_H
#define CTRLCLICK_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
class CCtrlClick : public CCtrl
{
	int m_nSize;
	STRING m_sClickHereStr;
public:
	CCtrlClick(const STRING& sCtrlId, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	int GetSize() override;
};
}

#endif // CTRLCLICK_H
