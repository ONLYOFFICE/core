#pragma once

#include "../RtfDocument.h"
#include "OOXReader.h"

class OOXAppReader
{
private: 
	OOX::CApp *m_ooxApp;
public: 
	OOXAppReader(OOX::CApp *ooxApp)
	{
		m_ooxApp = ooxApp;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxApp == NULL) return false;

		if(m_ooxApp->m_nTotalTime.IsInit())
		{
			oParam.oRtf->m_oInformation.m_nEndingTime = m_ooxApp->m_nTotalTime.get2();
		}
		if(m_ooxApp->m_nPages.IsInit())
		{
			oParam.oRtf->m_oInformation.m_nNumberOfPages = m_ooxApp->m_nPages.get2();
		}
		if(m_ooxApp->m_nWords.IsInit())
		{
			oParam.oRtf->m_oInformation.m_nNumberOfWords = m_ooxApp->m_nWords.get2();
		}
		if(m_ooxApp->m_nCharacters.IsInit())
		{
			oParam.oRtf->m_oInformation.m_nNumberOfCharactersWithoutSpace = m_ooxApp->m_nCharacters.get2();
		}
		if(m_ooxApp->m_nCharactersWithSpaces.IsInit())
		{
			oParam.oRtf->m_oInformation.m_nNumberOfCharactersWithSpace = m_ooxApp->m_nCharactersWithSpaces.get2();
		}
		if(m_ooxApp->m_sAppVersion.IsInit())
		{
			oParam.oRtf->m_oInformation.m_nVersion= Strings::ToInteger( m_ooxApp->m_sAppVersion.get2() );
		}		

		return true;
	}
};
