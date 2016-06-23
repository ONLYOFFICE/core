#pragma once
#include "OOXColorReader.h"
#include "../RtfProperty.h"

class OOXColorSchemeReader
{
private:
	OOX::Drawing::CColorScheme * m_ooxColorScheme;
public: 
	OOXColorSchemeReader(OOX::Drawing::CColorScheme * ooxColorScheme)
	{
		m_ooxColorScheme = ooxColorScheme;
	}

	bool Parse( ReaderParameter oParam )
	{
		if (m_ooxColorScheme == NULL) return false;

		RtfColor		oNewColor; 
		OOXColorReader	oColorReader;

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent1, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentone;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent2, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccenttwo;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent3, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentthree;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );
		
		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent4, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentfour;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent5, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentfive;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oAccent6, oNewColor);
			oNewColor.m_eTheme = RtfColor::caccentsix;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oDk1, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmaindarkone;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );
		
		oColorReader.Parse( oParam, m_ooxColorScheme->m_oDk2, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmaindarktwo;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oLt1, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmainlightone;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oLt2, oNewColor);
			oNewColor.m_eTheme = RtfColor::cmainlighttwo;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oHlink, oNewColor);
			oNewColor.m_eTheme = RtfColor::chyperlink;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		oColorReader.Parse( oParam, m_ooxColorScheme->m_oFolHlink, oNewColor);
			oNewColor.m_eTheme = RtfColor::cfollowedhyperlink;
			oParam.oRtf->m_oColorTable.AddItem( oNewColor );

		//else if( _T("a:bg1") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::cbackgroundone;
		//else if( _T("a:bg2") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::cbackgroundtwo;
		//else if( _T("a:phClr") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::cmainlighttwo;
		//else if( _T("a:tx1") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::ctextone;
		//else if( _T("a:tx2") == sNodeName )
		//	oNewColor.m_eTheme = RtfColor::ctexttwo;


		return true;
	}
};