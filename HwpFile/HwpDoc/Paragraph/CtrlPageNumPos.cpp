#include "CtrlPageNumPos.h"

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

ENumPos GetNumPos(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(TOP_LEFT, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(TOP_CENTER, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(TOP_RIGHT, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(BOTTOM_LEFT, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(BOTTOM_CENTER, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(BOTTOM_RIGHT, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(TOP_OUTER, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(BOTTOM_OUTER, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(TOP_INNER, sValue, ENumPos);
	ELSE_IF_STRING_IN_ENUM(BOTTOM_INNER, sValue, ENumPos);
	ELSE_STRING_IN_ENUM(NONE, ENumPos);
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

CCtrlPageNumPos::CCtrlPageNumPos(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion)
	: CCtrl(sCtrlID)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("pos" == sAttributeName)
			m_ePos = GetNumPos(oReader.GetText2());
		else if ("formatType" == sAttributeName)
			m_eNumShape = GetNumberShape2(oReader.GetInt());
		else if ("sideChar" == sAttributeName)
			m_sPostfix = oReader.GetText2();
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
