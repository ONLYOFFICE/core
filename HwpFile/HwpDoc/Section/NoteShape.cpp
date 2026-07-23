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

#include "NoteShape.h"

namespace HWP
{
ENoteNumbering GetNoteNumbering(int nValue)
{
	switch(static_cast<ENoteNumbering>(nValue))
	{
		case ENoteNumbering::CONTINUOUS: return ENoteNumbering::CONTINUOUS;
		case ENoteNumbering::ON_SECTION: return ENoteNumbering::ON_SECTION;
		case ENoteNumbering::ON_PAGE: return ENoteNumbering::ON_PAGE;
		default:
			return ENoteNumbering::UNKNOWN;
	}
}

CNoteShape::CNoteShape()
{}

CNoteShape::CNoteShape(CXMLReader& oReader, EHanType eType)
{
	//TODO:: Implement
}

CNoteShape* CNoteShape::Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CNoteShape* pNoteShape = new CNoteShape();

	if (nullptr == pNoteShape)
	{
		oBuffer.Skip(nSize);
		return nullptr;
	}

	oBuffer.SavePosition();

	pNoteShape->m_eNumberShape = GetNumberShape2(oBuffer.ReadByte());

	HWP_BYTE chAttr;
	oBuffer.ReadByte(chAttr);

	pNoteShape->m_chPlacement = (HWP_BYTE)(chAttr & 0x03);
	pNoteShape->m_eNumbering = GetNoteNumbering((chAttr >> 2) & 0x03);
	pNoteShape->m_bSuperscript = CHECK_FLAG(chAttr >> 4, 0x01);
	pNoteShape->m_bBeneathText = CHECK_FLAG(chAttr >> 5, 0x01);
	oBuffer.Skip(2);

	oBuffer.ReadChar(pNoteShape->m_chUserChar);
	oBuffer.ReadChar(pNoteShape->m_chPrefixChar);
	oBuffer.ReadChar(pNoteShape->m_chSuffixChar);
	oBuffer.ReadShort(pNoteShape->m_shNewNumber);
	oBuffer.ReadInt(pNoteShape->m_nNoteLineLength);
	oBuffer.ReadShort(pNoteShape->m_shSpacingAboveLine);
	oBuffer.ReadShort(pNoteShape->m_shSpacingBelowLine);
	oBuffer.ReadShort(pNoteShape->m_shSpacingBetweenNotes);

	pNoteShape->m_eNoteLineType = GetLineStyle1(oBuffer.ReadByte());
	oBuffer.ReadByte(pNoteShape->m_chNoteLineWidth);
	oBuffer.ReadColor(pNoteShape->m_nNoteLineColor);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return pNoteShape;
}


}
