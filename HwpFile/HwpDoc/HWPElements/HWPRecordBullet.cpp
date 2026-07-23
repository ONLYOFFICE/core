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

#include "HWPRecordBullet.h"

namespace HWP
{
CHWPRecordBullet::CHWPRecordBullet(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	int nTypeBits;
	oBuffer.ReadInt(nTypeBits);

	m_oHeaderInfo.m_chAlign = (HWP_BYTE)((nTypeBits) & 0x03);
	m_oHeaderInfo.m_bUseInstWidth = CHECK_FLAG(nTypeBits, 0x40);
	m_oHeaderInfo.m_bAutoIndent = CHECK_FLAG(nTypeBits, 0x80);
	m_oHeaderInfo.m_chTextOffsetType = (HWP_BYTE)((nTypeBits >> 4) & 0x01);

	oBuffer.ReadShort(m_oHeaderInfo.m_shWidthAdjust);
	oBuffer.ReadShort(m_oHeaderInfo.m_shTextOffset);
	oBuffer.ReadInt(m_oHeaderInfo.m_nCharShape);

	oBuffer.ReadChar(m_chBulletChar);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadInt(m_nBulletImage);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chBright);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chContrast);

	if (nSize > oBuffer.GetDistanceToLastPos())
		oBuffer.ReadByte(m_chImageEffect);

	if (nSize > oBuffer.GetDistanceToLastPos())
	{
		short shValue;
		oBuffer.ReadShort(shValue);

		m_sBinItemRefID = TO_HWP_STRING(shValue);
	}

	if (nSize > oBuffer.GetDistanceToLastPos(true))
		oBuffer.ReadChar(m_chCheckBulletChar);
}

CHWPRecordBullet::CHWPRecordBullet(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_BULLET, 0, 0), m_pParent(&oDocInfo)
{
	if (EHanType::HWPML == eType)
		return; // TODO:: Implement when example is encountered
	
	START_READ_ATTRIBUTES(oReader)
	{
		if ("char" == sAttributeName)
		{
			const HWP_STRING wsValue{oReader.GetText()};

			if (!wsValue.empty())
				m_chBulletChar = wsValue.at(0);
		}
		else if ("checkedChar" == sAttributeName)
		{
			const HWP_STRING wsValue{oReader.GetText()};

			if (!wsValue.empty())
				m_chCheckBulletChar = wsValue.at(0);
		}
		else if ("useImage" == sAttributeName)
			m_nBulletImage = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hc:img")
		m_sBinItemRefID = oReader.GetAttribute("binaryItemIDRef");
	END_WHILE
}
}
