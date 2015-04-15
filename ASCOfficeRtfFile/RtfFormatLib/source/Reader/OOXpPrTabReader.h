#pragma once
#include "OOXReaderBasic.h"
#include "../RtfProperty.h"

class OOXpPrTabReader
{
private:
	OOX::Logic::CTabs *m_ooxTabs;
public: 
	OOXpPrTabReader( OOX::Logic::CTabs *ooxTabs)
	{
		m_ooxTabs = ooxTabs;
	}
	bool Parse( ReaderParameter oParam ,RtfTabs& oOutputProperty)
	{
		if (m_ooxTabs == NULL) return false;

		oOutputProperty.SetDefault();

		for( int i = 0; i < m_ooxTabs->m_arrTabs.size() ; i++ )
		{
			if (m_ooxTabs->m_arrTabs[i] == NULL) continue;

			RtfTab oCurTab;

			if (m_ooxTabs->m_arrTabs[i]->m_oLeader.IsInit())
			{
				switch(m_ooxTabs->m_arrTabs[i]->m_oLeader->GetValue())
				{
					case SimpleTypes::tabtlcDot        : oCurTab.m_eLeader = RtfTab::tl_dot;	break;
					case SimpleTypes::tabtlcHeavy      : break;
					case SimpleTypes::tabtlcHyphen     : oCurTab.m_eLeader = RtfTab::tl_hyph;	break;
					case SimpleTypes::tabtlcMiddleDot  : oCurTab.m_eLeader = RtfTab::tl_mdot;	break;
					case SimpleTypes::tabtlcNone       : break;
					case SimpleTypes::tabtlcUnderscore : oCurTab.m_eLeader = RtfTab::tl_ul;		break;
				}
			}
			if (m_ooxTabs->m_arrTabs[i]->m_oVal.IsInit())
			{
				switch(m_ooxTabs->m_arrTabs[i]->m_oVal->GetValue())
				{
					case SimpleTypes::tabjcBar     : oCurTab.m_eKind = RtfTab::tk_tqbar;	break;
					case SimpleTypes::tabjcCenter  : oCurTab.m_eKind = RtfTab::tk_tqc;		break;
					case SimpleTypes::tabjcClear   : break;
					case SimpleTypes::tabjcDecimal : oCurTab.m_eKind = RtfTab::tk_tqdec;	break;
					case SimpleTypes::tabjcEnd     : oCurTab.m_eKind = RtfTab::tk_tqr;		break;
					case SimpleTypes::tabjcNum     : oCurTab.m_eKind = RtfTab::tk_tqdec; 	break;//??
					case SimpleTypes::tabjcStart   : oCurTab.m_eKind = RtfTab::tk_tql;		break;
					case SimpleTypes::tabjcRight   : oCurTab.m_eKind = RtfTab::tk_tqr;		break;
					case SimpleTypes::tabjcLeft    : oCurTab.m_eKind = RtfTab::tk_tql;		break;	
				}
			}
			if (m_ooxTabs->m_arrTabs[i]->m_oPos.IsInit())
				oCurTab.m_nTab = m_ooxTabs->m_arrTabs[i]->m_oPos->ToTwips();
			
			oOutputProperty.m_aTabs.push_back( oCurTab );
		}
		return true;
	}
};
