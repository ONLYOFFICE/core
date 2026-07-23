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

CCtrlPageNumPos::CCtrlPageNumPos(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
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
