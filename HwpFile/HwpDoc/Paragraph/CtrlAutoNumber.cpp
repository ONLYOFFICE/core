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

CCtrlAutoNumber::CCtrlAutoNumber(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion)
	: CCtrl(sCtrlID)
{
	m_eNumType = ::HWP::GetNumType(oReader.GetAttribute("numType"));

	WHILE_READ_NEXT_NODE(oReader)
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("supscript" == sAttributeName)
				m_bSuperscript = oReader.GetBool();
			else if ("type" == sAttributeName)
			{
				const std::string sType{oReader.GetText2A()};

				if ("DIGIT" == sType)
					m_eNumShape = ENumberShape2::DIGIT;
				else if ("CIRCLE_DIGIT" == sType)
					m_eNumShape = ENumberShape2::CIRCLE_DIGIT;
				else if ("ROMAN_CAPITAL" == sType)
					m_eNumShape = ENumberShape2::ROMAN_CAPITAL;
				else if ("ROMAN_SMALL" == sType)
					m_eNumShape = ENumberShape2::ROMAN_SMALL;
				else if ("LATIN_CAPITAL" == sType)
					m_eNumShape = ENumberShape2::LATIN_CAPITAL;
				else if ("LATIN_SMALL" == sType)
					m_eNumShape = ENumberShape2::LATIN_SMALL;
				else if ("CIRCLED_LATIN_CAPITAL" == sType)
					m_eNumShape = ENumberShape2::CIRCLED_LATIN_CAPITAL;
				else if ("CIRCLED_LATIN_SMALL" == sType)
					m_eNumShape = ENumberShape2::CIRCLED_LATIN_SMALL;
				else if ("CIRCLED_HANGUL_SYLLABLE" == sType)
					m_eNumShape = ENumberShape2::CIRCLED_HANGUL_SYLLABLE;
				else if ("HANGUL_JAMO" == sType)
					m_eNumShape = ENumberShape2::HANGUL_JAMO;
				else if ("CIRCLED_HANGUL_JAMO" == sType)
					m_eNumShape = ENumberShape2::CIRCLED_HANGUL_JAMO;
				else if ("HANGUL_PHONETIC" == sType)
					m_eNumShape = ENumberShape2::HANGUL_PHONETIC;
				else if ("IDEOGRAPH" == sType)
					m_eNumShape = ENumberShape2::IDEOGRAPH;
				else if ("CIRCLED_IDEOGRAPH" == sType)
					m_eNumShape = ENumberShape2::CIRCLED_IDEOGRAPH;
				else if ("DECAGON_CIRCLE" == sType)
					m_eNumShape = ENumberShape2::DECAGON_CIRCLE;
				else if ("DECAGON_CRICLE_HANGJA" == sType)
					m_eNumShape = ENumberShape2::DECAGON_CRICLE_HANGJA;
				else if ("SYMBOL" == sType)
					m_eNumShape = ENumberShape2::SYMBOL;
				else if ("USER_CHAR" == sType)
					m_eNumShape = ENumberShape2::USER_HWP_CHAR;
			}
			else if ("hp:autoNumFormat" == sAttributeName ||
			         "autoNumFormat" == sAttributeName)
				m_eNumShape = GetNumberShape2(oReader.GetInt());
		}
		END_READ_ATTRIBUTES(oReader)
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
