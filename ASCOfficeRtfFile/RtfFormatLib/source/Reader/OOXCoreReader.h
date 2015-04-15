#pragma once

#include "../RtfDocument.h"
#include "OOXReader.h"

class OOXCoreReader
{
private: 
	OOX::CCore * m_ooxCore;
public: 
	OOXCoreReader(OOX::CCore * ooxCore)
	{
		m_ooxCore = ooxCore;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxCore == NULL) return false;

		if (m_ooxCore->m_sTitle.IsInit())
		{
			oParam.oRtf->m_oInformation.m_sTitle = m_ooxCore->m_sTitle.get2();
		}
		if (m_ooxCore->m_sSubject.IsInit())
		{
			oParam.oRtf->m_oInformation.m_sSubject = m_ooxCore->m_sSubject.get2();
		}
		if (m_ooxCore->m_sCreator.IsInit())
		{
			oParam.oRtf->m_oInformation.m_sAuthor = m_ooxCore->m_sCreator.get2();
		}
		if (m_ooxCore->m_sDescription.IsInit())
		{
			oParam.oRtf->m_oInformation.m_sComment = m_ooxCore->m_sDescription.get2();
		}
		if (m_ooxCore->m_sKeywords.IsInit())
		{
			oParam.oRtf->m_oInformation.m_sKeywords = m_ooxCore->m_sKeywords.get2();
		}
		return true;
	}

};
