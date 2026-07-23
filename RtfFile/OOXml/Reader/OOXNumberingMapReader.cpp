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

#include "OOXNumberingMapReader.h"

OOXNumberingMapReader::OOXNumberingMapReader(OOX::Numbering::CNum *ooxNum)
{
	m_ooxNum = ooxNum;
}
bool OOXNumberingMapReader::Parse( ReaderParameter oParam )
{
	if (m_ooxNum == NULL) return false;

	RtfListOverrideProperty oNewOverride;
	if (m_ooxNum->m_oNumId.IsInit())
		oNewOverride.m_nIndex = *m_ooxNum->m_oNumId;

	if (m_ooxNum->m_oAbstractNumId.IsInit() && m_ooxNum->m_oAbstractNumId->m_oVal.IsInit())
		oNewOverride.m_nListID = *m_ooxNum->m_oAbstractNumId->m_oVal;

	for (size_t i = 0 ; i < m_ooxNum->m_arrLvlOverride.size(); i++)
	{
		if (m_ooxNum->m_arrLvlOverride[i] == NULL) continue;

		RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel oOverrideLevel;

		if (m_ooxNum->m_arrLvlOverride[i]->m_oIlvl.IsInit())
			oOverrideLevel.m_nLevelIndex = *m_ooxNum->m_arrLvlOverride[i]->m_oIlvl;
		else
			oOverrideLevel.m_nLevelIndex = 0;

		if (m_ooxNum->m_arrLvlOverride[i]->m_oStartOverride.IsInit() && m_ooxNum->m_arrLvlOverride[i]->m_oStartOverride->m_oVal.IsInit())
		{
			oOverrideLevel.m_nStart = *m_ooxNum->m_arrLvlOverride[i]->m_oStartOverride->m_oVal;
		}

		if (m_ooxNum->m_arrLvlOverride[i]->m_oLvl.IsInit())
		{
			OOXLevelReader oLevelReader(m_ooxNum->m_arrLvlOverride[i]->m_oLvl.GetPointer());
			oLevelReader.Parse( oParam, oOverrideLevel.m_oLevel );
		}

		oNewOverride.m_oOverrideLevels.m_aOverrideLevels.push_back( oOverrideLevel );
	}
	oParam.oRtf->m_oListOverrideTable.AddItem( oNewOverride );

	return true;
}
