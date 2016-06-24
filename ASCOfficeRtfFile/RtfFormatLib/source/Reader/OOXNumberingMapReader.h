/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "../RtfDocument.h"
#include "OOXLevelReader.h"

class OOXNumberingMapReader
{
private:
	OOX::Numbering::CNum *m_ooxNum;
public: 
	OOXNumberingMapReader(OOX::Numbering::CNum *ooxNum)
	{
		m_ooxNum = ooxNum;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxNum == NULL) return false;
			
		RtfListOverrideProperty oNewOverride;
		if (m_ooxNum->m_oNumId.IsInit())
			oNewOverride.m_nIndex = m_ooxNum->m_oNumId->GetValue();

		if (m_ooxNum->m_oAbstractNumId.IsInit() && m_ooxNum->m_oAbstractNumId->m_oVal.IsInit())
			oNewOverride.m_nListID = m_ooxNum->m_oAbstractNumId->m_oVal->GetValue();

		for (long i=0 ;i < m_ooxNum->m_arrLvlOverride.size(); i++)
		{
			if (m_ooxNum->m_arrLvlOverride[i] == NULL) continue;

			RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevel oOverrideLevel;
			
			if (m_ooxNum->m_arrLvlOverride[i]->m_oIlvl.IsInit())
				oOverrideLevel.m_nLevelIndex = m_ooxNum->m_arrLvlOverride[i]->m_oIlvl->GetValue();
			else
				oOverrideLevel.m_nLevelIndex = 0;	

			if (m_ooxNum->m_arrLvlOverride[i]->m_oStartOverride.IsInit() && m_ooxNum->m_arrLvlOverride[i]->m_oStartOverride->m_oVal.IsInit())
			{
				oOverrideLevel.m_nStart = m_ooxNum->m_arrLvlOverride[i]->m_oStartOverride->m_oVal->GetValue();
			}

			if (m_ooxNum->m_arrLvlOverride[i]->m_oLvl.IsInit())
			{
				OOXLevelReader oLevelReader(m_ooxNum->m_arrLvlOverride[i]->m_oLvl.GetPointer());
				oLevelReader.Parse( oParam, oOverrideLevel.m_oLevel );
			}

			oNewOverride.m_oOverrideLevels.m_aOverrideLevels.push_back( oOverrideLevel );
		}
		oParam.oRtf->m_oListOverrideTabel.AddItem( oNewOverride );

		return true;
	}
};
