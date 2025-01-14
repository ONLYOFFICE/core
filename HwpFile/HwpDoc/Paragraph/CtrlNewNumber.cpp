#include "CtrlNewNumber.h"

namespace HWP
{
CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_eNumType = GetNumType(nAttr & 0xF);
	m_eNumShape = GetNumberShape2((nAttr >> 4) & 0xF);

	oBuffer.ReadShort(m_shNum);

	m_bFullFilled = true;
}

ECtrlObjectType CCtrlNewNumber::GetCtrlType() const
{
	return ECtrlObjectType::NewNumber;
}
}
