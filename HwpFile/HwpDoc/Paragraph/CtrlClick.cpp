#include "CtrlClick.h"

namespace HWP
{
CCtrlClick::CCtrlClick(const STRING& sCtrlId, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlId)
{
	BYTE* pOldCurrentPos = oBuffer.GetCurPtr();

	oBuffer.Skip(4);
	oBuffer.Skip(1);

	oBuffer.ReadString(m_sClickHereStr, EStringCharacter::UTF16);

	oBuffer.Skip(4);
	oBuffer.Skip(4);

	m_nSize = oBuffer.GetCurPtr() - pOldCurrentPos;
}

int CCtrlClick::GetSize()
{
	return m_nSize;
}
}
