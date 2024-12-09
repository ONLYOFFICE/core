#include "CtrlNewNumber.h"

namespace HWP
{
CCtrlNewNumber::CCtrlNewNumber(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	oBuffer.SavePosition();

	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_eNumType = GetNumType(nAttr & 0xF);
	m_eNumShape = GetNumberShape2((nAttr >> 4) & 0xF);

	oBuffer.ReadShort(m_shNum);

	m_bFullFilled = true;

	m_nSize = oBuffer.GetDistanceToLastPos();
}

int CCtrlNewNumber::GetSize()
{
	return m_nSize;
}
}
