#include "CtrlPageNumPos.h"

#include "../Common/NodeNames.h"

namespace HWP
{
ENumPos GetNumPos(int nValue)
{
	SWITCH(ENumPos, nValue)
	{
		DEFAULT(ENumPos::NONE);
		CASE(ENumPos::TOP_LEFT);
		CASE(ENumPos::TOP_CENTER);
		CASE(ENumPos::TOP_RIGHT);
		CASE(ENumPos::BOTTOM_LEFT);
		CASE(ENumPos::BOTTOM_CENTER);
		CASE(ENumPos::BOTTOM_RIGHT);
		CASE(ENumPos::TOP_OUTER);
		CASE(ENumPos::BOTTOM_OUTER);
		CASE(ENumPos::TOP_INNER);
		CASE(ENumPos::BOTTOM_INNER);
	}
}

ENumPos GetNumPos(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::None, eType) == sValue)
		return ENumPos::NONE;
	if (GetValueName(EValue::TopLeft, eType) == sValue)
		return ENumPos::TOP_LEFT;
	if (GetValueName(EValue::TopCenter,eType) == sValue)
		return ENumPos::TOP_CENTER;
	if (GetValueName(EValue::TopRight, eType) == sValue)
		return ENumPos::TOP_RIGHT;
	if (GetValueName(EValue::BottomLeft, eType) == sValue)
		return ENumPos::BOTTOM_LEFT;
	if (GetValueName(EValue::BottomCenter, eType) == sValue)
		return ENumPos::BOTTOM_CENTER;
	if (GetValueName(EValue::BottomRight, eType) == sValue)
		return ENumPos::BOTTOM_RIGHT;
	if (GetValueName(EValue::TopOuter, eType) == sValue)
		return ENumPos::TOP_OUTER;
	if (GetValueName(EValue::BottomOuter, eType) == sValue)
		return ENumPos::BOTTOM_OUTER;
	if (GetValueName(EValue::TopInner, eType) == sValue)
		return ENumPos::TOP_INNER;
	if (GetValueName(EValue::BottomInner, eType) == sValue)
		return ENumPos::BOTTOM_INNER;

	return ENumPos::NONE;
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

CCtrlPageNumPos::CCtrlPageNumPos(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
	: CCtrl(sCtrlID)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Pos, eType) == sAttributeName)
			m_ePos = GetNumPos(oReader.GetTextA(), eType);
		else if (GetAttributeName(EAttribute::FormatType, eType) == sAttributeName)
			m_eNumShape = GetNumberShape2(oReader.GetInt());
		else if (GetAttributeName(EAttribute::SideChar, eType) == sAttributeName)
			m_sPostfix = oReader.GetText();
	}
	END_READ_ATTRIBUTES(oReader)

	m_sPrefix  = m_sPostfix;
}

ENumPos CCtrlPageNumPos::GetPos() const
{
	return m_ePos;
}

HWP_STRING CCtrlPageNumPos::GetPrefix() const
{
	return m_sPrefix;
}

HWP_STRING CCtrlPageNumPos::GetPostfix() const
{
	return m_sPostfix;
}

ENumberShape2 CCtrlPageNumPos::GetFormatType() const
{
	return m_eNumShape;
}

ECtrlObjectType CCtrlPageNumPos::GetCtrlType() const
{
	return ECtrlObjectType::PageNumPos;
}
}
