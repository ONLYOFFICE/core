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

#include "OOXDocDefaultsReader.h"
#include "OOXStyleReader.h"

#include "OOXStyleTableReader.h"

OOXStyleTableReader::OOXStyleTableReader(OOX::CStyles* ooxStyles)
{
	m_ooxStyles = ooxStyles;
}

bool OOXStyleTableReader::Parse( ReaderParameter oParam )
{
	if (m_ooxStyles == NULL) return false;

	if (m_ooxStyles->m_oDocDefaults.IsInit())
	{
		OOXDocDefaultsReader oDocDefaultsReader(m_ooxStyles->m_oDocDefaults.GetPointer());
		oDocDefaultsReader.Parse( oParam );
	}

	for (size_t i = 0; i< m_ooxStyles->m_arrStyle.size(); i++)
	{
		RtfStylePtr oNewStyle;
		OOXStyleReader oStyleReader(m_ooxStyles->m_arrStyle[i]);

		oStyleReader.ParseStyle( oParam, oNewStyle);

		if( NULL != oNewStyle )
			oParam.oRtf->m_oStyleTable.AddItem( oNewStyle );
	}
	//only to add links between styles

	for (size_t i=0; i< m_ooxStyles->m_arrStyle.size(); i++)
	{
		OOXStyleReader oStyleReader(m_ooxStyles->m_arrStyle[i]);
		oStyleReader.ParseRelations( oParam );
	}
	return true;
}
