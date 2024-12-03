#include "CtrlAutoNumber.h"

namespace HWP
{

ENumType GetNumType(int nValue)
{
	switch(static_cast<ENumType>(nValue))
	{
		case ENumType::PAGE:
		case ENumType::FOOTNOTE:
		case ENumType::ENDNOTE:
		case ENumType::FIGURE:
		case ENumType::TABLE:
		case ENumType::EQUATION:
		case ENumType::TOTAL_PAGE:
			return static_cast<ENumType>(nValue);
		default:
			return ENumType::null;
	}
}

CCtrlAutoNumber::CCtrlAutoNumber(const std::string& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlAutoNumber::CCtrlAutoNumber(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_eNumType = GetNumType(nAttr & 0xF);
	m_eNumShape = GetNumberShape2(nAttr >> 4 & 0xFF);
	m_bSuperscript = CHECK_FLAG(nAttr >> 12, 0x01);
}

int CCtrlAutoNumber::GetSize()
{
	return m_nSize;
}
}
