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

#include "HWPRecordStyle.h"

#include "../Common/NodeNames.h"
#include <iostream>
namespace HWP
{
CHWPRecordStyle::CHWPRecordStyle(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr)
{}

CHWPRecordStyle::CHWPRecordStyle(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.ReadString(m_sName, EStringCharacter::UTF16);
	oBuffer.ReadString(m_sEngName, EStringCharacter::UTF16);

	m_chType = (HWP_BYTE)(oBuffer.ReadByte() & 0x00FF);
	m_chNextStyle = (HWP_BYTE)(oBuffer.ReadByte() & 0x00FF);

	oBuffer.ReadShort(m_shLangID);
	m_nParaShape = oBuffer.ReadShort();
	m_nCharShape = oBuffer.ReadShort();
}

CHWPRecordStyle::CHWPRecordStyle(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_STYLE, 0, 0), m_pParent(&oDocInfo)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if (GetValueName(EValue::Para, eType) == sType)
				m_chType = 0;
			else if (GetValueName(EValue::Char, eType) == sType)
				m_chType = 1;
		}
		else if (GetAttributeName(EAttribute::Name, eType) == sAttributeName)
			m_sName = oReader.GetText();
		else if (GetAttributeName(EAttribute::EngName, eType) == sAttributeName)
			m_sEngName = oReader.GetText();
		else if (GetAttributeName(EAttribute::ParaShape, eType) == sAttributeName)
			m_nParaShape = oReader.GetInt();
		else if (GetAttributeName(EAttribute::CharShape, eType) == sAttributeName)
			m_nCharShape = oReader.GetInt();
		else if (GetAttributeName(EAttribute::NextStyle, eType) == sAttributeName)
			m_chNextStyle = oReader.GetInt();
		else if (GetAttributeName(EAttribute::LangId, eType) == sAttributeName)
			m_shLangID = oReader.GetInt();
		else if (GetAttributeName(EAttribute::LockForm, eType) == sAttributeName)
			m_bLockForm = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)
}

HWP_STRING CHWPRecordStyle::GetName() const
{
	return m_sName;
}

HWP_STRING CHWPRecordStyle::GetEngName() const
{
	return m_sEngName;
}

HWP_BYTE CHWPRecordStyle::GetType() const
{
	return m_chType;
}

HWP_BYTE CHWPRecordStyle::GetNextStyle() const
{
	return m_chNextStyle;
}

short CHWPRecordStyle::GetLangId() const
{
	return m_shLangID;
}

int CHWPRecordStyle::GetParaShapeId() const
{
	return m_nParaShape;
}

int CHWPRecordStyle::GetCharShapeId() const
{
	return m_nCharShape;
}

bool CHWPRecordStyle::LockForm() const
{
	return m_bLockForm;
}
}
