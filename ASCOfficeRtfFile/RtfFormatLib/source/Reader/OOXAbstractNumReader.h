#pragma once
#include "OOXLevelReader.h"


class OOXAbstractNumReader
{
private:
	OOX::Numbering::CAbstractNum  *m_ooxAbstractNum;
public: 
	OOXAbstractNumReader(OOX::Numbering::CAbstractNum  *ooxAbstractNum)
	{
		m_ooxAbstractNum = ooxAbstractNum;
	}
	
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxAbstractNum == NULL) return false;

		RtfListProperty oNewList;
		
		if (m_ooxAbstractNum->m_oAbstractNumId.IsInit())
			oNewList.m_nID = m_ooxAbstractNum->m_oAbstractNumId->GetValue();

		if (m_ooxAbstractNum->m_oMultiLevelType.IsInit())
		{
            switch(m_ooxAbstractNum->m_oMultiLevelType->m_oVal->GetValue())
			{
			case SimpleTypes::emultileveltypeHybridMultilevel : 	oNewList.m_bListHybrid = 1; break;
			case SimpleTypes::emultileveltypeSingleLevel      : 	oNewList.m_nListSimple = 1; break;
			case SimpleTypes::emultileveltypeMultilevel       : 	
			default											  :		break;
			}					
		}
		if (m_ooxAbstractNum->m_oName.IsInit())
			oNewList.m_sName = m_ooxAbstractNum->m_oName->m_sVal.get2();

		if (m_ooxAbstractNum->m_oTmpl.IsInit() && m_ooxAbstractNum->m_oTmpl->m_oVal.IsInit())
			oNewList.m_nTemplateId = m_ooxAbstractNum->m_oTmpl->m_oVal->GetValue();

		for (long i=0 ; i < m_ooxAbstractNum->m_arrLvl.size(); i++)
		{
			OOXLevelReader oLevelReader(m_ooxAbstractNum->m_arrLvl[i]);
			RtfListLevelProperty oNewLevel;
			
			if( true == oLevelReader.Parse( oParam, oNewLevel) )
					oNewList.AddItem( oNewLevel );
		}
		oParam.oRtf->m_oListTabel.AddItem( oNewList );

		return true;
	}
};