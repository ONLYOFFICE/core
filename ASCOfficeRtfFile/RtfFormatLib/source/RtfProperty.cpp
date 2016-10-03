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
#include "RtfWriter.h"
#include "Writer/OOXWriter.h"
#include "RtfProperty.h"
#include "RtfDocument.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/FileTypes.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/App.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Core.h"

CString RtfFont::RenderToRtf(RenderParameter oRenderParameter)
{
	if ( IsValid() == false) return L"";

	CString sResult;
	if( RENDER_TO_RTF_PARAM_FONT_TBL == oRenderParameter.nType )
	{
		sResult += _T("{");
		switch( m_eFontTheme )
		{
			case ft_flomajor:	sResult += _T("\\flomajor");	break;
			case ft_fhimajor:	sResult += _T("\\fhimajor");	break;
			case ft_fdbmajor:	sResult += _T("\\fdbmajor");	break;
			case ft_fbimajor:	sResult += _T("\\fbimajor");	break;
			case ft_flominor:	sResult += _T("\\flominor");	break;
			case ft_fhiminor:	sResult += _T("\\fhiminor");	break;
			case ft_fdbminor:	sResult += _T("\\fdbminor");	break;
			case ft_fbiminor:	sResult += _T("\\fbiminor");	break;
		}
		sResult.AppendFormat(_T("\\f%d"),m_nID);

		switch( m_eFontFamily )
		{
			case ff_fnil:		sResult += _T("\\fnil");	break;
			case ff_froman:		sResult += _T("\\froman");	break;
			case ff_fswiss:		sResult += _T("\\fswiss");	break;
			case ff_fmodern:	sResult += _T("\\fmodern");	break;
			case ff_fscript:	sResult += _T("\\fscript");	break;
			case ff_fdecor:		sResult += _T("\\fdecor");	break;
			case ff_ftech:		sResult += _T("\\ftech");	break;
			case ff_fbidi:		sResult += _T("\\fbidi");	break;
		}
		RENDER_RTF_INT( m_nCharset, sResult, _T("fcharset") )
		RENDER_RTF_INT( m_nPitch, sResult, _T("fprq") )

		if( _T("") != m_sPanose )
            sResult += _T("{\\*\\panose ") + m_sPanose + _T("}");
		
		RENDER_RTF_INT( m_nCodePage, sResult, _T("cpg") )

		RtfCharProperty oFontNameCharProp;
        sResult += _T(" ") + RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, &oFontNameCharProp );
		
//ALL FONTS NEW.docx
//		if(!m_sAltName.IsEmpty() )
//          sResult += _T("{\\*\\falt ") + RtfChar::renderRtfText( m_sAltName, oRenderParameter.poDocument, &oFontNameCharProp ) + _T("}");

		sResult += _T(";}");
	}
	else
		sResult.AppendFormat(_T("\\f%d"),m_nID);
	return sResult;
}

CString RtfFont::RenderToOOX(RenderParameter oRenderParameter)
{
	if( IsValid() == false) return L"";

	CString sResult;
	
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	CString sFontName = m_sName;

	if ((sFontName.GetLength() > 0 ) && (sFontName[0] == 0x00b9 || sFontName[0] > 0xff00) )//fondj.rtf
	{
		if (m_sAltName.GetLength() > 0) sFontName = m_sAltName;
		else sFontName.Empty();
	}
	if( sFontName.IsEmpty() )
	{
		if( PROP_DEF != poRtfDocument->m_oProperty.m_nDeffFont )
		{
			RtfFont oDefFont;
			poRtfDocument->m_oFontTable.GetFont( poRtfDocument->m_oProperty.m_nDeffFont, oDefFont );
			sFontName = oDefFont.m_sName;
		}
		if( sFontName.IsEmpty())
			sFontName = _T("Arial");
	}
	if( RENDER_TO_OOX_PARAM_MINOR_FONT == oRenderParameter.nType )
	{
		CString sTag;
		switch(m_eFontTheme)
		{
			case ft_flominor: 
			case ft_fhiminor: sTag = _T("latin");	break;
			case ft_fdbminor: sTag = _T("ea");		break;
			case ft_fbiminor: sTag = _T("cs");		break;
		}
		if( !sTag.IsEmpty() )
		{
            sResult += _T("<") ;
			sResult += sTag;
			sResult += _T(" typeface=\"");
			sResult += Utils::PrepareToXML( sFontName );
			sResult += _T("\"/>");
		}
	}
	else if( RENDER_TO_OOX_PARAM_MAJOR_FONT == oRenderParameter.nType )
	{
		CString sTag;
		switch(m_eFontTheme)
		{
			case ft_flomajor: 
			case ft_fhiminor: sTag = _T("latin");break;
			case ft_fdbmajor: sTag = _T("ea");break;
			case ft_fbimajor: sTag = _T("cs");break;
		}
		if( !sTag.IsEmpty() )
		{
			sResult += _T("<");
			sResult += sTag;
            sResult += _T(" typeface=\"");
			sResult += Utils::PrepareToXML( sFontName );
			sResult += _T("\"/>");
		}
	}
	else if( RENDER_TO_OOX_PARAM_FONTTABLE == oRenderParameter.nType )
	{
        sResult += _T("<w:font w:name=\"");
		sResult += Utils::PrepareToXML( sFontName );
		sResult += _T("\">");
		
		if( !m_sAltName.IsEmpty() )
		{
            sResult += _T("<w:altName w:val=\"");
			sResult += Utils::PrepareToXML( m_sAltName );
			sResult += _T("\" />");
		}
		if( !m_sPanose.IsEmpty() )
		{
            sResult += _T("<w:panose1 w:val=\"");
			sResult += m_sPanose;
			sResult += _T("\" />");
		}

		CString sFamily = _T("");
		switch( m_eFontFamily )
		{
			case ff_fnil:		sFamily = _T("auto");		break;
			case ff_froman:		sFamily = _T("roman");		break;
			case ff_fswiss:		sFamily = _T("swiss");		break;
			case ff_fmodern:	sFamily = _T("modern");		break;
			case ff_fscript:	sFamily = _T("script");		break;
			case ff_fdecor:		sFamily = _T("decorative");	break;
			case ff_ftech:		sFamily = _T("auto");		break;
			case ff_fbidi:		sFamily = _T("auto");		break;
		}
		if( !sFamily.IsEmpty() )
		{
            sResult += _T("<w:family w:val=\"");
			sResult += sFamily;
			sResult += _T("\" />");
		}
		if( PROP_DEF != m_nCharset )
		{
            sResult += _T("<w:charset w:val=\"");
			sResult += Convert::ToStringHex( m_nCharset, 2 );
			sResult += _T("\" />");
		}

		if( PROP_DEF != m_nPitch )
		{
			CString sPitch;
			switch( m_nPitch )
			{
				case 0: sPitch = _T("default");	break;
				case 1: sPitch = _T("fixed");	break;
				case 2: sPitch = _T("variable");break;
			}
            sResult += _T("<w:pitch w:val=\"");
			sResult += sPitch;
			sResult += _T("\"/>");
		}

		//важно
		sResult += _T("</w:font>");
	}
	else
	{
		RtfFont oCurFont;
		if( true == poRtfDocument->m_oFontTable.GetFont(m_nID,oCurFont) )
		{
			CString sHint;
			switch(m_nCharset)
			{
				case 128://Japanese
				case 129://Korean
				case 130://Korean
				case 134://China
				case 136://China
				case 163://Vietnamese
				case 222://Thai
				{
					sHint = _T(" w:hint=\"eastAsia\"/>");
				}break;
				case 177://Hebrew
				case 178://Arabic
				case 179://Arabic
				case 180://Arabic
				case 181://Hebrew
				{
					sHint = _T(" w:hint=\"cs\"/>");
				}break;
				//?? нужно ли описывать default??? todooo
			}
			sResult += _T("<w:rFonts w:ascii=\"");
			sResult += sFontName;
			sResult += _T("\" w:eastAsia=\"");
			sResult += sFontName;
			sResult += _T("\" w:hAnsi=\"");
			sResult += sFontName;
			sResult += _T("\" w:cs=\"");
			sResult += sFontName;
			sResult += _T("\"");
			sResult += sHint;
			sResult += _T("/>");
		}
	}

	return sResult;
}

CString RtfColor::RenderToRtf(RenderParameter  oRenderParameter )
{
	RtfWriter* poRtfWriter = static_cast<RtfWriter*>( oRenderParameter.poWriter );
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
	CString sResult;
	if( RENDER_TO_RTF_PARAM_COLOR_TBL == oRenderParameter.nType )
	{
		//CString sTheme;
		//switch(m_eTheme)
		//{
		//	case cmaindarkone: sTheme = _T("\\cmaindarkone");break;
		//	case cmainlightone: sTheme = _T("\\cmainlightone");break;
		//	case cmaindarktwo: sTheme = _T("\\cmaindarktwo");break;
		//	case cmainlighttwo: sTheme = _T("\\cmainlighttwo");break;
		//	case caccentthree: sTheme = _T("\\caccentthree");break;
		//	case caccenttwo: sTheme = _T("\\caccenttwo");break;
		//	case caccentfour: sTheme = _T("\\caccentfour");break;
		//	case caccentfive: sTheme = _T("\\caccentfive");break;
		//	case caccentsix: sTheme = _T("\\caccentsix");break;
		//	case chyperlink: sTheme = _T("\\chyperlink");break;
		//	case cfollowedhyperlink: sTheme = _T("\\cfollowedhyperlink");break;
		//	case cbackgroundone: sTheme = _T("\\cbackgroundone");break;
		//	case ctextone: sTheme = _T("\\ctextone");break;
		//	case cbackgroundtwo: sTheme = _T("\\cbackgroundtwo");break;
		//	case ctexttwo: sTheme = _T("\\ctexttwo");break;
		//}
		//if( _T("") != sTheme )
		//{
		//	sResult += sTheme;
		//	sResult.AppendFormat(_T("\\ctint%d\\cshade%d"), m_byteTint, m_byteShade);
		//}
		sResult.AppendFormat(_T("\\red%d\\green%d\\blue%d"), m_byteRed, m_byteGreen, m_byteBlue);
		//важно
		sResult += _T(";");
	}
	else
	{
		int nColor;
		if( true == poRtfDocument->m_oColorTable.GetColor(*this, nColor) )
			sResult.AppendFormat(_T("\\c%d"),nColor); //todo
	}
	return sResult;
}

CString RtfColor::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( RENDER_TO_OOX_PARAM_COLOR_VALUE == oRenderParameter.nType )
	{
		sResult +=  ToHexColor();
	}
	else if( RENDER_TO_OOX_PARAM_COLOR_ATTRIBUTE == oRenderParameter.nType )
	{
		sResult +=  oRenderParameter.sValue;
	}
	else if( RENDER_TO_OOX_PARAM_COLOR_TAG == oRenderParameter.nType )
	{
        sResult += _T("<w:color ") + WriteOOXAttribute(oRenderParameter.sValue) + _T("/>");
	}
	else if (m_bAuto)
	{
        sResult += _T("<w:color w:val=\"auto\"/>");
	}
	else
	{
        sResult += _T("<w:color w:val=\"") + ToHexColor() + _T("\"/>");
	}
	return sResult;
}

CString RtfShadingChar::RenderToRtf(RenderParameter oRenderParameter)
{
	CString  sResult;
	RENDER_RTF_INT( m_nValue, sResult, _T("chshdng") )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += _T("\\chbghoriz");		break;
		case st_chbgvert:		sResult += _T("\\chbgvert");		break;
		case st_chbgfdiag:		sResult += _T("\\chbgfdiag");		break;
		case st_chbgbdiag:		sResult += _T("\\chbgbdiag");		break;
		case st_chbgcross:		sResult += _T("\\chbgcross");		break;
		case st_chbgdcross:		sResult += _T("\\chbgdcross");		break;
		case st_chbgdkhoriz:	sResult += _T("\\chbgdkhoriz");		break;
		case st_chbgdkvert:		sResult += _T("\\chbgdkvert");		break;
		case st_chbgdkfdiag:	sResult += _T("\\chbgdkfdiag");		break;
		case st_chbgdkbdiag:	sResult += _T("\\chbgdkbdiag");		break;
		case st_chbgdkcross:	sResult += _T("\\chbgdkcross");		break;
		case st_chbgdkdcross:	sResult += _T("\\chbgdkdcross");	break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, _T("chcfpat") )
	RENDER_RTF_INT( m_nBackColor, sResult, _T("chcbpat") )
	return sResult;
}

CString RtfShadingPar::RenderToRtf(RenderParameter oRenderParameter)
{
	CString  sResult;
	RENDER_RTF_INT( m_nValue, sResult, _T("shading") )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += _T("\\bghoriz");		break;
		case st_chbgvert:		sResult += _T("\\bgvert");		break;
		case st_chbgfdiag:		sResult += _T("\\bgfdiag");		break;
		case st_chbgbdiag:		sResult += _T("\\bgbdiag");		break;
		case st_chbgcross:		sResult += _T("\\bgcross");		break;
		case st_chbgdcross:		sResult += _T("\\bgdcross");	break;
		case st_chbgdkhoriz:	sResult += _T("\\bgdkhoriz");	break;
		case st_chbgdkvert:		sResult += _T("\\bgdkvert");	break;
		case st_chbgdkfdiag:	sResult += _T("\\bgdkfdiag");	break;
		case st_chbgdkbdiag:	sResult += _T("\\bgdkbdiag");	break;
		case st_chbgdkcross:	sResult += _T("\\bgdkcross");	break;
		case st_chbgdkdcross:	sResult += _T("\\bgdkdcross");	break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, _T("cfpat") )
	RENDER_RTF_INT( m_nBackColor, sResult, _T("cbpat") )
	return sResult;
}

CString RtfShadingCell::RenderToRtf(RenderParameter oRenderParameter)
{
	CString  sResult;
	RENDER_RTF_INT( m_nValue, sResult, _T("clshdng") )
	switch( m_eType )
	{
		case st_clshdrawnil:	sResult += _T("\\clshdrawnil");		break;
		case st_chbghoriz:		sResult += _T("\\clbghoriz");		break;
		case st_chbgvert:		sResult += _T("\\clbgvert");		break;
		case st_chbgfdiag:		sResult += _T("\\clbgfdiag");		break;
		case st_chbgbdiag:		sResult += _T("\\clbgbdiag");		break;
		case st_chbgcross:		sResult += _T("\\clbgcross");		break;
		case st_chbgdcross:		sResult += _T("\\clbgdcross");		break;
		case st_chbgdkhoriz:	sResult += _T("\\clbgdkhor");		break;
		case st_chbgdkvert:		sResult += _T("\\clbgdkvert");		break;
		case st_chbgdkfdiag:	sResult += _T("\\clbgdkfdiag");		break;
		case st_chbgdkbdiag:	sResult += _T("\\clbgdkbdiag");		break;
		case st_chbgdkcross:	sResult += _T("\\clbgdkcross");		break;
		case st_chbgdkdcross:	sResult += _T("\\clbgdkdcross");	break;
	}	
	RENDER_RTF_INT( m_nForeColor, sResult, _T("clcfpat") )
	if( PROP_DEF != m_nBackColor )
		sResult.AppendFormat(_T("\\clcbpat%d\\clcfpatraw%d"), m_nBackColor,m_nBackColor);
	return sResult;
}

CString RtfShadingRow::RenderToRtf(RenderParameter oRenderParameter)
{
	CString  sResult;
	RENDER_RTF_INT( m_nValue, sResult, _T("trshdng") )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += _T("\\trbghoriz");		break;
		case st_chbgvert:		sResult += _T("\\trbgvert");		break;
		case st_chbgfdiag:		sResult += _T("\\trbgfdiag");		break;
		case st_chbgbdiag:		sResult += _T("\\trbgbdiag");		break;
		case st_chbgcross:		sResult += _T("\\trbgcross");		break;
		case st_chbgdcross:		sResult += _T("\\trbgdcross");		break;
		case st_chbgdkhoriz:	sResult += _T("\\trbgdkhor");		break;
		case st_chbgdkvert:		sResult += _T("\\trbgdkvert");		break;
		case st_chbgdkfdiag:	sResult += _T("\\trbgdkfdiag");		break;
		case st_chbgdkbdiag:	sResult += _T("\\trbgdkbdiag");		break;
		case st_chbgdkcross:	sResult += _T("\\trbgdkcross");		break;
		case st_chbgdkdcross:	sResult += _T("\\trbgdkdcross");	break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, _T("trcfpat") )
	RENDER_RTF_INT( m_nBackColor, sResult, _T("trcbpat") )
	return sResult;
}

CString RtfShadingTableStyle::RenderToRtf(RenderParameter oRenderParameter)
{
	CString  sResult;
	RENDER_RTF_INT( m_nValue, sResult, _T("tscellpct") )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += _T("\\tsbghoriz");		break;
		case st_chbgvert:		sResult += _T("\\tsbgvert");		break;
		case st_chbgfdiag:		sResult += _T("\\tsbgfdiag");		break;
		case st_chbgbdiag:		sResult += _T("\\tsbgbdiag");		break;
		case st_chbgcross:		sResult += _T("\\tsbgcross");		break;
		case st_chbgdcross:		sResult += _T("\\tsbgdcross");		break;
		case st_chbgdkhoriz:	sResult += _T("\\tsbgdkhor");		break;
		case st_chbgdkvert:		sResult += _T("\\tsbgdkvert");		break;
		case st_chbgdkfdiag:	sResult += _T("\\tsbgdkfdiag");		break;
		case st_chbgdkbdiag:	sResult += _T("\\tsbgdkbdiag");		break;
		case st_chbgdkcross:	sResult += _T("\\tsbgdkcross");		break;
		case st_chbgdkdcross:	sResult += _T("\\tsbgdkdcross");	break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, _T("tscellcfpat") )
	RENDER_RTF_INT( m_nBackColor, sResult, _T("tscellcbpat") )
	return sResult;
}

CString RtfShading::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfWriter* poRtfWriter = static_cast<RtfWriter*>(oRenderParameter.poWriter);
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	CString sResult;
	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_COLOR_VALUE;

	if(PROP_DEF != m_nBackColor && PROP_DEF == m_nValue && st_none == m_eType)
		m_nValue = 0;

	CString sShading;
	if( PROP_DEF != m_nForeColor )
	{
		RtfColor oForeColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nForeColor, oForeColor ) )
            sShading += _T(" w:color=\"") + oForeColor.RenderToOOX(oNewParam) + _T("\"");
	}
	if( PROP_DEF != m_nBackColor )
	{
		RtfColor oBackColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nBackColor, oBackColor ) )
            sShading += _T(" w:fill=\"") + oBackColor.RenderToOOX(oNewParam) + _T("\"");
	}
	if( PROP_DEF != m_nValue )
	{
		CString sValue;
		if( 0 <= m_nValue &&  m_nValue <= 2 )		sShading += _T(" w:val=\"clear\"");
		else if( 2 < m_nValue && m_nValue <= 7 )	sShading += _T(" w:val=\"pct5\"");
		else if( 7 < m_nValue && m_nValue <= 11 )	sShading += _T(" w:val=\"pct10\"");
		else if( 11 < m_nValue && m_nValue <= 13 )	sShading += _T(" w:val=\"pct12\"");
		else if( 13 < m_nValue && m_nValue <= 17 )	sShading += _T(" w:val=\"pct15\"");
		else if( 17 < m_nValue && m_nValue <= 22 )	sShading += _T(" w:val=\"pct20\"");
		else if( 22 < m_nValue && m_nValue <= 27 )	sShading += _T(" w:val=\"pct25\"");
		else if( 27 < m_nValue && m_nValue <= 32 )	sShading += _T(" w:val=\"pct30\"");
		else if( 32 < m_nValue && m_nValue <= 36 )	sShading += _T(" w:val=\"pct35\"");
		else if( 36 < m_nValue && m_nValue <= 38 )	sShading += _T(" w:val=\"pct37\"");
		else if( 38 < m_nValue && m_nValue <= 42 )	sShading += _T(" w:val=\"pct40\"");
		else if( 42 < m_nValue && m_nValue <= 47 )	sShading += _T(" w:val=\"pct45\"");
		else if( 47 < m_nValue && m_nValue <= 52 )	sShading += _T(" w:val=\"pct50\"");
		else if( 52 < m_nValue && m_nValue <= 57 )	sShading += _T(" w:val=\"pct55\"");
		else if( 57 < m_nValue && m_nValue <= 61 )	sShading += _T(" w:val=\"pct60\"");
		else if( 61 < m_nValue && m_nValue <= 63 )	sShading += _T(" w:val=\"pct62\"");
		else if( 63 < m_nValue && m_nValue <= 70 )	sShading += _T(" w:val=\"pct65\"");
		else if( 70 < m_nValue && m_nValue <= 80 )	sShading += _T(" w:val=\"pct75\"");
		else if( 80 < m_nValue && m_nValue <= 86 )	sShading += _T(" w:val=\"pct85\"");
		else if( 86 < m_nValue && m_nValue <= 88 )	sShading += _T(" w:val=\"pct87\"");
		else if( 88 < m_nValue && m_nValue <= 92 )	sShading += _T(" w:val=\"pct90\"");
		else if( 92 < m_nValue && m_nValue <= 97 )	sShading += _T(" w:val=\"pct95\"");
		else if( 97 < m_nValue && m_nValue <= 100 )	sShading += _T(" w:val=\"solid\"");
	}
	else
	{
		switch( m_eType )
		{
			case st_clshdrawnil:	sShading += _T(" w:val=\"nil\"");						break;
			case st_chbghoriz:		sShading += _T(" w:val=\"thinHorzStripehorzStripe\"");	break;
			case st_chbgvert:		sShading += _T(" w:val=\"thinVertStripe\"");			break;
			case st_chbgfdiag:		sShading += _T(" w:val=\"thinReverseDiagStripe\"");		break;
			case st_chbgbdiag:		sShading += _T(" w:val=\"thinDiagStripe\"");			break;
			case st_chbgcross:		sShading += _T(" w:val=\"thinHorzCross\"");				break;
			case st_chbgdcross:		sShading += _T(" w:val=\"thinDiagCross\"");				break;
			case st_chbgdkhoriz:	sShading += _T(" w:val=\"horzStripe\"");				break;
			case st_chbgdkvert:		sShading += _T(" w:val=\"vertStripe\"");				break;
			case st_chbgdkfdiag:	sShading += _T(" w:val=\"reverseDiagStripe\"");			break;
			case st_chbgdkbdiag:	sShading += _T(" w:val=\"diagStripe\"");				break;
			case st_chbgdkcross:	sShading += _T(" w:val=\"horzCross\"");					break;
			case st_chbgdkdcross:	sShading += _T(" w:val=\"diagCross\"");					break;
		}
	}
	if( false == sShading.IsEmpty() )
		sResult = _T("<w:shd ") + sShading + _T("/>");
	return sResult;
}
CString RtfBorder::RenderToRtf(RenderParameter oRenderParameter)
{
	CString  sResult;
	switch( m_eType )
	{
		case bt_brdrs:			sResult = _T("\\brdrs");		break;
		case bt_brdrth:			sResult = _T("\\brdrth");		break;
		case bt_brdrsh:			sResult = _T("\\brdrsh");		break;
		case bt_brdrdb:			sResult = _T("\\brdrdb");		break;
		case bt_brdrdot:		sResult = _T("\\brdrdot");		break;
		case bt_brdrdash:		sResult = _T("\\brdrdash");		break;
		case bt_brdrhair:		sResult = _T("\\brdrhair");		break;
		case bt_brdrinset:		sResult = _T("\\brdrinset");	break;
		case bt_brdrdashsm:		sResult = _T("\\brdrdashsm");	break;
		case bt_brdrdashd:		sResult = _T("\\brdrdashd");	break;
		case bt_brdrdashdd:		sResult = _T("\\brdrdashdd");	break;
		case bt_brdrdashdot:	sResult = _T("\\brdrdashdot");	break;
		case bt_brdrtriple:		sResult = _T("\\brdrtriple");	break;
		case bt_brdrtnthsg:		sResult = _T("\\brdrtnthsg");	break;
		case bt_brdrthtnsg:		sResult = _T("\\brdrthtnsg");	break;
		case bt_brdrtnthtnsg:	sResult = _T("\\brdrtnthtnsg"); break;
		case bt_brdrtnthtnmg:	sResult = _T("\\brdrtnthtnmg"); break;
		case bt_brdrtnthlg:		sResult = _T("\\brdrtnthlg");	break;
		case bt_brdrthtnlg:		sResult = _T("\\brdrthtnlg");	break;
		case bt_brdrtnthtnlg:	sResult = _T("\\brdrtnthtnlg"); break;
		case bt_brdrwavy:		sResult = _T("\\brdrwavy");		break;
		case bt_brdrwavydb:		sResult = _T("\\brdrwavydb");	break;
		case bt_brdrdashdotstr: sResult = _T("\\brdrdashdotstr"); break;
		case bt_brdremboss:		sResult = _T("\\brdremboss");	break;
		case bt_brdrengrave:	sResult = _T("\\brdrengrave");	break;
		case bt_brdroutset:		sResult = _T("\\brdroutset");	break;
		case bt_brdrnone:		sResult = _T("\\brdrnone");		break;
	}
	if( PROP_DEF != m_nWidth && m_nWidth > 0)
		sResult.AppendFormat( _T("\\brdrw%d"), m_nWidth );
	if( PROP_DEF != m_nSpace  && m_nSpace > 0)
		sResult.AppendFormat( _T("\\brsp%d"),	m_nSpace );
	if( PROP_DEF != m_nColor )
		sResult.AppendFormat( _T("\\brdrcf%d"), m_nColor );
	return sResult;
}
CString RtfBorder::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	CString  sResult;
	if( PROP_DEF != m_nColor )
	{
		RtfColor oColor;
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_COLOR_VALUE;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nColor, oColor ) )
            sResult += _T(" w:color=\"") + oColor.RenderToOOX(oNewParam) + _T("\"");
	}
	if( PROP_DEF != m_nWidth ) //w:sz  1/8 twips (equivalent to 1/576th of an inch)
		sResult.AppendFormat( _T(" w:sz=\"%d\""), 2 * m_nWidth / 5 );
	if( PROP_DEF != m_nSpace )
		sResult.AppendFormat( _T(" w:space=\"%d\""), (int)RtfUtility::Twip2pt( m_nSpace ) );

	switch( m_eType )
	{
		case bt_brdrs:			sResult += _T(" w:val=\"single\"");					break;
		case bt_brdrth:			sResult += _T(" w:val=\"thick\"");					break;
		case bt_brdrsh:			sResult += _T(" w:val=\"\"thin");					break;
		case bt_brdrdb:			sResult += _T(" w:val=\"double\"");					break;
		case bt_brdrdot:		sResult += _T(" w:val=\"dotted\"");					break;
		case bt_brdrdash:		sResult += _T(" w:val=\"dashed\"");					break;
		case bt_brdrhair:		sResult += _T(" w:val=\"hair\"");					break;
		case bt_brdrdashsm:		sResult += _T(" w:val=\"dashSmallGap\"");			break;
		case bt_brdrdashd:		sResult += _T(" w:val=\"dotDash\"");				break;
		case bt_brdrdashdd:		sResult += _T(" w:val=\"dotDotDash\"");				break;
		case bt_brdrinset:		sResult += _T(" w:val=\"inset\"");					break;
		case bt_brdrnone:		sResult += _T(" w:val=\"nil\"");					break;
		case bt_brdroutset:		sResult += _T(" w:val=\"outset\"");					break;
		case bt_brdrtriple:		sResult += _T(" w:val=\"triple\"");					break;
		case bt_brdrtnthsg:		sResult += _T(" w:val=\"thinThickSmallGap\"");		break;
		case bt_brdrthtnsg:		sResult += _T(" w:val=\"thickThinSmallGap\"");		break;
		case bt_brdrtnthtnsg:	sResult += _T(" w:val=\"thinThickThinSmallGap\""); 	break;
		case bt_brdrtnthtnmg:	sResult += _T(" w:val=\"thinThickThinMediumGap\"");	break;
		case bt_brdrtnthmg:		sResult += _T(" w:val=\"thinThickMediumGap\"");		break;
		case bt_brdrthtnmg:		sResult += _T(" w:val=\"thickThinMediumGap\"");		break;
		case bt_brdrtnthlg:		sResult += _T(" w:val=\"thinThickLargeGap\"");		break;
		case bt_brdrthtnlg:		sResult += _T(" w:val=\"thickThinLargeGap\"");		break;
		case bt_brdrtnthtnlg:	sResult += _T(" w:val=\"thinThickThinLargeGap\"");	break;
		case bt_brdrwavy:		sResult += _T(" w:val=\"wave\"");					break;
		case bt_brdrwavydb:		sResult += _T(" w:val=\"doubleWave\"");				break;
		case bt_brdrdashdotstr: sResult += _T(" w:val=\"dashDotStroked\"");			break;
		case bt_brdremboss:		sResult += _T(" w:val=\"threeDEmboss\"");			break;
		case bt_brdrengrave:	sResult += _T(" w:val=\"threeDEngrave\"");			break;
	}
	if( false == sResult.IsEmpty() )
	{
		if( RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE == oRenderParameter.nType )
			;
		else if( RENDER_TO_OOX_PARAM_BORDER_TAG == oRenderParameter.nType )
			sResult = _T("<") + oRenderParameter.sValue + sResult + _T("/>");
		else
			sResult = _T("<w:bdr") + sResult + _T("/>");
	}
	return sResult;
}
CString RtfCharProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	RENDER_RTF_INT( m_nAnimated, sResult, _T("animtext") )
	RENDER_RTF_BOOL( m_bBold, sResult, _T("b") )
	RENDER_RTF_BOOL( m_bCaps, sResult, _T("caps") )
	RENDER_RTF_INT( m_nScalex, sResult, _T("charscalex") )
	//RENDER_RTF_INT( m_nCharStyle, sResult, _T("cs") )
	RENDER_RTF_INT( m_nDown, sResult, _T("dn") )
	RENDER_RTF_BOOL( m_bEmbo, sResult, _T("embo") )
	RENDER_RTF_INT( m_nCharacterSpacing, sResult, _T("expndtw") )
	if( PROP_DEF != m_nCharacterSpacing )
		sResult.AppendFormat( _T("\\expnd%d"), m_nCharacterSpacing / 5  );
	if( PROP_DEF != m_nFitText )
		sResult.AppendFormat( _T("\\fittext%d"), m_nFitText / 5  );
	RENDER_RTF_INT( m_nFont, sResult, _T("f") )
	//RENDER_RTF_INT( m_nFont2, sResult, _T("fittext") )
	//RENDER_RTF_INT( m_nFont3, sResult, _T("fittext") )
	RENDER_RTF_INT( m_nFontSize, sResult, _T("fs") )
	RENDER_RTF_BOOL( m_bItalic, sResult, _T("i") )
	RENDER_RTF_BOOL( m_bImprint, sResult, _T("impr") )
	RENDER_RTF_INT( m_nKerning, sResult, _T("kerning") )
	
	if (m_bRightToLeft != PROP_DEF )
	{
		if( m_bRightToLeft !=0)	sResult += _T("\\rtlch");
		else					sResult += _T("\\ltrch");
	}

	if( PROP_DEF != m_nComplexScript )
		sResult.AppendFormat(_T("\\fcs%d"), m_nComplexScript);
	RENDER_RTF_BOOL( m_bOutline, sResult, _T("outl") )
	RENDER_RTF_BOOL( m_bScaps, sResult, _T("scaps") )
	RENDER_RTF_BOOL( m_bShadow, sResult, _T("shad") )
	RENDER_RTF_BOOL( m_bStrike, sResult, _T("strike") )
	RENDER_RTF_INT( m_nStriked, sResult, _T("striked") )
	RENDER_RTF_BOOL( m_bSub, sResult, _T("sub") )
	RENDER_RTF_BOOL( m_bSuper, sResult, _T("super") )
	RENDER_RTF_INT( m_bHidden, sResult, _T("v") )
	RENDER_RTF_INT( m_nHightlited, sResult, _T("highlight") )
	RENDER_RTF_INT( m_nForeColor, sResult, _T("cf") )
	switch( m_eUnderStyle )
	{
		case uls_Single:				sResult += _T("\\ul");			break;
		case uls_Dotted:				sResult += _T("\\uld");			break;
		case uls_Dashed:				sResult += _T("\\uldash");		break;
		case uls_Dash_dotted:			sResult += _T("\\uldashd");		break;
		case uls_Dash_dot_dotted:		sResult += _T("\\uldashdd");	break;
		case uls_Double:				sResult += _T("\\uldb");		break;
		case uls_Heavy_wave:			sResult += _T("\\ulhwave");		break;
		case uls_Long_dashe:			sResult += _T("\\ulldash");		break;
		case uls_Stops_all:				sResult += _T("\\ulnone");		break;
		case uls_Thick:					sResult += _T("\\ulth");		break;
		case uls_Thick_dotted:			sResult += _T("\\ulthd");		break;
		case uls_Thick_dashed:			sResult += _T("\\ulthdash");	break;
		case uls_Thick_dash_dotted:		sResult += _T("\\ulthdashd");	break;
		case uls_Thick_dash_dot_dotted: sResult += _T("\\ulthdashdd");	break;
		case uls_Thick_long_dashed:		sResult += _T("\\ulthldash");	break;
		case uls_Double_wave:			sResult += _T("\\ululdbwave");	break;
		case uls_Word:					sResult += _T("\\ulw");			break;
		case uls_Wave:					sResult += _T("\\ulwave");		break;
	}
	RENDER_RTF_INT( m_nUnderlineColor, sResult, _T("ulc") )
	RENDER_RTF_INT( m_nUp, sResult, _T("up") )

	if( m_poBorder.IsValid() == true )
	{
		sResult += _T("\\chbrdr");
		sResult +=  m_poBorder.RenderToRtf( oRenderParameter );
	}
	if( m_poShading.IsValid() == true )
		sResult +=  m_poShading.RenderToRtf( oRenderParameter );

	return sResult;
}
CString RtfCharProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	OOXWriter* poOOXWriter = static_cast<OOXWriter*>(oRenderParameter.poWriter);
	
	if( RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
		sResult += _T("<w:rPr>");
	
	switch( m_nAnimated )
	{
		case 0:	sResult += _T("<w:effect w:val=\"none\"/>");			break;
		case 1:	sResult += _T("<w:effect w:val=\"lights\"/>");			break;
		case 2:	sResult += _T("<w:effect w:val=\"blinkBackground\"/>");	break;
		case 3:	sResult += _T("<w:effect w:val=\"sparkle\"/>");			break;
		case 4:	sResult += _T("<w:effect w:val=\"antsBlack\"/>");		break;
		case 5:	sResult += _T("<w:effect w:val=\"antsRed\"/>");			break;
		case 6:	sResult += _T("<w:effect w:val=\"shimmer\"/>");			break;
	}
	RENDER_OOX_BOOL( m_bBold, sResult, _T("w:b") )
	RENDER_OOX_BOOL( m_bCaps, sResult, _T("w:caps") )
	RENDER_OOX_INT( (int)m_nScalex, sResult, _T("w:w") )
	
	if( PROP_DEF != m_nCharStyle )
	{
		RtfStylePtr oStyle;
		if( true == poRtfDocument->m_oStyleTable.GetStyle( m_nCharStyle, oStyle ) )
		{
			sResult += _T("<w:pStyle w:val=\"");
			sResult += oStyle->m_sName;
			sResult += _T("\"/>");
		}
	}
	
	if( PROP_DEF != m_nDown )
	{
		sResult.AppendFormat(_T("<w:position w:val=\"-%d\" />"),m_nDown);
	}
	RENDER_OOX_BOOL( m_bEmbo, sResult, _T("w:emboss") )
	RENDER_OOX_INT( m_nCharacterSpacing, sResult, _T("w:spacing") )
	
	if( PROP_DEF != m_nFitText )
	{
		if( -1 == m_nFitText )
		{
            sResult.AppendFormat(_T("<w:fitText w:id=\"%d\" w:val=\"%d\" />"),poOOXWriter->nCurFitId.GetBuffer(), poOOXWriter->nCurFitWidth);
		}
		else 
		{
			poOOXWriter->nCurFitId = poRtfDocument->m_oIdGenerator.Generate_FitTextId();
			poOOXWriter->nCurFitWidth = m_nFitText;
            sResult.AppendFormat(_T("<w:fitText w:id=\"%d\" w:val=\"%d\" />"),poOOXWriter->nCurFitId.GetBuffer(), poOOXWriter->nCurFitWidth);
		}
	}
	if( PROP_DEF == m_nFont)
	{
		if (RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
			m_nFont = poRtfDocument->m_oProperty.m_nDeffMathFont;
		else
			m_nFont = poRtfDocument->m_oProperty.m_nDeffFont;
	}
	if( PROP_DEF != m_nFont )
	{
		RtfFont oCurFont;
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		if( true == poRtfDocument->m_oFontTable.GetFont(m_nFont,oCurFont) )
		{
			sResult +=  oCurFont.RenderToOOX(oNewParam);
		}
	}
	RENDER_OOX_INT( m_nFontSize, sResult, _T("w:sz") )
	RENDER_OOX_BOOL( m_bItalic, sResult, _T("w:i") )
	RENDER_OOX_BOOL( m_bImprint, sResult, _T("w:imprint") )
	RENDER_OOX_INT( m_nKerning, sResult, _T("w:kern") )
	
	if(m_bRightToLeft !=0 && m_bRightToLeft != PROP_DEF)
		sResult += _T("<w:rtl/>");
	
	RENDER_OOX_BOOL( m_nComplexScript, sResult, _T("w:cs") )
	RENDER_OOX_BOOL( m_bOutline, sResult, _T("w:outline") )
	RENDER_OOX_BOOL( m_bScaps, sResult, _T("w:smallCaps") )
	RENDER_OOX_BOOL( m_bShadow, sResult, _T("w:shadow") )
	RENDER_OOX_BOOL( m_bStrike, sResult, _T("w:strike") )
	RENDER_OOX_BOOL( m_nStriked, sResult, _T("w:dstrike") )

	if( 1 == m_bSub )
		sResult += _T("<w:vertAlign w:val=\"subscript\"/>");
	if( 1 == m_bSuper )
		sResult += _T("<w:vertAlign w:val=\"superscript\"/>");

	if( PROP_DEF != m_nHightlited )
	{
		RtfColor oCurColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nHightlited, oCurColor ) )
		{
            sResult += _T("<w:highlight w:val=\"") + oCurColor.GetHighLight() + _T("\"/>");
		}
	}
		
	if( PROP_DEF != m_nForeColor )
	{
		RtfColor oCurColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nForeColor, oCurColor ) )
		{
			RenderParameter oNewParam = oRenderParameter;
			oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
			sResult +=  oCurColor.RenderToOOX(oNewParam);
		}
	}

	CString sUnderColor;
	if(  PROP_DEF != m_nUnderlineColor  )
	{
		RtfColor oCurColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nUnderlineColor, oCurColor ) )
		{
			RenderParameter oNewParam = oRenderParameter;
			oNewParam.nType = RENDER_TO_OOX_PARAM_COLOR_ATTRIBUTE;
			sUnderColor +=  oCurColor.RenderToOOX(oNewParam);
		}
	}
	if( uls_none != m_eUnderStyle )
	{
		sResult += _T("<w:u ") + sUnderColor;
		switch( m_eUnderStyle )
		{
            case uls_Single:				sResult += _T(" w:val=\"single\"/>");			break;
            case uls_Dotted:				sResult += _T(" w:val=\"dotted\"/>");			break;
            case uls_Dashed:				sResult += _T(" w:val=\"dash\"/>");				break;
            case uls_Dash_dotted:			sResult += _T(" w:val=\"dotDash\"/>");			break;
            case uls_Dash_dot_dotted:		sResult += _T(" w:val=\"dotDotDash\"/>");		break;
            case uls_Double:				sResult += _T(" w:val=\"double\"/>");			break;
            case uls_Heavy_wave:			sResult += _T(" w:val=\"wavyHeavy\"/>");		break;
            case uls_Long_dashe:			sResult += _T(" w:val=\"dashLong\"/>");			break;
            case uls_Stops_all:				sResult += _T(" w:val=\"single\"/>");			break;
            case uls_Thick:					sResult += _T(" w:val=\"thick\"/>");			break;
            case uls_Thick_dotted:			sResult += _T(" w:val=\"dottedHeavy\"/>");		break;
            case uls_Thick_dashed:			sResult += _T(" w:val=\"dashedHeavy\"/>");		break;
            case uls_Thick_dash_dotted:		sResult += _T(" w:val=\"dashDotHeavy\"/>");		break;
            case uls_Thick_dash_dot_dotted: sResult += _T(" w:val=\"dashDotDotHeavy\"/>");	break;
            case uls_Thick_long_dashed:		sResult += _T(" w:val=\"dashLongHeavy\"/>");	break;
            case uls_Double_wave:			sResult += _T(" w:val=\"wavyDouble\"/>");		break;
            case uls_Word:					sResult += _T(" w:val=\"words\"/>");			break;
            case uls_Wave:					sResult += _T(" w:val=\"wave\"/>");				break;
		}
	}

	RENDER_OOX_INT( m_nUp, sResult, _T("w:position") )
	
    if (m_nLanguage != PROP_DEF && RENDER_TO_OOX_PARAM_MATH != oRenderParameter.nType) 
			//todooo сделаь map для используемых в доке
	{
        std::wstring str_lang;
#if defined(_WIN32) || defined(_WIN64)
		wchar_t buf[29] = {};

		int ccBuf = GetLocaleInfo(m_nLanguage, LOCALE_SISO639LANGNAME, buf, 29);
		
		if (ccBuf > 0)
		{
            str_lang += buf;
            str_lang += _T("-");
		}
		
		ccBuf = GetLocaleInfo(m_nLanguage, LOCALE_SISO3166CTRYNAME, buf, 29);
		
        if (ccBuf > 0) str_lang.append(buf);
#else
        for (int i = 0; i < 136; i++)
        {
            if (LCID_ms_convert[i].LCID_int == m_nLanguage)
            {
                str_lang = LCID_ms_convert[i].LCID_string;
                break;
            }
        }
#endif
        if (str_lang.length() > 0)
            sResult += CString(_T("<w:lang w:val=\"")) + str_lang.c_str() + _T("\"/>");
    }

	sResult +=  m_poBorder.RenderToOOX( oRenderParameter );
	sResult +=  m_poShading.RenderToOOX( oRenderParameter );
	
	if( RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
		sResult += _T("</w:rPr>");
	return sResult;
}

CString RtfListLevelProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument);
	RtfWriter* poRtfWriter = static_cast<RtfWriter*>( oRenderParameter.poWriter);
	
	CString sResult;
	sResult += _T("{\\listlevel");

	RENDER_RTF_INT( m_nNumberType, sResult, _T("levelnfc") )
	RENDER_RTF_INT( m_nNumberType, sResult, _T("levelnfcn") )
	RENDER_RTF_BOOL( m_bTentative, sResult, _T("lvltentative") )
	RENDER_RTF_INT( m_nJustification, sResult, _T("leveljc") )
	RENDER_RTF_INT( m_nJustification, sResult, _T("leveljcn") )
	RENDER_RTF_INT( m_nFollow, sResult, _T("levelfollow") )
	RENDER_RTF_INT( m_nStart, sResult, _T("levelstartat") )
	RENDER_RTF_INT( m_nNoRestart, sResult, _T("levelnorestart") )
	RENDER_RTF_INT( m_nPictureIndex, sResult, _T("levelpicture") )
	//чтобы при последующем чтении из rtf не потерялась информация о шрифте
	sResult +=  m_oCharProp.RenderToRtf( oRenderParameter );

    sResult += _T("{\\leveltext ") + RtfChar::renderRtfText( m_sText, oRenderParameter.poDocument, &m_oCharProp ) + _T(";}");
    sResult += _T("{\\levelnumbers ") + RtfChar::renderRtfText( m_sNumber, oRenderParameter.poDocument, &m_oCharProp ) + _T(";}");

	RENDER_RTF_INT( m_nFirstIndent, sResult, _T("fi") )
	RENDER_RTF_INT( m_nIndent, sResult, _T("li") )
	RENDER_RTF_INT( m_nIndentStart, sResult, _T("lin") )

	for( int i = 0 ; i < (int)m_oTabs.m_aTabs.size(); i++ )
	{
		sResult += _T("\\jclisttab");
		sResult +=  m_oTabs.m_aTabs[i].RenderToRtf( oRenderParameter );
	}


	sResult += _T("}");
	return sResult;
}
CString RtfListLevelProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	return RenderToOOX2(oRenderParameter);
}
CString RtfListLevelProperty::RenderToOOX2(RenderParameter oRenderParameter, int nLvl)
{
	CString sResult;
	int nLevel = PROP_DEF;
	if(PROP_DEF != nLvl)
		nLevel = nLvl;
	else if(PROP_DEF != m_nLevel)
		nLevel = m_nLevel;
	if(PROP_DEF != nLevel)
	{
		sResult.AppendFormat( _T("<w:lvl w:ilvl=\"%d\""), nLevel ); 
		RENDER_OOX_BOOL_ATTRIBUTE( m_bTentative, sResult, _T("w:tentative"))
		sResult += _T(">"); 

		if( PROP_DEF != m_nJustification )
		{
			switch( m_nJustification )
			{
				case 0:	sResult += _T("<w:lvlJc w:val=\"left\"/>");		break;
				case 1:	sResult += _T("<w:lvlJc w:val=\"center\"/>");	break;
				case 2:	sResult += _T("<w:lvlJc w:val=\"right\"/>");	break;
			}
		}
		if( 1 == m_nNoRestart)
			sResult += _T("<w:lvlRestart w:val=\"0\"/>");
		if( 1 ==  m_nLegal)
			sResult += _T("<w:isLgl/>"); 

        sResult += _T("<w:lvlText w:val=\"") + GetLevelTextOOX() + _T("\"/>");
        sResult += _T("<w:numFmt w:val=\"") + GetFormat(m_nNumberType) + _T("\"/>");
		
		RENDER_OOX_INT( m_nPictureIndex, sResult, _T("w:lvlPicBulletId") )
		RENDER_OOX_INT( m_nStart, sResult, _T("w:start") )
		
		if( PROP_DEF != m_nFollow )
		{
			switch( m_nFollow )
			{
				case 0: sResult += _T("<w:suff w:val=\"tab\"/>");		break;
				case 1: sResult += _T("<w:suff w:val=\"space\"/>");		break;
				case 2: sResult += _T("<w:suff w:val=\"nothing\"/>");	break;
			}
		}

		CString spPr;
		CString sIndent;
		if( PROP_DEF != m_nFirstIndent )
		{
			if( m_nFirstIndent >= 0 )
				sIndent.AppendFormat( _T(" w:firstLine=\"%d\""), m_nFirstIndent );
			else
				sIndent.AppendFormat( _T(" w:hanging=\"%d\""), -m_nFirstIndent );
		}
		RENDER_OOX_INT_ATTRIBUTE( m_nIndent, sIndent, _T("w:left") )
		RENDER_OOX_INT_ATTRIBUTE( m_nIndentStart, sIndent, _T("w:start") )
		
		if( !sIndent.IsEmpty() )
            spPr += _T("<w:ind ") + sIndent + _T("/>");

		spPr += m_oTabs.RenderToOOX( oRenderParameter );

		if( !spPr.IsEmpty() )
            sResult += _T("<w:pPr>") + spPr + _T("</w:pPr>");

		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		
		CString srPr = m_oCharProp.RenderToOOX(oNewParam);

		if( !srPr.IsEmpty() )
            sResult += _T("<w:rPr>") + srPr + _T("</w:rPr>");

		sResult += _T("</w:lvl>"); 
	}
	return sResult;		
}
CString RtfListProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	sResult += _T("\\list");

	RENDER_RTF_INT( m_nTemplateId, sResult, _T("listtemplateid") )
	//RENDER_RTF_INT( m_nListSimple, sResult, _T("listsimple") )
	//RENDER_RTF_BOOL( m_bListHybrid, sResult, _T("listhybrid") )
	if( 1 == m_bListHybrid )
		sResult += _T("\\listhybrid");
	else if( 1 == m_nListSimple )
		sResult.AppendFormat(_T("\\listsimple%d") ,m_nListSimple);

	for( int i = 0; i < (int)m_aArray.size(); i++ )
	{
		sResult +=  m_aArray[i].RenderToRtf( oRenderParameter );
	}
    sResult += _T("{\\listname ") + RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument ) + _T(";}");
	
	RENDER_RTF_INT( m_nID, sResult, _T("listid") )
	return sResult;
}
CString RtfListProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	OOXWriter* poOOXWriter = static_cast<OOXWriter*>(oRenderParameter.poWriter);
	
	CString sResult;
	
	sResult.AppendFormat( _T("<w:abstractNum w:abstractNumId=\"%d\">"), m_nID );
	
	if( 1 == m_bListHybrid )
		sResult += _T("<w:multiLevelType w:val=\"hybridMultilevel\"/>");
	else if( 1 == m_nListSimple )
		sResult += _T("<w:multiLevelType w:val=\"singleLevel\"/>");
	else
		sResult += _T("<w:multiLevelType w:val=\"multilevel\"/>");

	if( !m_sName.IsEmpty() )
        sResult += _T("<w:name w:val=\"") + Utils::PrepareToXML( m_sName ) + _T("\"/>");
	//if( false == m_nStyleName )
	//{
	//	sResult += _T("<w:name w:val=\"") + Utils::PrepareToXML( m_sName ) + _T("\"/>");
	//	if( PROP_DEF !=  m_nStyleID)
	//		sResult += _T("<w:styleLink w:val=\"") + Utils::PrepareToXML( m_sName ) + _T("\"/>");
	//}
	//else
	//	sResult += _T("<w:numStyleLink w:val=\"") + Utils::PrepareToXML( m_sName) + _T(\"/>");

	if( PROP_DEF != m_nTemplateId )
		sResult.AppendFormat(_T("<w:tmpl w:val=\"%x\"/>") ,m_nTemplateId);

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	for( int i = 0; i < (int)m_aArray.size(); i++ )
		sResult +=  m_aArray[i].RenderToOOX(oNewParam);

	sResult += _T("</w:abstractNum>");
	return sResult;
}
CString RtfListOverrideProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	if( true == IsValid() )
	{
		sResult += _T("\\listoverride");
		RENDER_RTF_INT( m_nListID, sResult, _T("listid") )
		RENDER_RTF_INT( m_nIndex, sResult, _T("ls") )
		sResult +=  m_oOverrideLevels.RenderToRtf( oRenderParameter );
	}
	return sResult;
}

CString RtfListOverrideProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( true == IsValid() )
	{
		sResult.AppendFormat(_T("<w:num w:numId=\"%d\">"), m_nIndex);
		sResult.AppendFormat(_T("<w:abstractNumId w:val=\"%d\" />"), m_nListID);
		sResult += m_oOverrideLevels.RenderToOOX( oRenderParameter );
		sResult += _T("</w:num>");
	}
	return sResult;		
}

CString RtfStyle::RenderToOOXBegin(RenderParameter oRenderParameter)
{
	RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
	CString sResult;
	CString sType;
	switch( m_eType )
	{
		case stParagraph :	sType = _T("paragraph");	break;
		case stCharacter :	sType = _T("character");	break;
		case stSection :	sType = _T("numbering");	break;
		case stTable :		sType = _T("table");		break;
	}
    sResult += _T("<w:style w:type=\"") + sType + _T("\" w:styleId=\"") + Utils::PrepareToXML( m_sName ) + _T("\">");
    sResult += _T("<w:name w:val=\"") + Utils::PrepareToXML( m_sName ) + _T("\"/>");

	if( PROP_DEF != m_nBasedOn )
	{
		RtfStylePtr oBaseStyle;
		if( true == poDocument->m_oStyleTable.GetStyle( m_nBasedOn, oBaseStyle ) )
            sResult += _T("<w:basedOn w:val=\"") + Utils::PrepareToXML( oBaseStyle->m_sName ) + _T("\"/>");
	}
	//if( PROP_DEF != m_nNext )
	//	sResult += _T("<w:next w:val=\"") + Utils::PrepareToXML( m_sName ) + _T("\"/>"), );//Todo
	//if( PROP_DEF != m_nLink )
	//	sResult += _T("<w:link w:val=\"") + Utils::PrepareToXML( m_sName ) + _T("\"/>"), );//Todo
	if( PROP_DEF != m_bHidden )				sResult += _T("<w:hidden/>");
	if( PROP_DEF != m_bLocked )				sResult += _T("<w:locked/>");
	if( PROP_DEF != m_bPersonal )			sResult += _T("<w:personal w:val=\"true\" />");
	if( PROP_DEF != m_bCompose )			sResult += _T("<w:personalCompose w:val=\"true\" />");
	if( PROP_DEF != m_bReply )				sResult += _T("<w:personalReply w:val=\"true\" />");
	if( PROP_DEF == m_nSemiHidden )			sResult += _T("<w:semiHidden/>");
	if( PROP_DEF != m_bQFormat )			sResult += _T("<w:qformat/>");
	if( PROP_DEF != m_nPriority )			sResult.AppendFormat(_T("<w:uiPriority w:val=\"%d\"/>"),m_nPriority);
	if( PROP_DEF != m_bUnhiddenWhenUse )	sResult += _T("<w:unhideWhenUsed/>");

	return sResult;
}

CString RtfStyle::RenderToOOXEnd(RenderParameter oRenderParameter)
{
	//if( false == IsValid() )
	//	return false;
	CString sResult;
	sResult += _T("</w:style>");
	return sResult;
}

CString RtfStyle::RenderToRtfEnd( RenderParameter oRenderParameter )
{
	if( false == IsValid() ) return _T("");

	CString sResult;
	RENDER_RTF_INT( m_nBasedOn, sResult, _T("sbasedon") )
	RENDER_RTF_INT( m_nNext, sResult, _T("snext") )
	RENDER_RTF_BOOL( m_bHidden, sResult, _T("shidden") )
	RENDER_RTF_INT( m_nLink, sResult, _T("slink") )
	RENDER_RTF_BOOL( m_bLocked, sResult, _T("slocked") )
	RENDER_RTF_BOOL( m_bPersonal, sResult, _T("spersonal") )
	RENDER_RTF_BOOL( m_bCompose, sResult, _T("scompose") )
	RENDER_RTF_BOOL( m_bReply, sResult, _T("sreply") )
	RENDER_RTF_BOOL( m_nSemiHidden, sResult, _T("ssemihidden") )
	RENDER_RTF_BOOL( m_bQFormat, sResult, _T("sqformat") )
	RENDER_RTF_INT( m_nPriority, sResult, _T("spriority") )
	RENDER_RTF_BOOL( m_bUnhiddenWhenUse, sResult, _T("sunhideused") )

    sResult += _T(" ") + RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument ) + _T(";}");
	return sResult;
}
CString RtfCharStyle::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult = RenderToRtfBegin( oRenderParameter ) ;
	sResult +=  m_oCharProp.RenderToRtf(  oRenderParameter );
	sResult +=  RenderToRtfEnd( oRenderParameter );
	return sResult;
}
CString RtfCharStyle::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult = RenderToOOXBegin( oRenderParameter ) ;
	
	CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
	if( false == sCharProp.IsEmpty() )
	{
        sResult += _T("<w:rPr>");
		sResult += sCharProp;
		sResult += _T("</w:rPr>");
	}
	sResult += RenderToOOXEnd( oRenderParameter );
	return sResult;
}
CString RtfParagraphStyle::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult = RenderToRtfBegin( oRenderParameter );

	sResult += m_oParProp.RenderToRtf(  oRenderParameter ) ;
	sResult += m_oCharProp.RenderToRtf(  oRenderParameter );
	sResult += RenderToRtfEnd( oRenderParameter );
	return sResult;
}

CString RtfParagraphStyle::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult = RenderToOOXBegin( oRenderParameter );
	
	CString sParProp = m_oParProp.RenderToOOX(oRenderParameter);
	if( false == sParProp.IsEmpty() )
	{
        sResult += _T("<w:pPr>");
			sResult += sParProp;
		sResult += _T("</w:pPr>");
	}
	CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
	if( false == sCharProp.IsEmpty() )
	{
        sResult += _T("<w:rPr>");
			sResult += sCharProp;
		sResult += _T("</w:rPr>");
	}
	sResult += RenderToOOXEnd( oRenderParameter ) ;
	return sResult;
}

//CString RtfTableStyleProperty::RenderToRtf(RenderParameter oRenderParameter)
//		{
//			CString sResult;
//			if( m_oShading.IsValid() )
//				sResult +=  m_oShading.RenderToRtf( oRenderParameter ) );
//			RENDER_RTF_INT( m_nPaddingTop, sResult, _T("tscellpaddt") )
//			RENDER_RTF_INT( m_nPaddingLeft, sResult, _T("tscellpaddl") )
//			RENDER_RTF_INT( m_nPaddingRight, sResult, _T("tscellpaddr") )
//			RENDER_RTF_INT( m_nPaddingBottom, sResult, _T("tscellpaddb") )
//			RENDER_RTF_INT( m_nIsPaddingTop, sResult, _T("tscellpaddft") )
//			RENDER_RTF_INT( m_nIsPaddingLeft, sResult, _T("tscellpaddfl") )
//			RENDER_RTF_INT( m_nIsPaddingRight, sResult, _T("tscellpaddfr") )
//			RENDER_RTF_INT( m_nIsPaddingBottom, sResult, _T("tscellpaddfb") )
//
//			switch( m_eAlign )
//			{
//				case av_tsvertalt: sResult += _T("\\tsvertalt");break;
//				case av_tsvertalc: sResult += _T("\\tsvertalc");break;
//				case av_tsvertalb: sResult += _T("\\tsvertalb");break;
//
//			}
//			RENDER_RTF_BOOL( m_bNoCellWrap, sResult, _T("tsnowrap") )
//
//			if( true == m_oCellTopBorder.IsValid() )
//			{
//				sResult += _T("\\tsbrdrt");
//				sResult +=  m_oCellTopBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellBottomBorder.IsValid() )
//			{
//				sResult += _T("\\tsbrdrb");
//				sResult +=  m_oCellBottomBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellLeftBorder.IsValid() )
//			{
//				sResult += _T("\\tsbrdrl");
//				sResult +=  m_oCellLeftBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellRightBorder.IsValid() )
//			{
//				sResult += _T("\\tsbrdrr");
//				sResult +=  m_oCellRightBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellHorBorder.IsValid() )
//			{
//				sResult += _T("\\tsbrdrh");
//				sResult +=  m_oCellHorBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellVerBorder.IsValid() )
//			{
//				sResult += _T("\\tsbrdrv");
//				sResult +=  m_oCellVerBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			RENDER_RTF_INT( m_nRowBandNumber, sResult, _T("tscbandsh") )
//			RENDER_RTF_INT( m_nCellBandNumber, sResult, _T("tscbandsv") )
//
//			return sResult;
//		}
//CString RtfTableStyleProperty::RenderToOOX(RenderParameter oRenderParameter)
//		{
//			CString sResult;
//			return sResult;
//		}

CString RtfTableStyle::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult = RenderToRtfBegin( oRenderParameter ) ;

	RenderParameter oNewParameter = oRenderParameter;
	oNewParameter.nType = RENDER_TO_RTF_PARAM_NO_WROWD;

	sResult +=  m_oTableProp.RenderToRtf( oNewParameter );
	sResult +=  m_oRowProp.RenderToRtf( oNewParameter );
	//sResult +=  m_oTableStyleProperty.RenderToRtf(  oRenderParameter );
	sResult +=  m_oParProp.RenderToRtf(  oRenderParameter );
	sResult +=  m_oCharProp.RenderToRtf(  oRenderParameter );
	sResult +=  RenderToRtfEnd( oRenderParameter );
	
	if( NULL != m_oFirstRow )	sResult +=  m_oFirstRow->RenderToRtf( oRenderParameter );
	if( NULL != m_oLastRow )	sResult +=  m_oLastRow->RenderToRtf( oRenderParameter );
	if( NULL != m_oFirstCol )	sResult +=  m_oFirstCol->RenderToRtf( oRenderParameter );
	if( NULL != m_oLastCol )	sResult +=  m_oLastCol->RenderToRtf( oRenderParameter );
	if( NULL != m_oBandHorEven )sResult +=  m_oBandHorEven->RenderToRtf( oRenderParameter );
	if( NULL != m_oBandVerEven )sResult +=  m_oBandVerEven->RenderToRtf( oRenderParameter );
	if( NULL != m_oBandHorOdd )	sResult +=  m_oBandHorOdd->RenderToRtf( oRenderParameter );
	if( NULL != m_oBandVerOdd )	sResult +=  m_oBandVerOdd->RenderToRtf( oRenderParameter );
	if( NULL != m_oNWCell )		sResult +=  m_oNWCell->RenderToRtf( oRenderParameter );
	if( NULL != m_oNECell )		sResult +=  m_oNECell->RenderToRtf( oRenderParameter );
	if( NULL != m_oSWCell )		sResult +=  m_oSWCell->RenderToRtf( oRenderParameter );
	if( NULL != m_oSECell )		sResult +=  m_oSECell->RenderToRtf( oRenderParameter );

	return sResult;
}

CString RtfTableStyle::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( RENDER_TO_OOX_PARAM_NESTED != oRenderParameter.nType )
	{
		sResult += RenderToOOXBegin( oRenderParameter ) ;

		CString sTablProp = m_oTableProp.RenderToOOX(oRenderParameter);
		if( false == sTablProp.IsEmpty() )
		{
            sResult += _T("<w:tblPr>");
				sResult += sTablProp;
			sResult += _T("</w:tblPr>");
		}
		CString sRowProp = m_oRowProp.RenderToOOX(oRenderParameter);
		if( false == sRowProp.IsEmpty() )
		{
            sResult += _T("<w:trPr>");
				sResult += sRowProp;
			sResult += _T("</w:trPr>");
		}
		CString sCellProp = m_oCellProp.RenderToOOX(oRenderParameter);
		if( false == sCellProp.IsEmpty() )
		{
            sResult += _T("<w:tcPr>");
				sResult += sCellProp;
			sResult += _T("</w:tcPr>");
		}
		CString sParProp = m_oParProp.RenderToOOX(oRenderParameter);
		if( false == sParProp.IsEmpty() )
		{
            sResult += _T("<w:pPr>");
				sResult += sParProp;
			sResult += _T("</w:pPr>");
		}
		CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
		if( false == sCharProp.IsEmpty() )
		{
            sResult += _T("<w:rPr>");
				sResult += sCharProp;
			sResult += _T("</w:rPr>");
		}

		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_NESTED;

		if( NULL != m_oFirstRow )
		{
			oNewParam.sValue = _T("firstRow");
			sResult += m_oFirstRow->RenderToOOX(oNewParam) ;
		}
		if( NULL != m_oLastRow )
		{
			oNewParam.sValue = _T("lastRow");
			sResult += m_oLastRow->RenderToOOX(oNewParam);
		}
		if( NULL != m_oFirstCol )
		{
			oNewParam.sValue = _T("firstCol");
			sResult += m_oFirstCol->RenderToOOX(oNewParam);
		}
		if( NULL != m_oLastCol )
		{
			oNewParam.sValue = _T("lastCol");
			sResult += m_oLastCol->RenderToOOX(oNewParam);
		}
		if( NULL != m_oBandHorEven )
		{
			oNewParam.sValue = _T("band2Horz");
			sResult += m_oBandHorEven->RenderToOOX(oNewParam) ;
		}
		if( NULL != m_oBandVerEven )
		{
			oNewParam.sValue = _T("band2Vert");
			sResult += m_oBandVerEven->RenderToOOX(oNewParam);
		}
		if( NULL != m_oBandHorOdd )
		{
			oNewParam.sValue = _T("band1Horz");
			sResult += m_oBandHorOdd->RenderToOOX(oNewParam);
		}
		if( NULL != m_oBandVerOdd )
		{
			oNewParam.sValue = _T("band1Vert");
			sResult += m_oBandVerOdd->RenderToOOX(oNewParam);
		}
		if( NULL != m_oNWCell )
		{
			oNewParam.sValue = _T("nwCell");
			sResult += m_oNWCell->RenderToOOX(oNewParam);
		}
		if( NULL != m_oNECell )
		{
			oNewParam.sValue = _T("neCell");
			sResult += m_oNECell->RenderToOOX(oNewParam);
		}
		if( NULL != m_oSWCell )
		{
			oNewParam.sValue = _T("swCell");
			sResult += m_oSWCell->RenderToOOX(oNewParam);
		}
		if( NULL != m_oSECell )
		{
			oNewParam.sValue = _T("seCell");
			sResult += m_oSECell->RenderToOOX(oNewParam);
		}
		sResult += RenderToOOXEnd( oRenderParameter ) ;
	}
	else
	{
		sResult += _T("<w:tblStylePr w:type=\"") + oRenderParameter.sValue + _T("\">");
		
		CString sTablProp = m_oTableProp.RenderToOOX(oRenderParameter);
		if( false == sTablProp.IsEmpty() )
		{
            sResult += _T("<w:tblPr>");
			sResult += sTablProp;
			sResult += _T("</w:tblPr>");
		}
		CString sRowProp = m_oRowProp.RenderToOOX(oRenderParameter);
		if( false == sRowProp.IsEmpty() )
		{
            sResult += _T("<w:trPr>");
			sResult += sRowProp;
			sResult += _T("</w:trPr>");
		}
		CString sCellProp = m_oCellProp.RenderToOOX(oRenderParameter);
		if( false == sCellProp.IsEmpty() )
		{
            sResult += _T("<w:tcPr>");
			sResult += sCellProp;
			sResult += _T("</w:tcPr>");
		}
		CString sParProp = m_oParProp.RenderToOOX(oRenderParameter);
		if( false == sParProp.IsEmpty() )
		{
            sResult += _T("<w:pPr>");
				sResult += sParProp;
			sResult += _T("</w:pPr>");
		}
		CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
		if( false == sCharProp.IsEmpty() )
		{
            sResult += _T("<w:rPr>");
				sResult += sCharProp;
			sResult += _T("</w:rPr>");
		}
		sResult += _T("</w:tblStylePr>") ;
	}
	return sResult;
}

CString RtfTab::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	if( PROP_DEF != m_nTab )
	{
		if( tk_tqbar == m_eKind )
		{
			switch( m_eLeader )
			{
				case tl_dot:	sResult += _T("\\tldot");	break;
				case tl_mdot:	sResult += _T("\\tlmdot");	break;
				case tl_hyph:	sResult += _T("\\tlhyph");	break;
				case tl_ul:		sResult += _T("\\tlul");	break;
			}
			sResult.AppendFormat( _T("\\tb%d"), m_nTab );
		}
		else
		{
			switch( m_eLeader )
			{
				case tl_dot:	sResult += _T("\\tldot");	break;
				case tl_mdot:	sResult += _T("\\tlmdot");	break;
				case tl_hyph:	sResult += _T("\\tlhyph");	break;
				case tl_ul:		sResult += _T("\\tlul");	break;
			}
			switch( m_eKind )
			{
				case tk_tqr:	sResult += _T("\\tqr");		break;
				case tk_tqc:	sResult += _T("\\tqc");		break;
				case tk_tqdec:	sResult += _T("\\tqdec");	break;
			}
			sResult.AppendFormat( _T("\\tx%d"), m_nTab );
		}
	}
	return sResult;
}
CString RtfTab::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;

	CString sTab;
	switch( m_eLeader )
	{
		case tl_dot:	sTab += _T(" w:leader=\"dot\"");		break;
		case tl_mdot:	sTab += _T(" w:leader=\"middleDot\"");	break;
		case tl_hyph:	sTab += _T(" w:leader=\"hyphen\"");		break;
		case tl_ul:		sTab += _T(" w:leader=\"underscore\"");	break;
	}
	switch( m_eKind )
	{
		case tk_tql:	sTab += _T(" w:val=\"left\"");		break;
		case tk_tqr:	sTab += _T(" w:val=\"right\"");		break;
		case tk_tqc:	sTab += _T(" w:val=\"center\"");	break;
		case tk_tqdec:	sTab += _T(" w:val=\"decimal\"");	break;
		case tk_tqbar:	sTab += _T(" w:val=\"bar\"");		break;
	}
	if( PROP_DEF != m_nTab )
		sTab.AppendFormat( _T(" w:pos=\"%d\""), m_nTab );
	
	if( false == sTab.IsEmpty() )
		sResult = _T("<w:tab ") + sTab + _T("/>");
	return sResult;
}
CString RtfFrame::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	RENDER_RTF_INT( m_nWidth, sResult, _T("absw") )
	RENDER_RTF_INT( m_nHeight, sResult, _T("absh") )
	switch ( m_eHRef )
	{
		case hr_phmrg:	sResult += _T("\\phmrg");	break;
		case hr_phpg:	sResult += _T("\\phpg");	break;
		case hr_phcol:	sResult += _T("\\phcol");	break;
	}
	if( PROP_DEF != m_nHPos )
	{
		if( m_nHPos > 0 )
			sResult.AppendFormat(_T("\\posx%d"),m_nHPos);
		else
			sResult.AppendFormat(_T("\\posnegx%d"),m_nHPos);
	}
	switch ( m_eHPos )
	{
		case hp_posxc: sResult += _T("\\posxc");	break;
		case hp_posxi: sResult += _T("\\posxi");	break;
		case hp_posxo: sResult += _T("\\posxo");	break;
		case hp_posxl: sResult += _T("\\posxl");	break;
		case hp_posxr: sResult += _T("\\posxr");	break;
	}

	switch ( m_eVRef )
	{	
		case vr_pvmrg:	sResult += _T("\\pvmrg");	break;
		case vr_pvpg:	sResult += _T("\\pvpg");	break;
		case vr_pvpara: sResult += _T("\\pvpara");	break;
	}
	if( PROP_DEF != m_nVPos )
	{
		if( m_nVPos > 0 )
			sResult.AppendFormat(_T("\\posy%d"),m_nVPos);
		else 
			sResult.AppendFormat(_T("\\posnegy%d"),m_nVPos);
	}
	switch ( m_eVPos )
	{
		case vp_posyt:		sResult += _T("\\posyt");	break;
		case vp_posyil:		sResult += _T("\\posyil");	break;
		case vp_posyb:		sResult += _T("\\posyb");	break;
		case vp_posyc:		sResult += _T("\\posyc");	break;
		case vp_posyin:		sResult += _T("\\posyin");	break;
		case vp_posyout:	sResult += _T("\\posyout");	break;
	}
	RENDER_RTF_INT( m_bLockAnchor, sResult, _T("abslock") )

	switch ( m_eWrap )
	{
		case tw_wrapdefault:	sResult += _T("\\wrapdefault");	break;
		case tw_wraparound:		sResult += _T("\\wraparound");	break;
		case tw_wraptight:		sResult += _T("\\wraptight");	break;
		case tw_wrapthrough:	sResult += _T("\\wrapthrough");	break;
	}
	RENDER_RTF_INT( m_DropcapType, sResult, _T("dropcapt") )
	RENDER_RTF_INT( m_DropcapLines, sResult, _T("dropcapli") )

	RENDER_RTF_INT( m_nHorSpace, sResult, _T("dfrmtxtx") )
	RENDER_RTF_INT( m_nVerSpace, sResult, _T("dfrmtxty") )

	return sResult;
}
CString RtfFrame::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	CString sFrame;
	RENDER_OOX_INT_ATTRIBUTE( m_nWidth, sFrame, _T("w:w") )
	if(PROP_DEF != m_nHeight)
	{
		if( m_nHeight > 0 )
		{
			RENDER_OOX_INT_ATTRIBUTE( m_nHeight, sFrame, _T("w:h") )
			sFrame += _T(" w:hRule=\"atLeast\"");
		}
		else if( m_nHeight == 0 )
		{
			sFrame += _T(" w:hRule=\"auto\"");
		}
		else
		{
			m_nHeight *= -1;
			RENDER_OOX_INT_ATTRIBUTE( m_nHeight, sFrame, _T("w:h") )
			sFrame += _T(" w:hRule=\"exact\"");
		}
	}
	
	switch ( m_eHRef )
	{
		case hr_phmrg:	sFrame += _T(" w:hAnchor=\"margin\"");	break;
		case hr_phpg:	sFrame += _T(" w:hAnchor=\"page\"");	break;
		case hr_phcol:	sFrame += _T(" w:hAnchor=\"text\"");	break;
	}
	RENDER_OOX_INT_ATTRIBUTE( m_nHPos, sFrame, _T("w:x") )
	switch ( m_eHPos )
	{
		case hp_posxc: sFrame += _T(" w:xAlign=\"center\"");	break;
		case hp_posxi: sFrame += _T(" w:xAlign=\"inside\"");	break;
		case hp_posxo: sFrame += _T(" w:xAlign=\"outside\"");	break;
		case hp_posxl: sFrame += _T(" w:xAlign=\"left\"");		break;
		case hp_posxr: sFrame += _T(" w:xAlign=\"right\"");		break;
	}
	switch ( m_eVRef )
	{	
		case vr_pvmrg:	sFrame += _T(" w:vAnchor=\"margin\"");	break;
		case vr_pvpg:	sFrame += _T(" w:vAnchor=\"page\"");	break;
		case vr_pvpara: sFrame += _T(" w:vAnchor=\"text\"");	break;
	}
	RENDER_OOX_INT_ATTRIBUTE( m_nVPos, sFrame, _T("w:y") )
	switch ( m_eVPos )
	{
		case vp_posyt:		sFrame += _T(" w:yAlign=\"top\"");		break;
		case vp_posyil:		sFrame += _T(" w:yAlign=\"inline\"");	break;
		case vp_posyb:		sFrame += _T(" w:yAlign=\"bottom\"");	break;
		case vp_posyc:		sFrame += _T(" w:yAlign=\"center\"");	break;
		case vp_posyin:		sFrame += _T(" w:yAlign=\"inside\"");	break;
		case vp_posyout:	sFrame += _T(" w:yAlign=\"outside\"");	break;
	}
	RENDER_OOX_BOOL_ATTRIBUTE( m_bLockAnchor, sFrame, _T("w:anchorLock") )
	switch ( m_eWrap )
	{
		//case tw_wrapdefault: sFrame += _T(" w:wrap=\"auto\""));break;
		case tw_wraparound:		sFrame += _T(" w:wrap=\"around\"");		break;
		case tw_wraptight:		sFrame += _T(" w:wrap=\"tight\"");		break;
		case tw_wrapthrough:	sFrame += _T(" w:wrap=\"through\"");	break;
	}
	if( 1 == m_DropcapType )		sFrame += _T(" w:dropCap=\"drop\"");
	else if( 2 == m_DropcapType )	sFrame += _T(" w:dropCap=\"margin\"");
	
	RENDER_OOX_INT_ATTRIBUTE( m_DropcapLines, sFrame, _T("w:lines") )

	if( PROP_DEF != m_nHorSpace )		sFrame.AppendFormat( _T(" w:hSpace=\"%d\""), m_nHorSpace );
	else if( PROP_DEF != m_nAllSpace )	sFrame.AppendFormat( _T(" w:hSpace=\"%d\""), m_nAllSpace );
	
	if( PROP_DEF != m_nVerSpace )		sFrame.AppendFormat( _T(" w:vSpace=\"%d\""), m_nVerSpace );
	else if( PROP_DEF != m_nAllSpace )	sFrame.AppendFormat( _T(" w:vSpace=\"%d\""), m_nAllSpace );

	if( !sFrame.IsEmpty() )
	{
		sResult = _T("<w:framePr ") + sFrame + _T("/>");

		sResult += _T("<w:widowControl w:val=\"0\"/>");
	}
	return sResult;
}
CString RtfParagraphProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	//RENDER_RTF_INT( m_nStyle, sResult, _T("s") );
	RENDER_RTF_BOOL( m_bAutoHyphenation	, sResult, _T("hyphpar") );
	RENDER_RTF_BOOL( m_bInTable			, sResult, _T("intbl") );
	
	if( PROP_DEF != m_nItap && 1 != m_nItap )
		sResult.AppendFormat( _T("\\itap%d"),m_nItap );
	
	RENDER_RTF_BOOL	( m_bKeep			, sResult, _T("keep") );
	RENDER_RTF_BOOL	( m_bKeepNext		, sResult, _T("keepn") );
	RENDER_RTF_INT	( m_nOutlinelevel	, sResult, _T("outlinelevel") );
	RENDER_RTF_BOOL	( m_bPageBB			, sResult, _T("pagebb") );


	switch(m_eAlign)
	{
		case  pa_qc:	sResult += _T("\\qc");		break;  
		case  pa_qj:	sResult += _T("\\qj");		break;  
		case  pa_ql:	sResult += _T("\\ql");		break;  
		case  pa_qr:	sResult += _T("\\qr");		break;  
		case  pa_qd:	sResult += _T("\\qd");		break;  
		case  pa_qk0:	sResult += _T("\\qk0");		break;  
		case  pa_qk10:	sResult += _T("\\qk10");	break;  
		case  pa_qk20:	sResult += _T("\\qk20");	break;  
	}
	switch( m_eFontAlign )
	{
		case fa_faauto:		sResult += _T("\\faauto");	break;
		case fa_fahang:		sResult += _T("\\fahang");	break;
		case fa_facenter:	sResult += _T("\\facenter");break;
		case fa_faroman:	sResult += _T("\\faroman");	break;
		case fa_favar:		sResult += _T("\\favar");	break;
		case fa_fafixed:	sResult += _T("\\fafixed");	break;
	}
	RENDER_RTF_INT( m_nIndFirstLine		, sResult, _T("fi") );
	RENDER_RTF_INT( m_nIndLeft			, sResult, _T("li") );
	RENDER_RTF_INT( m_nIndStart			, sResult, _T("lin") );
	RENDER_RTF_INT( m_nIndRight			, sResult, _T("ri") );
	RENDER_RTF_INT( m_nIndEnd			, sResult, _T("rin") );
	RENDER_RTF_BOOL( m_bIndRightAuto	, sResult, _T("adjustright") );
	RENDER_RTF_BOOL( m_bIndMirror		, sResult, _T("indmirror") );
	RENDER_RTF_INT( m_nSpaceBefore		, sResult, _T("sb") );
	RENDER_RTF_INT( m_nSpaceAfter		, sResult, _T("sa") );
	RENDER_RTF_INT( m_nSpaceBeforeAuto	, sResult, _T("sbauto") );
	RENDER_RTF_INT( m_nSpaceAfterAuto	, sResult, _T("saauto") );
	RENDER_RTF_INT( m_nSpaceBeforeLine	, sResult, _T("lisb") );
	RENDER_RTF_INT( m_nSpaceAfterLine	, sResult, _T("lisa") );
	RENDER_RTF_INT( m_nSpaceBetween		, sResult, _T("sl") );
	RENDER_RTF_INT( m_nSpaceMultiLine	, sResult, _T("slmult") );
	RENDER_RTF_BOOL( m_bContextualSpacing, sResult, _T("contextualspace") );
	
	if( 0 == m_bRtl )				sResult += _T("\\ltrpar");
	else if( PROP_DEF != m_bRtl )	sResult += _T("\\rtlpar");
	
	RENDER_RTF_BOOL( m_bNoWordWrap, sResult, _T("nowwrap") );
	
	if( 0 == m_bSnapToGrid )				sResult += _T("\\nosnaplinegrid");
	else if(  PROP_DEF != m_bSnapToGrid  )	sResult += _T("\\nosnaplinegrid0");
	switch ( m_eTextBoxWrap )
	{
		case tbw_txbxtwno:			sResult += _T("\\txbxtwno");		break;
		case tbw_txbxtwalways:		sResult += _T("\\txbxtwalways");	break;
		case tbw_txbxtwfirstlast:	sResult += _T("\\txbxtwfirstlast");	break;
		case tbw_txbxtwfirst:		sResult += _T("\\txbxtwfirst");		break;
		case tbw_txbxtwlast:		sResult += _T("\\txbxtwlast");		break;
	}
	if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
	{
		sResult.AppendFormat( _T("\\ls%d"), m_nListId );
		sResult.AppendFormat( _T("\\ilvl%d"), m_nListLevel );
	}

	if( true == m_oShading.IsValid() )
		sResult +=  m_oShading.RenderToRtf( oRenderParameter );

	bool border_sides = false;
	if( true == m_oBorderTop.IsValid() )
	{
		sResult += _T("\\brdrt");
		sResult +=  m_oBorderTop.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderLeft.IsValid() )
	{
		sResult += _T("\\brdrl");
		sResult +=  m_oBorderLeft.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		sResult += _T("\\brdrb");
		sResult +=  m_oBorderBottom.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderRight.IsValid() )
	{
		sResult += _T("\\brdrr");
		sResult +=  m_oBorderRight.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderBar.IsValid() )
	{
		sResult += _T("\\brdrbar");
		sResult +=  m_oBorderBar.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderBox.IsValid() && !border_sides)
	{
		sResult += _T("\\box");
		sResult +=  m_oBorderBox.RenderToRtf( oRenderParameter );
	}

	if( true == m_oFrame.IsValid() )
		sResult +=  m_oFrame.RenderToRtf( oRenderParameter );

	RENDER_RTF_BOOL( m_bOverlap, sResult, _T("absnoovrlp") );

	switch ( m_eTextFollow )
	{
		case tf_frmtxlrtb:	sResult += _T("\\frmtxlrtb");	break;
		case tf_frmtxtbrl:	sResult += _T("\\frmtxtbrl");	break;
		case tf_frmtxbtlr:	sResult += _T("\\frmtxbtlr");	break;
		case tf_frmtxlrtbv: sResult += _T("\\frmtxlrtbv");	break;
		case tf_frmtxtbrlv: sResult += _T("\\frmtxtbrlv");	break;
	}
	if( true == m_oTabs.IsValid() )
		sResult +=  m_oTabs.RenderToRtf( oRenderParameter );


	//RENDER_RTF_INT( m_nTableStyle, sResult, _T("yts") );
	//RENDER_RTF_BOOL( m_bStyleFirstRow, sResult, _T("tscfirstrow") );
	//RENDER_RTF_BOOL( m_bStyleLastRow, sResult, _T("tsclastrow") );
	//RENDER_RTF_BOOL( m_bStyleFirstCollumn, sResult, _T("tscfirstcol") );
	//RENDER_RTF_BOOL( m_bStyleLastCollumn, sResult, _T("tsclastcol") );
	//RENDER_RTF_BOOL( m_bStyleOddRowBand, sResult, _T("tscbandhorzodd") );
	//RENDER_RTF_BOOL( m_bStyleEvenRowBand, sResult, _T("tscbandhorzeven") );
	//RENDER_RTF_BOOL( m_bStyleOddColBand, sResult, _T("tscbandvertodd") );
	//RENDER_RTF_BOOL( m_bStyleEvenColBand, sResult, _T("tscbandverteven") );
	//RENDER_RTF_BOOL( m_bStyleNWCell, sResult, _T("tscnwcell") );
	//RENDER_RTF_BOOL( m_bStyleNECell, sResult, _T("tscnecell") );
	//RENDER_RTF_BOOL( m_bStyleSWCell, sResult, _T("tscswcell") );
	//RENDER_RTF_BOOL( m_bStyleSECell, sResult, _T("tscsecell") );


	//дописываем текст списка (для старых reader например  wordPad)
	if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
	{
		RtfListProperty oListProperty;
		RtfDocument* poRtfDocument = static_cast<RtfDocument*>(  oRenderParameter.poDocument );
		RtfListOverrideProperty oListOverrideProperty;
		//ищем по override table
		if( true == poRtfDocument->m_oListOverrideTabel.GetList( m_nListId, oListOverrideProperty ) )
		{
			//Ищем по List Table
			if( true == poRtfDocument->m_oListTabel.GetList( oListOverrideProperty.m_nListID, oListProperty) )
			{
				//дописываем свойства параграфа firstIndent Indent
				RtfListLevelProperty poLevelProp ;
				if( true == oListProperty.GetItem( poLevelProp , m_nListLevel ) )
				{
					sResult += _T("{\\listtext\\pard\\plain");
					sResult +=  poLevelProp.m_oCharProp.RenderToRtf( oRenderParameter );
					//пишем текст 
                    CString strLevelProp = poLevelProp.GenerateListText();
                    CString strChar = RtfChar::renderRtfText( strLevelProp, oRenderParameter.poDocument, NULL );
                    sResult +=  strChar;
					//или картинку
					if( PROP_DEF != poLevelProp.m_nPictureIndex )
					{
						int nIndex = poLevelProp.m_nPictureIndex;

						if( 0 < nIndex && nIndex < poRtfDocument->m_oListTabel.m_aPictureList.GetCount() )
							sResult +=  poRtfDocument->m_oListTabel.m_aPictureList[nIndex]->RenderToRtf( oRenderParameter );
					}
					//ставим tab
					if( PROP_DEF != poLevelProp.m_nFollow )
					{
						switch( poLevelProp.m_nFollow )
						{
							case 0: sResult += _T("\\tab");	break; // tab
							case 1: sResult += _T(" ");		break; // space
							case 2: 
								break; // nothing
						}
					}
					sResult += _T("}");
				}
			}
		}
	}
	return sResult;
}
CString RtfParagraphProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
	CString sResult;
	if( PROP_DEF != m_nStyle )
	{
		RtfStylePtr oCurStile;
		if( true == poRtfDocument->m_oStyleTable.GetStyle( m_nStyle, oCurStile ) )
		{
			sResult += _T("<w:pStyle w:val=\"");
			sResult += oCurStile->m_sName;
			sResult += _T("\"/>") ;
		}
	}
	if		( 0 == m_bAutoHyphenation )	sResult += _T("<w:suppressAutoHyphens/>");
	else if	( 1 == m_bAutoHyphenation )	sResult += _T("<w:suppressAutoHyphens w:val=\"false\"/>");

	RENDER_OOX_BOOL	( m_bKeep			, sResult, _T("w:keepLines") );
	RENDER_OOX_BOOL	( m_bKeepNext		, sResult, _T("w:keepNext") );
	RENDER_OOX_INT	( m_nOutlinelevel	, sResult, _T("w:outlineLvl") );
	RENDER_OOX_BOOL	( m_bPageBB			, sResult, _T("w:pageBreakBefore") );


	switch(m_eAlign)
	{
		case  pa_qc:	sResult += _T("<w:jc w:val=\"center\" />") ;		break;
		case  pa_qj:	sResult += _T("<w:jc w:val=\"both\" />");			break;
		case  pa_ql:	sResult += _T("<w:jc w:val=\"left\" />");			break;
		case  pa_qr:	sResult += _T("<w:jc w:val=\"right\" />");			break;
		case  pa_qd:	sResult += _T("<w:jc w:val=\"distribute\" />");		break;
		case  pa_qk0:	sResult += _T("<w:jc w:val=\"lowKashida\" />");		break;
		case  pa_qk10:	sResult += _T("<w:jc w:val=\"mediumKashida\" />");	break;
		case  pa_qk20:	sResult += _T("<w:jc w:val=\"highKashida\" />");	break;
	}
	switch( m_eFontAlign )
	{
		case fa_faauto:		sResult += _T("<w:textAlignment w:val=\"auto\" />");	break;
		case fa_fahang:		sResult += _T("<w:textAlignment w:val=\"top\" />");		break;
		case fa_facenter:	sResult += _T("<w:textAlignment w:val=\"center\" />");	break;
		case fa_faroman:	sResult += _T("<w:textAlignment w:val=\"baseline\" />");break;
		case fa_favar:		sResult += _T("<w:textAlignment w:val=\"bottom\" />");	break;
		case fa_fafixed:	sResult += _T("<w:textAlignment w:val=\"bottom\" />");	break;
	}
	CString sIndent;
	if( PROP_DEF != m_nIndFirstLine )
	{
		if( m_nIndFirstLine >= 0 )	sIndent.AppendFormat( _T(" w:firstLine=\"%d\""), m_nIndFirstLine );
		else						sIndent.AppendFormat( _T(" w:hanging=\"%d\""), -m_nIndFirstLine );
	}
	else if (m_bOldList && PROP_DEF != m_nIndLeft)
		sIndent.AppendFormat( _T(" w:firstLine=\"%d\""), 0 );

	RENDER_OOX_INT_ATTRIBUTE	( m_nIndLeft,	sIndent, _T("w:left") );
	RENDER_OOX_INT_ATTRIBUTE	( m_nIndRight,	sIndent, _T("w:right") );
	//RENDER_OOX_INT_ATTRIBUTE	( m_nIndStart,	sIndent, _T("w:start") );
	//RENDER_OOX_INT_ATTRIBUTE	( m_nIndEnd,	sIndent, _T("w:end") );
	
	if( !sIndent.IsEmpty() )
	{
        sResult += _T("<w:ind ");
		sResult += sIndent;
		sResult += _T("/>");
	}

	RENDER_OOX_BOOL( m_bIndRightAuto, sResult, _T("w:adjustRightInd") );
	RENDER_OOX_BOOL( m_bIndMirror, sResult, _T("w:mirrorIndents") );

	CString sSpacing;
	if( PROP_DEF == m_nSpaceAfter )		m_nSpaceAfter = 0;
	//if( PROP_DEF == m_nSpaceBefore )	m_nSpaceBefore = 0;

	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBefore,		sSpacing, _T("w:before") );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfter,		sSpacing, _T("w:after") );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBeforeAuto,	sSpacing, _T("w:beforeAutospacing") );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfterAuto,	sSpacing, _T("w:afterAutospacing") );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBeforeLine,	sSpacing, _T("w:beforeLines") );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfterLine,	sSpacing, _T("w:afterLines") );

	if( PROP_DEF != m_nSpaceBetween && 0 != m_nSpaceBetween )
	{
		if( m_nSpaceBetween > 0 )
			sSpacing.AppendFormat(_T(" w:line=\"%d\""), m_nSpaceBetween);
		else
			sSpacing.AppendFormat(_T(" w:line=\"%d\""), -m_nSpaceBetween);
		if( 0 == m_nSpaceMultiLine )
		{
			if( m_nSpaceBetween > 0 )	sSpacing += _T(" w:lineRule=\"atLeast\"");
			else						sSpacing += _T(" w:lineRule=\"exact\"");
		}
	}
	//else
	//	sSpacing.AppendFormat(_T(" w:line=\"240\"")); //по умолчанию - единичный
	if( !sSpacing.IsEmpty() )
	{
		sResult += _T("<w:spacing ");
		sResult +=  sSpacing;
		sResult += _T("/>");
	}

	RENDER_OOX_BOOL( m_bSnapToGrid, sResult, _T("w:snapToGrid") );
	RENDER_OOX_BOOL( m_bContextualSpacing, sResult, _T("w:contextualSpacing") );
	RENDER_OOX_BOOL( m_bRtl, sResult, _T("w:bidi") );
	
	if( 0 == m_bNoWordWrap )				sResult += _T("<w:wordWrap w:val=\"false\"/>");
	else if(  PROP_DEF != m_bNoWordWrap  )	sResult += _T("<w:wordWrap w:val=\"true\"/>");

	switch ( m_eTextBoxWrap )
	{
		case tbw_txbxtwno:			sResult += _T("<w:textboxTightWrap w:val=\"none\"/>");				break;
		case tbw_txbxtwalways:		sResult += _T("<w:textboxTightWrap w:val=\"allLines\"/>");			break;
		case tbw_txbxtwfirstlast:	sResult += _T("<w:textboxTightWrap w:val=\"firstAndLastLine\"/>");	break;
		case tbw_txbxtwfirst:		sResult += _T("<w:textboxTightWrap w:val=\"firstLineOnly\"/>");		break;
		case tbw_txbxtwlast:		sResult += _T("<w:textboxTightWrap w:val=\"lastLineOnly\"/>");		break;
	}
	if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
		sResult.AppendFormat( _T("<w:numPr><w:ilvl w:val=\"%d\" /><w:numId w:val=\"%d\" /></w:numPr>"), m_nListLevel, m_nListId );


	if( true == m_oShading.IsValid() )
		sResult +=  m_oShading.RenderToOOX(oRenderParameter);

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE;
	CString sBorder;
	if( true == m_oBorderBox.IsValid() )
	{
		CString sBorderContent = m_oBorderBox.RenderToOOX(oNewParam);
        sBorder += _T("<w:left ");		sBorder += sBorderContent;	sBorder += _T(" />");
        sBorder += _T("<w:top ");		sBorder += sBorderContent;	sBorder += _T(" />");
        sBorder += _T("<w:right ");		sBorder += sBorderContent;	sBorder += _T(" />");
        sBorder += _T("<w:bottom ");	sBorder += sBorderContent;	sBorder += _T(" />");
	}
	else
	{
		if( true == m_oBorderTop.IsValid() )
		{
			sBorder += _T("<w:top ");		sBorder += m_oBorderTop.RenderToOOX(oNewParam);		sBorder += _T(" />");
		}
		if( true == m_oBorderLeft.IsValid() )
		{
			sBorder += _T("<w:left ");		sBorder += m_oBorderLeft.RenderToOOX(oNewParam);	sBorder += _T(" />");
		}
		if( true == m_oBorderBottom.IsValid() )
		{
			sBorder += _T("<w:bottom ");	sBorder += m_oBorderBottom.RenderToOOX(oNewParam);	sBorder += _T(" />");
		}
		if( true == m_oBorderRight.IsValid() )
		{
			sBorder += _T("<w:right ");		sBorder += m_oBorderRight.RenderToOOX(oNewParam);	sBorder += _T(" />");
		}
	}

	if( true == m_oBorderBar.IsValid() )
	{
        sBorder += _T("<w:between ");
		sBorder += m_oBorderBar.RenderToOOX(oNewParam);
		sBorder += _T("/>");
	}
	if( false == sBorder.IsEmpty() )
	{
        sResult += _T("<w:pBdr>");
		sResult += sBorder;
		sResult += _T("</w:pBdr>");
	}

	if( true == m_oFrame.IsValid() )
		sResult +=  m_oFrame.RenderToOOX( oRenderParameter );

	RENDER_OOX_BOOL( m_bOverlap, sResult, _T("w:suppressOverlap") );

	switch ( m_eTextFollow )
	{
		case tf_frmtxlrtb:	sResult += _T("<w:textFlow w:val=\"lrtb\" />");	break;
		case tf_frmtxtbrl:	sResult += _T("<w:textFlow w:val=\"tbrl\" />");	break;
		case tf_frmtxbtlr:	sResult += _T("<w:textFlow w:val=\"btlr\" />");	break;
		case tf_frmtxlrtbv: sResult += _T("<w:textFlow w:val=\"lrtbV\" />");break;
		case tf_frmtxtbrlv: sResult += _T("<w:textFlow w:val=\"tbrlV\" />");break;
	}
	if( true == m_oTabs.IsValid() )
	{
		sResult += m_oTabs.RenderToOOX( oRenderParameter );
	}
	CString sCharProp = m_oCharProperty.RenderToOOX( oRenderParameter );
	
	if( !sCharProp.IsEmpty() )
	{
        sResult += _T("<w:rPr>");
		sResult += sCharProp;
		sResult += _T("</w:rPr>");
	}
	return sResult;
}
CString RtfCellProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	RENDER_RTF_BOOL( m_bMergeFirst, sResult, _T("clmgf") )
	RENDER_RTF_BOOL( m_bMerge, sResult, _T("clmrg") )
	RENDER_RTF_BOOL( m_bMergeFirstVertical, sResult, _T("clvmgf") )
	RENDER_RTF_BOOL( m_bMergeVertical, sResult, _T("clvmrg") )

	RENDER_RTF_BOOL( m_bFitText, sResult, _T("clFitText") )
	RENDER_RTF_BOOL( m_bNoWrap, sResult, _T("clNoWrap") )

	RENDER_RTF_INT( m_nIsPaddingLeft, sResult, _T("clpadfl") )
	RENDER_RTF_INT( m_nPaddingLeft, sResult, _T("clpadl") )
	RENDER_RTF_INT( m_nIsPaddingRight, sResult, _T("clpadfr") )
	RENDER_RTF_INT( m_nPaddingRight, sResult, _T("clpadr") )
	RENDER_RTF_INT( m_nIsPaddingTop, sResult, _T("clpadft") )
	RENDER_RTF_INT( m_nPaddingTop, sResult, _T("clpadt") )
	RENDER_RTF_INT( m_nIsPaddingBottom, sResult, _T("clpadfb") )
	RENDER_RTF_INT( m_nPaddingBottom, sResult, _T("clpadb") )

	RENDER_RTF_INT( m_nIsSpacingLeft, sResult, _T("clspfl") )
	RENDER_RTF_INT( m_nSpacingLeft, sResult, _T("clspl") )
	RENDER_RTF_INT( m_nIsSpacingRight, sResult, _T("clspl") )
	RENDER_RTF_INT( m_nSpacingRight, sResult, _T("clspr") )

	RENDER_RTF_INT( m_nIsSpacingTop, sResult, _T("clspft") )
	RENDER_RTF_INT( m_nSpacingTop, sResult, _T("clspt") )
	RENDER_RTF_INT( m_nIsSpacingBottom, sResult, _T("clspfb") )
	RENDER_RTF_INT( m_nSpacingBottom, sResult, _T("clspb") )
	
	RENDER_RTF_BOOL( m_bHideMark, sResult, _T("clhidemark") )

	if( true == m_oBorderDiagonalLR.IsValid() )
	{
		sResult += _T("\\cldglu");
		sResult +=  m_oBorderDiagonalLR.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderDiagonalRL.IsValid() )
	{
		sResult += _T("\\cldgll");
		sResult +=  m_oBorderDiagonalRL.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderLeft.IsValid() )
	{
		sResult += _T("\\clbrdrl");
		sResult +=  m_oBorderLeft.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		sResult += _T("\\clbrdrt");
		sResult +=  m_oBorderTop.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderRight.IsValid() )
	{
		sResult += _T("\\clbrdrr");
		sResult +=  m_oBorderRight.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		sResult += _T("\\clbrdrb");
		sResult +=  m_oBorderBottom.RenderToRtf( oRenderParameter );
	}
	if( true == m_oShading.IsValid() )
		sResult +=  m_oShading.RenderToRtf( oRenderParameter );

	switch( m_eAlign )
	{
		case ca_Top:	sResult += _T("\\clvertalt");	break;
		case ca_Center: sResult += _T("\\clvertalc");	break;
		case ca_Bottom: sResult += _T("\\clvertalb");	break;
	}
	switch( m_oCellFlow )
	{
		case cf_lrtb:	sResult += _T("\\cltxlrtb");	break;
		case cf_tbrl:	sResult += _T("\\cltxtbrl");	break;
		case cf_btlr:	sResult += _T("\\cltxbtlr");	break;
		case cf_lrtbv:	sResult += _T("\\cltxlrtbv");	break;
		case cf_tbrlv:	sResult += _T("\\cltxtbrlv");	break;
	}
	RENDER_RTF_INT( m_nWidth, sResult, _T("clwWidth") )
	switch( m_eWidthUnits )
	{
		case mu_none:		sResult += _T("\\clftsWidth0");	break;
		case mu_Auto:		sResult += _T("\\clftsWidth1");	break;
		case mu_Percent:	sResult += _T("\\clftsWidth2");	break;
		case mu_Twips:		sResult += _T("\\clftsWidth3");	break;
	}
	//ATLASSERT(PROP_DEF != m_nCellx );
	sResult.AppendFormat(_T("\\cellx%d"),m_nCellx);

	return sResult;
}
CString RtfCellProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( 1 == m_bMergeFirst )			sResult += _T("<w:hMerge w:val=\"restart\"/>");
	if( 1 == m_bMerge )					sResult += _T("<w:hMerge w:val=\"continue\"/>");
	if( 1 == m_bMergeFirstVertical )	sResult += _T("<w:vMerge w:val=\"restart\"/>");
	if( 1 == m_bMergeVertical)			sResult += _T("<w:vMerge w:val=\"continue\"/>");

	RENDER_OOX_BOOL( m_bFitText, sResult, _T("w:tcFitText") )
	RENDER_OOX_BOOL( m_bNoWrap, sResult, _T("w:noWrap") )

	CString sMargin;
	if( 3 == m_nIsPaddingLeft && PROP_DEF != m_nPaddingLeft)
		sResult.AppendFormat(_T("<w:left w:w=\"%d\" w:type=\"dxa\"/>"),m_nPaddingLeft);
	if( 3 == m_nIsPaddingRight && PROP_DEF != m_nPaddingRight)
		sResult.AppendFormat(_T("<w:right w:w=\"%d\" w:type=\"dxa\"/>"),m_nPaddingRight);
	if( 3 == m_nIsPaddingTop && PROP_DEF != m_nPaddingTop)
		sResult.AppendFormat(_T("<w:top w:w=\"%d\" w:type=\"dxa\"/>"),m_nPaddingTop);
	if( 3 == m_nIsPaddingBottom && PROP_DEF != m_nPaddingBottom)
		sResult.AppendFormat(_T("<w:bottom w:w=\"%d\" w:type=\"dxa\"/>"),m_nPaddingBottom);
	
	if( !sMargin.IsEmpty() )
	{
        sResult += _T("<w:tcMar>");
		sResult += sMargin;
		sResult += _T("</w:tcMar>");
	}

	//if( true == m_bIsSpacingLeft )
	//	sResult += _T("trspdl3"));
	//else	
	//	sResult += _T("trspdl0"));
	//if( PROP_DEF != m_nSpacingLeft )
	//	sResult.AppendFormat(_T("trspdfl%d"),m_nSpacingLeft);
	//if( true == m_bIsSpacingRight )
	//	sResult += _T("trspdr3"));
	//else	
	//	sResult += _T("trspdr0"));
	//if( PROP_DEF != m_nSpacingRight )
	//	sResult.AppendFormat(_T("trspdfr%d"),m_nSpacingRight);
	//if( true == m_bIsSpacingTop )
	//	sResult += _T("trspdt3"));
	//else	
	//	sResult += _T("trspdt0"));
	//if( PROP_DEF != m_nSpacingTop )
	//	sResult.AppendFormat(_T("trspdft%d"),m_nSpacingTop);
	//if( true == m_bIsSpacingBottom )
	//	sResult += _T("trspdb3"));
	//else	
	//	sResult += _T("trspdb0"));
	//if( PROP_DEF != m_nSpacingBottom )
	//	sResult.AppendFormat(_T("trspdfb%d"),m_nSpacingBottom);

	if( PROP_DEF != m_nWidth )
	{
		if( mu_Percent ==  m_eWidthUnits )
			sResult.AppendFormat( _T("<w:tcW w:type=\"pct\" w:w=\"%d%%\"/>"), m_nWidth);
		else if( mu_Twips ==  m_eWidthUnits )
			sResult.AppendFormat( _T("<w:tcW w:type=\"dxa\" w:w=\"%d\"/>"), m_nWidth );
	}
	RENDER_OOX_BOOL( m_bHideMark, sResult, _T("w:hideMark") )

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_TAG;
	
	CString sBorder;
	if( true == m_oBorderDiagonalLR.IsValid() )
	{
		oNewParam.sValue = _T("w:tl2br");
		sBorder +=  m_oBorderDiagonalLR.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderDiagonalRL.IsValid() )
	{
		oNewParam.sValue = _T("w:tr2bl");
		sBorder +=  m_oBorderDiagonalRL.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderLeft.IsValid() )
	{
		oNewParam.sValue = _T("w:left");
		sBorder +=  m_oBorderLeft.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderTop.IsValid() )
	{
		oNewParam.sValue = _T("w:top");
		sBorder +=  m_oBorderTop.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderRight.IsValid() )
	{
		oNewParam.sValue = _T("w:right");
		sBorder +=  m_oBorderRight.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		oNewParam.sValue = _T("w:bottom");
		sBorder +=  m_oBorderBottom.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderInsideH.IsValid() )
	{
		oNewParam.sValue = _T("w:insideH");
		sResult += m_oBorderInsideH.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderInsideV.IsValid() )
	{
		oNewParam.sValue = _T("w:insideV");
		sResult += m_oBorderInsideV.RenderToOOX(oNewParam);
	}
	if( false == sBorder.IsEmpty() )
	{
        sResult += _T("<w:tcBorders>");
		sResult += sBorder;
		sResult += _T("</w:tcBorders>");
	}

	if( true == m_oShading.IsValid() )
	{
		if (PROP_DEF != m_nShadingPctFrom && PROP_DEF != m_oShading.m_nValue && m_oShading.m_nValue != 0)
		{
			// todooo тут цвет подложки подкладывается от ячейки, таблицы или еще какой хрени
			// пока берем второй цвет паттерна Romanization_Armenian.rtf
			m_oShading.m_nBackColor = m_oShading.m_nForeColor * (m_oShading.m_nValue / 10000.);
		}
		sResult +=  m_oShading.RenderToOOX(oRenderParameter);
	}
	
	switch( m_eAlign )
	{
		case ca_Top:	sResult += _T("<w:vAlign w:val=\"top\"/>");		break;
		case ca_Center: sResult += _T("<w:vAlign w:val=\"center\"/>");	break;
		case ca_Bottom: sResult += _T("<w:vAlign w:val=\"bottom\"/>");	break;
	}
	switch( m_oCellFlow )
	{
		case cf_lrtb:	sResult += _T("<w:textDirection w:val=\"lrTb\"/>");		break;
		case cf_tbrl:	sResult += _T("<w:textDirection w:val=\"tbRl\"/>");		break;
		case cf_btlr:	sResult += _T("<w:textDirection w:val=\"btLr\"/>");		break;
		case cf_lrtbv:	sResult += _T("<w:textDirection w:val=\"lrTbV\"/>");	break;
		case cf_tbrlv:	sResult += _T("<w:textDirection w:val=\"tbRlV\"/>");	break;

	}

	RENDER_OOX_INT( m_nSpan, sResult, _T("w:gridSpan") );

	//CString scnfStyle;
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstRow, scnfStyle, _T("w:firstRow") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastRow, scnfStyle, _T("w:lastRow") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstCol, scnfStyle, _T("w:firstColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastCol, scnfStyle, _T("w:lastColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddRowBand, scnfStyle, _T("w:oddHBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenRowBand, scnfStyle, _T("w:evenHBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddColBand, scnfStyle, _T("w:oddVBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenColBand, scnfStyle, _T("w:evenvBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNWCell, scnfStyle, _T("w:firstRowFirstColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNECell, scnfStyle, _T("w:firstRowLastColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSWCell, scnfStyle, _T("w:lastRowFirstColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSECell, scnfStyle, _T("w:lastRowLastColumn") )
	//if( !scnfStyle.IsEmpty() )
	//	sResult += _T("<w:cnfStyle ") + scnfStyle + T("(/>");

	//cellx

	return sResult;
}

CString RtfTableProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	
	RENDER_RTF_BOOL( m_bBidi, sResult, _T("taprtl") )
	if( PROP_DEF == m_nAutoFit || 1 == m_nAutoFit )
			sResult += _T("\\trautofit1");
	else if( 0 == m_nAutoFit )
			sResult += _T("\\trautofit0");
	RENDER_RTF_INT( m_nAutoFit, sResult, _T("trautofit") )
	RENDER_RTF_INT( m_nGraph, sResult, _T("trgaph") )
	RENDER_RTF_INT( nTableIndent, sResult, _T("tblind") )
	RENDER_RTF_INT( nTableIndentUnits, sResult, _T("tblindtype") )

	RENDER_RTF_INT( m_nWrapLeft, sResult, _T("tdfrmtxtLeft") )
	RENDER_RTF_INT( m_nWrapRight, sResult, _T("tdfrmtxtRight") )
	RENDER_RTF_INT( m_nWrapTop, sResult, _T("tdfrmtxtTop") )
	RENDER_RTF_INT( m_nWrapBottom, sResult, _T("tdfrmtxtBottom") )
	RENDER_RTF_BOOL( m_bOverlap, sResult, _T("tabsnoovrlp") )

	switch ( m_eHRef )
	{
		case hr_phmrg:	sResult += _T("\\tphmrg");	break;
		case hr_phpg:	sResult += _T("\\tphpg");	break;
		case hr_phcol:	sResult += _T("\\tphcol");	break;
	}
	if( PROP_DEF != m_nHPos )
	{
		if( m_nHPos > 0 )
			sResult.AppendFormat(_T("\\tposx%d"),m_nHPos);
		else
			sResult.AppendFormat(_T("\\tposnegx%d"),m_nHPos);
	}
	switch ( m_eHPos )
	{
		case hp_posxc: sResult += _T("\\tposxc");	break;
		case hp_posxi: sResult += _T("\\tposxi");	break;
		case hp_posxo: sResult += _T("\\tposxo");	break;
		case hp_posxl: sResult += _T("\\tposxl");	break;
		case hp_posxr: sResult += _T("\\tposxr");	break;
	}

	switch ( m_eVRef )
	{	
		case vr_pvmrg:	sResult += _T("\\tpvmrg");	break;
		case vr_pvpg:	sResult += _T("\\tpvpg");	break;
		case vr_pvpara: sResult += _T("\\tpvpara");	break;
	}
	if( PROP_DEF != m_nVPos )
	{
		if( m_nVPos > 0 )
			sResult.AppendFormat(_T("\\tposy%d"),m_nVPos);
		else 
			sResult.AppendFormat(_T("\\tposnegy%d"),m_nVPos);
	}
	switch ( m_eVPos )
	{
		case vp_posyt:		sResult += _T("\\tposyt");		break;
		case vp_posyil:		sResult += _T("\\tposyil");		break;
		case vp_posyb:		sResult += _T("\\tposyb");		break;
		case vp_posyc:		sResult += _T("\\tposyc");		break;
		case vp_posyin:		sResult += _T("\\tposyin");		break;
		case vp_posyout:	sResult += _T("\\tposyout");	break;
	}
	RENDER_RTF_INT( m_nLeft, sResult, _T("trleft") )

	RENDER_RTF_INT( m_nDefCellMarLeft, sResult, _T("trpaddl") );
	RENDER_RTF_INT( m_nDefCellMarRight, sResult, _T("trpaddr") );
	RENDER_RTF_INT( m_nDefCellMarTop, sResult, _T("trpaddt") );
	RENDER_RTF_INT( m_nDefCellMarBottom, sResult, _T("trpaddb") );
	RENDER_RTF_INT( m_nDefCellMarLeftUnits, sResult, _T("trpaddfl") );
	RENDER_RTF_INT( m_nDefCellMarRightUnits, sResult, _T("trpaddfr") );
	RENDER_RTF_INT( m_nDefCellMarTopUnits, sResult, _T("trpaddft") );
	RENDER_RTF_INT( m_nDefCellMarBottomUnits, sResult, _T("trpaddfb") );
	RENDER_RTF_INT( m_nDefCellSpBottom, sResult, _T("trspdb") );
	RENDER_RTF_INT( m_nDefCellSpLeft, sResult, _T("trspdl") );
	RENDER_RTF_INT( m_nDefCellSpRight, sResult, _T("trspdr") );
	RENDER_RTF_INT( m_nDefCellSpTop, sResult, _T("trspdt") );
	RENDER_RTF_INT( m_nDefCellSpBottomUnits, sResult, _T("trspdfb") );
	RENDER_RTF_INT( m_nDefCellSpLeftUnits, sResult, _T("trspdfl") );
	RENDER_RTF_INT( m_nDefCellSpRightUnits, sResult, _T("trspdfr") );
	RENDER_RTF_INT( m_nDefCellSpTopUnits, sResult, _T("trspdft") );

	if( m_oBorderLeft.IsValid() == true )
	{
		sResult += _T("\\trbrdrl");
		sResult += m_oBorderLeft.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderRight.IsValid() == true )
	{
		sResult += _T("\\trbrdrr");
		sResult += m_oBorderRight.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderTop.IsValid() == true )
	{
		sResult += _T("\\trbrdrt");
		sResult += m_oBorderTop.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderBottom.IsValid() == true )
	{
		sResult += _T("\\trbrdrb");
		sResult += m_oBorderBottom.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderVert.IsValid() == true && m_bAutoNoColBand != 1)
	{
		sResult += _T("\\trbrdrv");
		sResult += m_oBorderVert.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderHor.IsValid() == true  && m_bAutoNoRowBand != 1)
	{
		sResult += _T("\\trbrdrh");
		sResult += m_oBorderHor.RenderToRtf( oRenderParameter );
	}
	if( m_oShading.IsValid() == true )
		sResult += m_oShading.RenderToRtf( oRenderParameter );

	//RENDER_RTF_INT( m_nStyle, sResult, _T("ts") )

	RENDER_RTF_BOOL( m_bAutoFirstRow, sResult, _T("tbllkhdrrows") )
	RENDER_RTF_BOOL( m_bAutoLastRow, sResult, _T("tbllklastrow") )
	RENDER_RTF_BOOL( m_bAutoFirstCol, sResult, _T("tbllkhdrcols") )
	RENDER_RTF_BOOL( m_bAutoLastCol, sResult, _T("tbllklastcol") )
	RENDER_RTF_BOOL( m_bAutoNoRowBand, sResult, _T("tbllknorowband") )
	RENDER_RTF_BOOL( m_bAutoNoColBand, sResult, _T("tbllknocolband") )

	RENDER_RTF_INT( m_nRowBandSize, sResult, _T("tscbandsh") )
	RENDER_RTF_INT( m_nColBandSize, sResult, _T("tscbandsv") )

	return sResult;
}
CString RtfTableProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( PROP_DEF != m_nStyle )
	{
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		RtfStylePtr oCurStyle;
		if( true == poDocument->m_oStyleTable.GetStyle( m_nStyle, oCurStyle) )
		{
			sResult += _T("<w:tblStyle w:val=\"");
			sResult += oCurStyle->m_sName;
			sResult += _T("\"/>");
		}
	}
	RENDER_OOX_BOOL( m_bBidi, sResult, _T("w:bidiVisual") );

	if( PROP_DEF == m_nAutoFit || 0 == m_nAutoFit )
		sResult += _T("<w:tblLayout w:type=\"fixed\"/>");
	else if( 1 == m_nAutoFit )
		sResult += _T("<w:tblLayout w:type=\"autofit\"/>");

	//сделаем не по документации, а как все остальные юниты !!!
	if( PROP_DEF != nTableIndent && 1 != nTableIndentUnits  )
	{
		switch(nTableIndentUnits)
		{
			//case 0: sResult += _T("<w:tblInd w:w=\"0\" w:type=\"dxa\"/>") );break;
			//case 2: sResult.AppendFormat( _T("<w:tblInd w:w=\"%d%%\" w:type=\"pct\"/>"), nTableIndent );break;
			case 3: sResult.AppendFormat( _T("<w:tblInd w:w=\"%d\" w:type=\"dxa\"/>"), nTableIndent );break;
			//default: sResult.AppendFormat( _T("<w:tblInd w:w=\"%d\" w:type=\"dxa\"/>"), nTableIndent );break;
		}
	}

	if( 1 == m_bOverlap )
		sResult += _T("<w:tblOverlap w:val=\"overlap\"/>");

	switch( m_eJust )
	{
		case rj_trql : sResult += _T("<w:jc w:val=\"left\"/>");		break;
		case rj_trqr : sResult += _T("<w:jc w:val=\"right\"/>");	break;
		case rj_trqc : sResult += _T("<w:jc w:val=\"center\"/>");	break;
	}

	CString sFloatingPosition;
	if( PROP_DEF != m_nWrapLeft )
		sFloatingPosition.AppendFormat( _T(" w:leftFromText=\"%d\""), m_nWrapLeft );
	if( PROP_DEF != m_nWrapRight )
		sFloatingPosition.AppendFormat( _T(" w:rightFromText=\"%d\""), m_nWrapRight );
	if( PROP_DEF != m_nWrapTop )
		sFloatingPosition.AppendFormat( _T(" w:topFromText=\"%d\""), m_nWrapTop );
	if( PROP_DEF != m_nWrapBottom )
		sFloatingPosition.AppendFormat( _T(" w:bottomFromText=\"%d\""), m_nWrapBottom );

	switch ( m_eHRef )
	{
		case hr_phmrg:	sFloatingPosition += _T(" w:horzAnchor=\"margin\"");break;
		case hr_phpg:	sFloatingPosition += _T(" w:horzAnchor=\"page\"");	break;
		case hr_phcol:	sFloatingPosition += _T(" w:horzAnchor=\"text\"");	break;
	}
	if( PROP_DEF != m_nHPos )
		sFloatingPosition.AppendFormat( _T(" w:tblpX=\"%d\""), m_nHPos );
	//else
	switch ( m_eHPos )
	{
		case hp_posxc: sFloatingPosition += _T(" w:tblpXSpec=\"center\"");	break;
		case hp_posxi: sFloatingPosition += _T(" w:tblpXSpec=\"inside\"");	break;
		case hp_posxo: sFloatingPosition += _T(" w:tblpXSpec=\"outside\"");	break;
		case hp_posxl: sFloatingPosition += _T(" w:tblpXSpec=\"left\"");	break;
		case hp_posxr: sFloatingPosition += _T(" w:tblpXSpec=\"right\"");	break;
	}

	switch ( m_eVRef )
	{	
		case vr_pvmrg:	sFloatingPosition += _T(" w:vertAnchor=\"margin\"");break;
		case vr_pvpg:	sFloatingPosition += _T(" w:vertAnchor=\"page\"");	break;
		case vr_pvpara:	sFloatingPosition += _T(" w:vertAnchor=\"text\"");	break;
	}

	if( PROP_DEF != m_nVPos )
		sFloatingPosition.AppendFormat( _T(" w:tblpY=\"%d\""), m_nVPos );
	//else
	switch ( m_eVPos )
	{
		case vp_posyt:		sFloatingPosition += _T(" w:tblpYSpec=\"top\"");		break;
		case vp_posyil:		sFloatingPosition += _T(" w:tblpYSpec=\"inline\"");		break;
		case vp_posyb:		sFloatingPosition += _T(" w:tblpYSpec=\"bottom\"");		break;
		case vp_posyc:		sFloatingPosition += _T(" w:tblpYSpec=\"center\"");		break;
		case vp_posyin:		sFloatingPosition += _T(" w:tblpYSpec=\"inside\"");		break;
		case vp_posyout:	sFloatingPosition += _T(" w:tblpYSpec=\"outside\"");	break;
	}
	if( false == sFloatingPosition.IsEmpty() )
	{
        sResult += _T("<w:tblpPr ");
		sResult += sFloatingPosition;
		sResult += _T("/>");
	}

	if( PROP_DEF != m_nWidth && ( 2 == m_eMUWidth || 3 == m_eMUWidth ))
	{
		if( 2 == m_eMUWidth )
			sResult.AppendFormat( _T("<w:tblW w:w=\"%d\" w:type=\"pct\"/>"), m_nWidth );
		else
			sResult.AppendFormat( _T("<w:tblW w:w=\"%d\" w:type=\"dxa\"/>"), m_nWidth );
	}
	else
		sResult += _T("<w:tblW w:w=\"0\" w:type=\"auto\"/>");

	CString sDefCellMargins;
	if( PROP_DEF != m_nDefCellMarBottom && 3 == m_nDefCellMarBottomUnits )
		sDefCellMargins.AppendFormat( _T("<w:bottom w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarBottom );
	if( PROP_DEF != m_nDefCellMarLeft && 3 == m_nDefCellMarLeftUnits )
		sDefCellMargins.AppendFormat( _T("<w:left w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarLeft );
	if( PROP_DEF != m_nDefCellMarRight && 3 == m_nDefCellMarRightUnits )
		sDefCellMargins.AppendFormat( _T("<w:right w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarRight );
	if( PROP_DEF != m_nDefCellMarTop && 3 == m_nDefCellMarTopUnits )
		sDefCellMargins.AppendFormat( _T("<w:top w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarTop );
	
	if( !sDefCellMargins.IsEmpty() )
        sResult += _T("<w:tblCellMar>") + sDefCellMargins + _T("</w:tblCellMar>");
	
	if( PROP_DEF != m_nDefCellSpBottom && 3 == m_nDefCellSpBottomUnits ) 
		sResult.AppendFormat( _T("<w:tblCellSpacing w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellSpBottom );

	//if( PROP_DEF != m_nLeft )
	//	sResult.AppendFormat(_T("<w:tblInd w:w=\"%d\" w:type=\"dxa\"/>"), m_nLeft );

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE;
	CString sBorders;
	if( m_oBorderLeft.IsValid() )	sBorders += _T("<w:left ")		+ m_oBorderLeft.RenderToOOX(oNewParam)	+ _T("/>");
	if( m_oBorderRight.IsValid() )	sBorders += _T("<w:right ")		+ m_oBorderRight.RenderToOOX(oNewParam)	+ _T("/>");
	if( m_oBorderTop.IsValid() )	sBorders += _T("<w:top ")		+ m_oBorderTop.RenderToOOX(oNewParam)	+ _T("/>");
	if( m_oBorderBottom.IsValid() )	sBorders += _T("<w:bottom ")	+ m_oBorderBottom.RenderToOOX(oNewParam) + _T("/>");
	if( m_oBorderVert.IsValid() )	sBorders += _T("<w:insideV ")	+ m_oBorderVert.RenderToOOX(oNewParam)	+ _T("/>");
	if( m_oBorderHor.IsValid() )	sBorders += _T("<w:insideH ")	+ m_oBorderHor.RenderToOOX(oNewParam)	+ _T("/>");
	
	if( !sBorders.IsEmpty() )
		sResult += _T("<w:tblBorders>") + sBorders+ _T("</w:tblBorders>");

	if( m_oShading.IsValid() == true )
		sResult += m_oShading.RenderToOOX(oRenderParameter);


	//if( RENDER_TO_OOX_PARAM_STYLES == oRenderParameter.nType )
	//{
	//	RENDER_OOX_INT( m_nRowBandSize, sResult, _T("w:tblStyleRowBandSize") )
	//	RENDER_OOX_INT( m_nColBandSize, sResult, _T("w:tblStyleColBandSize ") )
	//}
	//else
	//{
	//	CString sTableLook;
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoFirstRow, sTableLook, _T("w:firstRow") )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoLastRow, sTableLook, _T("w:lastRow") )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoFirstCol, sTableLook, _T("w:firstColumn") )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoLastCol, sTableLook, _T("w:lastColumn") )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoNoRowBand, sTableLook, _T("w:noHBand") )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoNoColBand, sTableLook, _T("w:noVBand") )

	//	if( false == sTableLook.IsEmpty() )
	//		sResult += _T("<w:tblLook ") + sTableLook + _T("/>"));
	//}

	if( !sResult.IsEmpty() )
		sResult = _T("<w:tblPr>") + sResult + _T("</w:tblPr>");
	return sResult;
}

CString RtfRowProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	if( RENDER_TO_RTF_PARAM_NO_WROWD != oRenderParameter.nType )
		sResult += _T("\\trowd");

	RENDER_RTF_INT( m_nIndex, sResult, _T("irow") );
	RENDER_RTF_INT( m_nBandIndex, sResult, _T("irowband") );
	RENDER_RTF_BOOL( m_bLastRow, sResult, _T("lastrow") );
	RENDER_RTF_INT( m_nAutoFit, sResult, _T("trautofit") );
	RENDER_RTF_BOOL( m_bIsHeader, sResult, _T("trhdr") );
	RENDER_RTF_BOOL( m_bKeep, sResult, _T("trkeep") );

	switch( m_eJust )
	{
		case rj_trql : sResult += _T("\\trql");	break;
		case rj_trqr : sResult += _T("\\trqr");	break;
		case rj_trqc : sResult += _T("\\trqc");	break;
	}
	RENDER_RTF_INT( m_nHeight, sResult, _T("trrh") );

	RENDER_RTF_INT( m_nWidth, sResult, _T("trwWidth") );
	switch( m_eMUWidth )
	{
		case mu_Auto:		sResult += _T("\\trftsWidth1");	break;
		case mu_Percent:	sResult += _T("\\trftsWidth2");	break;
		case mu_Twips:		sResult += _T("\\trftsWidth3");	break;
	}
	RENDER_RTF_INT( m_nWidthStartInvCell, sResult, _T("trwWidthB") );
	switch( m_eMUStartInvCell )
	{
		case mu_Auto:		sResult += _T("\\trftsWidthB1");	break;
		case mu_Percent:	sResult += _T("\\trftsWidthB2");	break;
		case mu_Twips:		sResult += _T("\\trftsWidthB3");	break;
	}
	RENDER_RTF_INT( m_nWidthEndInvCell, sResult, _T("trwWidthA") );
	switch( m_eMUEndInvCell )
	{
		case mu_Auto:		sResult += _T("\\trftsWidthA1");	break;
		case mu_Percent:	sResult += _T("\\trftsWidthA2");	break;
		case mu_Twips:		sResult += _T("\\trftsWidthA3");	break;
	}
	sResult +=  RtfTableProperty::RenderToRtf(  oRenderParameter  );

	for( int i = 0; i < (int)m_aArray.size(); i++ )
		sResult +=  m_aArray[i].RenderToRtf(  oRenderParameter  );

	return sResult;
}
CString RtfRowProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	RENDER_OOX_BOOL( m_bIsHeader, sResult, _T("w:tblHeader") )
	RENDER_OOX_BOOL( m_bKeep, sResult, _T("w:cantSplit") )

	switch( m_eJust )
	{
		case rj_trql : sResult += _T("<w:jc w:val=\"left\"/>");		break;
		case rj_trqr : sResult += _T("<w:jc w:val=\"right\"/>");	break;
		case rj_trqc : sResult += _T("<w:jc w:val=\"center\"/>");	break;
	}
	if( PROP_DEF != m_nHeight )
	{
		if( m_nHeight > 0 )
			sResult.AppendFormat(_T("<w:trHeight w:val=\"%d\" w:hRule=\"atLeast\"/>"), m_nHeight );
		else if( m_nHeight < 0 )
			sResult.AppendFormat(_T("<w:trHeight w:val=\"%d\" w:hRule=\"exact\"/>"), -m_nHeight );
	}

	if( PROP_DEF != m_nWidthStartInvCell )
	{
		switch( m_eMUStartInvCell )
		{
			case mu_Percent:	sResult.AppendFormat(_T("<w:wBefore w:type=\"pct\" w:w=\"%d%%\"/>"), m_nWidthStartInvCell);break;
			case mu_Twips:		sResult.AppendFormat(_T("<w:wBefore w:type=\"dxa\" w:w=\"%d\"/>"), m_nWidthStartInvCell);break;
		}
	}
	if( PROP_DEF != m_nWidthEndInvCell )
	{
		switch( m_eMUEndInvCell )
		{
			case mu_Percent:	sResult.AppendFormat(_T("<w:wAfter w:type=\"pct\" w:w=\"%d%%\"/>"), m_nWidthEndInvCell);break;
			case mu_Twips:		sResult.AppendFormat(_T("<w:wAfter w:type=\"dxa\" w:w=\"%d\"/>"), m_nWidthEndInvCell);break;
		}
	}
	RENDER_OOX_INT( m_nGridBefore, sResult, _T("w:gridBefore") )
	RENDER_OOX_INT( m_nGridAfter, sResult, _T("w:gridAfter") )

	//CString scnfStyle;
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstRow, scnfStyle, _T("w:firstRow") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastRow, scnfStyle, _T("w:lastRow") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstCol, scnfStyle, _T("w:firstColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastCol, scnfStyle, _T("w:lastColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddRowBand, scnfStyle, _T("w:oddHBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenRowBand, scnfStyle, _T("w:evenHBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddColBand, scnfStyle, _T("w:oddVBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenColBand, scnfStyle, _T("w:evenvBand") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNWCell, scnfStyle, _T("w:firstRowFirstColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNECell, scnfStyle, _T("w:firstRowLastColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSWCell, scnfStyle, _T("w:lastRowFirstColumn") )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSECell, scnfStyle, _T("w:lastRowLastColumn") )
	//if(!scnfStyle.IsEmpty() )
	//	sResult += _T("<w:cnfStyle ") + scnfStyle + _T("/>") );

//	_bstr_t hk;
	return sResult;
}

CString RtfInformation::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;

	if( _T("") != m_sTitle )
	{
        sResult += _T("{\\title ");		sResult += RtfChar::renderRtfText( m_sTitle, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sSubject )
	{
        sResult += _T("{\\subject ");	sResult += RtfChar::renderRtfText( m_sSubject, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sAuthor )
	{
        sResult += _T("{\\author ");	sResult += RtfChar::renderRtfText( m_sAuthor, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sManager )
	{
        sResult += _T("{\\manager ");	sResult += RtfChar::renderRtfText( m_sManager, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sCompany )
	{
        sResult += _T("{\\company ");	sResult += RtfChar::renderRtfText( m_sCompany, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sOperator )
	{
        sResult += _T("{\\operator ");	sResult += RtfChar::renderRtfText( m_sOperator, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sCategory )
	{
        sResult += _T("{\\category ");	sResult += RtfChar::renderRtfText( m_sCategory, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sKeywords )
	{
        sResult += _T("{\\keywords ");	sResult += RtfChar::renderRtfText( m_sKeywords, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( _T("") != m_sComment )
	{
        sResult += _T("{\\comment ");	sResult += RtfChar::renderRtfText( m_sComment, oRenderParameter.poDocument );	sResult += _T("}");
	}

	if( PROP_DEF != m_nVersion )
		sResult.AppendFormat( _T("{\\version%d}"),m_nVersion );

	if( _T("") != m_sDocCom )
	{
        sResult += _T("{\\doccomm ");	sResult += RtfChar::renderRtfText( m_sDocCom, oRenderParameter.poDocument );	sResult += _T("}");
	}
	if( PROP_DEF != m_nInternalVersion )
	{
		sResult.AppendFormat( _T("{\\vern%d}"),m_nInternalVersion );
	}
	if( _T("") != m_sLinkBase )
	{
        sResult += _T("{\\hlinkbase ");	sResult += RtfChar::renderRtfText( m_sLinkBase, oRenderParameter.poDocument );	sResult += _T("}");
	}


	CString sCreateTime = m_oCreateTime.RenderToRtf( oRenderParameter );
	if( _T("") != sCreateTime )
	{
        sResult += _T("{\\creatim");	sResult += sCreateTime;	sResult += _T("}");
	}
	CString sRevTime = m_oRevTime.RenderToRtf( oRenderParameter );
	if( _T("") != sRevTime )
	{
		sResult += _T("{\\revtim");		sResult += sRevTime;	sResult += _T("}");
	}
	CString sPrintTime = m_oPrintTime.RenderToRtf( oRenderParameter );
	if( _T("") != sPrintTime )
	{
        sResult += _T("{\\printim");	sResult += sPrintTime;	sResult += _T("}");
	}
	CString sBackupTime = m_oBackupTime.RenderToRtf( oRenderParameter );
	if( _T("") != sBackupTime )
	{
        sResult += _T("{\\buptim");		sResult += sBackupTime;	sResult += _T("}");
	}
	if( PROP_DEF != m_nEndingTime )
	{
		sResult.AppendFormat( _T("{\\edmins%d}"),	m_nEndingTime );
	}
	if( PROP_DEF != m_nNumberOfPages )
	{
		sResult.AppendFormat( _T("{\\nofpages%d}"),	m_nNumberOfPages );
	}
	if( PROP_DEF != m_nNumberOfWords )
	{
		sResult.AppendFormat( _T("{\\nofwords%d}"),m_nNumberOfWords );
	}
	if( PROP_DEF != m_nNumberOfCharactersWithSpace )
	{
		sResult.AppendFormat( _T("{\\nofchars%d}"),m_nNumberOfCharactersWithSpace );
	}
	if( PROP_DEF != m_nNumberOfCharactersWithoutSpace )
	{
		sResult.AppendFormat( _T("{\\nofcharsws%d}"),m_nNumberOfCharactersWithoutSpace );
	}

	if( PROP_DEF != m_nInternalId )
		sResult.AppendFormat( _T("{\\id%d}"),m_nInternalId );

	if( false == sResult.IsEmpty() )
		sResult = _T("{\\info") + sResult + _T("}");
	return sResult;
}
CString RtfInformation::RenderToOOX(RenderParameter oRenderParameter)
{
	if( RENDER_TO_OOX_PARAM_CORE == oRenderParameter.nType )
	{
		OOX::CCore *pCore = static_cast<OOX::CCore*>( oRenderParameter.poWriter );
		
		if( m_sTitle.GetLength() > 0 )		pCore->m_sTitle			= Utils::PrepareToXML(m_sTitle);
		if( m_sSubject.GetLength() > 0 )	pCore->m_sSubject		= Utils::PrepareToXML(m_sSubject);
		if( m_sAuthor.GetLength() > 0 )		pCore->m_sCreator		= m_sAuthor;
		if( m_sKeywords.GetLength() > 0 )	pCore->m_sKeywords		= m_sKeywords;
		if(  m_sComment.GetLength() > 0 )	pCore->m_sDescription	= Utils::PrepareToXML(m_sComment);

		//CString sCreateTime = m_oCreateTime.RenderToOOX( oRenderParameter );
		//if( false == sCreateTime.IsEmpty() ) pCore->m_sCreated		= sCreateTime;
		//
		//CString sRevTime = m_oRevTime.RenderToOOX( oRenderParameter );
		//if( false == sRevTime.IsEmpty() )	pCore->m_sModified		= sRevTime;

		//CString sPrintTime = m_oPrintTime.RenderToOOX( oRenderParameter );
		//if( _T("") != sPrintTime )
		//	sResult += _T("<dcterms:created xsi:type=\"dcterms:W3CDTF\">") + sCreateTime + _T("</dcterms:created>");
		//CString sBackupTime = m_oBackupTime.RenderToOOX( oRenderParameter );
		//if( _T("") != sBackupTime )
		//	sResult += _T("<dcterms:created xsi:type=\"dcterms:W3CDTF\">") + sCreateTime + _T("</dcterms:created>");
	}
	else if( RENDER_TO_OOX_PARAM_APP == oRenderParameter.nType )
	{
		OOX::CApp *pApp = static_cast<OOX::CApp*>( oRenderParameter.poWriter );
		
		if( PROP_DEF != m_nEndingTime)						pApp->m_nTotalTime				= m_nEndingTime;
		if( PROP_DEF != m_nNumberOfPages )					pApp->m_nPages					= m_nNumberOfPages;
		if( PROP_DEF != m_nNumberOfWords )					pApp->m_nWords					= m_nNumberOfWords;
		if( PROP_DEF != m_nNumberOfCharactersWithSpace )	pApp->m_nCharactersWithSpaces	= m_nNumberOfCharactersWithSpace;
		
		//if( PROP_DEF != m_nNumberOfCharactersWithoutSpace )	
		//	sResult.AppendFormat( _T("<CharactersWithSpaces>%d</CharactersWithSpaces>"),m_nNumberOfCharactersWithoutSpace );		
	}
	return _T("");
}
CString RtfTime::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	if( PROP_DEF != m_nYear )
		sResult.AppendFormat(_T("\\yr%d"),m_nYear);
	if( PROP_DEF != m_nMonth )
		sResult.AppendFormat(_T("\\mo%d"),m_nMonth);
	if( PROP_DEF != m_nDay )
		sResult.AppendFormat(_T("\\dy%d"),m_nDay);
	if( PROP_DEF != m_nHour )
		sResult.AppendFormat(_T("\\hr%d"),m_nHour);
	if( PROP_DEF != m_nMin )
		sResult.AppendFormat(_T("\\min%d"),m_nMin);
	if( PROP_DEF != m_nSecond )
		sResult.AppendFormat(_T("\\sec%d"),m_nSecond);
	return sResult;
}
CString RtfTime::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( PROP_DEF != m_nYear || PROP_DEF != m_nMonth || PROP_DEF != m_nDay )
	{
		int nYear = 0;
		if( PROP_DEF != m_nYear )
			nYear = m_nYear;
		int nMonth = 0;
		if( PROP_DEF != m_nMonth )
			nMonth = m_nMonth;
		int nDay = 0;
		if( PROP_DEF != m_nDay )
			nDay = m_nDay;
		sResult.AppendFormat( _T("%d-%d-%dT"), nYear, nMonth, nDay );
	}
	if( PROP_DEF != m_nHour || PROP_DEF != m_nMin || PROP_DEF != m_nSecond )
	{
		int nHour = 0;
		if( PROP_DEF != m_nHour )
			nHour = m_nHour;
		int nMin = 0;
		if( PROP_DEF != m_nMin )
			nMin = m_nMin;
		int nSecond = 0;
		if( PROP_DEF != m_nSecond )
			nSecond = m_nSecond;
		sResult.AppendFormat( _T("%d:%d:%dZ"), nHour, nMin, nSecond );
	}
	return sResult;
}


void RtfCharStyle::Merge( RtfStylePtr oStyle )
{
	RtfStyle::Merge( oStyle );
	if( TYPE_RTF_PROPERTY_STYLE_CHAR == oStyle->GetType() )
	{
        RtfCharStylePtr oCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( oStyle );
		m_oCharProp.Merge( oCharStyle->m_oCharProp );
	}
	else if( TYPE_RTF_PROPERTY_STYLE_PARAGRAPH == oStyle->GetType() )
	{
        RtfParagraphStylePtr oParagraphStyle = boost::static_pointer_cast<RtfParagraphStyle, RtfStyle>( oStyle );
		m_oCharProp.Merge( oParagraphStyle->m_oCharProp );
	}
	else if( TYPE_RTF_PROPERTY_STYLE_TABLE == oStyle->GetType() )
	{
        RtfTableStylePtr oTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oStyle );
		m_oCharProp.Merge( oTableStyle->m_oCharProp );
	}
}

void RtfParagraphStyle::Merge( RtfStylePtr oStyle )
{
	RtfStyle::Merge( oStyle );
	if( TYPE_RTF_PROPERTY_STYLE_CHAR == oStyle->GetType() )
	{
		RtfCharStylePtr oCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( oStyle );
		m_oCharProp.Merge( oCharStyle->m_oCharProp );
	}
	else if( TYPE_RTF_PROPERTY_STYLE_PARAGRAPH == oStyle->GetType() )
	{
		RtfParagraphStylePtr oParagraphStyle = boost::static_pointer_cast<RtfParagraphStyle, RtfStyle>( oStyle );
		m_oParProp.Merge( oParagraphStyle->m_oParProp );
		m_oCharProp.Merge( oParagraphStyle->m_oCharProp );
	}
	else if( TYPE_RTF_PROPERTY_STYLE_TABLE == oStyle->GetType() )
	{
		RtfTableStylePtr oTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oStyle );
		m_oParProp.Merge( oTableStyle->m_oParProp );
		m_oCharProp.Merge( oTableStyle->m_oCharProp );
	}
}
void RtfTableStyle::Merge( RtfStylePtr oStyle )
{
	RtfStyle::Merge( oStyle );
	if( TYPE_RTF_PROPERTY_STYLE_CHAR == oStyle->GetType() )
	{
		RtfCharStylePtr oCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( oStyle );
		
		m_oCharProp.Merge( oCharStyle->m_oCharProp );
	}
	else if( TYPE_RTF_PROPERTY_STYLE_PARAGRAPH == oStyle->GetType() )
	{
		RtfParagraphStylePtr oParagraphStyle = boost::static_pointer_cast<RtfParagraphStyle, RtfStyle>( oStyle );
		
		m_oParProp.Merge( oParagraphStyle->m_oParProp );
		m_oCharProp.Merge( oParagraphStyle->m_oCharProp );
	}
	if( TYPE_RTF_PROPERTY_STYLE_TABLE == oStyle->GetType() )
	{
		RtfTableStylePtr oTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oStyle );
		
		m_oTableProp.Merge( oTableStyle->m_oTableProp );
		m_oRowProp.Merge( oTableStyle->m_oRowProp );
		m_oCellProp.Merge( oTableStyle->m_oCellProp );
		m_oParProp.Merge( oTableStyle->m_oParProp );
		m_oCharProp.Merge( oTableStyle->m_oCharProp );
		if( NULL != oTableStyle->m_oFirstRow )
		{
			m_oFirstRow = RtfTableStylePtr( new RtfTableStyle() );
			m_oFirstRow->Merge( oTableStyle->m_oFirstRow );
		}
		if( NULL != oTableStyle->m_oLastRow )
		{
			m_oLastRow = RtfTableStylePtr( new RtfTableStyle() );
			m_oLastRow->Merge( oTableStyle->m_oLastRow );
		}
		if( NULL != oTableStyle->m_oFirstCol )
		{
			m_oFirstCol = RtfTableStylePtr( new RtfTableStyle() );
			m_oFirstCol->Merge( oTableStyle->m_oFirstCol );
		}
		if( NULL != oTableStyle->m_oLastCol )
		{
			m_oLastCol = RtfTableStylePtr( new RtfTableStyle() );
			m_oLastCol->Merge( oTableStyle->m_oLastCol );
		}
		if( NULL != oTableStyle->m_oBandHorEven )
		{
			m_oBandHorEven = RtfTableStylePtr( new RtfTableStyle() );
			m_oBandHorEven->Merge( oTableStyle->m_oBandHorEven );
		}
		if( NULL != oTableStyle->m_oBandVerEven )
		{
			m_oBandVerEven = RtfTableStylePtr( new RtfTableStyle() );
			m_oBandVerEven->Merge( oTableStyle->m_oBandVerEven );
		}
		if( NULL != oTableStyle->m_oBandHorOdd )
		{
			m_oBandHorOdd = RtfTableStylePtr( new RtfTableStyle() );
			m_oBandHorOdd->Merge( oTableStyle->m_oBandHorOdd );
		}
		if( NULL != oTableStyle->m_oBandVerOdd )
		{
			m_oBandVerOdd = RtfTableStylePtr( new RtfTableStyle() );
			m_oBandVerOdd->Merge( oTableStyle->m_oBandVerOdd );
		}
		if( NULL != oTableStyle->m_oNWCell )
		{
			m_oNWCell = RtfTableStylePtr( new RtfTableStyle() );
			m_oNWCell->Merge( oTableStyle->m_oNWCell );
		}
		if( NULL != oTableStyle->m_oNECell )
		{
			m_oNECell = RtfTableStylePtr( new RtfTableStyle() );
			m_oNECell->Merge( oTableStyle->m_oNECell );
		}
		if( NULL != oTableStyle->m_oSWCell )
		{
			m_oSWCell = RtfTableStylePtr( new RtfTableStyle() );
			m_oSWCell->Merge( oTableStyle->m_oSWCell );
		}
		if( NULL != oTableStyle->m_oSECell )
		{
			m_oSECell = RtfTableStylePtr( new RtfTableStyle() );
			m_oSECell->Merge( oTableStyle->m_oSECell );
		}
	}
}
