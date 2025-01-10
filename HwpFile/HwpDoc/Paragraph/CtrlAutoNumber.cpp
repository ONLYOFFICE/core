#include "CtrlAutoNumber.h"

namespace HWP
{
CCtrlAutoNumber::CCtrlAutoNumber(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlAutoNumber::CCtrlAutoNumber(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_eNumType = ::HWP::GetNumType(nAttr & 0xF);
	m_eNumShape = GetNumberShape2(nAttr >> 4 & 0xFF);
	m_bSuperscript = CHECK_FLAG(nAttr >> 12, 0x1);

	m_bFullFilled = true;
}

ECtrlObjectType CCtrlAutoNumber::GetCtrlType() const
{
	return ECtrlObjectType::AutoNumber;
}

ENumType CCtrlAutoNumber::GetNumType() const
{
	return m_eNumType;
}
}
