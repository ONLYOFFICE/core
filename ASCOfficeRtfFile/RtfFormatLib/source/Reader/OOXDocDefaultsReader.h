#pragma once
#include "OOXrPrReader.h"
#include "OOXpPrReader.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Styles.h"

class OOXDocDefaultsReader
{
private:
	OOX::CDocDefaults* m_ooxDefaults;
public: 

	OOXDocDefaultsReader(OOX::CDocDefaults* ooxDefaults)
	{
		m_ooxDefaults = ooxDefaults;
	}

	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxDefaults == NULL) return false;

		if (m_ooxDefaults->m_oRunPr.IsInit())
		{
			RtfCharProperty oNewProperty;
			oNewProperty.SetDefaultOOX();
			
			OOXrPrReader orPrReader(m_ooxDefaults->m_oRunPr.GetPointer());
			orPrReader.m_bDefStyle = false;
			
			if( true == orPrReader.Parse( oParam, oNewProperty ) )
				oParam.oRtf->m_oDefaultCharProp = oNewProperty;
		}
		if (m_ooxDefaults->m_oParPr.IsInit())
		{
			RtfParagraphProperty oNewProperty;
			OOXpPrReader opPrReader(m_ooxDefaults->m_oParPr.GetPointer());
			
			opPrReader.m_bDefStyle = false;
			
			bool bStartNewSectoion;
			
			if( true == opPrReader.Parse( oParam, oNewProperty, CcnfStyle(), bStartNewSectoion ) )
				oParam.oRtf->m_oDefaultParagraphProp = oNewProperty;
		}
		return true;
	}
};
