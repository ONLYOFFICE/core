#ifndef CTRLCLICK_H
#define CTRLCLICK_H

#include "Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
class CCtrlClick : public CCtrl
{
	HWP_STRING m_sClickHereStr;
public:
	CCtrlClick(const HWP_STRING& sCtrlId, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);

	ECtrlObjectType GetCtrlType() const override;
};
}

#endif // CTRLCLICK_H
