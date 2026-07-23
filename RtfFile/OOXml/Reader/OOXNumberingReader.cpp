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

#include "OOXReader.h"
#include "OOXNumberingReader.h"

OOXNumberingReader::OOXNumberingReader( OOX::CNumbering* ooxNumbering)
{
	m_ooxNumbering = ooxNumbering;
}
bool OOXNumberingReader::Parse( ReaderParameter oParam )
{
	if (m_ooxNumbering == NULL) return false;

	for (size_t i=0; i < m_ooxNumbering->m_arrNumPicBullet.size(); i++)
	{
		if (m_ooxNumbering->m_arrNumPicBullet[i] == NULL) continue;
		if (false == m_ooxNumbering->m_arrNumPicBullet[i]->m_oNumPicBulletId.IsInit()) continue;

		int nID = *m_ooxNumbering->m_arrNumPicBullet[i]->m_oNumPicBulletId;

		if (m_ooxNumbering->m_arrNumPicBullet[i]->m_oVmlDrawing.IsInit())
		{
			OOXShapeReader oShapeReader(m_ooxNumbering->m_arrNumPicBullet[i]->m_oVmlDrawing.GetPointer());

			RtfShapePtr oNewShape ( new RtfShape() );
			if( true == oShapeReader.Parse( oParam, oNewShape ) )
			{
				oNewShape->m_eAnchorTypeShape	= RtfShape::st_inline;
				oNewShape->m_nPositionHRelative = 3;
				oNewShape->m_nPositionVRelative = 3;

				int nIndex = oParam.oRtf->m_oListTable.m_aPictureList.GetCount();
				oParam.oReader->m_mapPictureBullet[nID] = nIndex;
				oParam.oRtf->m_oListTable.m_aPictureList.AddItem( oNewShape );
			}
		}
		if (m_ooxNumbering->m_arrNumPicBullet[i]->m_oDrawing.IsInit())
		{
			//???? todooo
		}
	}

	for (size_t i = 0; i < m_ooxNumbering->m_arrAbstractNum.size(); i++)
	{
		OOXAbstractNumReader oParagraph(m_ooxNumbering->m_arrAbstractNum[i]);
		oParagraph.Parse( oParam );
	}
	for (size_t i = 0; i < m_ooxNumbering->m_arrNum.size(); i++)
	{
		OOXNumberingMapReader oParagraph(m_ooxNumbering->m_arrNum[i]);
		oParagraph.Parse( oParam );
	}


	return true;
}
