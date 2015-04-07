#pragma once
#include "../RtfDocument.h"

#include "../RtfProperty.h"
#include "../Utils.h"

class OOXLevelReader
{
	OOX::Numbering::CLvl* m_ooxLvlNum;
public: 
	OOXLevelReader(OOX::Numbering::CLvl* ooxLvlNum)
	{
		m_ooxLvlNum = ooxLvlNum;
	}
	bool Parse( ReaderParameter oParam, RtfListLevelProperty& oLevelProperty )
	{
		if (m_ooxLvlNum == NULL) return false;

		if (m_ooxLvlNum->m_oIlvl.IsInit())
			oLevelProperty.m_nLevel = m_ooxLvlNum->m_oIlvl->GetValue();
		
		if (m_ooxLvlNum->m_oTentative.IsInit())
			oLevelProperty.m_bTentative = m_ooxLvlNum->m_oTentative->ToBool();

		if (m_ooxLvlNum->m_oLvlJc.IsInit() && m_ooxLvlNum->m_oLvlJc->m_oVal.IsInit())
		{
			switch(m_ooxLvlNum->m_oLvlJc->m_oVal->GetValue())
			{
			case SimpleTypes::jcCenter          : oLevelProperty.m_nJustification = 1; break;
			case SimpleTypes::jcEnd             : oLevelProperty.m_nJustification = 2; break;
			case SimpleTypes::jcStart           : oLevelProperty.m_nJustification = 0; break;
			case SimpleTypes::jcLeft            : oLevelProperty.m_nJustification = 0; break;
			case SimpleTypes::jcRight           : oLevelProperty.m_nJustification = 2; break;
			}
		}
		if (m_ooxLvlNum->m_oLvlRestart.IsInit() && m_ooxLvlNum->m_oLvlRestart->m_oVal.IsInit())
		{
			int nVal = m_ooxLvlNum->m_oLvlRestart->m_oVal->GetValue();
			if( 0 == nVal )
				oLevelProperty.m_nNoRestart = 0;
			//todooo непонятка что будет если рестарт списка с не "0"
		}
		if (m_ooxLvlNum->m_oIsLgl.IsInit() )
			oLevelProperty.m_nLegal = m_ooxLvlNum->m_oIsLgl->m_oVal.ToBool();

		if (m_ooxLvlNum->m_oLvlText.IsInit() && m_ooxLvlNum->m_oLvlText->m_sVal.IsInit())
		{
			oLevelProperty.SetLevelTextOOX( m_ooxLvlNum->m_oLvlText->m_sVal.get2() );
				//null???? todooo
		}
		if (m_ooxLvlNum->m_oNumFmt.IsInit() && m_ooxLvlNum->m_oNumFmt->m_oVal.IsInit())
		{
			oLevelProperty.m_nNumberType = oLevelProperty.GetFormat( m_ooxLvlNum->m_oNumFmt->m_oVal->ToString() );
		}
		
		if (m_ooxLvlNum->m_oLvlPicBulletId.IsInit() && m_ooxLvlNum->m_oLvlPicBulletId->m_oVal.IsInit())
		{
			int nID = m_ooxLvlNum->m_oLvlPicBulletId->m_oVal->GetValue();
			std::map<int, int>::iterator poPair =  oParam.oReader->m_mapPictureBullet.find( nID );
			
			if( oParam.oReader->m_mapPictureBullet.end() != poPair )
			{
				oLevelProperty.m_nPictureIndex = poPair->second;
			}
		}

		if (m_ooxLvlNum->m_oStart.IsInit() && m_ooxLvlNum->m_oStart->m_oVal.IsInit())
			oLevelProperty.m_nStart = m_ooxLvlNum->m_oStart->m_oVal->GetValue();

		if (m_ooxLvlNum->m_oSuffix.IsInit() && m_ooxLvlNum->m_oSuffix->m_oVal.IsInit())
		{
            switch(m_ooxLvlNum->m_oSuffix->m_oVal->GetValue())
			{
			case SimpleTypes::levelsuffixSpace   : oLevelProperty.m_nFollow = 1; break;
			case SimpleTypes::levelsuffixTab     : oLevelProperty.m_nFollow = 0; break;
			case SimpleTypes::levelsuffixNothing : oLevelProperty.m_nFollow = 2; break;
			}
		}		

		if (m_ooxLvlNum->m_oRPr.IsInit())
		{
			RtfCharProperty oNewProperty;
			oNewProperty.SetDefaultOOX();
			
			OOXrPrReader orPrReader(m_ooxLvlNum->m_oRPr.GetPointer());
			orPrReader.m_bDefStyle = false;
			
			if( true == orPrReader.Parse( oParam, oNewProperty ) )
				oLevelProperty.m_oCharProp = oNewProperty;
		}

		if (m_ooxLvlNum->m_oPPr.IsInit())
		{
			RtfParagraphProperty oNewProperty;
			OOXpPrReader opPrReader(m_ooxLvlNum->m_oPPr.GetPointer());
			bool bStartNewSectoion;
			
			if( true == opPrReader.Parse( oParam, oNewProperty, CcnfStyle(), bStartNewSectoion ) )
			{
				oLevelProperty.m_nFirstIndent	= oNewProperty.m_nIndFirstLine;
				oLevelProperty.m_nIndent		= oNewProperty.m_nIndLeft;
				oLevelProperty.m_nIndentStart	= oNewProperty.m_nIndStart;
				oLevelProperty.m_oTabs			= oNewProperty.m_oTabs;
			}
		}
		return true;
	}
};
