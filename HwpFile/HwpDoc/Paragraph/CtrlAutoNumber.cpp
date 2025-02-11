#include "CtrlAutoNumber.h"

namespace HWP
{
ENumType GetNumType(int nValue)
{
	SWITCH(ENumType, nValue)
	{
		DEFAULT(ENumType::PAGE);
		CASE(ENumType::FOOTNOTE);
		CASE(ENumType::ENDNOTE);
		CASE(ENumType::FIGURE);
		CASE(ENumType::TABLE);
		CASE(ENumType::EQUATION);
		CASE(ENumType::TOTAL_PAGE);
	}
}

ENumType GetNumType(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(FOOTNOTE, sValue, ENumType);
	ELSE_IF_STRING_IN_ENUM(ENDNOTE, sValue, ENumType);
	ELSE_IF_STRING_IN_ENUM(FIGURE, sValue, ENumType);
	ELSE_IF_STRING_IN_ENUM(TABLE, sValue, ENumType);
	ELSE_IF_STRING_IN_ENUM(EQUATION, sValue, ENumType);
	ELSE_IF_STRING_IN_ENUM(TOTAL_PAGE, sValue, ENumType);
	ELSE_STRING_IN_ENUM(PAGE, ENumType);
}

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

CCtrlAutoNumber::CCtrlAutoNumber(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID)
{
	m_eNumType = ::HWP::GetNumType(oNode.GetAttribute(L"numType"));

	HWP_STRING sType;

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		m_bSuperscript = oChild.GetAttributeBool(L"supscript");

		sType = oChild.GetAttribute(L"type");

		if (L"DIGIT" == sType)
			m_eNumShape = ENumberShape2::DIGIT;
		else if (L"CIRCLE_DIGIT" == sType)
			m_eNumShape = ENumberShape2::CIRCLE_DIGIT;
		else if (L"ROMAN_CAPITAL" == sType)
			m_eNumShape = ENumberShape2::ROMAN_CAPITAL;
		else if (L"ROMAN_SMALL" == sType)
			m_eNumShape = ENumberShape2::ROMAN_SMALL;
		else if (L"LATIN_CAPITAL" == sType)
			m_eNumShape = ENumberShape2::LATIN_CAPITAL;
		else if (L"LATIN_SMALL" == sType)
			m_eNumShape = ENumberShape2::LATIN_SMALL;
		else if (L"CIRCLED_LATIN_CAPITAL" == sType)
			m_eNumShape = ENumberShape2::CIRCLED_LATIN_CAPITAL;
		else if (L"CIRCLED_LATIN_SMALL" == sType)
			m_eNumShape = ENumberShape2::CIRCLED_LATIN_SMALL;
		else if (L"CIRCLED_HANGUL_SYLLABLE" == sType)
			m_eNumShape = ENumberShape2::CIRCLED_HANGUL_SYLLABLE;
		else if (L"HANGUL_JAMO" == sType)
			m_eNumShape = ENumberShape2::HANGUL_JAMO;
		else if (L"CIRCLED_HANGUL_JAMO" == sType)
			m_eNumShape = ENumberShape2::CIRCLED_HANGUL_JAMO;
		else if (L"HANGUL_PHONETIC" == sType)
			m_eNumShape = ENumberShape2::HANGUL_PHONETIC;
		else if (L"IDEOGRAPH" == sType)
			m_eNumShape = ENumberShape2::IDEOGRAPH;
		else if (L"CIRCLED_IDEOGRAPH" == sType)
			m_eNumShape = ENumberShape2::CIRCLED_IDEOGRAPH;
		else if (L"DECAGON_CIRCLE" == sType)
			m_eNumShape = ENumberShape2::DECAGON_CIRCLE;
		else if (L"DECAGON_CRICLE_HANGJA" == sType)
			m_eNumShape = ENumberShape2::DECAGON_CRICLE_HANGJA;
		else if (L"SYMBOL" == sType)
			m_eNumShape = ENumberShape2::SYMBOL;
		else if (L"USER_CHAR" == sType)
			m_eNumShape = ENumberShape2::USER_HWP_CHAR;

		m_eNumShape = GetNumberShape2(oChild.GetAttributeInt(L"hp:autoNumFormat", oChild.GetAttributeInt(L"autoNumFormat")));
	}

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
