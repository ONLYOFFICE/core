/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "CtrlAutoNumber.h"

#include "../Common/NodeNames.h"

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

ENumType GetNumType(std::string sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::Page, eType) == sValue)
		return ENumType::PAGE;
	else if (GetValueName(EValue::TotalPage, eType) == sValue)
		return ENumType::TOTAL_PAGE;
	else if (GetValueName(EValue::Footnote, eType) == sValue)
		return ENumType::FOOTNOTE;
	else if (GetValueName(EValue::Endnote, eType) == sValue)
		return ENumType::ENDNOTE;
	else if (GetValueName(EValue::Figure, eType) == sValue)
		return ENumType::TABLE;
	else if (GetValueName(EValue::Equation, eType) == sValue)
		return ENumType::EQUATION;

	return ENumType::PAGE;
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

CCtrlAutoNumber::CCtrlAutoNumber(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
	: CCtrl(sCtrlID)
{
	m_eNumType = ::HWP::GetNumType(oReader.GetAttributeA(GetAttributeName(EAttribute::NumberType, eType)), eType);

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::AutoNumFormat, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::SuperScript, eType) == sAttributeName)
				m_bSuperscript = oReader.GetBool();
			else if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
			{
				const std::string sType{oReader.GetTextA()};

				if (GetValueName(EValue::Digit, eType) == sType)
					m_eNumShape = ENumberShape2::DIGIT;
				else if (GetValueName(EValue::CircledDigit, eType) == sType)
					m_eNumShape = ENumberShape2::CIRCLE_DIGIT;
				else if (GetValueName(EValue::RomanCapital, eType) == sType)
					m_eNumShape = ENumberShape2::ROMAN_CAPITAL;
				else if (GetValueName(EValue::RomanSmall, eType) == sType)
					m_eNumShape = ENumberShape2::ROMAN_SMALL;
				else if (GetValueName(EValue::LatinCapital, eType) == sType)
					m_eNumShape = ENumberShape2::LATIN_CAPITAL;
				else if (GetValueName(EValue::LatinSmall, eType) == sType)
					m_eNumShape = ENumberShape2::LATIN_SMALL;
				else if (GetValueName(EValue::CircledLatinCapital, eType) == sType)
					m_eNumShape = ENumberShape2::CIRCLED_LATIN_CAPITAL;
				else if (GetValueName(EValue::CircledLatinSmall, eType) == sType)
					m_eNumShape = ENumberShape2::CIRCLED_LATIN_SMALL;
				else if (GetValueName(EValue::CircledHangulSyllable, eType) == sType)
					m_eNumShape = ENumberShape2::CIRCLED_HANGUL_SYLLABLE;
				else if (GetValueName(EValue::HangulJamo, eType) == sType)
					m_eNumShape = ENumberShape2::HANGUL_JAMO;
				else if (GetValueName(EValue::CircledHangulJamo, eType) == sType)
					m_eNumShape = ENumberShape2::CIRCLED_HANGUL_JAMO;
				else if (GetValueName(EValue::HangulPhonetic, eType) == sType)
					m_eNumShape = ENumberShape2::HANGUL_PHONETIC;
				else if (GetValueName(EValue::Ideograph, eType) == sType)
					m_eNumShape = ENumberShape2::IDEOGRAPH;
				else if (GetValueName(EValue::CircledIdeograph, eType) == sType)
					m_eNumShape = ENumberShape2::CIRCLED_IDEOGRAPH;
				else if (GetValueName(EValue::DecagonCircle, eType) == sType)
					m_eNumShape = ENumberShape2::DECAGON_CIRCLE;
				else if (GetValueName(EValue::DecagonCircleHanja, eType) == sType)
					m_eNumShape = ENumberShape2::DECAGON_CRICLE_HANGJA;
				else if (GetValueName(EValue::Symbol, eType) == sType)
					m_eNumShape = ENumberShape2::SYMBOL;
				else if (GetValueName(EValue::UserChar, eType) == sType)
					m_eNumShape = ENumberShape2::USER_HWP_CHAR;
			}
		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE

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
