#include "CtrlPageNumPos.h"

namespace HWP
{
ENumPos GetNumPos(int nValue)
{
	switch(static_cast<ENumPos>(nValue))
	{
		case ENumPos::LEFT_TOP:
		case ENumPos::CENTER_TOP:
		case ENumPos::RIGHT_TOP:
		case ENumPos::LEFT_BOTTOM:
		case ENumPos::BOTTOM_CENTER:
		case ENumPos::RIGHT_BOTTOM:
		case ENumPos::OUTER_TOP:
		case ENumPos::OUTER_BOTTOM:
		case ENumPos::INNER_TOP:
		case ENumPos::INNER_BOTTOM:
			return static_cast<ENumPos>(nValue);
		case ENumPos::NONE:
		default:
			return ENumPos::NONE;
	}
}

CCtrlPageNumPos::CCtrlPageNumPos(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlPageNumPos::CCtrlPageNumPos(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	int nAttr;
	oBuffer.ReadInt(nAttr);
	m_eNumShape = GetNumberShape2(nAttr & 0xFF);
	m_ePos = GetNumPos((nAttr >> 8) & 0xF);

	oBuffer.Skip(2);
	oBuffer.ReadString(m_sUserDef, 2, EStringCharacter::UTF16);
	oBuffer.ReadString(m_sPrefix, 2, EStringCharacter::UTF16);
	oBuffer.ReadString(m_sPostfix, 2, EStringCharacter::UTF16);
	oBuffer.ReadString(m_sConstantDash, 2, EStringCharacter::UTF16);
}

CCtrlPageNumPos::CCtrlPageNumPos(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID)
{
	m_ePos = GetNumPos(oNode.GetAttributeInt(L"pos"));
	m_eNumShape = GetNumberShape2(oNode.GetAttributeInt(L"formatType"));
}

ECtrlObjectType CCtrlPageNumPos::GetCtrlType() const
{
	return ECtrlObjectType::PageNumPos;
}
}
