#include "stdafx.h"
#include "RtfDocument.h"
#include "RtfWriter.h"
#include "OOXWriter.h"
#include "RtfProperty.h"
#include "../Common/XmlUtils.h"

CString RtfFont::RenderToRtf(RenderParameter oRenderParameter)
		{
			//ATLASSERT( IsValid() );
			CString sResult;
			if( RENDER_TO_RTF_PARAM_FONT_TBL == oRenderParameter.nType )
			{
				sResult.AppendFormat(_T("{"));
				switch( m_eFontTheme )
				{
					case ft_flomajor:	sResult.AppendFormat(_T("\\flomajor"));break;
					case ft_fhimajor:	sResult.AppendFormat(_T("\\fhimajor"));break;
					case ft_fdbmajor:	sResult.AppendFormat(_T("\\fdbmajor"));break;
					case ft_fbimajor:	sResult.AppendFormat(_T("\\fbimajor"));break;
					case ft_flominor:	sResult.AppendFormat(_T("\\flominor"));break;
					case ft_fhiminor:	sResult.AppendFormat(_T("\\fhiminor"));break;
					case ft_fdbminor:	sResult.AppendFormat(_T("\\fdbminor"));break;
					case ft_fbiminor:	sResult.AppendFormat(_T("\\fbiminor"));break;
				}
				sResult.AppendFormat(_T("\\f%d"),m_nID);

				switch( m_eFontFamily )
				{
					case ff_fnil:sResult.AppendFormat(_T("\\fnil"));break;
					case ff_froman:sResult.AppendFormat(_T("\\froman"));break;
					case ff_fswiss:sResult.AppendFormat(_T("\\fswiss"));break;
					case ff_fmodern:sResult.AppendFormat(_T("\\fmodern"));break;
					case ff_fscript:sResult.AppendFormat(_T("\\fscript"));break;
					case ff_fdecor:sResult.AppendFormat(_T("\\fdecor"));break;
					case ff_ftech:	sResult.AppendFormat(_T("\\ftech"));break;
					case ff_fbidi:	sResult.AppendFormat(_T("\\fbidi"));break;
				}
				RENDER_RTF_INT( m_nCharset, sResult, _T("fcharset") )
				RENDER_RTF_INT( m_nPitch, sResult, _T("fprq") )

				if( _T("") != m_sPanose )
					sResult.AppendFormat(_T("{\\*\\panose %s}"),m_sPanose);
				RENDER_RTF_INT( m_nCodePage, sResult, _T("cpg") )

				RtfCharProperty oFontNameCharProp;
				sResult.AppendFormat(_T(" %s"), RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, &oFontNameCharProp ));
				if( _T("") != m_sAltName )
					sResult.AppendFormat(_T("{\\*\\falt %s}"),  RtfChar::renderRtfText( m_sAltName, oRenderParameter.poDocument ) );
				//важно
				sResult.AppendFormat(_T(";"));
				sResult.AppendFormat(_T("}"));
			}
			else
				sResult.AppendFormat(_T("\\f%d"),m_nID);
			return sResult;
		}
CString RtfFont::RenderToOOX(RenderParameter oRenderParameter)
		{
			//ATLASSERT( IsValid() );
			CString sResult;
			if( IsValid() )
			{
				RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
				CString sFontName = m_sName;
				if( _T("") == sFontName )
				{
					if( PROP_DEF != poRtfDocument->m_oProperty.m_nDeffFont )
					{
						RtfFont oDefFont;
						poRtfDocument->m_oFontTable.GetFont( poRtfDocument->m_oProperty.m_nDeffFont, oDefFont );
						sFontName = oDefFont.m_sName;
					}
					if( _T("") == sFontName )
						sFontName = _T("Arial");
				}
				if( RENDER_TO_OOX_PARAM_MINOR_FONT == oRenderParameter.nType )
				{
					CString sTag;
					switch(m_eFontTheme)
					{
						case ft_flominor: 
						case ft_fhiminor: sTag = _T("latin");break;
						case ft_fdbminor: sTag = _T("ea");break;
						case ft_fbiminor: sTag = _T("cs");break;
					}
					if( _T("") != sTag )
						sResult.AppendFormat(_T("<%s typeface=\"%s\"/>"), sTag, Utils::PrepareToXML( sFontName ) );
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
					if( _T("") != sTag )
						sResult.AppendFormat(_T("<%s typeface=\"%s\"/>"), sTag, Utils::PrepareToXML( sFontName ));
				}
				else if( RENDER_TO_OOX_PARAM_FONTTABLE == oRenderParameter.nType )
				{
					sResult.AppendFormat(_T("<w:font w:name=\"%s\">"), Utils::PrepareToXML( sFontName ) );
					if( _T("") != m_sAltName )
						sResult.AppendFormat(_T("<w:altName w:val=\"%s\" />"), Utils::PrepareToXML( m_sAltName ));
					if( _T("") != m_sPanose )
						sResult.AppendFormat(_T("<w:panose1 w:val=\"%s\" />"),m_sPanose);

					CString sFamily = _T("");
					switch( m_eFontFamily )
					{
						case ff_fnil:sFamily = _T("auto");break;
						case ff_froman:sFamily = _T("roman");break;
						case ff_fswiss:sFamily = _T("swiss");break;
						case ff_fmodern:sFamily = _T("modern");break;
						case ff_fscript:sFamily = _T("script");break;
						case ff_fdecor:sFamily = _T("decorative");break;
						case ff_ftech:sFamily = _T("auto");break;
						case ff_fbidi:sFamily = _T("auto");break;
					}
					if( _T("") != sFamily )
						sResult.AppendFormat(_T("<w:family w:val=\"%s\" />"),sFamily);
					if( PROP_DEF != m_nCharset )
						sResult.AppendFormat(_T("<w:charset w:val=\"%s\" />"), Convert::ToStringHex( m_nCharset, 2 ));

  					if( PROP_DEF != m_nPitch )
					{
						CString sPitch;
						switch( m_nPitch )
						{
							case 0: sPitch = _T("default");break;
							case 1: sPitch = _T("fixed");break;
							case 2: sPitch = _T("variable");break;
						}
						sResult.AppendFormat(_T("<w:pitch w:val=\"%s\" />"),sPitch);
					}

					//важно
					sResult.AppendFormat(_T("</w:font>"));
				}
				else
				{
					RtfFont oCurFont;
					if( true == poRtfDocument->m_oFontTable.GetFont(m_nID,oCurFont) )
					{
						sResult.AppendFormat(_T("<w:rFonts w:ascii=\"%s\" w:eastAsia=\"%s\" w:hAnsi=\"%s\" w:cs=\"%s\"/>"), sFontName, sFontName, sFontName, sFontName);
					}
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
				//	sResult.Append(sTheme);
				//	sResult.AppendFormat(_T("\\ctint%d\\cshade%d"), m_byteTint, m_byteShade);
				//}
				sResult.AppendFormat(_T("\\red%d\\green%d\\blue%d"), m_byteRed, m_byteGreen, m_byteBlue);
				//важно
				sResult.Append(_T(";"));
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
				sResult.Append( ToHexColor() );
			}
			else if( RENDER_TO_OOX_PARAM_COLOR_ATTRIBUTE == oRenderParameter.nType )
			{
				sResult.Append( oRenderParameter.sValue );
			}
			else if( RENDER_TO_OOX_PARAM_COLOR_TAG == oRenderParameter.nType )
			{
				sResult.AppendFormat( _T("<w:color %s/>"),WriteOOXAttribute(oRenderParameter.sValue) );
			}
			else
			{
				sResult.AppendFormat( _T("<w:color  w:val=\"%s\"/>"),ToHexColor() );
			}
			return sResult;
		}
CString RtfShadingChar::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString  sResult;
			RENDER_RTF_INT( m_nValue, sResult, _T("chshdng") )
			switch( m_eType )
			{
				case st_chbghoriz:sResult.Append(_T("\\chbghoriz"));break;
				case st_chbgvert:sResult.Append(_T("\\chbgvert"));break;
				case st_chbgfdiag:sResult.Append(_T("\\chbgfdiag"));break;
				case st_chbgbdiag:sResult.Append(_T("\\chbgbdiag"));break;
				case st_chbgcross:sResult.Append(_T("\\chbgcross"));break;
				case st_chbgdcross:sResult.Append(_T("\\chbgdcross"));break;
				case st_chbgdkhoriz:sResult.Append(_T("\\chbgdkhoriz"));break;
				case st_chbgdkvert:sResult.Append(_T("\\chbgdkvert"));break;
				case st_chbgdkfdiag:sResult.Append(_T("\\chbgdkfdiag"));break;
				case st_chbgdkbdiag:sResult.Append(_T("\\chbgdkbdiag"));break;
				case st_chbgdkcross:sResult.Append(_T("\\chbgdkcross"));break;
				case st_chbgdkdcross:sResult.Append(_T("\\chbgdkdcross"));break;
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
				case st_chbghoriz:sResult.Append(_T("\\bghoriz"));break;
				case st_chbgvert:sResult.Append(_T("\\bgvert"));break;
				case st_chbgfdiag:sResult.Append(_T("\\bgfdiag"));break;
				case st_chbgbdiag:sResult.Append(_T("\\bgbdiag"));break;
				case st_chbgcross:sResult.Append(_T("\\bgcross"));break;
				case st_chbgdcross:sResult.Append(_T("\\bgdcross"));break;
				case st_chbgdkhoriz:sResult.Append(_T("\\bgdkhoriz"));break;
				case st_chbgdkvert:sResult.Append(_T("\\bgdkvert"));break;
				case st_chbgdkfdiag:sResult.Append(_T("\\bgdkfdiag"));break;
				case st_chbgdkbdiag:sResult.Append(_T("\\bgdkbdiag"));break;
				case st_chbgdkcross:sResult.Append(_T("\\bgdkcross"));break;
				case st_chbgdkdcross:sResult.Append(_T("\\bgdkdcross"));break;
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
				case st_clshdrawnil:sResult.Append(_T("\\clshdrawnil"));break;
				case st_chbghoriz:sResult.Append(_T("\\clbghoriz"));break;
				case st_chbgvert:sResult.Append(_T("\\clbgvert"));break;
				case st_chbgfdiag:sResult.Append(_T("\\clbgfdiag"));break;
				case st_chbgbdiag:sResult.Append(_T("\\clbgbdiag"));break;
				case st_chbgcross:sResult.Append(_T("\\clbgcross"));break;
				case st_chbgdcross:sResult.Append(_T("\\clbgdcross"));break;
				case st_chbgdkhoriz:sResult.Append(_T("\\clbgdkhor"));break;
				case st_chbgdkvert:sResult.Append(_T("\\clbgdkvert"));break;
				case st_chbgdkfdiag:sResult.Append(_T("\\clbgdkfdiag"));break;
				case st_chbgdkbdiag:sResult.Append(_T("\\clbgdkbdiag"));break;
				case st_chbgdkcross:sResult.Append(_T("\\clbgdkcross"));break;
				case st_chbgdkdcross:sResult.Append(_T("\\clbgdkdcross"));break;
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
				case st_chbghoriz:sResult.Append(_T("\\trbghoriz"));break;
				case st_chbgvert:sResult.Append(_T("\\trbgvert"));break;
				case st_chbgfdiag:sResult.Append(_T("\\trbgfdiag"));break;
				case st_chbgbdiag:sResult.Append(_T("\\trbgbdiag"));break;
				case st_chbgcross:sResult.Append(_T("\\trbgcross"));break;
				case st_chbgdcross:sResult.Append(_T("\\trbgdcross"));break;
				case st_chbgdkhoriz:sResult.Append(_T("\\trbgdkhor"));break;
				case st_chbgdkvert:sResult.Append(_T("\\trbgdkvert"));break;
				case st_chbgdkfdiag:sResult.Append(_T("\\trbgdkfdiag"));break;
				case st_chbgdkbdiag:sResult.Append(_T("\\trbgdkbdiag"));break;
				case st_chbgdkcross:sResult.Append(_T("\\trbgdkcross"));break;
				case st_chbgdkdcross:sResult.Append(_T("\\trbgdkdcross"));break;
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
				case st_chbghoriz:sResult.Append(_T("\\tsbghoriz"));break;
				case st_chbgvert:sResult.Append(_T("\\tsbgvert"));break;
				case st_chbgfdiag:sResult.Append(_T("\\tsbgfdiag"));break;
				case st_chbgbdiag:sResult.Append(_T("\\tsbgbdiag"));break;
				case st_chbgcross:sResult.Append(_T("\\tsbgcross"));break;
				case st_chbgdcross:sResult.Append(_T("\\tsbgdcross"));break;
				case st_chbgdkhoriz:sResult.Append(_T("\\tsbgdkhor"));break;
				case st_chbgdkvert:sResult.Append(_T("\\tsbgdkvert"));break;
				case st_chbgdkfdiag:sResult.Append(_T("\\tsbgdkfdiag"));break;
				case st_chbgdkbdiag:sResult.Append(_T("\\tsbgdkbdiag"));break;
				case st_chbgdkcross:sResult.Append(_T("\\tsbgdkcross"));break;
				case st_chbgdkdcross:sResult.Append(_T("\\tsbgdkdcross"));break;
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
					sShading.AppendFormat( _T(" w:color=\"%s\""), oForeColor.RenderToOOX(oNewParam));
			}
			if( PROP_DEF != m_nBackColor )
			{
				RtfColor oBackColor;
				if( true == poRtfDocument->m_oColorTable.GetColor( m_nBackColor, oBackColor ) )
					sShading.AppendFormat( _T(" w:fill=\"%s\""), oBackColor.RenderToOOX(oNewParam));
			}
			if( PROP_DEF != m_nValue )
			{
				CString sValue;
				if( 0 <= m_nValue &&  m_nValue <= 2 )
					sShading.Append( _T(" w:val=\"clear\"") );
				else if( 2 < m_nValue && m_nValue <= 7 )
					sShading.Append( _T(" w:val=\"pct5\"") );
				else if( 7 < m_nValue && m_nValue <= 11 )
					sShading.Append( _T(" w:val=\"pct10\"") );
				else if( 11 < m_nValue && m_nValue <= 13 )
					sShading.Append( _T(" w:val=\"pct12\"") );
				else if( 13 < m_nValue && m_nValue <= 17 )
					sShading.Append( _T(" w:val=\"pct15\"") );
				else if( 17 < m_nValue && m_nValue <= 22 )
					sShading.Append( _T(" w:val=\"pct20\"") );
				else if( 22 < m_nValue && m_nValue <= 27 )
					sShading.Append( _T(" w:val=\"pct25\"") );
				else if( 27 < m_nValue && m_nValue <= 32 )
					sShading.Append( _T(" w:val=\"pct30\"") );
				else if( 32 < m_nValue && m_nValue <= 36 )
					sShading.Append( _T(" w:val=\"pct35\"") );
				else if( 36 < m_nValue && m_nValue <= 38 )
					sShading.Append( _T(" w:val=\"pct37\"") );
				else if( 38 < m_nValue && m_nValue <= 42 )
					sShading.Append( _T(" w:val=\"pct40\"") );
				else if( 42 < m_nValue && m_nValue <= 47 )
					sShading.Append( _T(" w:val=\"pct45\"") );
				else if( 47 < m_nValue && m_nValue <= 52 )
					sShading.Append( _T(" w:val=\"pct50\"") );
				else if( 52 < m_nValue && m_nValue <= 57 )
					sShading.Append( _T(" w:val=\"pct55\"") );
				else if( 57 < m_nValue && m_nValue <= 61 )
					sShading.Append( _T(" w:val=\"pct60\"") );
				else if( 61 < m_nValue && m_nValue <= 63 )
					sShading.Append( _T(" w:val=\"pct62\"") );
				else if( 63 < m_nValue && m_nValue <= 70 )
					sShading.Append( _T(" w:val=\"pct65\"") );
				else if( 70 < m_nValue && m_nValue <= 80 )
					sShading.Append( _T(" w:val=\"pct75\"") );
				else if( 80 < m_nValue && m_nValue <= 86 )
					sShading.Append( _T(" w:val=\"pct85\"") );
				else if( 86 < m_nValue && m_nValue <= 88 )
					sShading.Append( _T(" w:val=\"pct87\"") );
				else if( 88 < m_nValue && m_nValue <= 92 )
					sShading.Append( _T(" w:val=\"pct90\"") );
				else if( 92 < m_nValue && m_nValue <= 97 )
					sShading.Append( _T(" w:val=\"pct95\"") );
				else if( 97 < m_nValue && m_nValue <= 100 )
					sShading.Append( _T(" w:val=\"solid\"") );
			}
			else
			{
				switch( m_eType )
				{
					case st_clshdrawnil:sShading.Append( _T(" w:val=\"nil\"") );break;
					case st_chbghoriz:sShading.Append( _T(" w:val=\"thinHorzStripehorzStripe\"") );break;
					case st_chbgvert:sShading.Append( _T(" w:val=\"thinVertStripe\"") );break;
					case st_chbgfdiag:sShading.Append( _T(" w:val=\"thinReverseDiagStripe\"") );break;
					case st_chbgbdiag:sShading.Append( _T(" w:val=\"thinDiagStripe\"") );break;
					case st_chbgcross:sShading.Append( _T(" w:val=\"thinHorzCross\"") );break;
					case st_chbgdcross:sShading.Append( _T(" w:val=\"thinDiagCross\"") );break;
					case st_chbgdkhoriz:sShading.Append( _T(" w:val=\"horzStripe\"") );break;
					case st_chbgdkvert:sShading.Append( _T(" w:val=\"vertStripe\"") );break;
					case st_chbgdkfdiag:sShading.Append( _T(" w:val=\"reverseDiagStripe\"") );break;
					case st_chbgdkbdiag:sShading.Append( _T(" w:val=\"diagStripe\"") );break;
					case st_chbgdkcross:sShading.Append( _T(" w:val=\"horzCross\"") );break;
					case st_chbgdkdcross:sShading.Append( _T(" w:val=\"diagCross\"") );break;
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
				case bt_brdrs: sResult = _T("\\brdrs"); break;
				case bt_brdrth: sResult = _T("\\brdrth"); break;
				case bt_brdrsh: sResult = _T("\\brdrsh"); break;
				case bt_brdrdb: sResult = _T("\\brdrdb"); break;
				case bt_brdrdot: sResult = _T("\\brdrdot"); break;
				case bt_brdrdash: sResult = _T("\\brdrdash"); break;
				case bt_brdrhair: sResult = _T("\\brdrhair"); break;
				case bt_brdrinset: sResult = _T("\\brdrinset"); break;
				case bt_brdrdashsm: sResult = _T("\\brdrdashsm"); break;
				case bt_brdrdashd: sResult = _T("\\brdrdashd"); break;
				case bt_brdrdashdd: sResult = _T("\\brdrdashdd"); break;
				case bt_brdrdashdot: sResult = _T("\\brdrdashdot"); break;
				case bt_brdrtriple: sResult = _T("\\brdrtriple"); break;
				case bt_brdrtnthsg: sResult = _T("\\brdrtnthsg"); break;
				case bt_brdrthtnsg: sResult = _T("\\brdrthtnsg"); break;
				case bt_brdrtnthtnsg: sResult = _T("\\brdrtnthtnsg"); break;
				case bt_brdrtnthtnmg: sResult = _T("\\brdrtnthtnmg"); break;
				case bt_brdrtnthlg: sResult = _T("\\brdrtnthlg"); break;
				case bt_brdrthtnlg: sResult = _T("\\brdrthtnlg"); break;
				case bt_brdrtnthtnlg: sResult = _T("\\brdrtnthtnlg"); break;
				case bt_brdrwavy: sResult = _T("\\brdrwavy"); break;
				case bt_brdrwavydb: sResult = _T("\\brdrwavydb"); break;
				case bt_brdrdashdotstr: sResult = _T("\\brdrdashdotstr"); break;
				case bt_brdremboss: sResult = _T("\\brdremboss"); break;
				case bt_brdrengrave: sResult = _T("\\brdrengrave"); break;
				case bt_brdroutset: sResult = _T("\\brdroutset"); break;
				case bt_brdrnone: sResult = _T("\\brdrnone"); break;
			}
			if( PROP_DEF != m_nWidth )
				sResult.AppendFormat( _T("\\brdrw%d"),m_nWidth );
			if( PROP_DEF != m_nSpace )
				sResult.AppendFormat( _T("\\brsp%d"),m_nSpace );
			if( PROP_DEF != m_nColor )
				sResult.AppendFormat( _T("\\brdrcf%d"),m_nColor );
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
					sResult.AppendFormat( _T(" w:color=\"%s\""), oColor.RenderToOOX(oNewParam) );
			}
			if( PROP_DEF != m_nWidth ) //w:sz  1/8 twips (equivalent to 1/576th of an inch)
				sResult.AppendFormat( _T(" w:sz=\"%d\""), 2 * m_nWidth / 5 );
			if( PROP_DEF != m_nSpace )
				sResult.AppendFormat( _T(" w:space=\"%d\""), (int)RtfUtility::Twip2pt( m_nSpace ) );

			switch( m_eType )
			{
				case bt_brdrs: sResult.Append( _T(" w:val=\"single\"") ); break;
				case bt_brdrth: sResult.Append( _T(" w:val=\"thick\"") ); break;
				case bt_brdrsh: sResult.Append( _T(" w:val=\"\"thin") ); break;
				case bt_brdrdb: sResult.Append( _T(" w:val=\"double\"") ); break;
				case bt_brdrdot: sResult.Append( _T(" w:val=\"dotted\"") ); break;
				case bt_brdrdash: sResult.Append( _T(" w:val=\"dashed\"") ); break;
				case bt_brdrhair: sResult.Append( _T(" w:val=\"hair\"") ); break;
				case bt_brdrdashsm: sResult.Append( _T(" w:val=\"dashSmallGap\"") ); break;
				case bt_brdrdashd: sResult.Append( _T(" w:val=\"dotDash\"") ); break;
				case bt_brdrdashdd: sResult.Append( _T(" w:val=\"dotDotDash\"") ); break;
				case bt_brdrinset: sResult.Append( _T(" w:val=\"inset\"") ); break;
				case bt_brdrnone: sResult.Append( _T(" w:val=\"nil\"") ); break;
				case bt_brdroutset: sResult.Append( _T(" w:val=\"outset\"") ); break;
				case bt_brdrtriple: sResult.Append( _T(" w:val=\"triple\"") ); break;
				case bt_brdrtnthsg: sResult.Append( _T(" w:val=\"thinThickSmallGap\"") ); break;
				case bt_brdrthtnsg: sResult.Append( _T(" w:val=\"thickThinSmallGap\"") ); break;
				case bt_brdrtnthtnsg: sResult.Append( _T(" w:val=\"thinThickThinSmallGap\"") ); break;
				case bt_brdrtnthtnmg: sResult.Append( _T(" w:val=\"thinThickThinMediumGap\"") ); break;
				case bt_brdrtnthmg: sResult.Append( _T(" w:val=\"thinThickMediumGap\"") ); break;
				case bt_brdrthtnmg: sResult.Append( _T(" w:val=\"thickThinMediumGap\"") ); break;
				case bt_brdrtnthlg: sResult.Append( _T(" w:val=\"thinThickLargeGap\"") ); break;
				case bt_brdrthtnlg: sResult.Append( _T(" w:val=\"thickThinLargeGap\"") ); break;
				case bt_brdrtnthtnlg: sResult.Append( _T(" w:val=\"thinThickThinLargeGap\"") ); break;
				case bt_brdrwavy: sResult.Append( _T(" w:val=\"wave\"") ); break;
				case bt_brdrwavydb: sResult.Append( _T(" w:val=\"doubleWave\"") ); break;
				case bt_brdrdashdotstr: sResult.Append( _T(" w:val=\"dashDotStroked\"") ); break;
				case bt_brdremboss: sResult.Append( _T(" w:val=\"threeDEmboss\"") ); break;
				case bt_brdrengrave: sResult.Append( _T(" w:val=\"threeDEngrave\"") ); break;
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
			if( 1 == m_bRightToLeft )
				sResult.Append(_T("\\rtlch"));
			else
				sResult.Append(_T("\\ltrch"));
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
				case uls_Single: sResult.Append( _T("\\ul") );break;
				case uls_Dotted: sResult.Append( _T("\\uld") );break;
				case uls_Dashed: sResult.Append( _T("\\uldash") );break;
				case uls_Dash_dotted: sResult.Append( _T("\\uldashd") );break;
				case uls_Dash_dot_dotted: sResult.Append( _T("\\uldashdd") );break;
				case uls_Double: sResult.Append( _T("\\uldb") );break;
				case uls_Heavy_wave: sResult.Append( _T("\\ulhwave") );break;
				case uls_Long_dashe: sResult.Append( _T("\\ulldash") );break;
				case uls_Stops_all: sResult.Append( _T("\\ulnone") );break;
				case uls_Thick: sResult.Append( _T("\\ulth") );break;
				case uls_Thick_dotted: sResult.Append( _T("\\ulthd") );break;
				case uls_Thick_dashed: sResult.Append( _T("\\ulthdash") );break;
				case uls_Thick_dash_dotted: sResult.Append( _T("\\ulthdashd") );break;
				case uls_Thick_dash_dot_dotted: sResult.Append( _T("\\ulthdashdd") );break;
				case uls_Thick_long_dashed: sResult.Append( _T("\\ulthldash") );break;
				case uls_Double_wave: sResult.Append( _T("\\ululdbwave") );break;
				case uls_Word: sResult.Append( _T("\\ulw") );break;
				case uls_Wave: sResult.Append( _T("\\ulwave") );break;
			}
			RENDER_RTF_INT( m_nUnderlineColor, sResult, _T("ulc") )
			RENDER_RTF_INT( m_nUp, sResult, _T("up") )

			if( m_poBorder.IsValid() == true )
			{
				sResult.Append( _T("\\chbrdr") );
				sResult.Append( m_poBorder.RenderToRtf( oRenderParameter ));
			}
			if( m_poShading.IsValid() == true )
				sResult.Append( m_poShading.RenderToRtf( oRenderParameter ));

			return sResult;
		}
CString RtfCharProperty::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
			OOXWriter* poOOXWriter = static_cast<OOXWriter*>(oRenderParameter.poWriter);
			switch( m_nAnimated )
			{
				case 0:sResult.Append(_T("<w:effect w:val=\"none\"/>"));break;
				case 1:sResult.Append(_T("<w:effect w:val=\"lights\"/>"));break;
				case 2:sResult.Append(_T("<w:effect w:val=\"blinkBackground\"/>"));break;
				case 3:sResult.Append(_T("<w:effect w:val=\"sparkle\"/>"));break;
				case 4:sResult.Append(_T("<w:effect w:val=\"antsBlack\"/>"));break;
				case 5:sResult.Append(_T("<w:effect w:val=\"antsRed\"/>"));break;
				case 6:sResult.Append(_T("<w:effect w:val=\"shimmer\"/>"));break;
			}
			RENDER_OOX_BOOL( m_bBold, sResult, _T("w:b") )
			RENDER_OOX_BOOL( m_bCaps, sResult, _T("w:caps") )
			RENDER_OOX_INT( (int)m_nScalex, sResult, _T("w:w") )
			//if( PROP_DEF != m_nCharStyle )
			//{
			//	RtfStylePtr oStyle;
			//	if( true == poRtfDocument->m_oStyleTable.GetStyle( m_nCharStyle, oStyle ) )
			//		sResult.AppendFormat(_T("<w:pStyle w:val=\"%s\" />"), oStyle->m_sName );
			//}
			if( PROP_DEF != m_nDown )
				sResult.AppendFormat(_T("<w:position w:val=\"-%d\" />"),m_nDown);
			RENDER_OOX_BOOL( m_bEmbo, sResult, _T("w:emboss") )
			RENDER_OOX_INT( m_nCharacterSpacing, sResult, _T("w:spacing") )
			if( PROP_DEF != m_nFitText )
				if( -1 == m_nFitText )
				{
					sResult.AppendFormat(_T("<w:fitText w:id=\"%d\" w:val=\"%d\" />"),poOOXWriter->nCurFitId,poOOXWriter->nCurFitWidth);
				}
				else 
				{
					poOOXWriter->nCurFitId = poRtfDocument->m_oIdGenerator.Generate_FitTextId();
					poOOXWriter->nCurFitWidth = m_nFitText;
					sResult.AppendFormat(_T("<w:fitText w:id=\"%d\" w:val=\"%d\" />"),poOOXWriter->nCurFitId,poOOXWriter->nCurFitWidth);
				}
			if( PROP_DEF == m_nFont )
				m_nFont = poRtfDocument->m_oProperty.m_nDeffFont;
			if( PROP_DEF != m_nFont )
			{
				RtfFont oCurFont;
				RenderParameter oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
				if( true == poRtfDocument->m_oFontTable.GetFont(m_nFont,oCurFont) )
				{
					sResult.Append( oCurFont.RenderToOOX(oNewParam) );
				}
			}
			RENDER_OOX_INT( m_nFontSize, sResult, _T("w:sz") )
			RENDER_OOX_BOOL( m_bItalic, sResult, _T("w:i") )
			RENDER_OOX_BOOL( m_bImprint, sResult, _T("w:imprint") )
			RENDER_OOX_INT( m_nKerning, sResult, _T("w:kern") )
			if( 1 == m_bRightToLeft )
				sResult.Append(_T("<w:rtl />"));
			RENDER_OOX_BOOL( m_nComplexScript, sResult, _T("w:cs") )
			RENDER_OOX_BOOL( m_bOutline, sResult, _T("w:outline") )
			RENDER_OOX_BOOL( m_bScaps, sResult, _T("w:smallCaps") )
			RENDER_OOX_BOOL( m_bShadow, sResult, _T("w:shadow") )
			RENDER_OOX_BOOL( m_bStrike, sResult, _T("w:strike") )
			RENDER_OOX_BOOL( m_nStriked, sResult, _T("w:dstrike") )
			if( 1 == m_bSub )
				sResult.Append(_T("<w:vertAlign w:val=\"subscript\" />"));
			if( 1 == m_bSuper )
				sResult.Append(_T("<w:vertAlign w:val=\"superscript\" />"));

			if( PROP_DEF != m_nHightlited )
			{
				RtfColor oCurColor;
				if( true == poRtfDocument->m_oColorTable.GetColor( m_nHightlited, oCurColor ) )
				{
					sResult.AppendFormat(_T("<w:highlight w:val=\"%s\" />"), oCurColor.GetHighLight() );
				}
			}
				
			if( PROP_DEF != m_nForeColor )
			{
				RtfColor oCurColor;
				if( true == poRtfDocument->m_oColorTable.GetColor( m_nForeColor, oCurColor ) )
				{
					RenderParameter oNewParam = oRenderParameter;
					oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
					sResult.Append( oCurColor.RenderToOOX(oNewParam) );
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
					sUnderColor.Append( oCurColor.RenderToOOX(oNewParam) );
				}
			}
			if( uls_none != m_eUnderStyle )
			{
				switch( m_eUnderStyle )
				{
					case uls_Single: sResult.AppendFormat(_T("<w:u %s w:val=\"single\"/>"),sUnderColor);break;
					case uls_Dotted: sResult.AppendFormat(_T("<w:u %s w:val=\"dotted\"/>"),sUnderColor);break;
					case uls_Dashed: sResult.AppendFormat(_T("<w:u %s w:val=\"dash\"/>"),sUnderColor);break;
					case uls_Dash_dotted: sResult.AppendFormat(_T("<w:u %s w:val=\"dotDash\"/>"),sUnderColor);break;
					case uls_Dash_dot_dotted: sResult.AppendFormat(_T("<w:u %s w:val=\"dotDotDash\"/>"),sUnderColor);break;
					case uls_Double: sResult.AppendFormat(_T("<w:u %s w:val=\"double\"/>"),sUnderColor);break;
					case uls_Heavy_wave: sResult.AppendFormat(_T("<w:u %s w:val=\"wavyHeavy\"/>"),sUnderColor);break;
					case uls_Long_dashe: sResult.AppendFormat(_T("<w:u %s w:val=\"dashLong\"/>"),sUnderColor);break;
					case uls_Stops_all: sResult.AppendFormat(_T("<w:u %s w:val=\"single\"/>"),sUnderColor);break;//todo
					case uls_Thick: sResult.AppendFormat(_T("<w:u %s w:val=\"thick\"/>"),sUnderColor);break;
					case uls_Thick_dotted: sResult.AppendFormat(_T("<w:u %s w:val=\"dottedHeavy\"/>"),sUnderColor);break;
					case uls_Thick_dashed: sResult.AppendFormat(_T("<w:u %s w:val=\"dashedHeavy\"/>"),sUnderColor);break;
					case uls_Thick_dash_dotted: sResult.AppendFormat(_T("<w:u %s w:val=\"dashDotHeavy\"/>"),sUnderColor);break;
					case uls_Thick_dash_dot_dotted: sResult.AppendFormat(_T("<w:u %s w:val=\"dashDotDotHeavy\"/>"),sUnderColor);break;
					case uls_Thick_long_dashed: sResult.AppendFormat(_T("<w:u %s w:val=\"dashLongHeavy\"/>"),sUnderColor);break;
					case uls_Double_wave: sResult.AppendFormat(_T("<w:u %s w:val=\"wavyDouble\"/>"),sUnderColor);break;
					case uls_Word: sResult.AppendFormat(_T("<w:u %s w:val=\"words\"/>"),sUnderColor);break;
					case uls_Wave: sResult.AppendFormat(_T("<w:u %s w:val=\"wave\"/>"),sUnderColor);break;
				}
			}

			RENDER_OOX_INT( m_nUp, sResult, _T("w:position") )

			sResult.Append( m_poBorder.RenderToOOX( oRenderParameter ));
			sResult.Append( m_poShading.RenderToOOX( oRenderParameter ));
			return sResult;
		}
CString RtfListLevelProperty::RenderToRtf(RenderParameter oRenderParameter)
		{
			RtfDocument* poRtfDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument);
			RtfWriter* poRtfWriter = static_cast<RtfWriter*>( oRenderParameter.poWriter);
			CString sResult;
			sResult.Append(_T("{\\listlevel"));
			RENDER_RTF_INT( m_nNumberType, sResult, _T("levelnfc") )
			RENDER_RTF_INT( m_nNumberType, sResult, _T("levelnfcn") )
			RENDER_RTF_BOOL( m_bTentative, sResult, _T("lvltentative") )
			RENDER_RTF_INT( m_nJustification, sResult, _T("leveljc") )
			RENDER_RTF_INT( m_nJustification, sResult, _T("leveljcn") )
			RENDER_RTF_INT( m_nFollow, sResult, _T("levelfollow") )
			RENDER_RTF_INT( m_nStart, sResult, _T("levelstartat") )
			RENDER_RTF_INT( m_nNoRestart, sResult, _T("levelnorestart") )
			RENDER_RTF_INT( m_nPictureIndex, sResult, _T("levelpicture") )
			//чтобы при последующем чтении из rtf не потер€лась информаци€ о шрифте
			sResult.Append( m_oCharProp.RenderToRtf( oRenderParameter ) );

			sResult.AppendFormat( _T("{\\leveltext %s;}"), RtfChar::renderRtfText( m_sText, oRenderParameter.poDocument, &m_oCharProp ) );
			sResult.AppendFormat(_T("{\\levelnumbers %s;}"), RtfChar::renderRtfText( m_sNumber, oRenderParameter.poDocument, &m_oCharProp ) );

			RENDER_RTF_INT( m_nFirstIndent, sResult, _T("fi") )
			RENDER_RTF_INT( m_nIndent, sResult, _T("li") )
			RENDER_RTF_INT( m_nIndentStart, sResult, _T("lin") )

			for( int i = 0 ; i < (int)m_oTabs.m_aTabs.GetCount(); i++ )
			{
				sResult.Append(_T("\\jclisttab"));
				sResult.Append( m_oTabs.m_aTabs[i].RenderToRtf( oRenderParameter ) );
			}


			sResult.Append(_T("}"));
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
				sResult.Append(_T(">")); 

				if( PROP_DEF != m_nJustification )
				{
					switch( m_nJustification )
					{
						case 0:sResult.Append(_T("<w:lvlJc w:val=\"left\" />")); break;
						case 1:sResult.Append(_T("<w:lvlJc w:val=\"center\" />")); break;
						case 2:sResult.Append(_T("<w:lvlJc w:val=\"right\" />")); break;
					}
				}
				if( 1 == m_nNoRestart)
					sResult.Append(_T("<w:lvlRestart w:val=\"0\"/>"));
				if( 1 ==  m_nLegal)
					sResult.Append(_T("<w:isLgl />")); 

				sResult.AppendFormat( _T("<w:lvlText w:val=\"%s\"/>"), GetLevelTextOOX() ); 
				sResult.AppendFormat(_T("<w:numFmt w:val=\"%s\" />"), GetFormat(m_nNumberType) );
				RENDER_OOX_INT( m_nPictureIndex, sResult, _T("w:lvlPicBulletId") )
				RENDER_OOX_INT( m_nStart, sResult, _T("w:start") )
				if( PROP_DEF != m_nFollow )
				{
					switch( m_nFollow )
					{
						case 0: sResult.Append( _T("<w:suff w:val=\"tab\" />") );break;
						case 1: sResult.Append( _T("<w:suff w:val=\"space\" />") );break;
						case 2: sResult.Append( _T("<w:suff w:val=\"nothing\" />") );break;
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
				if( false == sIndent.IsEmpty() )
					spPr.AppendFormat(_T("<w:ind %s/>"), sIndent);

				spPr.Append( m_oTabs.RenderToOOX( oRenderParameter ) );

				if( false == spPr.IsEmpty() )
					sResult.AppendFormat(_T("<w:pPr>%s</w:pPr>"), spPr);

				CString srPr;
				RenderParameter oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
				srPr.Append( m_oCharProp.RenderToOOX(oNewParam) ); 
				if( false == srPr.IsEmpty() )
					sResult.AppendFormat(_T("<w:rPr>%s</w:rPr>"), srPr);

				sResult.Append(_T("</w:lvl>")); 
			}
			return sResult;		
		}
CString RtfListProperty::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append(_T("\\list"));

			RENDER_RTF_INT( m_nTemplateId, sResult, _T("listtemplateid") )
			//RENDER_RTF_INT( m_nListSimple, sResult, _T("listsimple") )
			//RENDER_RTF_BOOL( m_bListHybrid, sResult, _T("listhybrid") )
			if( 1 == m_bListHybrid )
				sResult.Append(_T("\\listhybrid"));
			else if( 1 == m_nListSimple )
				sResult.AppendFormat(_T("\\listsimple%d"),m_nListSimple);

			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
				sResult.Append( m_aArray[i].RenderToRtf( oRenderParameter ) );

			sResult.AppendFormat(_T("{\\listname %s;}"), RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument ));
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
				sResult.Append(_T("<w:multiLevelType w:val=\"hybridMultilevel\" />"));
			else if( 1 == m_nListSimple )
				sResult.Append(_T("<w:multiLevelType w:val=\"singleLevel\" />"));
			else
				sResult.Append(_T("<w:multiLevelType w:val=\"multilevel\" />"));

			if( false == m_sName.IsEmpty() )
				sResult.AppendFormat(_T("<w:name w:val=\"%s\" />"), Utils::PrepareToXML( m_sName ) );
			//if( false == m_nStyleName )
			//{
			//	sResult.AppendFormat(_T("<w:name w:val=\"%s\" />"), Utils::PrepareToXML( m_sName ) );
			//	if( PROP_DEF !=  m_nStyleID)
			//		sResult.AppendFormat(_T("<w:styleLink w:val=\"%s\" />"), Utils::PrepareToXML( m_sName) );
			//}
			//else
			//	sResult.AppendFormat(_T("<w:numStyleLink w:val=\"%s\" />"),Utils::PrepareToXML( m_sName) );

			if( PROP_DEF != m_nTemplateId )
				sResult.AppendFormat(_T("<w:tmpl w:val=\"%x\" />") ,m_nTemplateId);

			RenderParameter oNewParam = oRenderParameter;
			oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
				sResult.Append( m_aArray[i].RenderToOOX(oNewParam) );

			sResult.Append( _T("</w:abstractNum>") );
			return sResult;
		}
CString RtfListOverrideProperty::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( true == IsValid() )
			{
				sResult.Append( _T("\\listoverride") );
				RENDER_RTF_INT( m_nListID, sResult, _T("listid") )
				RENDER_RTF_INT( m_nIndex, sResult, _T("ls") )
				sResult.Append( m_oOverrideLevels.RenderToRtf( oRenderParameter ) );
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
				sResult.Append(m_oOverrideLevels.RenderToOOX( oRenderParameter ));
				sResult.Append(_T("</w:num>"));
			}
			return sResult;		
		}
CString RtfStyle::RenderToOOXBegin(RenderParameter oRenderParameter)
		{
			//if( false == IsValid() )
			//	return false;
			RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
			CString sResult;
			CString sType;
			switch( m_eType )
			{
				case stParagraph : sType = _T("paragraph");break;
				case stCharacter : sType = _T("character");break;
				case stSection : sType = _T("numbering");break;
				case stTable :sType = _T("table"); break;
			}
			sResult.AppendFormat(_T("<w:style w:type=\"%s\" w:styleId=\"%s\">"), sType, Utils::PrepareToXML( m_sName ) );//Todo
			sResult.AppendFormat(_T("<w:name w:val=\"%s\"/>"), Utils::PrepareToXML( m_sName ) );

			if( PROP_DEF != m_nBasedOn )
			{
				RtfStylePtr oBaseStyle;
				if( true == poDocument->m_oStyleTable.GetStyle( m_nBasedOn, oBaseStyle ) )
					sResult.AppendFormat(_T("<w:basedOn w:val=\"%s\"/>"), Utils::PrepareToXML( oBaseStyle->m_sName ));//Todo
			}
			//if( PROP_DEF != m_nNext )
			//	sResult.AppendFormat(_T("<w:next w:val=\"%s\"/>"), Utils::PrepareToXML( m_sName ));//Todo
			//if( PROP_DEF != m_nLink )
			//	sResult.AppendFormat(_T("<w:link w:val=\"%s\"/>"), Utils::PrepareToXML( m_sName ));//Todo
			if( PROP_DEF != m_bHidden )
				sResult.Append(_T("<w:hidden/>"));
			if( PROP_DEF != m_bLocked )
				sResult.Append(_T("<w:locked/>"));
			if( PROP_DEF != m_bPersonal )
				sResult.Append(_T("<w:personal w:val=\"true\" />"));
			if( PROP_DEF != m_bCompose )
				sResult.Append(_T("<w:personalCompose w:val=\"true\" />"));
			if( PROP_DEF != m_bReply )
				sResult.Append(_T("<w:personalReply w:val=\"true\" />"));
			if( PROP_DEF == m_nSemiHidden )
				sResult.Append(_T("<w:semiHidden/>"));
			if( PROP_DEF != m_bQFormat )
				sResult.Append(_T("<w:qformat/>"));
			if( PROP_DEF != m_nPriority )
				sResult.AppendFormat(_T("<w:uiPriority w:val=\"%d\"/>"),m_nPriority);
			if( PROP_DEF != m_bUnhiddenWhenUse )
				sResult.Append(_T("<w:unhideWhenUsed/>"));

			return sResult;
		}
CString RtfStyle::RenderToOOXEnd(RenderParameter oRenderParameter)
		{
			//if( false == IsValid() )
			//	return false;
			CString sResult;
			sResult.Append(_T("</w:style>"));
			return sResult;
		}
CString RtfStyle::RenderToRtfEnd( RenderParameter oRenderParameter )
		{
			if( false == IsValid() )
				return false;

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

			sResult.AppendFormat(_T(" %s;}"), RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument ));
			return sResult;
		}
CString RtfCharStyle::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append( RenderToRtfBegin( oRenderParameter ) );
			sResult.Append( m_oCharProp.RenderToRtf(  oRenderParameter ) );
			sResult.Append( RenderToRtfEnd( oRenderParameter ) );
			return sResult;
		}
CString RtfCharStyle::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append( RenderToOOXBegin( oRenderParameter ) );
			CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
			if( false == sCharProp.IsEmpty() )
				sResult.AppendFormat( _T("<w:rPr>%s</w:rPr>"), sCharProp );
			sResult.Append( RenderToOOXEnd( oRenderParameter ) );
			return sResult;
		}
CString RtfParagraphStyle::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append( RenderToRtfBegin( oRenderParameter ) );
			sResult.Append( m_oParProp.RenderToRtf(  oRenderParameter ) );
			sResult.Append( m_oCharProp.RenderToRtf(  oRenderParameter ) );
			sResult.Append( RenderToRtfEnd( oRenderParameter ) );
			return sResult;
		}
CString RtfParagraphStyle::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append( RenderToOOXBegin( oRenderParameter ) );
			CString sParProp = m_oParProp.RenderToOOX(oRenderParameter);
			if( false == sParProp.IsEmpty() )
				sResult.AppendFormat( _T("<w:pPr>%s</w:pPr>"), sParProp );
			CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
			if( false == sCharProp.IsEmpty() )
				sResult.AppendFormat( _T("<w:rPr>%s</w:rPr>"), sCharProp );
			sResult.Append( RenderToOOXEnd( oRenderParameter ) );
			return sResult;
		}

//CString RtfTableStyleProperty::RenderToRtf(RenderParameter oRenderParameter)
//		{
//			CString sResult;
//			if( m_oShading.IsValid() )
//				sResult.Append( m_oShading.RenderToRtf( oRenderParameter ) );
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
//				case av_tsvertalt: sResult.Append(_T("\\tsvertalt"));break;
//				case av_tsvertalc: sResult.Append(_T("\\tsvertalc"));break;
//				case av_tsvertalb: sResult.Append(_T("\\tsvertalb"));break;
//
//			}
//			RENDER_RTF_BOOL( m_bNoCellWrap, sResult, _T("tsnowrap") )
//
//			if( true == m_oCellTopBorder.IsValid() )
//			{
//				sResult.Append( _T("\\tsbrdrt"));
//				sResult.Append( m_oCellTopBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellBottomBorder.IsValid() )
//			{
//				sResult.Append( _T("\\tsbrdrb"));
//				sResult.Append( m_oCellBottomBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellLeftBorder.IsValid() )
//			{
//				sResult.Append( _T("\\tsbrdrl"));
//				sResult.Append( m_oCellLeftBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellRightBorder.IsValid() )
//			{
//				sResult.Append( _T("\\tsbrdrr"));
//				sResult.Append( m_oCellRightBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellHorBorder.IsValid() )
//			{
//				sResult.Append( _T("\\tsbrdrh"));
//				sResult.Append( m_oCellHorBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellVerBorder.IsValid() )
//			{
//				sResult.Append( _T("\\tsbrdrv"));
//				sResult.Append( m_oCellVerBorder.RenderToRtf(  oRenderParameter ) );
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
			CString sResult;
			sResult.Append( RenderToRtfBegin( oRenderParameter ) );
			RenderParameter oNewParameter = oRenderParameter;
			oNewParameter.nType = RENDER_TO_RTF_PARAM_NO_WROWD;
			sResult.Append( m_oTableProp.RenderToRtf( oNewParameter ) );
			sResult.Append( m_oRowProp.RenderToRtf( oNewParameter ) );
			//sResult.Append( m_oTableStyleProperty.RenderToRtf(  oRenderParameter ) );
			sResult.Append( m_oParProp.RenderToRtf(  oRenderParameter ) );
			sResult.Append( m_oCharProp.RenderToRtf(  oRenderParameter ) );
			sResult.Append( RenderToRtfEnd( oRenderParameter ) );
			if( NULL != m_oFirstRow )
				sResult.Append( m_oFirstRow->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oLastRow )
				sResult.Append( m_oLastRow->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oFirstCol )
				sResult.Append( m_oFirstCol->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oLastCol )
				sResult.Append( m_oLastCol->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oBandHorEven )
				sResult.Append( m_oBandHorEven->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oBandVerEven )
				sResult.Append( m_oBandVerEven->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oBandHorOdd )
				sResult.Append( m_oBandHorOdd->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oBandVerOdd )
				sResult.Append( m_oBandVerOdd->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oNWCell )
				sResult.Append( m_oNWCell->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oNECell )
				sResult.Append( m_oNECell->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oSWCell )
				sResult.Append( m_oSWCell->RenderToRtf( oRenderParameter ) );
			if( NULL != m_oSECell )
				sResult.Append( m_oSECell->RenderToRtf( oRenderParameter ) );

			return sResult;
		}
CString RtfTableStyle::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( RENDER_TO_OOX_PARAM_NESTED != oRenderParameter.nType )
			{
				sResult.Append( RenderToOOXBegin( oRenderParameter ) );
				CString sTablProp = m_oTableProp.RenderToOOX(oRenderParameter);
				if( false == sTablProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:tblPr>%s</w:tblPr>"), sTablProp );
				CString sRowProp = m_oRowProp.RenderToOOX(oRenderParameter);
				if( false == sRowProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:trPr>%s</w:trPr>"), sRowProp );
				CString sCellProp = m_oCellProp.RenderToOOX(oRenderParameter);
				if( false == sCellProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:tcPr>%s</w:tcPr>"), sCellProp );
				CString sParProp = m_oParProp.RenderToOOX(oRenderParameter);
				if( false == sParProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:pPr>%s</w:pPr>"), sParProp );
				CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
				if( false == sCharProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:rPr>%s</w:rPr>"), sCharProp );

				RenderParameter oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_NESTED;

				if( NULL != m_oFirstRow )
				{
					oNewParam.sValue = _T("firstRow");
					sResult.Append( m_oFirstRow->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oLastRow )
				{
					oNewParam.sValue = _T("lastRow");
					sResult.Append( m_oLastRow->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oFirstCol )
				{
					oNewParam.sValue = _T("firstCol");
					sResult.Append( m_oFirstCol->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oLastCol )
				{
					oNewParam.sValue = _T("lastCol");
					sResult.Append( m_oLastCol->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oBandHorEven )
				{
					oNewParam.sValue = _T("band2Horz");
					sResult.Append( m_oBandHorEven->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oBandVerEven )
				{
					oNewParam.sValue = _T("band2Vert");
					sResult.Append( m_oBandVerEven->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oBandHorOdd )
				{
					oNewParam.sValue = _T("band1Horz");
					sResult.Append( m_oBandHorOdd->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oBandVerOdd )
				{
					oNewParam.sValue = _T("band1Vert");
					sResult.Append( m_oBandVerOdd->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oNWCell )
				{
					oNewParam.sValue = _T("nwCell");
					sResult.Append( m_oNWCell->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oNECell )
				{
					oNewParam.sValue = _T("neCell");
					sResult.Append( m_oNECell->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oSWCell )
				{
					oNewParam.sValue = _T("swCell");
					sResult.Append( m_oSWCell->RenderToOOX(oNewParam) );
				}
				if( NULL != m_oSECell )
				{
					oNewParam.sValue = _T("seCell");
					sResult.Append( m_oSECell->RenderToOOX(oNewParam) );
				}
				sResult.Append( RenderToOOXEnd( oRenderParameter ) );
			}
			else
			{
				sResult.Append( _T("<w:tblStylePr w:type=\"") + oRenderParameter.sValue + _T("\">") );
				CString sTablProp = m_oTableProp.RenderToOOX(oRenderParameter);
				if( false == sTablProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:tblPr>%s</w:tblPr>"), sTablProp );
				CString sRowProp = m_oRowProp.RenderToOOX(oRenderParameter);
				if( false == sRowProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:trPr>%s</w:trPr>"), sRowProp );
				CString sCellProp = m_oCellProp.RenderToOOX(oRenderParameter);
				if( false == sCellProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:tcPr>%s</w:tcPr>"), sCellProp );
				CString sParProp = m_oParProp.RenderToOOX(oRenderParameter);
				if( false == sParProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:pPr>%s</w:pPr>"), sParProp );
				CString sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
				if( false == sCharProp.IsEmpty() )
					sResult.AppendFormat( _T("<w:rPr>%s</w:rPr>"), sCharProp );
				sResult.Append( _T("</w:tblStylePr>") );
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
						case tl_dot:sResult.Append( _T("\\tldot") );break;
						case tl_mdot:sResult.Append( _T("\\tlmdot") );break;
						case tl_hyph:sResult.Append( _T("\\tlhyph") );break;
						case tl_ul:sResult.Append( _T("\\tlul") );break;
					}
					sResult.AppendFormat( _T("\\tb%d"), m_nTab );
				}
				else
				{
					switch( m_eLeader )
					{
						case tl_dot:sResult.Append( _T("\\tldot") );break;
						case tl_mdot:sResult.Append( _T("\\tlmdot") );break;
						case tl_hyph:sResult.Append( _T("\\tlhyph") );break;
						case tl_ul:sResult.Append( _T("\\tlul") );break;
					}
					switch( m_eKind )
					{
						case tk_tqr:sResult.Append( _T("\\tqr") );break;
						case tk_tqc:sResult.Append( _T("\\tqc") );break;
						case tk_tqdec:sResult.Append( _T("\\tqdec") );break;
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
				case tl_dot:sTab.Append( _T(" w:leader=\"dot\"") );break;
				case tl_mdot:sTab.Append( _T(" w:leader=\"middleDot\"") );break;
				case tl_hyph:sTab.Append( _T(" w:leader=\"hyphen\"") );break;
				case tl_ul:sTab.Append( _T(" w:leader=\"underscore\"") );break;
			}
			switch( m_eKind )
			{
				case tk_tql:sTab.Append( _T(" w:val=\"left\"") );break;
				case tk_tqr:sTab.Append( _T(" w:val=\"right\"") );break;
				case tk_tqc:sTab.Append( _T(" w:val=\"center\"") );break;
				case tk_tqdec:sTab.Append( _T(" w:val=\"decimal\"") );break;
				case tk_tqbar:sTab.Append( _T(" w:val=\"bar\"") );break;
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
				case hr_phmrg: sResult.Append(_T("\\phmrg"));break;
				case hr_phpg: sResult.Append(_T("\\phpg"));break;
				case hr_phcol: sResult.Append(_T("\\phcol"));break;
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
				case hp_posxc: sResult.Append(_T("\\posxc"));break;
				case hp_posxi: sResult.Append(_T("\\posxi"));break;
				case hp_posxo: sResult.Append(_T("\\posxo"));break;
				case hp_posxl: sResult.Append(_T("\\posxl"));break;
				case hp_posxr: sResult.Append(_T("\\posxr"));break;
			}

			switch ( m_eVRef )
			{	
				case vr_pvmrg: sResult.Append(_T("\\pvmrg"));break;
				case vr_pvpg: sResult.Append(_T("\\pvpg"));break;
				case vr_pvpara: sResult.Append(_T("\\pvpara"));break;
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
				case vp_posyt: sResult.Append(_T("\\posyt"));break;
				case vp_posyil: sResult.Append(_T("\\posyil"));break;
				case vp_posyb: sResult.Append(_T("\\posyb"));break;
				case vp_posyc: sResult.Append(_T("\\posyc"));break;
				case vp_posyin: sResult.Append(_T("\\posyin"));break;
				case vp_posyout: sResult.Append(_T("\\posyout"));break;
			}
			RENDER_RTF_INT( m_bLockAnchor, sResult, _T("abslock") )

			switch ( m_eWrap )
			{
				case tw_wrapdefault: sResult.Append(_T("\\wrapdefault"));break;
				case tw_wraparound: sResult.Append(_T("\\wraparound"));break;
				case tw_wraptight: sResult.Append(_T("\\wraptight"));break;
				case tw_wrapthrough: sResult.Append(_T("\\wrapthrough"));break;
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
					sFrame.Append(_T(" w:hRule=\"atLeast\""));
				}
				else if( m_nHeight == 0 )
				{
					sFrame.Append(_T(" w:hRule=\"auto\""));
				}
				else
				{
					m_nHeight *= -1;
					RENDER_OOX_INT_ATTRIBUTE( m_nHeight, sFrame, _T("w:h") )
					sFrame.Append(_T(" w:hRule=\"exact\""));
				}
			}
			
			switch ( m_eHRef )
			{
				case hr_phmrg:sFrame.Append(_T(" w:hAnchor=\"margin\""));break;
				case hr_phpg: sFrame.Append(_T(" w:hAnchor=\"page\""));break;
				case hr_phcol: sFrame.Append(_T(" w:hAnchor=\"text\""));break;
			}
			RENDER_OOX_INT_ATTRIBUTE( m_nHPos, sFrame, _T("w:x") )
			switch ( m_eHPos )
			{
				case hp_posxc: sFrame.Append(_T(" w:xAlign=\"center\""));break;
				case hp_posxi: sFrame.Append(_T(" w:xAlign=\"inside\""));break;
				case hp_posxo: sFrame.Append(_T(" w:xAlign=\"outside\""));break;
				case hp_posxl: sFrame.Append(_T(" w:xAlign=\"left\""));break;
				case hp_posxr: sFrame.Append(_T(" w:xAlign=\"right\""));break;
			}
			switch ( m_eVRef )
			{	
				case vr_pvmrg: sFrame.Append(_T(" w:vAnchor=\"margin\""));break;
				case vr_pvpg: sFrame.Append(_T(" w:vAnchor=\"page\""));break;
				case vr_pvpara: sFrame.Append(_T(" w:vAnchor=\"text\""));break;
			}
			RENDER_OOX_INT_ATTRIBUTE( m_nVPos, sFrame, _T("w:y") )
			switch ( m_eVPos )
			{
				case vp_posyt: sFrame.Append(_T(" w:yAlign=\"top\""));break;
				case vp_posyil: sFrame.Append(_T(" w:yAlign=\"inline\""));break;
				case vp_posyb: sFrame.Append(_T(" w:yAlign=\"bottom\""));break;
				case vp_posyc: sFrame.Append(_T(" w:yAlign=\"center\""));break;
				case vp_posyin: sFrame.Append(_T(" w:yAlign=\"inside\""));break;
				case vp_posyout: sFrame.Append(_T(" w:yAlign=\"outside\""));break;
			}
			RENDER_OOX_BOOL_ATTRIBUTE( m_bLockAnchor, sFrame, _T("w:anchorLock") )
			switch ( m_eWrap )
			{
				//case tw_wrapdefault: sFrame.Append(_T(" w:wrap=\"auto\""));break;
				case tw_wraparound: sFrame.Append(_T(" w:wrap=\"around\""));break;
				case tw_wraptight: sFrame.Append(_T(" w:wrap=\"tight\""));break;
				case tw_wrapthrough: sFrame.Append(_T(" w:wrap=\"through\""));break;
			}
			if( 1 == m_DropcapType )
				sFrame.Append(_T(" w:dropCap=\"drop\""));
			else if( 2 == m_DropcapType )
				sFrame.Append(_T(" w:dropCap=\"margin\""));
			RENDER_OOX_INT_ATTRIBUTE( m_DropcapLines, sFrame, _T("w:lines") )

			if( PROP_DEF != m_nHorSpace )
				sFrame.AppendFormat( _T(" w:hSpace=\"%d\""), m_nHorSpace );
			else if( PROP_DEF != m_nAllSpace )
				sFrame.AppendFormat( _T(" w:hSpace=\"%d\""), m_nAllSpace );
			if( PROP_DEF != m_nVerSpace )
				sFrame.AppendFormat( _T(" w:vSpace=\"%d\""), m_nVerSpace );
			else if( PROP_DEF != m_nAllSpace )
				sFrame.AppendFormat( _T(" w:vSpace=\"%d\""), m_nAllSpace );

			if( false == sFrame.IsEmpty() )
				sResult = _T("<w:framePr ") + sFrame + _T("/>");

			return sResult;
		}
CString RtfParagraphProperty::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			//RENDER_RTF_INT( m_nStyle, sResult, _T("s") );
			RENDER_RTF_BOOL( m_bAutoHyphenation, sResult, _T("hyphpar") );
			RENDER_RTF_BOOL( m_bInTable, sResult, _T("intbl") );
			if( PROP_DEF != m_nItap && 1 != m_nItap )
				sResult.AppendFormat( _T("\\itap%d"),m_nItap );
			RENDER_RTF_BOOL( m_bKeep, sResult, _T("keep") );
			RENDER_RTF_BOOL( m_bKeepNext, sResult, _T("keepn") );
			RENDER_RTF_INT( m_nOutlinelevel, sResult, _T("outlinelevel") );
			RENDER_RTF_BOOL( m_bPageBB, sResult, _T("pagebb") );


			switch(m_eAlign)
			{
				case  pa_qc:sResult.Append( _T("\\qc") );break;  
				case  pa_qj:sResult.Append( _T("\\qj") );break;  
				case  pa_ql:sResult.Append( _T("\\ql") );break;  
				case  pa_qr:sResult.Append( _T("\\qr") );break;  
				case  pa_qd:sResult.Append( _T("\\qd") );break;  
				case  pa_qk0:sResult.Append( _T("\\qk0") );break;  
				case  pa_qk10:sResult.Append( _T("\\qk10") );break;  
				case  pa_qk20:sResult.Append( _T("\\qk20") );break;  
			}
			switch( m_eFontAlign )
			{
				case fa_faauto:sResult.Append( _T("\\faauto") );break;
				case fa_fahang:sResult.Append( _T("\\fahang") );break;
				case fa_facenter:sResult.Append( _T("\\facenter") );break;
				case fa_faroman:sResult.Append( _T("\\faroman") );break;
				case fa_favar:sResult.Append( _T("\\favar") );break;
				case fa_fafixed:sResult.Append( _T("\\fafixed") );break;
			}
			RENDER_RTF_INT( m_nIndFirstLine, sResult, _T("fi") );
			RENDER_RTF_INT( m_nIndLeft, sResult, _T("li") );
			RENDER_RTF_INT( m_nIndStart, sResult, _T("lin") );
			RENDER_RTF_INT( m_nIndRight, sResult, _T("ri") );
			RENDER_RTF_INT( m_nIndEnd, sResult, _T("rin") );
			RENDER_RTF_BOOL( m_bIndRightAuto, sResult, _T("adjustright") );
			RENDER_RTF_BOOL( m_bIndMirror, sResult, _T("indmirror") );
			RENDER_RTF_INT( m_nSpaceBefore, sResult, _T("sb") );
			RENDER_RTF_INT( m_nSpaceAfter, sResult, _T("sa") );
			RENDER_RTF_INT( m_nSpaceBeforeAuto, sResult, _T("sbauto") );
			RENDER_RTF_INT( m_nSpaceAfterAuto, sResult, _T("saauto") );
			RENDER_RTF_INT( m_nSpaceBeforeLine, sResult, _T("lisb") );
			RENDER_RTF_INT( m_nSpaceAfterLine, sResult, _T("lisa") );
			RENDER_RTF_INT( m_nSpaceBetween, sResult, _T("sl") );
			RENDER_RTF_INT( m_nSpaceMultiLine, sResult, _T("slmult") );
			RENDER_RTF_BOOL( m_bContextualSpacing, sResult, _T("contextualspace") );
			if( 0 == m_bRtl )
				sResult.Append(_T("\\ltrpar"));
			else if( PROP_DEF != m_bRtl )
				sResult.Append(_T("\\rtlpar"));
			RENDER_RTF_BOOL( m_bNoWordWrap, sResult, _T("nowwrap") );
			if( 0 == m_bSnapToGrid )
				sResult.Append( _T("\\nosnaplinegrid") );
			else if(  PROP_DEF != m_bSnapToGrid  )
				sResult.Append( _T("\\nosnaplinegrid0") );
			switch ( m_eTextBoxWrap )
			{
				case tbw_txbxtwno:sResult.Append(_T("\\txbxtwno"));break;
				case tbw_txbxtwalways:sResult.Append(_T("\\txbxtwalways"));break;
				case tbw_txbxtwfirstlast:sResult.Append(_T("\\txbxtwfirstlast"));break;
				case tbw_txbxtwfirst:sResult.Append(_T("\\txbxtwfirst"));break;
				case tbw_txbxtwlast:sResult.Append(_T("\\txbxtwlast"));break;
			}
			if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
			{
				sResult.AppendFormat( _T("\\ls%d"), m_nListId );
				sResult.AppendFormat( _T("\\ilvl%d"), m_nListLevel );
			}

			if( true == m_oShading.IsValid() )
				sResult.Append( m_oShading.RenderToRtf( oRenderParameter ) );

			if( true == m_oBorderTop.IsValid() )
			{
				sResult.Append(_T("\\brdrt"));
				sResult.Append( m_oBorderTop.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderLeft.IsValid() )
			{
				sResult.Append(_T("\\brdrl"));
				sResult.Append( m_oBorderLeft.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderBottom.IsValid() )
			{
				sResult.Append(_T("\\brdrb"));
				sResult.Append( m_oBorderBottom.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderRight.IsValid() )
			{
				sResult.Append(_T("\\brdrr"));
				sResult.Append( m_oBorderRight.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderBox.IsValid() )
			{
				sResult.Append(_T("\\box"));
				sResult.Append( m_oBorderBox.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderBar.IsValid() )
			{
				sResult.Append(_T("\\brdrbar"));
				sResult.Append( m_oBorderBar.RenderToRtf( oRenderParameter ) );
			}

			if( true == m_oFrame.IsValid() )
				sResult.Append( m_oFrame.RenderToRtf( oRenderParameter ) );
			RENDER_RTF_BOOL( m_bOverlap, sResult, _T("absnoovrlp") );

			switch ( m_eTextFollow )
			{
				case tf_frmtxlrtb: sResult.Append(_T("\\frmtxlrtb"));break;
				case tf_frmtxtbrl: sResult.Append(_T("\\frmtxtbrl"));break;
				case tf_frmtxbtlr: sResult.Append(_T("\\frmtxbtlr"));break;
				case tf_frmtxlrtbv: sResult.Append(_T("\\frmtxlrtbv"));break;
				case tf_frmtxtbrlv: sResult.Append(_T("\\frmtxtbrlv"));break;
			}
			if( true == m_oTabs.IsValid() )
				sResult.Append( m_oTabs.RenderToRtf( oRenderParameter ) );


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


			//дописываем текст списка (дл€ старых reader например  wordPad)
			if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
			{
				RtfListProperty oListProperty;
				RtfDocument* poRtfDocument = static_cast<RtfDocument*>(  oRenderParameter.poDocument );
				RtfListOverrideProperty oListOverrideProperty;
				//ищем по override table
				if( true == poRtfDocument->m_oListOverrideTabel.GetList( m_nListId, oListOverrideProperty ) )
				{
					//»щем по List Table
					if( true == poRtfDocument->m_oListTabel.GetList( oListOverrideProperty.m_nListID, oListProperty) )
					{
						//дописываем свойства параграфа firstIndent Indent
						RtfListLevelProperty poLevelProp ;
						if( true == oListProperty.GetItem( poLevelProp , m_nListLevel ) )
						{
							sResult.Append(_T("{\\listtext\\pard\\plain"));
							sResult.Append( poLevelProp.m_oCharProp.RenderToRtf( oRenderParameter ) );
							//пишем текст 
							sResult.Append( RtfChar::renderRtfText( poLevelProp.GenerateListText(), oRenderParameter.poDocument, NULL ) );
							//или картинку
							if( PROP_DEF != poLevelProp.m_nPictureIndex )
							{
								int nIndex = poLevelProp.m_nPictureIndex;

								if( 0 < nIndex && nIndex < poRtfDocument->m_oListTabel.m_aPictureList.GetCount() )
									sResult.Append( poRtfDocument->m_oListTabel.m_aPictureList[nIndex]->RenderToRtf( oRenderParameter ) );
							}
							//ставим tab
							if( PROP_DEF != poLevelProp.m_nFollow )
							{
								switch( poLevelProp.m_nFollow )
								{
									case 0: sResult.Append( _T("\\tab") );break; // tab
									case 1: sResult.Append( _T(" ") );break; // space
									case 2: break; // nothing
								}
							}
							sResult.Append(_T("}"));
						}
					}
				}
			}
			return sResult;
		}
CString RtfParagraphProperty::RenderToOOX(RenderParameter oRenderParameter)
		{
			RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
			OOXWriter* poOOXWriter = static_cast<OOXWriter*>(oRenderParameter.poWriter);
			CString sResult;
			//if( PROP_DEF != m_nStyle )
			//{
			//	RtfStylePtr oCurStile;
			//	if( true == poRtfDocument->m_oStyleTable.GetStyle( m_nStyle, oCurStile ) )
			//		RENDER_OOX_STRING( oCurStile->m_sName, sResult, _T("w:pStyle") );
			//}
			if( 0 == m_bAutoHyphenation )
				sResult.Append( _T("<w:suppressAutoHyphens/>") );
			else if( 1 == m_bAutoHyphenation )
				sResult.Append( _T("<w:suppressAutoHyphens w:val=\"false\"/>") );
			RENDER_OOX_BOOL( m_bKeep, sResult, _T("w:keepLines") );
			RENDER_OOX_BOOL( m_bKeepNext, sResult, _T("w:keepNext") );
			RENDER_OOX_INT( m_nOutlinelevel, sResult, _T("w:outlineLvl") );
			RENDER_OOX_BOOL( m_bPageBB, sResult, _T("w:pageBreakBefore") );


			switch(m_eAlign)
			{
				case  pa_qc:sResult.Append( _T("<w:jc w:val=\"center\" />") );break;
				case  pa_qj:sResult.Append( _T("<w:jc w:val=\"both\" />") );break;
				case  pa_ql:sResult.Append( _T("<w:jc w:val=\"left\" />") );break;
				case  pa_qr:sResult.Append( _T("<w:jc w:val=\"right\" />") );break;
				case  pa_qd:sResult.Append( _T("<w:jc w:val=\"distribute\" />") );break;
				case  pa_qk0:sResult.Append( _T("<w:jc w:val=\"lowKashida\" />") );break;
				case  pa_qk10:sResult.Append( _T("<w:jc w:val=\"mediumKashida\" />") );break;
				case  pa_qk20:sResult.Append( _T("<w:jc w:val=\"highKashida\" />") );break;
			}
			switch( m_eFontAlign )
			{
				case fa_faauto:sResult.Append( _T("<w:textAlignment w:val=\"auto\" />") );break;
				case fa_fahang:sResult.Append( _T("<w:textAlignment w:val=\"top\" />") );break;
				case fa_facenter:sResult.Append( _T("<w:textAlignment w:val=\"center\" />") );break;
				case fa_faroman:sResult.Append( _T("<w:textAlignment w:val=\"baseline\" />") );break;
				case fa_favar:sResult.Append( _T("<w:textAlignment w:val=\"bottom\" />") );break;
				case fa_fafixed:sResult.Append( _T("<w:textAlignment w:val=\"bottom\" />") );break;
			}
			CString sIndent;
			if( PROP_DEF != m_nIndFirstLine )
			{
				if( m_nIndFirstLine >= 0 )
					sIndent.AppendFormat( _T(" w:firstLine=\"%d\""), m_nIndFirstLine );
				else
					sIndent.AppendFormat( _T(" w:hanging=\"%d\""), -m_nIndFirstLine );
			}
			RENDER_OOX_INT_ATTRIBUTE( m_nIndLeft, sIndent, _T("w:left") );
			RENDER_OOX_INT_ATTRIBUTE( m_nIndRight, sIndent, _T("w:right") );
			RENDER_OOX_INT_ATTRIBUTE( m_nIndStart, sIndent, _T("w:start") );
			RENDER_OOX_INT_ATTRIBUTE( m_nIndEnd, sIndent, _T("w:end") );
			if( false == sIndent.IsEmpty() )
				sResult.AppendFormat(_T("<w:ind %s/>"), sIndent);

			RENDER_OOX_BOOL( m_bIndRightAuto, sResult, _T("w:adjustRightInd") );
			RENDER_OOX_BOOL( m_bIndMirror, sResult, _T("w:mirrorIndents") );

			CString sSpacing;
			if( PROP_DEF == m_nSpaceAfter )
				m_nSpaceAfter = 0;
			//if( PROP_DEF == m_nSpaceBefore )
			//	m_nSpaceBefore = 0;
			RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBefore, sSpacing, _T("w:before") );
			RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfter, sSpacing, _T("w:after") );
			RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBeforeAuto, sSpacing, _T("w:beforeAutospacing") );
			RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfterAuto, sSpacing, _T("w:afterAutospacing") );
			RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBeforeLine, sSpacing, _T("w:beforeLines") );
			RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfterLine, sSpacing, _T("w:afterLines") );
			if( PROP_DEF != m_nSpaceBetween && 0 != m_nSpaceBetween )
			{
				if( m_nSpaceBetween > 0 )
					sSpacing.AppendFormat(_T(" w:line=\"%d\""), m_nSpaceBetween);
				else
					sSpacing.AppendFormat(_T(" w:line=\"%d\""), -m_nSpaceBetween);
				if( 0 == m_nSpaceMultiLine )
				{
					if( m_nSpaceBetween > 0 )
						sSpacing.Append(_T(" w:lineRule=\"atLeast\""));
					else
						sSpacing.Append(_T(" w:lineRule=\"exact\""));
				}
			}
			//else
			//	sSpacing.AppendFormat(_T(" w:line=\"240\"")); //по умолчанию - единичный
			if( false == sSpacing.IsEmpty() )
				sResult.AppendFormat(_T("<w:spacing %s/>"), sSpacing);

			RENDER_OOX_BOOL( m_bSnapToGrid, sResult, _T("w:snapToGrid") );
			RENDER_OOX_BOOL( m_bContextualSpacing, sResult, _T("w:contextualSpacing") );
			RENDER_OOX_BOOL( m_bRtl, sResult, _T("w:bidi") );
			if( 0 == m_bNoWordWrap )
				sResult.Append( _T("<w:wordWrap w:val=\"false\"/>") );
			else if(  PROP_DEF != m_bNoWordWrap  )
				sResult.Append( _T("<w:wordWrap w:val=\"true\"/>") );
			switch ( m_eTextBoxWrap )
			{
				case tbw_txbxtwno:sResult.Append(_T("<w:textboxTightWrap w:val=\"none\" />"));break;
				case tbw_txbxtwalways:sResult.Append(_T("<w:textboxTightWrap w:val=\"allLines\" />"));break;
				case tbw_txbxtwfirstlast:sResult.Append(_T("<w:textboxTightWrap w:val=\"firstAndLastLine\" />"));break;
				case tbw_txbxtwfirst:sResult.Append(_T("<w:textboxTightWrap w:val=\"firstLineOnly\" />"));break;
				case tbw_txbxtwlast:sResult.Append(_T("<w:textboxTightWrap w:val=\"lastLineOnly\" />"));break;
			}
			if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
				sResult.AppendFormat( _T("<w:numPr><w:ilvl w:val=\"%d\" /><w:numId w:val=\"%d\" /></w:numPr>"), m_nListLevel, m_nListId );


			if( true == m_oShading.IsValid() )
				sResult.Append( m_oShading.RenderToOOX(oRenderParameter) );

			RenderParameter oNewParam = oRenderParameter;
			oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE;
			CString sBorder;
			if( true == m_oBorderBox.IsValid() )
			{
				CString sBorderContent = m_oBorderBox.RenderToOOX(oNewParam);
				sBorder.AppendFormat(_T("<w:left %s />"), sBorderContent );
				sBorder.AppendFormat(_T("<w:top %s />"), sBorderContent );
				sBorder.AppendFormat(_T("<w:right %s />"), sBorderContent );
				sBorder.AppendFormat(_T("<w:bottom %s />"), sBorderContent );
			}
			else
			{
				if( true == m_oBorderTop.IsValid() )
					sBorder.AppendFormat(_T("<w:top %s />"), m_oBorderTop.RenderToOOX(oNewParam) );
				if( true == m_oBorderLeft.IsValid() )
					sBorder.AppendFormat(_T("<w:left %s />"), m_oBorderLeft.RenderToOOX(oNewParam) );
				if( true == m_oBorderBottom.IsValid() )
					sBorder.AppendFormat( _T("<w:bottom %s />"),m_oBorderBottom.RenderToOOX(oNewParam) );
				if( true == m_oBorderRight.IsValid() )
					sBorder.AppendFormat(_T("<w:right %s />"), m_oBorderRight.RenderToOOX(oNewParam) );
			}

			if( true == m_oBorderBar.IsValid() )
				sBorder.AppendFormat(_T("<w:between %s />"), m_oBorderBar.RenderToOOX(oNewParam) );
			if( false == sBorder.IsEmpty() )
				sResult.AppendFormat( _T("<w:pBdr>%s</w:pBdr>"), sBorder );

			if( true == m_oFrame.IsValid() )
				sResult.Append( m_oFrame.RenderToOOX( oRenderParameter ) );
			RENDER_OOX_BOOL( m_bOverlap, sResult, _T("w:suppressOverlap") );

			switch ( m_eTextFollow )
			{
				case tf_frmtxlrtb: sResult.Append( _T("<w:textFlow w:val=\"lrtb\" />") );break;
				case tf_frmtxtbrl: sResult.Append( _T("<w:textFlow w:val=\"tbrl\" />") );break;
				case tf_frmtxbtlr: sResult.Append( _T("<w:textFlow w:val=\"btlr\" />") );break;
				case tf_frmtxlrtbv: sResult.Append( _T("<w:textFlow w:val=\"lrtbV\" />") );break;
				case tf_frmtxtbrlv: sResult.Append( _T("<w:textFlow w:val=\"tbrlV\" />") );break;
			}
			if( true == m_oTabs.IsValid() )
				sResult.AppendFormat( m_oTabs.RenderToOOX( oRenderParameter ) );
			CString sCharProp = m_oCharProperty.RenderToOOX( oRenderParameter );
			if( false == sCharProp.IsEmpty() )
				sResult.AppendFormat( _T("<w:rPr>%s</w:rPr>"), sCharProp );
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
				sResult.Append(_T("\\cldglu"));
				sResult.Append( m_oBorderDiagonalLR.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderDiagonalRL.IsValid() )
			{
				sResult.Append(_T("\\cldgll"));
				sResult.Append( m_oBorderDiagonalRL.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderLeft.IsValid() )
			{
				sResult.Append(_T("\\clbrdrl"));
				sResult.Append( m_oBorderLeft.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderTop.IsValid() )
			{
				sResult.Append(_T("\\clbrdrt"));
				sResult.Append( m_oBorderTop.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderRight.IsValid() )
			{
				sResult.Append(_T("\\clbrdrr"));
				sResult.Append( m_oBorderRight.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oBorderBottom.IsValid() )
			{
				sResult.Append(_T("\\clbrdrb"));
				sResult.Append( m_oBorderBottom.RenderToRtf( oRenderParameter ) );
			}
			if( true == m_oShading.IsValid() )
				sResult.Append( m_oShading.RenderToRtf( oRenderParameter ) );

			switch( m_eAlign )
			{
				case ca_Top: sResult.Append(_T("\\clvertalt"));break;
				case ca_Center: sResult.Append(_T("\\clvertalc"));break;
				case ca_Bottom: sResult.Append(_T("\\clvertalb"));break;
			}
			switch( m_oCellFlow )
			{
				case cf_lrtb: sResult.Append( _T("\\cltxlrtb") );break;
				case cf_tbrl: sResult.Append( _T("\\cltxtbrl") );break;
				case cf_btlr: sResult.Append( _T("\\cltxbtlr") );break;
				case cf_lrtbv: sResult.Append( _T("\\cltxlrtbv") );break;
				case cf_tbrlv: sResult.Append( _T("\\cltxtbrlv") );break;
			}
			RENDER_RTF_INT( m_nWidth, sResult, _T("clwWidth") )
			switch( m_eWidthUnits )
			{
				case mu_none: sResult.Append(_T("\\clftsWidth0"));break;
				case mu_Auto: sResult.Append(_T("\\clftsWidth1"));break;
				case mu_Percent: sResult.Append(_T("\\clftsWidth2"));break;
				case mu_Twips: sResult.Append(_T("\\clftsWidth3"));break;
			}
			//ATLASSERT(PROP_DEF != m_nCellx );
			sResult.AppendFormat(_T("\\cellx%d"),m_nCellx);

			return sResult;
		}
CString RtfCellProperty::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( 1 == m_bMergeFirst )
				sResult.Append(_T("<w:hMerge w:val=\"restart\"/>"));
			if( 1 == m_bMerge )
				sResult.Append(_T("<w:hMerge w:val=\"continue\"/>"));
			if( 1 == m_bMergeFirstVertical )
				sResult.Append(_T("<w:vMerge w:val=\"restart\"/>"));
			if( 1 == m_bMergeVertical)
				sResult.Append(_T("<w:vMerge w:val=\"continue\"/>"));

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
			if( false == sMargin.IsEmpty() )
				sResult.AppendFormat(_T("<w:tcMar>%s</w:tcMar>"),sMargin);

			//if( true == m_bIsSpacingLeft )
			//	sResult.Append(_T("trspdl3"));
			//else	
			//	sResult.Append(_T("trspdl0"));
			//if( PROP_DEF != m_nSpacingLeft )
			//	sResult.AppendFormat(_T("trspdfl%d"),m_nSpacingLeft);
			//if( true == m_bIsSpacingRight )
			//	sResult.Append(_T("trspdr3"));
			//else	
			//	sResult.Append(_T("trspdr0"));
			//if( PROP_DEF != m_nSpacingRight )
			//	sResult.AppendFormat(_T("trspdfr%d"),m_nSpacingRight);
			//if( true == m_bIsSpacingTop )
			//	sResult.Append(_T("trspdt3"));
			//else	
			//	sResult.Append(_T("trspdt0"));
			//if( PROP_DEF != m_nSpacingTop )
			//	sResult.AppendFormat(_T("trspdft%d"),m_nSpacingTop);
			//if( true == m_bIsSpacingBottom )
			//	sResult.Append(_T("trspdb3"));
			//else	
			//	sResult.Append(_T("trspdb0"));
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
				sBorder.Append( m_oBorderDiagonalLR.RenderToOOX(oNewParam) );
			}
			if( true == m_oBorderDiagonalRL.IsValid() )
			{
				oNewParam.sValue = _T("w:tr2bl");
				sBorder.Append( m_oBorderDiagonalRL.RenderToOOX(oNewParam) );
			}
			if( true == m_oBorderLeft.IsValid() )
			{
				oNewParam.sValue = _T("w:left");
				sBorder.Append( m_oBorderLeft.RenderToOOX(oNewParam) );
			}
			if( true == m_oBorderTop.IsValid() )
			{
				oNewParam.sValue = _T("w:top");
				sBorder.Append( m_oBorderTop.RenderToOOX(oNewParam) );
			}
			if( true == m_oBorderRight.IsValid() )
			{
				oNewParam.sValue = _T("w:right");
				sBorder.Append( m_oBorderRight.RenderToOOX(oNewParam) );
			}
			if( true == m_oBorderBottom.IsValid() )
			{
				oNewParam.sValue = _T("w:bottom");
				sBorder.Append( m_oBorderBottom.RenderToOOX(oNewParam) );
			}
			if( true == m_oBorderInsideH.IsValid() )
			{
				oNewParam.sValue = _T("w:insideH");
				sBorder.Append( m_oBorderInsideH.RenderToOOX(oNewParam) );
			}
			if( true == m_oBorderInsideV.IsValid() )
			{
				oNewParam.sValue = _T("w:insideV");
				sBorder.Append( m_oBorderInsideV.RenderToOOX(oNewParam) );
			}
			if( false == sBorder.IsEmpty() )
				sResult.AppendFormat( _T("<w:tcBorders>%s</w:tcBorders>"), sBorder );

			if( true == m_oShading.IsValid() )
				sResult.Append( m_oShading.RenderToOOX(oRenderParameter) );
			
			switch( m_eAlign )
			{
				case ca_Top: sResult.Append( _T("<w:vAlign w:val=\"top\" />") );break;
				case ca_Center: sResult.Append( _T("<w:vAlign w:val=\"center\" />") ) ;break;
				case ca_Bottom: sResult.Append( _T("<w:vAlign w:val=\"bottom\" />") );break;
			}
			switch( m_oCellFlow )
			{
				case cf_lrtb: sResult.Append( _T("<w:textDirection w:val=\"lrTb\" />") );break;
				case cf_tbrl: sResult.Append( _T("<w:textDirection w:val=\"tbRl\" />") );break;
				case cf_btlr: sResult.Append( _T("<w:textDirection w:val=\"btLr\" />") );break;
				case cf_lrtbv: sResult.Append( _T("<w:textDirection w:val=\"lrTbV\" />") );break;
				case cf_tbrlv: sResult.Append( _T("<w:textDirection w:val=\"tbRlV\" />") );break;

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
			//if( false == scnfStyle.IsEmpty() )
			//	sResult.Append( _T("<w:cnfStyle %s/>") );

			//cellx

			return sResult;
		}

CString RtfTableProperty::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			RENDER_RTF_BOOL( m_bBidi, sResult, _T("taprtl") )
			if( PROP_DEF == m_nAutoFit || 1 == m_nAutoFit )
					sResult.Append( _T("\\trautofit1") );
			else if( 0 == m_nAutoFit )
					sResult.Append( _T("\\trautofit0") );
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
				case hr_phmrg: sResult.Append(_T("\\tphmrg"));break;
				case hr_phpg: sResult.Append(_T("\\tphpg"));break;
				case hr_phcol: sResult.Append(_T("\\tphcol"));break;
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
				case hp_posxc: sResult.Append(_T("\\tposxc"));break;
				case hp_posxi: sResult.Append(_T("\\tposxi"));break;
				case hp_posxo: sResult.Append(_T("\\tposxo"));break;
				case hp_posxl: sResult.Append(_T("\\tposxl"));break;
				case hp_posxr: sResult.Append(_T("\\tposxr"));break;
			}

			switch ( m_eVRef )
			{	
				case vr_pvmrg: sResult.Append(_T("\\tpvmrg"));break;
				case vr_pvpg: sResult.Append(_T("\\tpvpg"));break;
				case vr_pvpara: sResult.Append(_T("\\tpvpara"));break;
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
				case vp_posyt: sResult.Append(_T("\\tposyt"));break;
				case vp_posyil: sResult.Append(_T("\\tposyil"));break;
				case vp_posyb: sResult.Append(_T("\\tposyb"));break;
				case vp_posyc: sResult.Append(_T("\\tposyc"));break;
				case vp_posyin: sResult.Append(_T("\\tposyin"));break;
				case vp_posyout: sResult.Append(_T("\\tposyout"));break;
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
				sResult.Append(_T("\\trbrdrl"));
				sResult.Append(m_oBorderLeft.RenderToRtf( oRenderParameter ) );
			}
			if( m_oBorderRight.IsValid() == true )
			{
				sResult.Append(_T("\\trbrdrr"));
				sResult.Append(m_oBorderRight.RenderToRtf( oRenderParameter ) );
			}
			if( m_oBorderTop.IsValid() == true )
			{
				sResult.Append(_T("\\trbrdrt"));
				sResult.Append(m_oBorderTop.RenderToRtf( oRenderParameter ) );
			}
			if( m_oBorderBottom.IsValid() == true )
			{
				sResult.Append(_T("\\trbrdrb"));
				sResult.Append(m_oBorderBottom.RenderToRtf( oRenderParameter ) );
			}
			if( m_oBorderVert.IsValid() == true )
			{
				sResult.Append(_T("\\trbrdrv"));
				sResult.Append(m_oBorderVert.RenderToRtf( oRenderParameter ) );
			}
			if( m_oBorderHor.IsValid() == true )
			{
				sResult.Append(_T("\\trbrdrh"));
				sResult.Append(m_oBorderHor.RenderToRtf( oRenderParameter ) );
			}
			if( m_oShading.IsValid() == true )
				sResult.Append(m_oShading.RenderToRtf( oRenderParameter ) );

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
			//if( PROP_DEF != m_nStyle )
			//{
			//	RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
			//	RtfStylePtr oCurStyle;
			//	if( true == poDocument->m_oStyleTable.GetStyle( m_nStyle, oCurStyle) )
			//	{
			//		sResult.AppendFormat( _T("<w:tblStyle w:val=\"%s\" />") , oCurStyle->m_sName );
			//	}
			//}
			RENDER_OOX_BOOL( m_bBidi, sResult, _T("w:bidiVisual") );
			if( PROP_DEF == m_nAutoFit || 0 == m_nAutoFit )
					sResult.Append( _T("<w:tblLayout w:type=\"fixed\"/>") );
			else if( 1 == m_nAutoFit )
					sResult.Append( _T("<w:tblLayout w:type=\"autofit\"/>") );

			//сделаем не по документации, а как все остальные юниты !!!
			if( PROP_DEF != nTableIndent && 1 != nTableIndentUnits  )
			{
				switch(nTableIndentUnits)
				{
					//case 0: sResult.Append( _T("<w:tblInd w:w=\"0\" w:type=\"dxa\"/>") );break;
					//case 2: sResult.AppendFormat( _T("<w:tblInd w:w=\"%d%%\" w:type=\"pct\"/>"), nTableIndent );break;
					case 3: sResult.AppendFormat( _T("<w:tblInd w:w=\"%d\" w:type=\"dxa\"/>"), nTableIndent );break;
					//default: sResult.AppendFormat( _T("<w:tblInd w:w=\"%d\" w:type=\"dxa\"/>"), nTableIndent );break;
				}
			}

			if( 1 == m_bOverlap )
				sResult.Append(_T("<w:tblOverlap w:val=\"overlap\"/>"));

			switch( m_eJust )
			{
				case rj_trql : sResult.Append( _T("<w:jc w:val=\"left\"/>") );break;
				case rj_trqr : sResult.Append( _T("<w:jc w:val=\"right\"/>") );break;
				case rj_trqc : sResult.Append( _T("<w:jc w:val=\"center\"/>") );break;
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
				case hr_phmrg:sFloatingPosition.Append( _T(" w:horzAnchor=\"margin\"") );break;
				case hr_phpg: sFloatingPosition.Append( _T(" w:horzAnchor=\"page\"") );break;
				case hr_phcol:sFloatingPosition.Append( _T(" w:horzAnchor=\"text\"") );break;
			}
			if( PROP_DEF != m_nHPos )
				sFloatingPosition.AppendFormat( _T(" w:tblpX=\"%d\""), m_nHPos );
			//else
			switch ( m_eHPos )
			{
				case hp_posxc: sFloatingPosition.Append( _T(" w:tblpXSpec=\"center\"") );break;
				case hp_posxi: sFloatingPosition.Append( _T(" w:tblpXSpec=\"inside\"") );break;
				case hp_posxo: sFloatingPosition.Append( _T(" w:tblpXSpec=\"outside\"") );break;
				case hp_posxl: sFloatingPosition.Append( _T(" w:tblpXSpec=\"left\"") );break;
				case hp_posxr: sFloatingPosition.Append( _T(" w:tblpXSpec=\"right\"") );break;
			}

			switch ( m_eVRef )
			{	
				case vr_pvmrg:sFloatingPosition.Append( _T(" w:vertAnchor=\"margin\"") );break;
				case vr_pvpg: sFloatingPosition.Append( _T(" w:vertAnchor=\"page\"") );break;
				case vr_pvpara:sFloatingPosition.Append( _T(" w:vertAnchor=\"text\"") );break;
			}

			if( PROP_DEF != m_nVPos )
				sFloatingPosition.AppendFormat( _T(" w:tblpY=\"%d\""), m_nVPos );
			//else
			switch ( m_eVPos )
			{
				case vp_posyt: sFloatingPosition.Append( _T(" w:tblpYSpec=\"top\"") );break;
				case vp_posyil: sFloatingPosition.Append( _T(" w:tblpYSpec=\"inline\"") );break;
				case vp_posyb: sFloatingPosition.Append( _T(" w:tblpYSpec=\"bottom\"") );break;
				case vp_posyc: sFloatingPosition.Append( _T(" w:tblpYSpec=\"center\"") );break;
				case vp_posyin: sFloatingPosition.Append( _T(" w:tblpYSpec=\"inside\"") );break;
				case vp_posyout: sFloatingPosition.Append( _T(" w:tblpYSpec=\"outside\"") );break;
			}
			if( false == sFloatingPosition.IsEmpty() )
				sResult.AppendFormat( _T("<w:tblpPr %s/>"), sFloatingPosition );

			if( PROP_DEF != m_nWidth && ( 2 == m_eMUWidth || 3 == m_eMUWidth ))
			{
				if( 2 == m_eMUWidth )
					sResult.AppendFormat( _T("<w:tblW w:w=\"%d\" w:type=\"pct\"/>"), m_nWidth );
				else
					sResult.AppendFormat( _T("<w:tblW w:w=\"%d\" w:type=\"dxa\"/>"), m_nWidth );
			}
			else
				sResult.Append(_T("<w:tblW w:w=\"0\" w:type=\"auto\"/>"));

			CString sDefCellMargins;
			if( PROP_DEF != m_nDefCellMarBottom && 3 == m_nDefCellMarBottomUnits )
				sDefCellMargins.AppendFormat( _T("<w:bottom w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarBottom );
			if( PROP_DEF != m_nDefCellMarLeft && 3 == m_nDefCellMarLeftUnits )
				sDefCellMargins.AppendFormat( _T("<w:left w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarLeft );
			if( PROP_DEF != m_nDefCellMarRight && 3 == m_nDefCellMarRightUnits )
				sDefCellMargins.AppendFormat( _T("<w:right w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarRight );
			if( PROP_DEF != m_nDefCellMarTop && 3 == m_nDefCellMarTopUnits )
				sDefCellMargins.AppendFormat( _T("<w:top w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellMarTop );
			if( false == sDefCellMargins.IsEmpty() )
				sResult.AppendFormat( _T("<w:tblCellMar>%s</w:tblCellMar>"), sDefCellMargins );
			
			if( PROP_DEF != m_nDefCellSpBottom && 3 == m_nDefCellSpBottomUnits ) 
				sResult.AppendFormat( _T("<w:tblCellSpacing w:w=\"%d\" w:type=\"dxa\"/>"), m_nDefCellSpBottom );

			//if( PROP_DEF != m_nLeft )
			//	sResult.AppendFormat(_T("<w:tblInd w:w=\"%d\" w:type=\"dxa\"/>"), m_nLeft );

			RenderParameter oNewParam = oRenderParameter;
			oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE;
			CString sBorders;
			if( m_oBorderLeft.IsValid() == true )
				sBorders.AppendFormat(_T("<w:left %s/>"),m_oBorderLeft.RenderToOOX(oNewParam) );
			if( m_oBorderRight.IsValid() == true )
				sBorders.AppendFormat(_T("<w:right %s/>"),m_oBorderRight.RenderToOOX(oNewParam) );
			if( m_oBorderTop.IsValid() == true )
				sBorders.AppendFormat(_T("<w:top %s/>"),m_oBorderTop.RenderToOOX(oNewParam) );
			if( m_oBorderBottom.IsValid() == true )
				sBorders.AppendFormat(_T("<w:bottom %s/>"),m_oBorderBottom.RenderToOOX(oNewParam) );
			if( m_oBorderVert.IsValid() == true )
				sBorders.AppendFormat(_T("<w:insideV %s/>"),m_oBorderVert.RenderToOOX(oNewParam) );
			if( m_oBorderHor.IsValid() == true )
				sBorders.AppendFormat(_T("<w:insideH %s/>"),m_oBorderHor.RenderToOOX(oNewParam) );
			if( false == sBorders.IsEmpty() )
			sResult.AppendFormat(_T("<w:tblBorders>%s</w:tblBorders>"), sBorders );

			if( m_oShading.IsValid() == true )
				sResult.Append(m_oShading.RenderToOOX(oRenderParameter) );


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
			//		sResult.AppendFormat( _T("<w:tblLook %s/>"), sTableLook );
			//}

			if( false == sResult.IsEmpty() )
				sResult = _T("<w:tblPr>") + sResult + _T("</w:tblPr>");
			return sResult;
		}

CString RtfRowProperty::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( RENDER_TO_RTF_PARAM_NO_WROWD != oRenderParameter.nType )
				sResult.Append( _T("\\trowd") );
			RENDER_RTF_INT( m_nIndex, sResult, _T("irow") );
			RENDER_RTF_INT( m_nBandIndex, sResult, _T("irowband") );
			RENDER_RTF_BOOL( m_bLastRow, sResult, _T("lastrow") );
			RENDER_RTF_INT( m_nAutoFit, sResult, _T("trautofit") );
			RENDER_RTF_BOOL( m_bIsHeader, sResult, _T("trhdr") );
			RENDER_RTF_BOOL( m_bKeep, sResult, _T("trkeep") );

			switch( m_eJust )
			{
				case rj_trql : sResult.Append( _T("\\trql") );break;
				case rj_trqr : sResult.Append( _T("\\trqr") );break;
				case rj_trqc : sResult.Append( _T("\\trqc") );break;
			}
			RENDER_RTF_INT( m_nHeight, sResult, _T("trrh") );

			RENDER_RTF_INT( m_nWidth, sResult, _T("trwWidth") );
			switch( m_eMUWidth )
			{
				case mu_Auto: sResult.Append(_T("\\trftsWidth1"));break;
				case mu_Percent: sResult.Append(_T("\\trftsWidth2"));break;
				case mu_Twips: sResult.Append(_T("\\trftsWidth3"));break;
			}
			RENDER_RTF_INT( m_nWidthStartInvCell, sResult, _T("trwWidthB") );
			switch( m_eMUStartInvCell )
			{
				case mu_Auto: sResult.Append(_T("\\trftsWidthB1"));break;
				case mu_Percent: sResult.Append(_T("\\trftsWidthB2"));break;
				case mu_Twips: sResult.Append(_T("\\trftsWidthB3"));break;
			}
			RENDER_RTF_INT( m_nWidthEndInvCell, sResult, _T("trwWidthA") );
			switch( m_eMUEndInvCell )
			{
				case mu_Auto: sResult.Append(_T("\\trftsWidthA1"));break;
				case mu_Percent: sResult.Append(_T("\\trftsWidthA2"));break;
				case mu_Twips: sResult.Append(_T("\\trftsWidthA3"));break;
			}
			sResult.Append( RtfTableProperty::RenderToRtf(  oRenderParameter  ) );
			for( int i = 0; i < (int)m_aArray.GetCount(); i++ )
				sResult.Append( m_aArray[i].RenderToRtf(  oRenderParameter  ) );

			return sResult;
		}
CString RtfRowProperty::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			RENDER_OOX_BOOL( m_bIsHeader, sResult, _T("w:tblHeader") )
			RENDER_OOX_BOOL( m_bKeep, sResult, _T("w:cantSplit") )

			switch( m_eJust )
			{
				case rj_trql : sResult.Append( _T("<w:jc w:val=\"left\"/>") );break;
				case rj_trqr : sResult.Append( _T("<w:jc w:val=\"right\"/>") );break;
				case rj_trqc : sResult.Append( _T("<w:jc w:val=\"center\"/>") );break;
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
					case mu_Percent: sResult.AppendFormat(_T("<w:wBefore w:type=\"pct\" w:w=\"%d%%\"/>"), m_nWidthStartInvCell);break;
					case mu_Twips: sResult.AppendFormat(_T("<w:wBefore w:type=\"dxa\" w:w=\"%d\"/>"), m_nWidthStartInvCell);break;
				}
			}
			if( PROP_DEF != m_nWidthEndInvCell )
			{
				switch( m_eMUEndInvCell )
				{
					case mu_Percent: sResult.AppendFormat(_T("<w:wAfter w:type=\"pct\" w:w=\"%d%%\"/>"), m_nWidthEndInvCell);break;
					case mu_Twips: sResult.AppendFormat(_T("<w:wAfter w:type=\"dxa\" w:w=\"%d\"/>"), m_nWidthEndInvCell);break;
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
			//if( false == scnfStyle.IsEmpty() )
			//	sResult.Append( _T("<w:cnfStyle %s/>") );

			_bstr_t hk;
			return sResult;
		}

CString RtfInformation::RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;

			if( _T("") != m_sTitle )
				sResult.AppendFormat( _T("{\\title %s}"), RtfChar::renderRtfText( m_sTitle, oRenderParameter.poDocument ) );
			if( _T("") != m_sSubject )
				sResult.AppendFormat( _T("{\\subject %s}"), RtfChar::renderRtfText( m_sSubject, oRenderParameter.poDocument ) );
			if( _T("") != m_sAuthor )
				sResult.AppendFormat( _T("{\\author %s}"), RtfChar::renderRtfText( m_sAuthor, oRenderParameter.poDocument ) );
			if( _T("") != m_sManager )
				sResult.AppendFormat( _T("{\\manager %s}"), RtfChar::renderRtfText( m_sManager, oRenderParameter.poDocument ) );
			if( _T("") != m_sCompany )
				sResult.AppendFormat( _T("{\\company %s}"), RtfChar::renderRtfText( m_sCompany, oRenderParameter.poDocument ) );
			if( _T("") != m_sOperator )
				sResult.AppendFormat( _T("{\\operator %s}"), RtfChar::renderRtfText( m_sOperator, oRenderParameter.poDocument ) );
			if( _T("") != m_sCategory )
				sResult.AppendFormat( _T("{\\category %s}"), RtfChar::renderRtfText( m_sCategory, oRenderParameter.poDocument ) );
			if( _T("") != m_sKeywords )
				sResult.AppendFormat( _T("{\\keywords %s}"), RtfChar::renderRtfText( m_sKeywords, oRenderParameter.poDocument ) );
			if( _T("") != m_sComment )
				sResult.AppendFormat( _T("{\\comment %s}"), RtfChar::renderRtfText( m_sComment, oRenderParameter.poDocument ) );

			if( PROP_DEF != m_nVersion )
				sResult.AppendFormat( _T("{\\version%d}"),m_nVersion );

			if( _T("") != m_sDocCom )
				sResult.AppendFormat( _T("{\\doccomm %s}"), RtfChar::renderRtfText( m_sDocCom, oRenderParameter.poDocument ) );
			if( PROP_DEF != m_nInternalVersion )
				sResult.AppendFormat( _T("{\\vern%d}"),m_nInternalVersion );
			if( _T("") != m_sLinkBase )
				sResult.AppendFormat( _T("{\\hlinkbase %s}"), RtfChar::renderRtfText( m_sLinkBase, oRenderParameter.poDocument ) );


			CString sCreateTime = m_oCreateTime.RenderToRtf( oRenderParameter );
			if( _T("") != sCreateTime )
				sResult.AppendFormat( _T("{\\creatim%s}"),sCreateTime );
			CString sRevTime = m_oRevTime.RenderToRtf( oRenderParameter );
			if( _T("") != sRevTime )
				sResult.AppendFormat( _T("{\\revtim%s}"),sRevTime );
			CString sPrintTime = m_oPrintTime.RenderToRtf( oRenderParameter );
			if( _T("") != sPrintTime )
				sResult.AppendFormat( _T("{\\printim%s}"),sPrintTime );
			CString sBackupTime = m_oBackupTime.RenderToRtf( oRenderParameter );
			if( _T("") != sBackupTime )
				sResult.AppendFormat( _T("{\\buptim%s}"),sBackupTime );
			if( PROP_DEF != m_nEndingTime )
				sResult.AppendFormat( _T("{\\edmins%d}"),m_nEndingTime );
			if( PROP_DEF != m_nNumberOfPages )
				sResult.AppendFormat( _T("{\\nofpages%d}"),m_nNumberOfPages );
			if( PROP_DEF != m_nNumberOfWords )
				sResult.AppendFormat( _T("{\\nofwords%d}"),m_nNumberOfWords );
			if( PROP_DEF != m_nNumberOfCharactersWithSpace )
				sResult.AppendFormat( _T("{\\nofchars%d}"),m_nNumberOfCharactersWithSpace );
			if( PROP_DEF != m_nNumberOfCharactersWithoutSpace )
				sResult.AppendFormat( _T("{\\nofcharsws%d}"),m_nNumberOfCharactersWithoutSpace );

			if( PROP_DEF != m_nInternalId )
				sResult.AppendFormat( _T("{\\id%d}"),m_nInternalId );

			if( false == sResult.IsEmpty() )
				sResult = _T("{\\info") + sResult + _T("}");
			return sResult;
		}
CString RtfInformation::RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			if( RENDER_TO_OOX_PARAM_CORE == oRenderParameter.nType )
			{
				if( _T("") != m_sTitle )
					sResult.AppendFormat( _T("<dc:title>%s</dc:title>"), Utils::PrepareToXML( m_sTitle ) );
				if( _T("") != m_sSubject )
					sResult.AppendFormat( _T("<dc:subject>%s</dc:subject>"), Utils::PrepareToXML( m_sSubject ) );
				if( _T("") != m_sAuthor )
					sResult.AppendFormat( _T("<dc:creator>%s</dc:creator>"), Utils::PrepareToXML( m_sAuthor ) );
				if( _T("") != m_sKeywords )
					sResult.AppendFormat( _T("<cp:keywords>%s</cp:keywords>"), Utils::PrepareToXML( m_sKeywords ) );
				if( _T("") != m_sComment )
					sResult.AppendFormat( _T("<dc:description>%s</dc:description>"), Utils::PrepareToXML( m_sComment ) );

				//CString sCreateTime = m_oCreateTime.RenderToOOX( oRenderParameter );
				//if( false == sCreateTime.IsEmpty() )
				//	sResult.AppendFormat( _T("<dcterms:created xsi:type=\"dcterms:W3CDTF\">%s</dcterms:created>"), sCreateTime );
				//CString sRevTime = m_oRevTime.RenderToOOX( oRenderParameter );
				//if( false == sRevTime.IsEmpty() )
				//	sResult.AppendFormat( _T("<dcterms:modified xsi:type=\"dcterms:W3CDTF\">%s</dcterms:modified>"), sCreateTime );
				//CString sPrintTime = m_oPrintTime.RenderToOOX( oRenderParameter );
				//if( _T("") != sPrintTime )
				//	sResult.AppendFormat( _T("<dcterms:created xsi:type=\"dcterms:W3CDTF\">%s</dcterms:created>"), sCreateTime );
				//CString sBackupTime = m_oBackupTime.RenderToOOX( oRenderParameter );
				//if( _T("") != sBackupTime )
				//	sResult.AppendFormat( _T("<dcterms:created xsi:type=\"dcterms:W3CDTF\">%s</dcterms:created>"), sCreateTime );
			}
			else if( RENDER_TO_OOX_PARAM_APP == oRenderParameter.nType )
			{
				if( PROP_DEF != m_nEndingTime )
					sResult.AppendFormat(_T("<TotalTime>%d</TotalTime>"), m_nEndingTime );
				if( PROP_DEF != m_nNumberOfPages )
					sResult.AppendFormat( _T("<Pages>%d</Pages>"),m_nNumberOfPages );
				if( PROP_DEF != m_nNumberOfWords )
					sResult.AppendFormat( _T("<Words>%d</Words>"),m_nNumberOfWords );
				if( PROP_DEF != m_nNumberOfCharactersWithSpace )
					sResult.AppendFormat( _T("<Characters>%d</Characters>"),m_nNumberOfCharactersWithSpace );
				if( PROP_DEF != m_nNumberOfCharactersWithoutSpace )
					sResult.AppendFormat( _T("<CharactersWithSpaces>%d</CharactersWithSpaces>"),m_nNumberOfCharactersWithoutSpace );		
				//if( PROP_DEF != m_nVersion )
				//	sResult.AppendFormat( _T("<AppVersion>%f</AppVersion>"),m_nVersion );
			}
			return sResult;
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
CString RtfMathProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	sResult.Append(_T("{\\*\\mmathPr"));

	RENDER_RTF_INT( mbrkBin, sResult, _T("mbrkBin") )
	RENDER_RTF_INT( mbrkBinSub, sResult, _T("mbrkBinSub") )
	RENDER_RTF_INT( mdefJc, sResult, _T("mdefJc") )
	RENDER_RTF_INT( mdispDef, sResult, _T("mdispDef") )
	RENDER_RTF_INT( minterSp, sResult, _T("minterSp") )
	RENDER_RTF_INT( mintraSp, sResult, _T("mintraSp") )
	RENDER_RTF_INT( mlMargin, sResult, _T("mlMargin") )
	RENDER_RTF_INT( mmathFont, sResult, _T("mmathFont") )
	RENDER_RTF_INT( mnaryLim, sResult, _T("mnaryLim") )
	RENDER_RTF_INT( mpostSp, sResult, _T("mpostSp") )
	RENDER_RTF_INT( mpreSp, sResult, _T("mpreSp") )
	RENDER_RTF_INT( mrMargin, sResult, _T("mrMargin") )
	RENDER_RTF_INT( msmallFrac, sResult, _T("msmallFrac") )
	RENDER_RTF_INT( mwrapIndent, sResult, _T("mwrapIndent") )
	RENDER_RTF_INT( mwrapRight, sResult, _T("mwrapRight") )

	sResult.Append(_T("}"));
	return sResult;
}
CString RtfMathProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sProperty;
	if( PROP_DEF != mmathFont )
	{
		RtfDocument* poDoc = static_cast<RtfDocument*>(oRenderParameter.poDocument);
		RtfFont oFont;
		if( true == poDoc->m_oFontTable.GetFont(mmathFont, oFont) )
			sProperty.AppendFormat(_T("<m:mathFont m:val=\"%s\"/>"), oFont.m_sName);

	}
	switch( mbrkBin )
	{
		case 0:sProperty.Append(_T("<m:brkBin m:val=\"before\"/>"));break;
		case 1:sProperty.Append(_T("<m:brkBin m:val=\"after\"/>"));break;
		case 2:sProperty.Append(_T("<m:brkBin m:val=\"repeat\"/>"));break;
	}
	switch( mbrkBinSub )
	{
		case 0:sProperty.Append(_T("<m:brkBinSub m:val=\"--\"/>"));break;
		case 1:sProperty.Append(_T("<m:brkBinSub m:val=\"+-\"/>"));break;
		case 2:sProperty.Append(_T("<m:brkBinSub m:val=\"-+\"/>"));break;
	}
	switch( mdefJc )
	{
		case 1:sProperty.Append(_T("<m:defJc m:val=\"centerGroup\"/>"));break;
		case 2:sProperty.Append(_T("<m:defJc m:val=\"center\"/>"));break;
		case 3:sProperty.Append(_T("<m:defJc m:val=\"left\"/>"));break;
		case 4:sProperty.Append(_T("<m:defJc m:val=\"right\"/>"));break;

	}
	RENDER_OOX_INT( mdispDef, sProperty, _T("m:dispDef") )
	RENDER_OOX_INT( minterSp, sProperty, _T("m:interSp") )
	RENDER_OOX_INT( mintraSp, sProperty, _T("m:intraSp") )
	RENDER_OOX_INT( mlMargin, sProperty, _T("m:lMargin") )
	RENDER_OOX_INT( mrMargin, sProperty, _T("m:rMargin") )
	switch( mnaryLim )
	{
		case 0:sProperty.Append(_T("<m:naryLim m:val=\"subSup\"/>"));break;
		case 1:sProperty.Append(_T("<m:naryLim m:val=\"undOvr\"/>"));break;
	}
	RENDER_OOX_INT( mpostSp, sProperty, _T("m:postSp") )
	RENDER_OOX_INT( mpreSp, sProperty, _T("m:preSp") )
	RENDER_OOX_INT( msmallFrac, sProperty, _T("m:smallFrac") )
	RENDER_OOX_INT( mwrapIndent, sProperty, _T("m:wrapIndent") )
	RENDER_OOX_INT( mwrapRight, sProperty, _T("m:wrapRight") )
	CString sResult;
	if( false == sProperty.IsEmpty() )
		sResult.AppendFormat( _T("<m:mathPr>%s</m:mathPr>"), sProperty );
	return sResult;
}
CString RtfMathSpecProp::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	RENDER_RTF_INT( moMathParaPr, sResult, _T("mjc") )
	RENDER_RTF_INT( Font, sResult, _T("f") )
	RENDER_RTF_BOOL( Break, sResult, _T("mbrk") )
	RENDER_RTF_INT( RowSpacing, sResult, _T("mrSp") )
	RENDER_RTF_INT( RowSpacingRule, sResult, _T("mrSpRule") )
	RENDER_RTF_INT( CellGap, sResult, _T("mcGp") )
	RENDER_RTF_INT( CellGapRule, sResult, _T("mcGpRule") )
	RENDER_RTF_INT( CellSpacing, sResult, _T("mcSp") )

	RENDER_RTF_BOOL( malnScr, sResult, _T("malnScr") )
	RENDER_RTF_BOOL( HideLeft, sResult, _T("mhideLeft") )
	RENDER_RTF_BOOL( HideTop, sResult, _T("mhideTop") )
	RENDER_RTF_BOOL( HideRight, sResult, _T("mhideRight") )
	RENDER_RTF_BOOL( HideBottom, sResult, _T("mhideBot") )

	RENDER_RTF_BOOL( StrikeHor, sResult, _T("mstrikeH") )
	RENDER_RTF_BOOL( StrikeVer, sResult, _T("mstrikeV") )
	RENDER_RTF_BOOL( StrikeLR, sResult, _T("mstrikeTLBR") )
	RENDER_RTF_BOOL( StrikeRL, sResult, _T("mstrikeBLTR") )
	RENDER_RTF_BOOL( Alignment, sResult, _T("maln") )
	RENDER_RTF_BOOL( Differential, sResult, _T("mdiff") )

	RENDER_RTF_BOOL( NoBreak, sResult, _T("mnoBreak") )
	RENDER_RTF_BOOL( Emulator, sResult, _T("mopEmu") )
	RENDER_RTF_BOOL( NormalText, sResult, _T("mnor") )
	RENDER_RTF_INT( mscr, sResult, _T("mscr") )
	RENDER_RTF_INT( msty, sResult, _T("msty") )

	RENDER_RTF_BOOL( mlit, sResult, _T("mlit") )
	RENDER_RTF_BOOL( mshow, sResult, _T("mshow") )
	RENDER_RTF_BOOL( mtransp, sResult, _T("mtransp") )
	RENDER_RTF_BOOL( mzeroAsc, sResult, _T("mzeroAsc") )
	RENDER_RTF_BOOL( mzeroDesc, sResult, _T("mzeroDesc") )
	RENDER_RTF_BOOL( mzeroWid, sResult, _T("mzeroWid") )

	if( PROP_DEF != mchr )
		sResult.AppendFormat( _T("\\u%d?"), mchr );

	sResult.Append( m_oCharProp.RenderToRtf( oRenderParameter ) );
	return sResult;
}
CString RtfMathSpecProp::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	switch( moMathParaPr )
	{
		case 1:sResult.Append(_T("<m:jc m:val=\"center\"/>"));break;
		case 2:sResult.Append(_T("<m:jc m:val=\"centerGroup\"/>"));break;
		case 3:sResult.Append(_T("<m:jc m:val=\"left\"/>"));break;
		case 4:sResult.Append(_T("<m:jc m:val=\"right\"/>"));break;
	}
	RENDER_OOX_BOOL( Break, sResult, _T("m:brk") )
	RENDER_OOX_INT( RowSpacing, sResult, _T("m:rSp") )
	RENDER_OOX_INT( RowSpacingRule, sResult, _T("m:rSpRule") )
	RENDER_OOX_INT( CellGap, sResult, _T("m:cGp") )
	RENDER_OOX_INT( CellGapRule, sResult, _T("m:cGpRule") )
	RENDER_OOX_INT( CellSpacing, sResult, _T("m:cSp") )
	RENDER_OOX_BOOL( malnScr, sResult, _T("m:alnScr") )
	RENDER_OOX_BOOL( HideLeft, sResult, _T("m:hideLeft") )
	RENDER_OOX_BOOL( HideTop, sResult, _T("m:hideTop") )
	RENDER_OOX_BOOL( HideRight, sResult, _T("m:hideRight") )
	RENDER_OOX_BOOL( HideBottom, sResult, _T("m:hideBottom") )
	RENDER_OOX_BOOL( StrikeHor, sResult, _T("m:strikeH") )
	RENDER_OOX_BOOL( StrikeVer, sResult, _T("m:strikeV") )
	RENDER_OOX_BOOL( StrikeLR, sResult, _T("m:strikeTLBR") )
	RENDER_OOX_BOOL( StrikeRL, sResult, _T("m:strikeBLTR") )
	RENDER_OOX_BOOL( Alignment, sResult, _T("m:aln") )
	RENDER_OOX_BOOL( Differential, sResult, _T("m:diff") )
	RENDER_OOX_BOOL( NoBreak, sResult, _T("m:noBreak") )
	RENDER_OOX_BOOL( Emulator, sResult, _T("m:opEmu") )
	RENDER_OOX_BOOL( NormalText, sResult, _T("m:nor") )

	switch( mscr )
	{
		case 0:	sResult.Append(_T("<m:scr m:val=\"roman\"/>"));break;
		case 1:	sResult.Append(_T("<m:scr m:val=\"script\"/>"));break;
		case 2:	sResult.Append(_T("<m:scr m:val=\"fraktur\"/>"));break;
		case 3:	sResult.Append(_T("<m:scr m:val=\"double-struck\"/>"));break;
		case 4:	sResult.Append(_T("<m:scr m:val=\"sans-serif\"/>"));break;
		case 5:	sResult.Append(_T("<m:scr m:val=\"monospace\"/>"));break;
	}
	switch( msty )
	{
		case 0:	sResult.Append(_T("<m:sty m:val=\"p\"/>"));break;
		case 1:	sResult.Append(_T("<m:sty m:val=\"b\"/>"));break;
		case 2:	sResult.Append(_T("<m:sty m:val=\"i\"/>"));break;
		case 3:	sResult.Append(_T("<m:sty m:val=\"bi\"/>"));break;
	}
	RENDER_OOX_BOOL( mlit, sResult, _T("m:lit") )
	RENDER_OOX_BOOL( mshow, sResult, _T("m:lit") )
	RENDER_OOX_BOOL( mtransp, sResult, _T("m:lit") )
	RENDER_OOX_BOOL( mzeroAsc, sResult, _T("m:lit") )
	RENDER_OOX_BOOL( mzeroDesc, sResult, _T("m:lit") )
	RENDER_OOX_BOOL( mzeroWid, sResult, _T("m:lit") )

	return sResult;
}

void RtfCharStyle::Merge( RtfStylePtr oStyle )
		{
			RtfStyle::Merge( oStyle );
			if( TYPE_RTF_PROPERTY_STYLE_CHAR == oStyle->GetType() )
			{
				RtfCharStylePtr oCharStyle = boost::shared_static_cast<RtfCharStyle, RtfStyle>( oStyle );
				m_oCharProp.Merge( oCharStyle->m_oCharProp );
			}
			else if( TYPE_RTF_PROPERTY_STYLE_PARAGRAPH == oStyle->GetType() )
			{
				RtfParagraphStylePtr oParagraphStyle = boost::shared_static_cast<RtfParagraphStyle, RtfStyle>( oStyle );
				m_oCharProp.Merge( oParagraphStyle->m_oCharProp );
			}
			else if( TYPE_RTF_PROPERTY_STYLE_TABLE == oStyle->GetType() )
			{
				RtfTableStylePtr oTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oStyle );
				m_oCharProp.Merge( oTableStyle->m_oCharProp );
			}
		}

void RtfParagraphStyle::Merge( RtfStylePtr oStyle )
	{
		RtfStyle::Merge( oStyle );
		if( TYPE_RTF_PROPERTY_STYLE_CHAR == oStyle->GetType() )
		{
			RtfCharStylePtr oCharStyle = boost::shared_static_cast<RtfCharStyle, RtfStyle>( oStyle );
			m_oCharProp.Merge( oCharStyle->m_oCharProp );
		}
		else if( TYPE_RTF_PROPERTY_STYLE_PARAGRAPH == oStyle->GetType() )
		{
			RtfParagraphStylePtr oParagraphStyle = boost::shared_static_cast<RtfParagraphStyle, RtfStyle>( oStyle );
			m_oParProp.Merge( oParagraphStyle->m_oParProp );
			m_oCharProp.Merge( oParagraphStyle->m_oCharProp );
		}
		else if( TYPE_RTF_PROPERTY_STYLE_TABLE == oStyle->GetType() )
		{
			RtfTableStylePtr oTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oStyle );
			m_oParProp.Merge( oTableStyle->m_oParProp );
			m_oCharProp.Merge( oTableStyle->m_oCharProp );
		}
	}
void RtfTableStyle::Merge( RtfStylePtr oStyle )
	{
		RtfStyle::Merge( oStyle );
		if( TYPE_RTF_PROPERTY_STYLE_CHAR == oStyle->GetType() )
		{
			RtfCharStylePtr oCharStyle = boost::shared_static_cast<RtfCharStyle, RtfStyle>( oStyle );
			m_oCharProp.Merge( oCharStyle->m_oCharProp );
		}
		else if( TYPE_RTF_PROPERTY_STYLE_PARAGRAPH == oStyle->GetType() )
		{
			RtfParagraphStylePtr oParagraphStyle = boost::shared_static_cast<RtfParagraphStyle, RtfStyle>( oStyle );
			m_oParProp.Merge( oParagraphStyle->m_oParProp );
			m_oCharProp.Merge( oParagraphStyle->m_oCharProp );
		}
		if( TYPE_RTF_PROPERTY_STYLE_TABLE == oStyle->GetType() )
		{
			RtfTableStylePtr oTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oStyle );
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
