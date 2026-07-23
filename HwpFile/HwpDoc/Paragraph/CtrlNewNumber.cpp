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

#include "CtrlNewNumber.h"

namespace HWP
{
CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_eNumType = ::HWP::GetNumType(nAttr & 0xF);
	m_eNumShape = GetNumberShape2((nAttr >> 4) & 0xF);

	oBuffer.ReadShort(m_shNum);

	m_bFullFilled = true;
}

CCtrlNewNumber::CCtrlNewNumber(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
	: CCtrl(sCtrlID)
{
	//TODO:: Verify when encountered in file

	START_READ_ATTRIBUTES(oReader)
	{
		if ("num" == sAttributeName)
			m_shNum = oReader.GetInt();
		else if ("numType" == sAttributeName)
			m_eNumType = ::HWP::GetNumType(oReader.GetTextA(), eType);
		//TODO:: Verify this behavior
		else if ("autoNumFormat" == sAttributeName)
			m_eNumShape = GetNumberShape2(oReader.GetInt());
	}
	END_READ_ATTRIBUTES(oReader)

	m_bFullFilled = true;
}

ECtrlObjectType CCtrlNewNumber::GetCtrlType() const
{
	return ECtrlObjectType::NewNumber;
}

ENumType CCtrlNewNumber::GetNumType() const
{
	return m_eNumType;
}

short CCtrlNewNumber::GetNum() const
{
	return m_shNum;
}
}
