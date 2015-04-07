#pragma once
#include "OOXFontReader.h"
#include "../RtfProperty.h"


class OOXFontSchemeReader
{
private:
	OOX::Drawing::CFontScheme * m_ooxFontScheme;
public: 
	OOXFontSchemeReader(OOX::Drawing::CFontScheme * ooxFontScheme)
	{
		m_ooxFontScheme = ooxFontScheme;
	}
	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxFontScheme == NULL) return false;
		
		if (m_ooxFontScheme->m_oMajorFont.m_oLatin.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_smajorAscii = m_ooxFontScheme->m_oMajorFont.m_oLatin.m_oTypeFace->GetValue();
			oParam.oReader->m_smajorHAnsi = m_ooxFontScheme->m_oMajorFont.m_oLatin.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMajorFont.m_oEa.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_smajorEastAsia = m_ooxFontScheme->m_oMajorFont.m_oEa.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMajorFont.m_oCs.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_smajorBidi = m_ooxFontScheme->m_oMajorFont.m_oCs.m_oTypeFace->GetValue();
		}
		//for (long i=0 ; i < m_ooxFontScheme->m_oMajorFont.m_arrFont.size(); i++)
		//{
		//}

		if (m_ooxFontScheme->m_oMinorFont.m_oLatin.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_sminorAscii = m_ooxFontScheme->m_oMinorFont.m_oLatin.m_oTypeFace->GetValue();
			oParam.oReader->m_sminorHAnsi = m_ooxFontScheme->m_oMinorFont.m_oLatin.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMinorFont.m_oEa.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_sminorEastAsia = m_ooxFontScheme->m_oMinorFont.m_oEa.m_oTypeFace->GetValue();
		}
		if (m_ooxFontScheme->m_oMinorFont.m_oCs.m_oTypeFace.IsInit())
		{
			oParam.oReader->m_sminorBidi = m_ooxFontScheme->m_oMinorFont.m_oCs.m_oTypeFace->GetValue();
		}
		//for (long i=0 ; i < m_ooxFontScheme->m_oMinorFont.m_arrFont.size(); i++)
		//{
		//}


		return true;
	}
};
