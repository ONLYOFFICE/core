#include "CtrlClick.h"

namespace HWP
{
CCtrlClick::CCtrlClick(const HWP_STRING& sCtrlId, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlId)
{
	oBuffer.SavePosition();

	oBuffer.Skip(4);
	oBuffer.Skip(1);

	oBuffer.ReadString(m_sClickHereStr, EStringCharacter::UTF16);

	oBuffer.Skip(4);
	oBuffer.Skip(4);

	m_nSize = oBuffer.GetDistanceToLastPos(true);
}

int CCtrlClick::GetSize()
{
	return m_nSize;
}
}
