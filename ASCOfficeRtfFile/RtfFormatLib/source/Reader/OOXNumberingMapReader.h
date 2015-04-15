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
