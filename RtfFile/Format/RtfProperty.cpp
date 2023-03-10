/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "../OOXml/Writer/OOXWriter.h"
#include "RtfProperty.h"
#include "RtfDocument.h"

#include "../../OOXML/DocxFormat/FileTypes.h"
#include "../../OOXML/DocxFormat/App.h"
#include "../../OOXML/DocxFormat/Core.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../Common/MS-LCID.h"

#include <boost/algorithm/string.hpp>

RtfFont::RtfFont()
{
	SetDefault();
}
int RtfFont::GetType()
{
	return TYPE_RTF_FONT;
}
bool RtfFont::operator==( const RtfFont& oFont)
{
	return m_eFontTheme == oFont.m_eFontTheme	&& m_eFontFamily	== oFont.m_eFontFamily	&&
			m_sPanose	== oFont.m_sPanose		&& m_nID			== oFont.m_nID			&&
			m_sName		== oFont.m_sName		&& m_sAltName		== oFont.m_sAltName		&&
			m_nCharset	== oFont.m_nCharset		&& m_nCodePage		== oFont.m_nCodePage	&&
			m_nPitch	== oFont.m_nPitch;
}
bool RtfFont::IsValid()
{
	return PROP_DEF != m_nID;
}
void RtfFont::SetDefaultRtf()
{
	SetDefault();
	m_nPitch = 0;
}
void RtfFont::SetDefaultOOX()
{
	SetDefault();
}
void RtfFont::SetDefault()
{
	DEFAULT_PROPERTY_DEF( m_eFontTheme, ft_none )
	DEFAULT_PROPERTY_DEF( m_eFontFamily, ff_fnil )
	DEFAULT_PROPERTY_DEF( m_sPanose, L"" )
	DEFAULT_PROPERTY	( m_nID )
	DEFAULT_PROPERTY_DEF( m_sName, L"" )
	DEFAULT_PROPERTY_DEF( m_sAltName, L"" )
	DEFAULT_PROPERTY	( m_nCharset )
	DEFAULT_PROPERTY	( m_nCodePage )
	DEFAULT_PROPERTY_DEF( m_nPitch, 2 )
}
std::wstring RtfFont::RenderToRtf(RenderParameter oRenderParameter)
{
	if ( IsValid() == false) return L"";

    std::wstring sResult;
	if( RENDER_TO_RTF_PARAM_FONT_TBL == oRenderParameter.nType )
	{
		sResult += L"{";
		switch( m_eFontTheme )
		{
			case ft_flomajor:	sResult += L"\\flomajor";	break;
			case ft_fhimajor:	sResult += L"\\fhimajor";	break;
			case ft_fdbmajor:	sResult += L"\\fdbmajor";	break;
			case ft_fbimajor:	sResult += L"\\fbimajor";	break;
			case ft_flominor:	sResult += L"\\flominor";	break;
			case ft_fhiminor:	sResult += L"\\fhiminor";	break;
			case ft_fdbminor:	sResult += L"\\fdbminor";	break;
			case ft_fbiminor:	sResult += L"\\fbiminor";	break;
		default:
			break;
		}
        sResult += L"\\f" + std::to_wstring(m_nID);

		switch( m_eFontFamily )
		{
			case ff_fnil:		sResult += L"\\fnil";		break;
			case ff_froman:		sResult += L"\\froman";		break;
			case ff_fswiss:		sResult += L"\\fswiss";		break;
			case ff_fmodern:	sResult += L"\\fmodern";	break;
			case ff_fscript:	sResult += L"\\fscript";	break;
			case ff_fdecor:		sResult += L"\\fdecor";		break;
			case ff_ftech:		sResult += L"\\ftech";		break;
			case ff_fbidi:		sResult += L"\\fbidi";		break;
		default:
			break;
		}
		RENDER_RTF_INT( m_nCharset, sResult, L"fcharset" )
		RENDER_RTF_INT( m_nPitch, sResult, L"fprq" )

		if( L"" != m_sPanose )
            sResult += L"{\\*\\panose " + m_sPanose + L"}";
		
		RENDER_RTF_INT( m_nCodePage, sResult, L"cpg" )

		RtfCharProperty oFontNameCharProp;
        sResult += L" " + RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, &oFontNameCharProp, true );
		
//ALL FONTS NEW.docx
//		if(!m_sAltName.empty() )
//          sResult += L"{\\*\\falt " + RtfChar::renderRtfText( m_sAltName, oRenderParameter.poDocument, &oFontNameCharProp ) + L"}";

		sResult += L";}";
	}
	else
        sResult += L"\\f" + std::to_wstring(m_nID);
	return sResult;
}
std::wstring RtfFont::RenderToOOX(RenderParameter oRenderParameter)
{
	if ( IsValid() == false) return L"";

    std::wstring sResult;
	
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
    std::wstring sFontName = m_sName;

    if( sFontName.empty() )
	{
		if( PROP_DEF != poRtfDocument->m_oProperty.m_nDefFont )
		{
			RtfFont oDefFont;
			poRtfDocument->m_oFontTable.GetFont( poRtfDocument->m_oProperty.m_nDefFont, oDefFont );
			sFontName = oDefFont.m_sName;
		}
        if( sFontName.empty())
			sFontName = L"Arial";
	}
	if( RENDER_TO_OOX_PARAM_MINOR_FONT == oRenderParameter.nType )
	{
        std::wstring sTag;
		switch(m_eFontTheme)
		{
			case ft_flominor: 
			case ft_fhiminor: sTag = L"latin";	break;
			case ft_fdbminor: sTag = L"ea";		break;
			case ft_fbiminor: sTag = L"cs";		break;
			default:
				break;
		}
        if( !sTag.empty() )
		{
            sResult += L"<" ;
			sResult += sTag;
			sResult += L" typeface=\"";
            sResult += XmlUtils::EncodeXmlString( sFontName );
			sResult += L"\"/>";
		}
	}
	else if( RENDER_TO_OOX_PARAM_MAJOR_FONT == oRenderParameter.nType )
	{
        std::wstring sTag;
		switch(m_eFontTheme)
		{
			case ft_flomajor: 
			case ft_fhiminor: sTag = L"latin";	break;
			case ft_fdbmajor: sTag = L"ea";		break;
			case ft_fbimajor: sTag = L"cs";		break;
			default:
				break;
		}
        if( !sTag.empty() )
		{
			sResult += L"<";
			sResult += sTag;
            sResult += L" typeface=\"";
            sResult += XmlUtils::EncodeXmlString( sFontName );
			sResult += L"\"/>";
		}
	}
	else if( RENDER_TO_OOX_PARAM_FONTTABLE == oRenderParameter.nType )
	{
        sResult += L"<w:font w:name=\"";
        sResult += XmlUtils::EncodeXmlString( sFontName, true );
		sResult += L"\">";
		
        if( !m_sAltName.empty() )
		{
            sResult += L"<w:altName w:val=\"";
            sResult += XmlUtils::EncodeXmlString( m_sAltName, true );
			sResult += L"\"/>";
		}
        if( !m_sPanose.empty() )
		{
            sResult += L"<w:panose1 w:val=\"";
			sResult += m_sPanose;
			sResult += L"\"/>";
		}

        std::wstring sFamily = L"";
		switch( m_eFontFamily )
		{
			case ff_fnil:		sFamily = L"auto";			break;
			case ff_froman:		sFamily = L"roman";			break;
			case ff_fswiss:		sFamily = L"swiss";			break;
			case ff_fmodern:	sFamily = L"modern";		break;
			case ff_fscript:	sFamily = L"script";		break;
			case ff_fdecor:		sFamily = L"decorative";	break;
			case ff_ftech:		sFamily = L"auto";			break;
			case ff_fbidi:		sFamily = L"auto";			break;
			default:
				break;
		}
        if( !sFamily.empty() )
		{
            sResult += L"<w:family w:val=\"";
			sResult += sFamily;
			sResult += L"\"/>";
		}
		if( PROP_DEF != m_nCharset )
		{
            sResult += L"<w:charset w:val=\"";
			sResult += Convert::ToStringHex( m_nCharset, 2 );
			sResult += L"\"/>";
		}

		if( PROP_DEF != m_nPitch )
		{
            std::wstring sPitch;
			switch( m_nPitch )
			{
				case 0: sPitch = L"default";	break;
				case 1: sPitch = L"fixed";		break;
				case 2: sPitch = L"variable";	break;
				default:
					break;
			}
            sResult += L"<w:pitch w:val=\"";
			sResult += sPitch;
			sResult += L"\"/>";
		}
		sResult += L"</w:font>";
	}
	else
	{
		RtfFont oCurFont;
		if( true == poRtfDocument->m_oFontTable.GetFont(m_nID, oCurFont) )
		{
            std::wstring sHint;
			switch(oCurFont.m_nCharset)
			{
				//case 128://Japanese //vedomost.rtf ????
				case 129://Korean
				case 130://Korean
				case 134://China
				case 136://China
				case 163://Vietnamese
				case 222://Thai
				{
					sHint = L" w:hint=\"eastAsia\"";
				}break;
				case 177://Hebrew
				case 178://Arabic
				case 179://Arabic
				case 180://Arabic
				case 181://Hebrew
				{
					sHint = L" w:hint=\"cs\"";
				}break;
				//?? нужно ли описывать default??? todooo
				default:
					break;
			}
			sResult += L"<w:rFonts w:ascii=\"";
			sResult += XmlUtils::EncodeXmlString( sFontName, true );
			sResult += L"\" w:eastAsia=\"";
			sResult += XmlUtils::EncodeXmlString( sFontName, true );
			sResult += L"\" w:hAnsi=\"";
			sResult += XmlUtils::EncodeXmlString( sFontName, true );
			sResult += L"\" w:cs=\"";
			sResult += XmlUtils::EncodeXmlString( sFontName, true );
			sResult += L"\"";
			sResult += sHint;
			sResult += L"/>";
		}
	}

	return sResult;
}

RtfColor::RtfColor()
{
	SetDefault();
}
RtfColor::RtfColor(_UINT32 nHex)
{
	SetHEX( nHex );
}
RtfColor::RtfColor(BYTE r, BYTE g, BYTE b)
{
	SetRGB( r, g, b );
}
void RtfColor::SetDefaultRtf()
{
	SetDefault();
}
void RtfColor::SetDefaultOOX()
{
	SetDefault();
}
void RtfColor::SetAuto()
{
	m_bAuto		= true;
}
void RtfColor::SetDefault()
{
	m_bAuto		= false;

	m_byteRed	= 0;
	m_byteGreen = 0;
	m_byteBlue	= 0;

	m_byteShade	= 0;
	m_byteTint	= 255;
	m_eTheme	= RtfColor::TC_NONE;
	m_index		= -1;
}
BYTE RtfColor::GetR()
{
	return m_byteRed;
}
BYTE RtfColor::GetG()
{
	return m_byteGreen;
}
BYTE RtfColor::GetB()
{
	return m_byteBlue;
}
bool RtfColor::operator==(const RtfColor& oColor)
{
	return  m_byteRed == oColor.m_byteRed && m_byteGreen == oColor.m_byteGreen && m_byteBlue == oColor.m_byteBlue &&
			m_eTheme == oColor.m_eTheme && m_byteTint	== oColor.m_byteTint && m_byteShade == oColor.m_byteShade;
}
const RtfColor& RtfColor::operator=( const RtfColor& oColor )
{
	m_byteRed	= oColor.m_byteRed;
	m_byteGreen	= oColor.m_byteGreen;
	m_byteBlue	= oColor.m_byteBlue;

	m_eTheme	= oColor.m_eTheme;
	m_byteShade	= oColor.m_byteShade;
	m_byteTint	= oColor.m_byteTint;

	m_bAuto		= oColor.m_bAuto;
	return (*this);
}
void RtfColor::SetHEX(_UINT32 color)
{
	SetDefault();

	m_byteRed	= (color&0xFF0000) >> 16;
	m_byteGreen = (color&0xFF00) >> 8;
	m_byteBlue	= (color&0xFF);
}
_UINT32 RtfColor::GetRGB()const
{
	return (m_byteRed << 16) | (m_byteGreen << 8) | m_byteBlue;
}
void RtfColor::SetRGB(_UINT32 color)
{
	SetDefault();

	m_byteRed	= (color & 0xFF);
	m_byteGreen = (color & 0xFF00)		>> 8;
	m_byteBlue	= (color & 0xFF0000)	>> 16;
}
void RtfColor::SetRGB(BYTE red, BYTE green, BYTE blue)
{
	SetDefault();

	m_byteRed	= red;
	m_byteGreen = green;
	m_byteBlue	= blue;
}
void RtfColor::GetHSL(double &dH, double &dS,double &dL)
{
	RGB2HSL(m_byteRed, m_byteGreen, m_byteBlue, dH, dS, dL);
}
void RtfColor::SetHSL(double dH, double dS,double dL)
{
	HSL2RGB(dH, dS, dL, m_byteRed, m_byteGreen, m_byteBlue);
}
void RtfColor::SetRGBPercent(_INT32 nRedPer, _INT32 nGreenPer, _INT32 nBluePer)
{
	m_byteRed	= (BYTE)(nRedPer	* 255 / 100);
	m_byteGreen = (BYTE)(nGreenPer	* 255 / 100);
	m_byteBlue	= (BYTE)(nBluePer	* 255 / 100);
}
void RtfColor::SetHEXString(std::wstring hex)
{
	if ( L"auto" != hex )
	{
		_UINT32 color	= Strings::ToColor(hex);

		SetRGB(color);
	}
	else
		SetDefault();
}
std::wstring RtfColor::ToHexColor(bool bBGR)
{
	if (m_bAuto) return L"auto";

	std::wstring sRed	= XmlUtils::ToString(m_byteRed,		L"%02X");
	std::wstring sGreen = XmlUtils::ToString(m_byteGreen,	L"%02X");;
	std::wstring sBlue	= XmlUtils::ToString(m_byteBlue,	L"%02X");

	if (bBGR)	return sBlue + sGreen + sRed ;
	else		return sRed + sGreen + sBlue ;
}
std::wstring RtfColor::RenderToRtf(RenderParameter  oRenderParameter )
{
	RtfWriter* poRtfWriter = static_cast<RtfWriter*>( oRenderParameter.poWriter );
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
    std::wstring sResult;
	if( RENDER_TO_RTF_PARAM_COLOR_TBL == oRenderParameter.nType )
	{
		sResult	+= L"\\red" + std::to_wstring(m_byteRed) + L"\\green" + std::to_wstring(m_byteGreen) + L"\\blue" + std::to_wstring(m_byteBlue);
		sResult += L";";
	}
	else
	{
		_INT32 nColor;
		if( true == poRtfDocument->m_oColorTable.GetColor(*this, nColor) )
            sResult += L"\\c" + std::to_wstring(nColor); //todo
	}
	return sResult;
}
std::wstring RtfColor::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
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
        sResult += L"<w:color " + WriteOOXAttribute(oRenderParameter.sValue) + L"/>";
	}
	else if (m_bAuto)
	{
        sResult += L"<w:color w:val=\"auto\"/>";
	}
	else
	{
        sResult += L"<w:color w:val=\"" + ToHexColor() + L"\"/>";
	}
	return sResult;
}
BYTE RtfColor::SetShade(BYTE bShade)
{
	m_byteShade = bShade;
	double dH, dS, dL;
	GetHSL(dH, dS, dL);
	dL = dL * bShade / 255.;
	SetHSL(dH, dS, dL);
	return bShade;
}
BYTE RtfColor::SetTint(BYTE bTint)
{
	m_byteTint = bTint;

	double dH,  dS,  dL;
	GetHSL(dH, dS,dL);
	dL = dL * bTint / 255. + (1 - bTint / 255.);
	SetHSL(dH, dS, dL);
	return bTint;
 }
bool RtfColor::GetHighlightByColor( RtfColor oOutputColor,std::wstring& oStr ) //todo
{
	if		( oOutputColor ==  RtfColor(0x000000) ) {oStr = L"black";		return true;}
	else if ( oOutputColor ==  RtfColor(0x0000FF) ) {oStr = L"blue";		return true;}
	else if ( oOutputColor ==  RtfColor(0x00FFFF) ) {oStr = L"cyan";		return true;}
	else if ( oOutputColor ==  RtfColor(0x00008B) ) {oStr = L"darkBlue";	return true;}
	else if ( oOutputColor ==  RtfColor(0x008B8B) ) {oStr = L"darkCyan";	return true;}
	else if ( oOutputColor ==  RtfColor(0xA9A9A9) ) {oStr = L"darkGray";	return true;}
	else if ( oOutputColor ==  RtfColor(0x006400) ) {oStr = L"darkGreen";	return true;}
	else if ( oOutputColor ==  RtfColor(0x800080) ) {oStr = L"darkMagenta";	return true;}
	else if ( oOutputColor ==  RtfColor(0x8B0000) ) {oStr = L"darkRed";		return true;}
	else if ( oOutputColor ==  RtfColor(0x808000) ) {oStr = L"darkYellow";	return true;}
	else if ( oOutputColor ==  RtfColor(0x00FF00) ) {oStr = L"green";		return true;}
	else if ( oOutputColor ==  RtfColor(0xD3D3D3) ) {oStr = L"lightGray";	return true;}
	else if ( oOutputColor ==  RtfColor(0xFF00FF) ) {oStr = L"magenta";		return true;}
	else if ( oOutputColor ==  RtfColor(0xFF0000) ) {oStr = L"red";			return true;}
	else if ( oOutputColor ==  RtfColor(0xFFFFFF) ) {oStr = L"white";		return true;}
	else if ( oOutputColor ==  RtfColor(0xFFFF00) ) {oStr = L"yellow";		return true;}
	return false;
}
RtfColor RtfColor::GetColorByPreset( std::wstring oStr )
{
	if		( oStr == L"aliceBlue" )	return RtfColor(240,248,255);
	else if ( oStr == L"aniqueWhite" )	return RtfColor(250,235,215);
	else if ( oStr == L"aqua" )			return RtfColor(0,255,255);
	else if ( oStr == L"aquamarine" )	return RtfColor(127,255,212);
	else if ( oStr == L"azure" )		return RtfColor(240,255,255);
	else if ( oStr == L"beige" )		return RtfColor(245,245,220);
	else if ( oStr == L"bisque" )		return RtfColor(255,228,196);
	else if ( oStr == L"black" )		return RtfColor(0,0,0);
	else if ( oStr == L"blanchedAlmond" ) return RtfColor(255,235,205);
	else if ( oStr == L"blueViolet" )	return RtfColor(138,43,226);
	else if ( oStr == L"brown" )		return RtfColor(165,42,42);
	else if ( oStr == L"burlyWood" )	return RtfColor(222,184,135);

	else if ( oStr == L"cyan" )			return RtfColor(0,255,255);
	else if ( oStr == L"gold" )			return RtfColor(255,215,0);
	else if ( oStr == L"green" )		return RtfColor(0,128,0);
	else if ( oStr == L"grey" )			return RtfColor(128,128,128);
	else if ( oStr == L"red" )			return RtfColor(255,0,0);
	else if ( oStr == L"yellow" )		return RtfColor(255,255,0);

	return RtfColor(0,0,0);
}
std::wstring RtfColor::GetPresetByColor( RtfColor oCol ) //стр. 3320
{
	if		( oCol == RtfColor(240,248,255))	return L"aliceBlue";
	else if ( oCol == RtfColor(250,235,215))	return L"aniqueWhite";
	else if ( oCol == RtfColor(0,255,255))		return L"aqua";
	else if ( oCol == RtfColor(127,255,212))	return L"aquamarine";
	else if ( oCol == RtfColor(240,255,255))	return L"azure";
	else if ( oCol == RtfColor(245,245,220))	return L"beige";
	else if ( oCol == RtfColor(255,228,196))	return L"bisque";
	else if ( oCol == RtfColor(0,0,0))			return L"black";
	else if ( oCol == RtfColor(255,235,205))	return L"blanchedAlmond";
	else if ( oCol == RtfColor(138,43,226))		return L"blueViolet";
	else if ( oCol == RtfColor(165,42,42))		return L"brown";
	else if ( oCol == RtfColor(222,184,135))	return L"burlyWood";

	else if ( oCol == RtfColor(0,255,255))		return L"cyan";
	else if ( oCol == RtfColor(255,215,0))		return L"gold";
	else if ( oCol == RtfColor(0,128,0))		return L"green";
	else if ( oCol == RtfColor(128,128,128))	return L"grey";
	else if ( oCol == RtfColor(255,0,0))		return L"red";
	else if ( oCol == RtfColor(255,255,0))		return L"yellow";

	return L"black";
}
bool RtfColor::GetThemeByString( std::wstring sTheme, _ThemeColor & oOutTheme )
{
	if		( sTheme == L"accent1" )	{oOutTheme = caccentone;		return true;}
	else if ( sTheme == L"accent2" )	{oOutTheme = caccenttwo;		return true;}
	else if ( sTheme == L"accent3" )	{oOutTheme = caccentthree;		return true;}
	else if ( sTheme == L"accent4" )	{oOutTheme = caccentfour;		return true;}
	else if ( sTheme == L"accent5" )	{oOutTheme = caccentfive;		return true;}
	else if ( sTheme == L"accent6" )	{oOutTheme = caccentsix;		return true;}
	else if ( sTheme == L"bg1" )		{oOutTheme = cbackgroundone;	return true;}
	else if ( sTheme == L"bg2" )		{oOutTheme = cbackgroundtwo;	return true;}
	else if ( sTheme == L"dk1" )		{oOutTheme = cmaindarkone;		return true;}
	else if ( sTheme == L"dk2" )		{oOutTheme = cmaindarktwo;		return true;}
	else if ( sTheme == L"folHlink" )	{oOutTheme = cfollowedhyperlink;return true;}
	else if ( sTheme == L"hlink" )		{oOutTheme = chyperlink;		return true;}
	else if ( sTheme == L"lt1" )		{oOutTheme = cmainlightone;		return true;}
	else if ( sTheme == L"lt2" )		{oOutTheme = cmainlighttwo;		return true;}
	else if ( sTheme == L"phClr" )		{oOutTheme = cmainlighttwo;		return true;}
	else if ( sTheme == L"tx1" )		{oOutTheme = ctextone;			return true;}
	else if ( sTheme == L"tx2" )		{oOutTheme = ctexttwo;			return true;}
	return false;
}
bool RtfColor::GetThemeByOOX( SimpleTypes::EShemeColorVal val, _ThemeColor & oOutTheme )
{
	switch(val)
	{
	case SimpleTypes::shemecolorvalAccent1:  {oOutTheme = caccentone;			return true;}
	case SimpleTypes::shemecolorvalAccent2:  {oOutTheme = caccenttwo;			return true;}
	case SimpleTypes::shemecolorvalAccent3:  {oOutTheme = caccentthree;			return true;}
	case SimpleTypes::shemecolorvalAccent4:  {oOutTheme = caccentfour;			return true;}
	case SimpleTypes::shemecolorvalAccent5:  {oOutTheme = caccentfive;			return true;}
	case SimpleTypes::shemecolorvalAccent6:  {oOutTheme = caccentsix;			return true;}
	case SimpleTypes::shemecolorvalBg1:      {oOutTheme = cbackgroundone;		return true;}
	case SimpleTypes::shemecolorvalBg2:      {oOutTheme = cbackgroundtwo;		return true;}
	case SimpleTypes::shemecolorvalDk1:      {oOutTheme = cmaindarkone;			return true;}
	case SimpleTypes::shemecolorvalDk2:      {oOutTheme = cmaindarktwo;			return true;}
	case SimpleTypes::shemecolorvalFolHlink: {oOutTheme = cfollowedhyperlink;	return true;}
	case SimpleTypes::shemecolorvalHlink:    {oOutTheme = chyperlink;			return true;}
	case SimpleTypes::shemecolorvalLt1:      {oOutTheme = cmainlightone;		return true;}
	case SimpleTypes::shemecolorvalLt2:      {oOutTheme = cmainlighttwo;		return true;}
	case SimpleTypes::shemecolorvalPhClr:    {oOutTheme = cmainlighttwo;		return true;} //???
	case SimpleTypes::shemecolorvalTx1:      {oOutTheme = ctextone;				return true;}
	case SimpleTypes::shemecolorvalTx2:      {oOutTheme = ctexttwo;				return true;}
	default :								 {oOutTheme = caccentone;			return true;}
	}

	return false;
}
bool RtfColor::GetStringByTheme( std::wstring sTheme , _ThemeColor& oOutTheme )
{
	if		( L"accent1"	== sTheme )	{oOutTheme = caccentone;		return true;}
	else if ( L"accent2"	== sTheme )	{oOutTheme = caccenttwo;		return true;}
	else if ( L"accent3"	== sTheme )	{oOutTheme = caccentthree;		return true;}
	else if ( L"accent4"	== sTheme )	{oOutTheme = caccentfour;		return true;}
	else if ( L"accent5"	== sTheme )	{oOutTheme = caccentfive;		return true;}
	else if ( L"accent6"	== sTheme )	{oOutTheme = caccentsix;		return true;}
	else if ( L"bg1"		== sTheme )	{oOutTheme = cbackgroundone;	return true;}
	else if ( L"bg2"		== sTheme )	{oOutTheme = cbackgroundtwo;	return true;}
	else if ( L"dk1"		== sTheme )	{oOutTheme = cmaindarkone;		return true;}
	else if ( L"dk2"		== sTheme )	{oOutTheme = cmaindarktwo;		return true;}
	else if ( L"folHlink"	== sTheme )	{oOutTheme = cfollowedhyperlink;return true;}
	else if ( L"hlink"		== sTheme )	{oOutTheme = chyperlink;		return true;}
	else if ( L"lt1"		== sTheme )	{oOutTheme = cmainlightone;		return true;}
	else if ( L"lt2"		== sTheme )	{oOutTheme = cmainlighttwo;		return true;}
	else if ( L"phClr"		== sTheme )	{oOutTheme = cmainlighttwo;		return true;}
	else if ( L"tx1"		== sTheme )	{oOutTheme = ctextone;			return true;}
	else if ( L"tx2"		== sTheme )	{oOutTheme = ctexttwo;			return true;}
	return false;
}
std::wstring RtfColor::GetHighLight()
{
	if (m_bAuto) return L"auto";

	std::vector< RtfColor > sColors;

	sColors.push_back( RtfColor( 0x000000 ) );
	sColors.push_back( RtfColor( 0x0000FF ) );
	sColors.push_back( RtfColor( 0x00FFFF ) );
	sColors.push_back( RtfColor( 0x00008B ) );
	sColors.push_back( RtfColor( 0x008B8B ) );
	sColors.push_back( RtfColor( 0xA9A9A9 ) );
	sColors.push_back( RtfColor( 0x006400 ) );
	sColors.push_back( RtfColor( 0x800080 ) );
	sColors.push_back( RtfColor( 0x8B0000 ) );
	sColors.push_back( RtfColor( 0x808000 ) );
	sColors.push_back( RtfColor( 0x00FF00 ) );
	sColors.push_back( RtfColor( 0xD3D3D3 ) );
	sColors.push_back( RtfColor( 0xFF00FF ) );
	sColors.push_back( RtfColor( 0xFF0000 ) );
	sColors.push_back( RtfColor( 0xFFFFFF ) );
	sColors.push_back( RtfColor( 0xFFFF00 ) );

	long nMinDelta = 0x7FFFFFFF; //MAXLONG;
	int nIndex = -1;
	for (int i = 0; i < (int)sColors.size(); i++ )
	{
		int nCurDelta = ( sColors[i].GetR() - GetR() ) * ( sColors[i].GetR() - GetR() ) +
						( sColors[i].GetG() - GetG() ) * ( sColors[i].GetG() - GetG() ) +
						( sColors[i].GetB() - GetB() ) * ( sColors[i].GetB() - GetB() );
		if ( nCurDelta < nMinDelta )
		{
			nIndex = i;
			nMinDelta = nCurDelta;
		}
	}
	switch ( nIndex )
	{
		case 0: return L"black";
		case 1: return L"blue";
		case 2: return L"cyan";
		case 3: return L"darkBlue";
		case 4: return L"darkCyan";
		case 5: return L"darkGray";
		case 6: return L"darkGreen";
		case 7: return L"darkMagenta";
		case 8: return L"darkRed";
		case 9: return L"darkYellow";
		case 10: return L"green";
		case 11: return L"lightGray";
		case 12: return L"magenta";
		case 13: return L"red";
		case 14: return L"white";
		case 15: return L"yellow";
		default: break;
	}
	return L"none";
}
void RtfColor::RGB2HSL(unsigned char unR, unsigned char unG, unsigned char unB, double& dH, double& dS, double& dL)
{
	int nmin   = (std::min)( unR, (std::min)( unG, unB ) );
	int nmax   = (std::max)( unR, (std::max)( unG, unB ) );
	int nDelta = nmax - nmin;
	double dmax   = ( nmax + nmin ) / 255.0;
	double dDelta = nDelta / 255.0;

	dL = dmax / 2.0;

	if ( 0 == nDelta ) //This is a gray, no chroma...
	{
		dH = 0.0;
		dS = 0.0;
	}
	else //Chromatic data...
	{
		if ( dL < 0.5 ) dS = dDelta / dmax;
		else            dS = dDelta / ( 2.0 - dmax );

		dDelta = dDelta * 1530.0;

		double dR = ( nmax - unR ) / dDelta;
		double dG = ( nmax - unG ) / dDelta;
		double dB = ( nmax - unB ) / dDelta;

		if      ( unR == nmax ) dH = dB - dG;
		else if ( unG == nmax ) dH = c_d_1_3 + dR - dB;
		else if ( unB == nmax ) dH = c_d_2_3 + dG - dR;

		if ( dH < 0.0 ) dH += 1.0;
		if ( dH > 1.0 ) dH -= 1.0;
	}
}
void RtfColor::HSL2RGB(double dH, double dS, double dL, unsigned char &unR, unsigned char& unG, unsigned char& unB)
{
	if ( 0 == dS )
	{
		unR = static_cast<unsigned char>(255 * dL);
		unG = static_cast<unsigned char>(255 * dL);
		unB = static_cast<unsigned char>(255 * dL);
	}
	else
	{
		double dV2;
		if ( dL < 0.5 ) dV2 = dL * ( 1.0 + dS );
		else            dV2 = dL + dS - dS * dL;

		double dV1 = 2.0 * dL - dV2;

		unR	= static_cast<unsigned char>(255 * Hue2RGB( dV1, dV2, dH + c_d_1_3 ) );
		unG	= static_cast<unsigned char>(255 * Hue2RGB( dV1, dV2, dH ) );
		unB = static_cast<unsigned char>(255 * Hue2RGB( dV1, dV2, dH - c_d_1_3 ) );
	}
}
double RtfColor::Hue2RGB(double dV1, double dV2, double dH)
{
	if ( dH < 0.0 ) dH += 1.0;
	if ( dH > 1.0 ) dH -= 1.0;
	if ( dH < c_d_1_6 ) return dV1 + ( dV2 - dV1 ) * 6.0 * dH;
	if ( dH < 0.5     ) return dV2;
	if ( dH < c_d_2_3 ) return dV1 + ( dV2 - dV1 ) * ( c_d_2_3 - dH ) * 6.0;
	return dV1;
}
std::wstring RtfColor::WriteOOXAttribute( std::wstring sParam )
 {
	 std::wstring sResult;
	 if ( m_eTheme == TC_NONE )
	 {
		 if ( L"" == sParam )
		 {
			 sResult = L"color = \"";
			 sResult += ToHexColor();
			 sResult += L"\"";
		 }
		 else if ( L"Fill" == sParam )
		 {
			 sResult = L"fill = \"";
			 sResult += ToHexColor();
			 sResult += L"\"";
		 }
	 }
	 else
	 {
		 std::wstring sTheme;
		 if ( true == GetStringByTheme( sTheme, m_eTheme ) )
		 {
			sResult += L"theme"		+ sParam + L"Color=\""	+ sTheme + L"\"";
			sResult += L" theme"	+ sParam + L"Shade=\""	+ std::to_wstring(m_byteShade) + L"\"";
			sResult += L" theme"	+ sParam + L"Tint=\""	+ std::to_wstring(m_byteTint) + L"\"";
		 }
	 }
	 return sResult;
 }

int RtfShadingChar::GetType()
{
	return TYPE_RTF_PROPERTY_SHADING_CHAR;
}
std::wstring RtfShadingChar::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring  sResult;
	RENDER_RTF_INT( m_nValue, sResult, L"chshdng" )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += L"\\chbghoriz";		break;
		case st_chbgvert:		sResult += L"\\chbgvert";		break;
		case st_chbgfdiag:		sResult += L"\\chbgfdiag";		break;
		case st_chbgbdiag:		sResult += L"\\chbgbdiag";		break;
		case st_chbgcross:		sResult += L"\\chbgcross";		break;
		case st_chbgdcross:		sResult += L"\\chbgdcross";		break;
		case st_chbgdkhoriz:	sResult += L"\\chbgdkhoriz";		break;
		case st_chbgdkvert:		sResult += L"\\chbgdkvert";		break;
		case st_chbgdkfdiag:	sResult += L"\\chbgdkfdiag";		break;
		case st_chbgdkbdiag:	sResult += L"\\chbgdkbdiag";		break;
		case st_chbgdkcross:	sResult += L"\\chbgdkcross";		break;
		case st_chbgdkdcross:	sResult += L"\\chbgdkdcross";	break;
		default:
			break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, L"chcfpat" )
	RENDER_RTF_INT( m_nBackColor, sResult, L"chcbpat" )
	return sResult;
}

int RtfShadingParGetType()
{
	return TYPE_RTF_PROPERTY_SHADING_PARAGRAPH;
}
std::wstring RtfShadingPar::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring  sResult;
	RENDER_RTF_INT( m_nValue, sResult, L"shading" )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += L"\\bghoriz";	break;
		case st_chbgvert:		sResult += L"\\bgvert";		break;
		case st_chbgfdiag:		sResult += L"\\bgfdiag";	break;
		case st_chbgbdiag:		sResult += L"\\bgbdiag";	break;
		case st_chbgcross:		sResult += L"\\bgcross";	break;
		case st_chbgdcross:		sResult += L"\\bgdcross";	break;
		case st_chbgdkhoriz:	sResult += L"\\bgdkhoriz";	break;
		case st_chbgdkvert:		sResult += L"\\bgdkvert";	break;
		case st_chbgdkfdiag:	sResult += L"\\bgdkfdiag";	break;
		case st_chbgdkbdiag:	sResult += L"\\bgdkbdiag";	break;
		case st_chbgdkcross:	sResult += L"\\bgdkcross";	break;
		case st_chbgdkdcross:	sResult += L"\\bgdkdcross";	break;
		default:
			break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, L"cfpat" )
	RENDER_RTF_INT( m_nBackColor, sResult, L"cbpat" )
	return sResult;
}

int RtfShadingCell::GetType()
{
	return TYPE_RTF_PROPERTY_SHADING_CELL;
}
std::wstring RtfShadingCell::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring  sResult;
	RENDER_RTF_INT( m_nValue, sResult, L"clshdng" )
	switch( m_eType )
	{
		case st_clshdrawnil:	sResult += L"\\clshdrawnil";	break;
		case st_chbghoriz:		sResult += L"\\clbghoriz";		break;
		case st_chbgvert:		sResult += L"\\clbgvert";		break;
		case st_chbgfdiag:		sResult += L"\\clbgfdiag";		break;
		case st_chbgbdiag:		sResult += L"\\clbgbdiag";		break;
		case st_chbgcross:		sResult += L"\\clbgcross";		break;
		case st_chbgdcross:		sResult += L"\\clbgdcross";		break;
		case st_chbgdkhoriz:	sResult += L"\\clbgdkhor";		break;
		case st_chbgdkvert:		sResult += L"\\clbgdkvert";		break;
		case st_chbgdkfdiag:	sResult += L"\\clbgdkfdiag";	break;
		case st_chbgdkbdiag:	sResult += L"\\clbgdkbdiag";	break;
		case st_chbgdkcross:	sResult += L"\\clbgdkcross";	break;
		case st_chbgdkdcross:	sResult += L"\\clbgdkdcross";	break;
		default:
			break;
	}	
	RENDER_RTF_INT( m_nForeColor, sResult, L"clcfpat" )
	if( PROP_DEF != m_nBackColor )
    {
        sResult += L"\\clcbpat" + std::to_wstring(m_nBackColor);
        sResult += L"\\clcfpatraw" + std::to_wstring(m_nBackColor);
    }
	return sResult;
}

int RtfShadingRow::GetType()
{
	return TYPE_RTF_PROPERTY_SHADING_CELL;
}
std::wstring RtfShadingRow::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring  sResult;
	RENDER_RTF_INT( m_nValue, sResult, L"trshdng" )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += L"\\trbghoriz";		break;
		case st_chbgvert:		sResult += L"\\trbgvert";		break;
		case st_chbgfdiag:		sResult += L"\\trbgfdiag";		break;
		case st_chbgbdiag:		sResult += L"\\trbgbdiag";		break;
		case st_chbgcross:		sResult += L"\\trbgcross";		break;
		case st_chbgdcross:		sResult += L"\\trbgdcross";		break;
		case st_chbgdkhoriz:	sResult += L"\\trbgdkhor";		break;
		case st_chbgdkvert:		sResult += L"\\trbgdkvert";		break;
		case st_chbgdkfdiag:	sResult += L"\\trbgdkfdiag";		break;
		case st_chbgdkbdiag:	sResult += L"\\trbgdkbdiag";		break;
		case st_chbgdkcross:	sResult += L"\\trbgdkcross";		break;
		case st_chbgdkdcross:	sResult += L"\\trbgdkdcross";	break;
		default:
			break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, L"trcfpat" )
	RENDER_RTF_INT( m_nBackColor, sResult, L"trcbpat" )
	return sResult;
}

int RtfShadingTableStyle::GetType()
{
	return TYPE_RTF_PROPERTY_SHADING_TABLESTYLE;
}
std::wstring RtfShadingTableStyle::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring  sResult;
	RENDER_RTF_INT( m_nValue, sResult, L"tscellpct" )
	switch( m_eType )
	{
		case st_chbghoriz:		sResult += L"\\tsbghoriz";		break;
		case st_chbgvert:		sResult += L"\\tsbgvert";		break;
		case st_chbgfdiag:		sResult += L"\\tsbgfdiag";		break;
		case st_chbgbdiag:		sResult += L"\\tsbgbdiag";		break;
		case st_chbgcross:		sResult += L"\\tsbgcross";		break;
		case st_chbgdcross:		sResult += L"\\tsbgdcross";		break;
		case st_chbgdkhoriz:	sResult += L"\\tsbgdkhor";		break;
		case st_chbgdkvert:		sResult += L"\\tsbgdkvert";		break;
		case st_chbgdkfdiag:	sResult += L"\\tsbgdkfdiag";		break;
		case st_chbgdkbdiag:	sResult += L"\\tsbgdkbdiag";		break;
		case st_chbgdkcross:	sResult += L"\\tsbgdkcross";		break;
		case st_chbgdkdcross:	sResult += L"\\tsbgdkdcross";	break;
		default:
			break;
	}		
	RENDER_RTF_INT( m_nForeColor, sResult, L"tscellcfpat" )
	RENDER_RTF_INT( m_nBackColor, sResult, L"tscellcbpat" )
	return sResult;
}

RtfShading::RtfShading()
{
	SetDefault();
}
bool RtfShading::operator==( const RtfShading& oChar )
{
	return m_eType == oChar.m_eType && m_nValue == oChar.m_nValue && m_nForeColor == oChar.m_nForeColor && m_nBackColor == oChar.m_nBackColor;
}
bool RtfShading::IsValid()
{
	return true;
}
int RtfShading::GetType()
{
	return TYPE_RTF_PROPERTY_SHADING;
}
void RtfShading::SetDefaultRtf( )
{
	SetDefault();
}
void RtfShading::SetDefaultOOX( )
{
	SetDefault();
}
void RtfShading::SetDefault( )
{
	DEFAULT_PROPERTY_DEF( m_eType, st_none )
	DEFAULT_PROPERTY( m_nValue )
	DEFAULT_PROPERTY( m_nForeColor )
	DEFAULT_PROPERTY( m_nBackColor )
}
void RtfShading::Merge( RtfShading& oParPr )
{
	//свойство должно быть как единое целое, поэтому если oBorPr задано, то переписыватся целиком
	if ( st_none != oParPr.m_eType || PROP_DEF != oParPr.m_nValue || PROP_DEF != oParPr.m_nForeColor || PROP_DEF != oParPr.m_nBackColor )
	{
		m_eType			= oParPr.m_eType;
		m_nValue		= oParPr.m_nValue;
		m_nForeColor	= oParPr.m_nForeColor;
		m_nBackColor	= oParPr.m_nBackColor;
	}
}
std::wstring RtfShading::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfWriter* poRtfWriter = static_cast<RtfWriter*>(oRenderParameter.poWriter);
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
    std::wstring sResult;
	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_COLOR_VALUE;

	if(PROP_DEF != m_nBackColor && PROP_DEF == m_nValue && st_none == m_eType)
		m_nValue = 0;

    std::wstring sShading;
	if( PROP_DEF != m_nForeColor )
	{
		RtfColor oForeColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nForeColor, oForeColor ) )
            sShading += L" w:color=\"" + oForeColor.RenderToOOX(oNewParam) + L"\"";
	}
	if( PROP_DEF != m_nBackColor )
	{
		RtfColor oBackColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nBackColor, oBackColor ) )
            sShading += L" w:fill=\"" + oBackColor.RenderToOOX(oNewParam) + L"\"";
	}
	if( PROP_DEF != m_nValue )
	{
        std::wstring sValue;
		if( 0 <= m_nValue &&  m_nValue <= 2 )		sShading += L" w:val=\"clear\"";
		else if( 2 < m_nValue && m_nValue <= 7 )	sShading += L" w:val=\"pct5\"";
		else if( 7 < m_nValue && m_nValue <= 11 )	sShading += L" w:val=\"pct10\"";
		else if( 11 < m_nValue && m_nValue <= 13 )	sShading += L" w:val=\"pct12\"";
		else if( 13 < m_nValue && m_nValue <= 17 )	sShading += L" w:val=\"pct15\"";
		else if( 17 < m_nValue && m_nValue <= 22 )	sShading += L" w:val=\"pct20\"";
		else if( 22 < m_nValue && m_nValue <= 27 )	sShading += L" w:val=\"pct25\"";
		else if( 27 < m_nValue && m_nValue <= 32 )	sShading += L" w:val=\"pct30\"";
		else if( 32 < m_nValue && m_nValue <= 36 )	sShading += L" w:val=\"pct35\"";
		else if( 36 < m_nValue && m_nValue <= 38 )	sShading += L" w:val=\"pct37\"";
		else if( 38 < m_nValue && m_nValue <= 42 )	sShading += L" w:val=\"pct40\"";
		else if( 42 < m_nValue && m_nValue <= 47 )	sShading += L" w:val=\"pct45\"";
		else if( 47 < m_nValue && m_nValue <= 52 )	sShading += L" w:val=\"pct50\"";
		else if( 52 < m_nValue && m_nValue <= 57 )	sShading += L" w:val=\"pct55\"";
		else if( 57 < m_nValue && m_nValue <= 61 )	sShading += L" w:val=\"pct60\"";
		else if( 61 < m_nValue && m_nValue <= 63 )	sShading += L" w:val=\"pct62\"";
		else if( 63 < m_nValue && m_nValue <= 70 )	sShading += L" w:val=\"pct65\"";
		else if( 70 < m_nValue && m_nValue <= 80 )	sShading += L" w:val=\"pct75\"";
		else if( 80 < m_nValue && m_nValue <= 86 )	sShading += L" w:val=\"pct85\"";
		else if( 86 < m_nValue && m_nValue <= 88 )	sShading += L" w:val=\"pct87\"";
		else if( 88 < m_nValue && m_nValue <= 92 )	sShading += L" w:val=\"pct90\"";
		else if( 92 < m_nValue && m_nValue <= 97 )	sShading += L" w:val=\"pct95\"";
		else if( 97 < m_nValue && m_nValue <= 100 )	sShading += L" w:val=\"solid\"";
	}
	else
	{
		switch( m_eType )
		{
			case st_clshdrawnil:	sShading += L" w:val=\"nil\"";						break;
			case st_chbghoriz:		sShading += L" w:val=\"thinHorzStripehorzStripe\"";	break;
			case st_chbgvert:		sShading += L" w:val=\"thinVertStripe\"";			break;
			case st_chbgfdiag:		sShading += L" w:val=\"thinReverseDiagStripe\"";	break;
			case st_chbgbdiag:		sShading += L" w:val=\"thinDiagStripe\"";			break;
			case st_chbgcross:		sShading += L" w:val=\"thinHorzCross\"";			break;
			case st_chbgdcross:		sShading += L" w:val=\"thinDiagCross\"";			break;
			case st_chbgdkhoriz:	sShading += L" w:val=\"horzStripe\"";				break;
			case st_chbgdkvert:		sShading += L" w:val=\"vertStripe\"";				break;
			case st_chbgdkfdiag:	sShading += L" w:val=\"reverseDiagStripe\"";		break;
			case st_chbgdkbdiag:	sShading += L" w:val=\"diagStripe\"";				break;
			case st_chbgdkcross:	sShading += L" w:val=\"horzCross\"";				break;
			case st_chbgdkdcross:	sShading += L" w:val=\"diagCross\"";				break;
			default:
				break;
		}
	}
    if( false == sShading.empty() )
		sResult = L"<w:shd " + sShading + L"/>";
	return sResult;
}

RtfBorder::RtfBorder()
{
	SetDefault();
}
bool RtfBorder::operator==( const RtfBorder& oChar )
{
	return m_eType == oChar.m_eType && m_nWidth == oChar.m_nWidth && m_nSpace == oChar.m_nSpace && m_nColor == oChar.m_nColor;
}
bool RtfBorder::IsValid()
{
	return bt_none != m_eType;
}
int RtfBorder::GetType()
{
	return TYPE_RTF_PROPERTY_BORDER;
}
void RtfBorder::SetDefaultRtf( )
{
	SetDefault();
}
void RtfBorder::SetDefaultOOX( )
{
	SetDefault();
}
void RtfBorder::SetDefault( )
{
	DEFAULT_PROPERTY_DEF( m_eType, bt_none )
	DEFAULT_PROPERTY	( m_nWidth )
	DEFAULT_PROPERTY	( m_nSpace )
	DEFAULT_PROPERTY	( m_nColor )
}
void RtfBorder::SetEmpty( )
{
	m_eType		= bt_brdrnone;
	m_nWidth	= 0;
	m_nSpace	= 0;
	m_nColor	= -1; //auto
}
void RtfBorder::Merge( RtfBorder& oBorPr )
{
	//свойство должно быть как единое целое, поэтому если oBorPr задано, то переписыватся целиком
	if ( bt_none != oBorPr.m_eType || PROP_DEF != oBorPr.m_nWidth || PROP_DEF != oBorPr.m_nSpace || PROP_DEF != oBorPr.m_nColor )
	{
		m_eType		= oBorPr.m_eType;
		m_nWidth	= oBorPr.m_nWidth;
		m_nSpace	= oBorPr.m_nSpace;
		m_nColor	= oBorPr.m_nColor;
	}
}
bool RtfBorder::GetStringRtfByType( _BorderType nValue, std::wstring& sValue )
{
	sValue = L"\\brdrs";
	//switch( nValue )
	//{
	//default: sResult = L"\\brdrs"; break;
	////}
	return false;
}
std::wstring RtfBorder::GetStringOOXByType( _BorderType nValue, std::wstring& sValue )
{
	std::wstring sResult;
	sResult = L"single";
	//switch( nValue )
	//{
	//default: sResult = L"single"; break;
	//}
	return sResult;
}
std::wstring RtfBorder::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring  sResult;
	switch( m_eType )
	{
		case bt_brdrs:			sResult = L"\\brdrs";			break;
		case bt_brdrth:			sResult = L"\\brdrth";			break;
		case bt_brdrsh:			sResult = L"\\brdrsh";			break;
		case bt_brdrdb:			sResult = L"\\brdrdb";			break;
		case bt_brdrdot:		sResult = L"\\brdrdot";			break;
		case bt_brdrdash:		sResult = L"\\brdrdash";		break;
		case bt_brdrhair:		sResult = L"\\brdrhair";		break;
		case bt_brdrinset:		sResult = L"\\brdrinset";		break;
		case bt_brdrdashsm:		sResult = L"\\brdrdashsm";		break;
		case bt_brdrdashd:		sResult = L"\\brdrdashd";		break;
		case bt_brdrdashdd:		sResult = L"\\brdrdashdd";		break;
		case bt_brdrdashdot:	sResult = L"\\brdrdashdot";		break;
		case bt_brdrtriple:		sResult = L"\\brdrtriple";		break;
		case bt_brdrtnthsg:		sResult = L"\\brdrtnthsg";		break;
		case bt_brdrthtnsg:		sResult = L"\\brdrthtnsg";		break;
		case bt_brdrtnthtnsg:	sResult = L"\\brdrtnthtnsg";	break;
		case bt_brdrtnthtnmg:	sResult = L"\\brdrtnthtnmg";	break;
		case bt_brdrtnthlg:		sResult = L"\\brdrtnthlg";		break;
		case bt_brdrthtnlg:		sResult = L"\\brdrthtnlg";		break;
		case bt_brdrtnthtnlg:	sResult = L"\\brdrtnthtnlg";	break;
		case bt_brdrwavy:		sResult = L"\\brdrwavy";		break;
		case bt_brdrwavydb:		sResult = L"\\brdrwavydb";		break;
		case bt_brdrdashdotstr: sResult = L"\\brdrdashdotstr";	break;
		case bt_brdremboss:		sResult = L"\\brdremboss";		break;
		case bt_brdrengrave:	sResult = L"\\brdrengrave";		break;
		case bt_brdroutset:		sResult = L"\\brdroutset";		break;
		case bt_brdrnone:		sResult = L"\\brdrnone";		break;
		default:
			break;
	}
	if( PROP_DEF != m_nWidth && m_nWidth > 0)
        sResult += L"\\brdrw" + std::to_wstring(m_nWidth );
	if( PROP_DEF != m_nSpace  && m_nSpace > 0)
        sResult += L"\\brsp" + std::to_wstring(m_nSpace );
	if( PROP_DEF != m_nColor )
        sResult += L"\\brdrcf" + std::to_wstring(m_nColor );
	return sResult;
}
std::wstring RtfBorder::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
    std::wstring  sResult;

	switch( m_eType )
	{
		case bt_brdrs:			sResult += L" w:val=\"single\"";				break;
		case bt_brdrth:			sResult += L" w:val=\"thick\"";					break;
		case bt_brdrsh:			sResult += L" w:val=\"thin";					break;
		case bt_brdrdb:			sResult += L" w:val=\"double\"";				break;
		case bt_brdrdot:		sResult += L" w:val=\"dotted\"";				break;
		case bt_brdrdash:		sResult += L" w:val=\"dashed\"";				break;
		case bt_brdrhair:		sResult += L" w:val=\"hair\"";					break;
		case bt_brdrdashsm:		sResult += L" w:val=\"dashSmallGap\"";			break;
		case bt_brdrdashd:		sResult += L" w:val=\"dotDash\"";				break;
		case bt_brdrdashdd:		sResult += L" w:val=\"dotDotDash\"";			break;
		case bt_brdrinset:		sResult += L" w:val=\"inset\"";					break;
		case bt_brdrnone:		sResult += L" w:val=\"nil\"";					break;
		case bt_brdroutset:		sResult += L" w:val=\"outset\"";				break;
		case bt_brdrtriple:		sResult += L" w:val=\"triple\"";				break;
		case bt_brdrtnthsg:		sResult += L" w:val=\"thinThickSmallGap\"";		break;
		case bt_brdrthtnsg:		sResult += L" w:val=\"thickThinSmallGap\"";		break;
		case bt_brdrtnthtnsg:	sResult += L" w:val=\"thinThickThinSmallGap\""; break;
		case bt_brdrtnthtnmg:	sResult += L" w:val=\"thinThickThinMediumGap\"";break;
		case bt_brdrtnthmg:		sResult += L" w:val=\"thinThickMediumGap\"";	break;
		case bt_brdrthtnmg:		sResult += L" w:val=\"thickThinMediumGap\"";	break;
		case bt_brdrtnthlg:		sResult += L" w:val=\"thinThickLargeGap\"";		break;
		case bt_brdrthtnlg:		sResult += L" w:val=\"thickThinLargeGap\"";		break;
		case bt_brdrtnthtnlg:	sResult += L" w:val=\"thinThickThinLargeGap\"";	break;
		case bt_brdrwavy:		sResult += L" w:val=\"wave\"";					break;
		case bt_brdrwavydb:		sResult += L" w:val=\"doubleWave\"";			break;
		case bt_brdrdashdotstr: sResult += L" w:val=\"dashDotStroked\"";		break;
		case bt_brdremboss:		sResult += L" w:val=\"threeDEmboss\"";			break;
		case bt_brdrengrave:	sResult += L" w:val=\"threeDEngrave\"";			break;
		default:
			break;
	}
	if( PROP_DEF != m_nColor )
	{
		RtfColor oColor;
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_COLOR_VALUE;
		
		if (m_nColor == -1)
		{
			oColor.m_bAuto = true;
            sResult += L" w:color=\"auto\"";
		}
		else if( true == poRtfDocument->m_oColorTable.GetColor( m_nColor, oColor ) )
            sResult += L" w:color=\"" + oColor.RenderToOOX(oNewParam) + L"\"";
	}
	if( PROP_DEF != m_nWidth ) //w:sz  1/8 twips (equivalent to 1/576th of an inch)
	{
        sResult += L" w:sz=\"" + std::to_wstring(2 * m_nWidth / 5 ) + L"\"";
	}
	if( PROP_DEF != m_nSpace )
	{
        sResult += L" w:space=\"" + std::to_wstring((int)RtfUtility::Twip2pt( m_nSpace )) + L"\"";
	}

    if( false == sResult.empty() )
	{
		if( RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE == oRenderParameter.nType )
			;
		else if( RENDER_TO_OOX_PARAM_BORDER_TAG == oRenderParameter.nType )
			sResult = L"<" + oRenderParameter.sValue + sResult + L"/>";
		else
			sResult = L"<w:bdr" + sResult + L"/>";
	}
	return sResult;
}

RtfCharProperty::RtfCharProperty()
{
	m_bListLevel = false;
	SetDefault();
}
bool RtfCharProperty::operator==( const RtfCharProperty& oChar )
{
	return m_nAnimated == oChar.m_nAnimated && m_bBold == oChar.m_bBold && m_bCaps == oChar.m_bCaps && m_nScalex == oChar.m_nScalex &&
			m_nCharStyle == oChar.m_nCharStyle && m_nDown == oChar.m_nDown && m_bEmbo == oChar.m_bEmbo && m_nCharacterSpacing == oChar.m_nCharacterSpacing &&
			m_nFitText == oChar.m_nFitText && m_nFont == oChar.m_nFont && m_nFont2 == oChar.m_nFont2 && m_nFont3 == oChar.m_nFont3 &&
			m_nFontSize == oChar.m_nFontSize && m_bItalic == oChar.m_bItalic && m_bImprint == oChar.m_bImprint && m_nKerning == oChar.m_nKerning &&
			m_bRightToLeft == oChar.m_bRightToLeft && m_nComplexScript == oChar.m_nComplexScript && m_bOutline == oChar.m_bOutline && m_bScaps == oChar.m_bScaps &&
			m_bShadow == oChar.m_bShadow && m_bStrike == oChar.m_bStrike && m_bSub == oChar.m_bSub && m_bSuper == oChar.m_bSuper &&
			m_bHidden == oChar.m_bHidden && m_nHightlited == oChar.m_nHightlited && m_nForeColor == oChar.m_nForeColor &&
			m_eUnderStyle == oChar.m_eUnderStyle && m_nUnderlineColor == oChar.m_nUnderlineColor && m_nUp == oChar.m_nUp &&
			m_poBorder == oChar.m_poBorder && m_poShading == oChar.m_poShading && m_nDeleted == oChar.m_nDeleted &&
			m_nRevised == oChar.m_nRevised && m_nCrAuth == oChar.m_nCrAuth && m_nRevauthDel == oChar.m_nRevauthDel &&
			m_nRevdttm == oChar.m_nRevdttm && m_nRevdttmDel == oChar.m_nRevdttmDel && m_nCrDate == oChar.m_nCrDate;
}
int RtfCharProperty::GetType()
{
	return TYPE_RTF_PROPERTY_CHAR;
}
void RtfCharProperty::SetDefaultRtf()
{
	SetDefault();

	m_poShading.SetDefaultRtf();
	m_poBorder.SetDefaultRtf();

	if (false == m_bListLevel)
		m_nFontSize = 24;
}
void RtfCharProperty::SetDefaultOOX()
{
	SetDefault();

	m_poShading.SetDefaultOOX();
	m_poBorder.SetDefaultOOX();

	if (false == m_bListLevel)
		m_nFontSize = 20;
}
void RtfCharProperty::SetDefault()
{
	DEFAULT_PROPERTY	( m_nAnimated )
	DEFAULT_PROPERTY	( m_bBold )
	DEFAULT_PROPERTY	( m_bCaps )
	DEFAULT_PROPERTY	( m_nScalex )
	DEFAULT_PROPERTY	( m_nCharStyle )
	DEFAULT_PROPERTY	( m_nDown )
	DEFAULT_PROPERTY	( m_bEmbo )
	DEFAULT_PROPERTY	( m_nCharacterSpacing )
	DEFAULT_PROPERTY	( m_nFitText )
	DEFAULT_PROPERTY	( m_nFont )
	DEFAULT_PROPERTY	( m_nFont2 )
	DEFAULT_PROPERTY	( m_nFont3 )
	DEFAULT_PROPERTY	( m_nFontSize )
	DEFAULT_PROPERTY	( m_bItalic )
	DEFAULT_PROPERTY	( m_bImprint )
	DEFAULT_PROPERTY	( m_nKerning )
	DEFAULT_PROPERTY	( m_bRightToLeft )
	DEFAULT_PROPERTY	( m_bOutline )
	DEFAULT_PROPERTY	( m_bScaps )
	DEFAULT_PROPERTY	( m_bShadow )
	DEFAULT_PROPERTY	( m_bStrike )
	DEFAULT_PROPERTY	( m_nStriked )
	DEFAULT_PROPERTY	( m_bSub )
	DEFAULT_PROPERTY	( m_bSuper )
	DEFAULT_PROPERTY	( m_bHidden )
	DEFAULT_PROPERTY	( m_nHightlited )
	DEFAULT_PROPERTY	( m_nForeColor )
	DEFAULT_PROPERTY_DEF( m_eUnderStyle, uls_none )
	DEFAULT_PROPERTY	( m_nUnderlineColor )
	DEFAULT_PROPERTY	( m_nUp )
	DEFAULT_PROPERTY	( m_nComplexScript )
	DEFAULT_PROPERTY	( m_nLanguage )
	DEFAULT_PROPERTY	( m_nLanguageAsian )

	DEFAULT_PROPERTY	( m_nCrAuth)
	DEFAULT_PROPERTY	( m_nCrDate)

	DEFAULT_PROPERTY	( m_nDeleted)
	DEFAULT_PROPERTY	( m_nRevised)
	DEFAULT_PROPERTY	( m_nRevauth)
	DEFAULT_PROPERTY	( m_nRevdttm)
	DEFAULT_PROPERTY	( m_nRevauthDel)
	DEFAULT_PROPERTY	( m_nRevdttmDel)

	DEFAULT_PROPERTY	( m_nInsrsid)

	m_poShading.SetDefault();
	m_poBorder.SetDefault();

	m_pOldCharProp = RtfCharPropertyPtr();
}
void RtfCharProperty::Merge( RtfCharProperty& oCharPr, bool bAll )
{
	MERGE_PROPERTY( m_nAnimated,	oCharPr )
	MERGE_PROPERTY( m_bBold,		oCharPr )
	MERGE_PROPERTY( m_bCaps,		oCharPr )
	MERGE_PROPERTY( m_nScalex,		oCharPr )
	MERGE_PROPERTY( m_nDown,		oCharPr )
	MERGE_PROPERTY( m_bEmbo,		oCharPr )
	MERGE_PROPERTY( m_nCharacterSpacing, oCharPr )
	MERGE_PROPERTY( m_nFitText,		oCharPr )
	MERGE_PROPERTY( m_nFont,		oCharPr )
	MERGE_PROPERTY( m_nFont2,		oCharPr )
	MERGE_PROPERTY( m_nFont3,		oCharPr )
	MERGE_PROPERTY( m_nFontSize,	oCharPr )
	MERGE_PROPERTY( m_bItalic,		oCharPr )
	MERGE_PROPERTY( m_bImprint,		oCharPr )
	MERGE_PROPERTY( m_nKerning,		oCharPr )
	MERGE_PROPERTY( m_bRightToLeft, oCharPr )
	MERGE_PROPERTY( m_nLanguage,	oCharPr )
	MERGE_PROPERTY( m_nLanguageAsian,oCharPr )
	MERGE_PROPERTY( m_bOutline,		oCharPr )
	MERGE_PROPERTY( m_bScaps,		oCharPr )
	MERGE_PROPERTY( m_bShadow,		oCharPr )
	MERGE_PROPERTY( m_bSub,			oCharPr )
	MERGE_PROPERTY( m_bSuper,		oCharPr )
	MERGE_PROPERTY( m_bHidden,		oCharPr )
	MERGE_PROPERTY( m_nForeColor,	oCharPr )
	MERGE_PROPERTY( m_nCrAuth,		oCharPr )
	MERGE_PROPERTY( m_nCrDate,		oCharPr )
	MERGE_PROPERTY( m_nDeleted,		oCharPr )
	MERGE_PROPERTY( m_nRevised,		oCharPr )
	MERGE_PROPERTY( m_nRevauth,		oCharPr )
	MERGE_PROPERTY( m_nRevdttm,		oCharPr )
	MERGE_PROPERTY( m_nRevauthDel,	oCharPr )
	MERGE_PROPERTY( m_nRevdttmDel,	oCharPr )
	MERGE_PROPERTY( m_nInsrsid,		oCharPr )

	if (bAll)
	{
		MERGE_PROPERTY( m_nCharStyle,	oCharPr )
		MERGE_PROPERTY( m_bStrike,		oCharPr )
		MERGE_PROPERTY( m_nStriked,		oCharPr )
		MERGE_PROPERTY( m_nHightlited,	oCharPr )
		//свойство должно быть как единое целое, поэтому если oBorPr задано, то переписыватся целиком
		if ( uls_none != oCharPr.m_eUnderStyle || PROP_DEF != oCharPr.m_nUnderlineColor )
		{
			m_eUnderStyle = oCharPr.m_eUnderStyle;
			m_nUnderlineColor = oCharPr.m_nUnderlineColor;
		}
	}
	MERGE_PROPERTY_DEF	( m_bSub, oCharPr, uls_none )
	MERGE_PROPERTY		( m_nUp, oCharPr )
	MERGE_PROPERTY		( m_nComplexScript, oCharPr )

	m_poBorder.Merge( oCharPr.m_poBorder );
	m_poShading.Merge( oCharPr.m_poShading );
}
std::wstring RtfCharProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	RENDER_RTF_INT( m_nAnimated,	sResult, L"animtext" )
	RENDER_RTF_BOOL( m_bBold,		sResult, L"b" )
	RENDER_RTF_BOOL( m_bCaps,		sResult, L"caps" )
	RENDER_RTF_INT( m_nScalex,		sResult, L"charscalex" )
	//RENDER_RTF_INT( m_nCharStyle, sResult, L"cs" )
	RENDER_RTF_INT( m_nDown,		sResult, L"dn" )
	RENDER_RTF_BOOL( m_bEmbo,		sResult, L"embo" )
	RENDER_RTF_INT( m_nCharacterSpacing, sResult, L"expndtw" )
	
	if( PROP_DEF != m_nCharacterSpacing )
        sResult += L"\\expnd" + std::to_wstring( m_nCharacterSpacing / 5  );
	if( PROP_DEF != m_nFitText )
        sResult += L"\\fittext" + std::to_wstring( m_nFitText / 5  );
	
	RENDER_RTF_INT( m_nFont,		sResult, L"f" )
	//RENDER_RTF_INT( m_nFont2,		sResult, L"fittext" )
	//RENDER_RTF_INT( m_nFont3,		sResult, L"fittext" )
	RENDER_RTF_INT( m_nFontSize,	sResult, L"fs" )
	RENDER_RTF_BOOL( m_bItalic,		sResult, L"i" )
	RENDER_RTF_BOOL( m_bImprint,	sResult, L"impr" )
	RENDER_RTF_INT( m_nKerning,		sResult, L"kerning" )
	
	if (m_bRightToLeft != PROP_DEF )
	{
		if( m_bRightToLeft !=0)	sResult += L"\\rtlch";
		else					sResult += L"\\ltrch";
	}

	if( PROP_DEF != m_nComplexScript )
        sResult += L"\\fcs" + std::to_wstring( m_nComplexScript);

	RENDER_RTF_BOOL	( m_bOutline, sResult, L"outl" )
	RENDER_RTF_BOOL	( m_bScaps	, sResult, L"scaps" )
	RENDER_RTF_BOOL	( m_bShadow	, sResult, L"shad" )
	RENDER_RTF_BOOL	( m_bStrike	, sResult, L"strike" )
	RENDER_RTF_INT	( m_nStriked, sResult, L"striked" )
	RENDER_RTF_BOOL	( m_bSub	, sResult, L"sub" )
	RENDER_RTF_BOOL	( m_bSuper	, sResult, L"super" )
	RENDER_RTF_INT	( m_bHidden	, sResult, L"v" )
	RENDER_RTF_INT	( m_nHightlited, sResult, L"highlight" )
	RENDER_RTF_INT	( m_nForeColor, sResult, L"cf" )
    RENDER_RTF_INT	( m_nLanguage, sResult, L"lang" )
	RENDER_RTF_INT	( m_nLanguageAsian, sResult, L"langfe" )
	
	switch( m_eUnderStyle )
	{
		case uls_Single:				sResult += L"\\ul";			break;
		case uls_Dotted:				sResult += L"\\uld";		break;
		case uls_Dashed:				sResult += L"\\uldash";		break;
		case uls_Dash_dotted:			sResult += L"\\uldashd";	break;
		case uls_Dash_dot_dotted:		sResult += L"\\uldashdd";	break;
		case uls_Double:				sResult += L"\\uldb";		break;
		case uls_Heavy_wave:			sResult += L"\\ulhwave";	break;
		case uls_Long_dashe:			sResult += L"\\ulldash";	break;
		case uls_Stops_all:				sResult += L"\\ulnone";		break;
		case uls_Thick:					sResult += L"\\ulth";		break;
		case uls_Thick_dotted:			sResult += L"\\ulthd";		break;
		case uls_Thick_dashed:			sResult += L"\\ulthdash";	break;
		case uls_Thick_dash_dotted:		sResult += L"\\ulthdashd";	break;
		case uls_Thick_dash_dot_dotted: sResult += L"\\ulthdashdd";	break;
		case uls_Thick_long_dashed:		sResult += L"\\ulthldash";	break;
		case uls_Double_wave:			sResult += L"\\ululdbwave";	break;
		case uls_Word:					sResult += L"\\ulw";		break;
		case uls_Wave:					sResult += L"\\ulwave";		break;
		default:
			break;
	}
	RENDER_RTF_INT( m_nUnderlineColor, sResult, L"ulc" )
	RENDER_RTF_INT( m_nUp, sResult, L"up" )

	if( m_poBorder.IsValid() == true )
	{
		sResult += L"\\chbrdr";
		sResult +=  m_poBorder.RenderToRtf( oRenderParameter );
	}
	if( m_poShading.IsValid() == true )
		sResult +=  m_poShading.RenderToRtf( oRenderParameter );


	if ( m_nDeleted != PROP_DEF)
	{
		sResult += L"\\deleted";
		RENDER_RTF_INT( m_nRevauthDel,	sResult, L"revauthdel" )
		RENDER_RTF_INT( m_nRevdttmDel,	sResult, L"revdttmdel" )
	}	
	
	if ( m_nRevised != PROP_DEF)
	{
		sResult += L"\\revised";
		RENDER_RTF_INT( m_nRevauth,		sResult, L"revauth" )
		RENDER_RTF_INT( m_nRevdttm,		sResult, L"revdttm" )
	}

	if (m_pOldCharProp)
	{
		RENDER_RTF_INT( m_nCrAuth,		sResult, L"crauth" )
		RENDER_RTF_INT( m_nCrDate,		sResult, L"crdate" )

        sResult += L"{\\*\\oldcprops";
		sResult += L"\\plain";
		sResult += m_pOldCharProp->RenderToRtf(oRenderParameter);
		sResult += L"}";
	}
	return sResult;
}
std::wstring RtfCharProperty::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	RtfDocument	* poRtfDocument = static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter	* poOOXWriter	= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
	bool bInsert = false;
	bool bDelete = false;
	
	if( RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
	{//w:rPr в m:ctrlPr 
		if (m_nRevised != PROP_DEF)
		{
			bInsert = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nRevauth);
            std::wstring sDate(RtfUtility::convertDateTime( m_nRevdttm ).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_nRevised = PROP_DEF;
		}
		if (m_nDeleted != PROP_DEF)
		{
			bDelete = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nRevauthDel);
            std::wstring sDate(RtfUtility::convertDateTime( m_nRevdttmDel ).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_nDeleted = PROP_DEF;
		}
		sResult += L"<w:rPr>";
	}

	if ( PROP_DEF != m_nDeleted )//для rPr в pPr
	{
        std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nRevauthDel);
        std::wstring sDate(RtfUtility::convertDateTime(m_nRevdttmDel).c_str());

		sResult += L"<w:del w:date=\"" + sDate + L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\"/>";
	}
	if ( PROP_DEF != m_nRevised )
	{
        std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nRevauth);
        std::wstring sDate(RtfUtility::convertDateTime(m_nRevdttm).c_str());
		
		sResult += L"<w:ins w:date=\"" + sDate + L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\"/>";
	}
	if( PROP_DEF != m_nCharStyle )
	{
		RtfStylePtr oStyle;
		if( true == poRtfDocument->m_oStyleTable.GetStyle( m_nCharStyle, oStyle ) )
		{
			sResult += L"<w:rStyle w:val=\"";
			sResult += L"Style_" + std::to_wstring(m_nCharStyle); //oStyle->m_sName;
			sResult += L"\"/>";
		}
	}	
	switch( m_nAnimated )
	{
		case 0:	sResult += L"<w:effect w:val=\"none\"/>";			break;
		case 1:	sResult += L"<w:effect w:val=\"lights\"/>";			break;
		case 2:	sResult += L"<w:effect w:val=\"blinkBackground\"/>";break;
		case 3:	sResult += L"<w:effect w:val=\"sparkle\"/>";		break;
		case 4:	sResult += L"<w:effect w:val=\"antsBlack\"/>";		break;
		case 5:	sResult += L"<w:effect w:val=\"antsRed\"/>";		break;
		case 6:	sResult += L"<w:effect w:val=\"shimmer\"/>";		break;
		default:
			break;
	}
	RENDER_OOX_BOOL	( m_bBold, sResult, L"w:b" )
	RENDER_OOX_BOOL	( m_bCaps, sResult, L"w:caps" )
	RENDER_OOX_INT	( (int)m_nScalex, sResult, L"w:w" )
	
	if( PROP_DEF != m_nDown )
	{
        sResult += L"<w:position w:val=\"-" + std::to_wstring(m_nDown) + L"\"/>";
	}
	RENDER_OOX_BOOL( m_bEmbo, sResult, L"w:emboss" )
	RENDER_OOX_INT( m_nCharacterSpacing, sResult, L"w:spacing" )
	
	if( PROP_DEF != m_nFitText )
	{
		if( -1 == m_nFitText )
		{
            sResult += L"<w:fitText w:id=\"" + poOOXWriter->m_nCurFitId + L"\" w:val=\"" + std::to_wstring(poOOXWriter->m_nCurFitWidth) + L"\"/>";
		}
		else 
		{
			poOOXWriter->m_nCurFitId = poRtfDocument->m_oIdGenerator.Generate_FitTextId();
			poOOXWriter->m_nCurFitWidth = m_nFitText;
            sResult += L"<w:fitText w:id=\"" + poOOXWriter->m_nCurFitId + L"\" w:val=\"" + std::to_wstring(poOOXWriter->m_nCurFitWidth) + L"\"/>";
		}
	}
	if( PROP_DEF == m_nFont)
	{
		if (RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
			m_nFont = poRtfDocument->m_oProperty.m_nDefMathFont;
		else
			m_nFont = poRtfDocument->m_oProperty.m_nDefFont;
	}
	if (PROP_DEF == m_nLanguage)
	{
		m_nLanguage = poRtfDocument->m_oProperty.m_nDefLang;
	}
	if (PROP_DEF == m_nLanguageAsian)
	{
		m_nLanguageAsian = poRtfDocument->m_oProperty.m_nDefLangAsian;
	}
	if (PROP_DEF != m_nFont)
	{
		RtfFont oCurFont;
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		if( true == poRtfDocument->m_oFontTable.GetFont(m_nFont,oCurFont) )
		{
			sResult += oCurFont.RenderToOOX(oNewParam);
		}
	}
	if (m_bHidden == 1)
	{
		sResult += L"<w:vanish/>";
	}
	RENDER_OOX_INT( m_nFontSize,	sResult, L"w:sz" )
	RENDER_OOX_BOOL( m_bItalic,		sResult, L"w:i" )

	if (m_nComplexScript != PROP_DEF || m_nLanguageAsian != PROP_DEF )
	{
		RENDER_OOX_INT( m_nFontSize,	sResult, L"w:szCs" )
		RENDER_OOX_INT( m_bItalic,		sResult, L"w:iCs" )
		RENDER_OOX_INT( m_bBold,		sResult, L"w:bCs" )
	}
	RENDER_OOX_BOOL( m_bImprint,	sResult, L"w:imprint" )
	RENDER_OOX_INT( m_nKerning,		sResult, L"w:kern" )
	
	if(m_bRightToLeft !=0 && m_bRightToLeft != PROP_DEF)
		sResult += L"<w:rtl/>";
	
	RENDER_OOX_BOOL( m_nComplexScript,	sResult, L"w:cs" )
	RENDER_OOX_BOOL( m_bOutline,		sResult, L"w:outline" )
	RENDER_OOX_BOOL( m_bScaps,			sResult, L"w:smallCaps" )
	RENDER_OOX_BOOL( m_bShadow,			sResult, L"w:shadow" )
	RENDER_OOX_BOOL( m_bStrike,			sResult, L"w:strike" )
	RENDER_OOX_BOOL( m_nStriked,		sResult, L"w:dstrike" )

	if( 1 == m_bSub )
		sResult += L"<w:vertAlign w:val=\"subscript\"/>";
	if( 1 == m_bSuper )
		sResult += L"<w:vertAlign w:val=\"superscript\"/>";

	if( PROP_DEF != m_nHightlited )
	{
		RtfColor oCurColor;
		if( true == poRtfDocument->m_oColorTable.GetColor( m_nHightlited, oCurColor ) )
		{
			if (!oCurColor.m_bAuto)
			{
				sResult += L"<w:highlight w:val=\"" + oCurColor.GetHighLight() + L"\"/>";
			}
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

    std::wstring sUnderColor;
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
		sResult += L"<w:u " + sUnderColor;
		switch( m_eUnderStyle )
		{
            case uls_Single:				sResult += L" w:val=\"single\"/>";			break;
            case uls_Dotted:				sResult += L" w:val=\"dotted\"/>";			break;
            case uls_Dashed:				sResult += L" w:val=\"dash\"/>";				break;
            case uls_Dash_dotted:			sResult += L" w:val=\"dotDash\"/>";			break;
            case uls_Dash_dot_dotted:		sResult += L" w:val=\"dotDotDash\"/>";		break;
            case uls_Double:				sResult += L" w:val=\"double\"/>";			break;
            case uls_Heavy_wave:			sResult += L" w:val=\"wavyHeavy\"/>";		break;
            case uls_Long_dashe:			sResult += L" w:val=\"dashLong\"/>";			break;
            case uls_Stops_all:				sResult += L" w:val=\"single\"/>";			break;
            case uls_Thick:					sResult += L" w:val=\"thick\"/>";			break;
            case uls_Thick_dotted:			sResult += L" w:val=\"dottedHeavy\"/>";		break;
            case uls_Thick_dashed:			sResult += L" w:val=\"dashedHeavy\"/>";		break;
            case uls_Thick_dash_dotted:		sResult += L" w:val=\"dashDotHeavy\"/>";		break;
            case uls_Thick_dash_dot_dotted: sResult += L" w:val=\"dashDotDotHeavy\"/>";	break;
            case uls_Thick_long_dashed:		sResult += L" w:val=\"dashLongHeavy\"/>";	break;
            case uls_Double_wave:			sResult += L" w:val=\"wavyDouble\"/>";		break;
            case uls_Word:					sResult += L" w:val=\"words\"/>";			break;
            case uls_Wave:					sResult += L" w:val=\"wave\"/>";				break;
			default:
				break;
		}
	}

	RENDER_OOX_INT( m_nUp, sResult, L"w:position" )
	
    if ((m_nLanguage != PROP_DEF || m_nLanguageAsian != PROP_DEF) && RENDER_TO_OOX_PARAM_MATH != oRenderParameter.nType) 
			//todooo сделаь map для используемых в доке
	{
        std::wstring str_lang, str_lang_asian;
#if defined(_WIN32) || defined(_WIN64)
		if (m_nLanguage != PROP_DEF)
		{
			wchar_t buf[29] = {};
			int ccBuf = GetLocaleInfo(m_nLanguage, LOCALE_SISO639LANGNAME, buf, 29);
			
			if (ccBuf > 0)
			{
				str_lang += buf;
				str_lang += L"-";
			}
			
			ccBuf = GetLocaleInfo(m_nLanguage, LOCALE_SISO3166CTRYNAME, buf, 29);
			
			if (ccBuf > 0) str_lang.append(buf);
		}
		if (m_nLanguageAsian != PROP_DEF)
		{
			wchar_t buf[29] = {};
			int ccBuf = GetLocaleInfo(m_nLanguageAsian, LOCALE_SISO639LANGNAME, buf, 29);
			
			if (ccBuf > 0)
			{
				str_lang_asian += buf;
				str_lang_asian += L"-";
			}
			
			ccBuf = GetLocaleInfo(m_nLanguageAsian, LOCALE_SISO3166CTRYNAME, buf, 29);
			
			if (ccBuf > 0) str_lang_asian.append(buf);
		}
#else
		str_lang		= (m_nLanguage != PROP_DEF)			? msLCID2wstring(m_nLanguage)		: L"";
		str_lang_asian	= (m_nLanguageAsian != PROP_DEF)	? msLCID2wstring(m_nLanguageAsian)	: L"";
#endif
        if (false == str_lang.empty() || false == str_lang_asian.empty() )
		{
            sResult += std::wstring(L"<w:lang");
			if (false == str_lang.empty())
			{	
				sResult += L" w:val=\"" + str_lang + L"\"";
			}
			if (false == str_lang_asian.empty())
			{
				sResult += L" w:bidi=\"" + str_lang_asian + L"\"";
			}
			sResult += L"/>";
		}
    }

	sResult +=  m_poBorder.RenderToOOX( oRenderParameter );
	sResult +=  m_poShading.RenderToOOX( oRenderParameter );

	if (m_pOldCharProp)
	{
        std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nCrAuth);
        std::wstring sDate(RtfUtility::convertDateTime(m_nCrDate).c_str());
		
		RenderParameter oRenderParameterNew = oRenderParameter;
		oRenderParameterNew.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

		sResult += L"<w:rPrChange w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			sResult += L"<w:rPr>";
				sResult += m_pOldCharProp->RenderToOOX(oRenderParameterNew);
			sResult += L"</w:rPr>";
		sResult += L"</w:rPrChange>";
	}
	
	if( RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
	{
		sResult += L"</w:rPr>";
		
		if (bDelete)sResult += L"</w:del>";
		if (bInsert)sResult += L"</w:ins>";
	}
	return sResult;
}

RtfListLevelProperty::RtfListLevelProperty()
{
	m_oCharProp.m_bListLevel = true;

	SetDefault();
}
bool RtfListLevelProperty::IsValid()
{
	//return -1 != m_nFollow && -1 != m_nStart && -1 != m_nNumberType && -1 != m_nJustification &&
	//	false == m_sText.empty() && false == m_sNumber.empty();
	return  PROP_DEF != m_nNumberType && false == m_sText.empty();
}
std::wstring RtfListLevelProperty::GenerateListText()
 {//заменяем на булеты
	std::wstring sResult;

	char cBullet[1];  cBullet[0] = (char)149;

	std::string sBullet = cBullet;
	std::wstring swBullet(sBullet.begin(), sBullet.end());

	sResult += swBullet.c_str();
	//std::wstring sOOXNumber = GetLevelTextOOX();
	//for (size_t i = 0; i < sOOXNumber.length(); i++ )
	//	if ( sOOXNumber[i] == '%' && i != sOOXNumber.length() - 1 )
	//	{
	//		sResult += swBullet;
	//		i++;
	//	}
	//	else
	//		sResult.AppendChar( sOOXNumber[i] );
	return sResult;
 }
void RtfListLevelProperty::SetDefault()
{
	DEFAULT_PROPERTY	( m_nSpace )
	DEFAULT_PROPERTY	( m_nLevel )
	DEFAULT_PROPERTY	( m_nNumberType )
	DEFAULT_PROPERTY	( m_bTentative )
	DEFAULT_PROPERTY	( m_nJustification )
	DEFAULT_PROPERTY	( m_nFollow )
	DEFAULT_PROPERTY	( m_nStart )
	DEFAULT_PROPERTY_DEF( m_sText, L"" )
	DEFAULT_PROPERTY_DEF( m_sNumber, L"" )
	DEFAULT_PROPERTY	( m_nNoRestart )
	DEFAULT_PROPERTY	( m_nLegal )
	DEFAULT_PROPERTY	( m_nPictureIndex )
	DEFAULT_PROPERTY	( m_nFirstIndent )
	DEFAULT_PROPERTY	( m_nIndent )
	DEFAULT_PROPERTY	( m_nIndentStart )

	m_oTabs.SetDefault();
	m_oCharProp.SetDefault();
}
std::wstring RtfListLevelProperty::GetFormat( int nNumFormat)
{
	std::wstring sResult;
	switch(nNumFormat)
	{
		case 0: sResult = L"decimal";			break;
		case 1: sResult = L"upperRoman";		break;
		case 2: sResult = L"lowerRoman";		break;
		case 3: sResult = L"upperLetter";		break;
		case 4: sResult = L"lowerLetter";		break;
		case 5: sResult = L"ordinal";			break;
		case 6: sResult = L"cardinalText";		break;
		case 7: sResult = L"ordinalText";		break;
		case 8: sResult = L"hex";				break;
		case 9: sResult = L"chicago";			break;
		case 10: sResult = L"ideographDigital";	break;
		case 11: sResult = L"japaneseCounting";	break;
		case 12: sResult = L"aiueo";			break;
		case 13: sResult = L"iroha";			break;
		case 14: sResult = L"decimalFullWidth";	break;
		case 15: sResult = L"decimalHalfWidth";	break;
		case 16: sResult = L"japaneseLegal";	break;
		case 17: sResult = L"japaneseDigitalTenThousand";	break;
		case 18: sResult = L"decimalEnclosedCircle";		break;
		case 19: sResult = L"decimalFullWidth2";			break;
		case 20: sResult = L"aiueoFullWidth";	break;
		case 21: sResult = L"irohaFullWidth";	break;
		case 22: sResult = L"decimalZero";		break;
		case 23: sResult = L"bullet";			break;
		case 24: sResult = L"ganada";			break;
		case 25: sResult = L"chosung";			break;
		case 26: sResult = L"decimalEnclosedFullstop";		break;
		case 27: sResult = L"decimalEnclosedParen";			break;
		case 28: sResult = L"decimalEnclosedCircleChinese";	break;
		case 29: sResult = L"ideographEnclosedCircle";		break;
		case 30: sResult = L"ideographTraditional";			break;
		case 31: sResult = L"ideographZodiac";				break;
		case 32: sResult = L"ideographZodiacTraditional";	break;
		case 33: sResult = L"taiwaneseCounting";			break;
		case 34: sResult = L"ideographLegalTraditional";	break;
		case 35: sResult = L"taiwaneseCountingThousand";	break;
		case 36: sResult = L"taiwaneseDigital";				break;
		case 37: sResult = L"chineseCounting";				break;
		case 38: sResult = L"chineseLegalSimplified";		break;
		case 39: sResult = L"chineseCountingThousand";		break;
		case 40: sResult = L"chineseCounting";				break;
		case 41: sResult = L"koreanDigital";		break;
		case 42: sResult = L"koreanCounting";		break;
		case 43: sResult = L"koreanLegal";			break;
		case 44: sResult = L"koreanDigital2";		break;
		case 45: sResult = L"hebrew1";				break;
		case 46: sResult = L"arabicAlpha";			break;
		case 47: sResult = L"hebrew2";				break;
		case 48: sResult = L"arabicAbjad";			break;
		case 49: sResult = L"hindiVowels";			break;
		case 50: sResult = L"hindiConsonants";		break;
		case 51: sResult = L"hindiNumbers";			break;
		case 52: sResult = L"hindiCounting";		break;
		case 53: sResult = L"thaiLetters";			break;
		case 54: sResult = L"thaiNumbers";			break;
		case 55: sResult = L"thaiCounting";			break;
		case 56: sResult = L"vietnameseCounting";	break;
		case 57: sResult = L"numberInDash";			break;
		case 58: sResult = L"russianLower";			break;
		case 59: sResult = L"russianUpper";			break;

		case 70: sResult = L"chicago";				break;

		case 255: sResult = L"none";				break;
		default: sResult = L"decimal";
	}
	return sResult;
}
int RtfListLevelProperty::GetFormat( std::wstring sFormat)
{
	if		( L"aiueo" == sFormat )							return 12;
	else if ( L"aiueoFullWidth" == sFormat )				return 20;
	else if ( L"arabicAbjad" == sFormat )					return 48;
	else if ( L"arabicAlpha" == sFormat )					return 46;
	else if ( L"bahtText" == sFormat )						return 0;
	else if ( L"bullet" == sFormat )						return 23;
	else if ( L"cardinalText" == sFormat )					return 6;
	else if ( L"chicago" == sFormat )						return 9;
	else if ( L"chineseCounting" == sFormat )				return 37;
	else if ( L"chineseCountingThousand" == sFormat )		return 39;
	else if ( L"chineseLegalSimplified" == sFormat )		return 38;
	else if ( L"chosung" == sFormat )						return 25;
	else if ( L"custom" == sFormat )						return 0;
	else if ( L"decimal" == sFormat )						return 0;
	else if ( L"decimalEnclosedCircle" == sFormat )			return 18;
	else if ( L"decimalEnclosedCircleChinese" == sFormat )	return 28;
	else if ( L"decimalEnclosedFullstop" == sFormat )		return 26;
	else if ( L"decimalEnclosedParen" == sFormat )			return 27;
	else if ( L"decimalFullWidth" == sFormat )				return 14;
	else if ( L"decimalFullWidth2" == sFormat )				return 19;
	else if ( L"decimalHalfWidth" == sFormat )				return 15;
	else if ( L"decimalZero" == sFormat )					return 22;
	else if ( L"dollarText" == sFormat )					return 0;
	else if ( L"ganada" == sFormat )						return 24;
	else if ( L"hebrew1" == sFormat )						return 45;
	else if ( L"hebrew1" == sFormat )						return 45;
	else if ( L"hebrew2" == sFormat )						return 47;
	else if ( L"hex" == sFormat )							return 8;
	else if ( L"hindiConsonants" == sFormat )				return 50;
	else if ( L"hindiCounting" == sFormat )					return 52;
	else if ( L"hindiNumbers" == sFormat )					return 51;
	else if ( L"hindiVowels" == sFormat )					return 49;
	else if ( L"ideographDigital" == sFormat )				return 10;
	else if ( L"ideographEnclosedCircle" == sFormat )		return 29;
	else if ( L"ideographLegalTraditional" == sFormat )		return 34;
	else if ( L"ideographTraditional" == sFormat )			return 30;
	else if ( L"ideographZodiac" == sFormat )				return 31;
	else if ( L"ideographZodiacTraditional" == sFormat )	return 32;
	else if ( L"iroha" == sFormat )							return 13;
	else if ( L"irohaFullWidth" == sFormat )				return 21;
	else if ( L"japaneseCounting" == sFormat )				return 11;
	else if ( L"japaneseDigitalTenThousand" == sFormat )	return 17;
	else if ( L"japaneseLegal" == sFormat )					return 16;
	else if ( L"koreanCounting" == sFormat )				return 42;
	else if ( L"koreanDigital" == sFormat )					return 41;
	else if ( L"koreanDigital2" == sFormat )				return 44;
	else if ( L"koreanLegal" == sFormat )					return 43;
	else if ( L"lowerLetter" == sFormat )					return 4;
	else if ( L"lowerRoman" == sFormat )					return 2;
	else if ( L"none" == sFormat )							return 255;
	else if ( L"numberInDash" == sFormat )					return 57;
	else if ( L"ordinal" == sFormat )						return 5;
	else if ( L"ordinalText" == sFormat )					return 7;
	else if ( L"russianLower" == sFormat )					return 58;
	else if ( L"russianUpper" == sFormat )					return 59;
	else if ( L"taiwaneseCounting" == sFormat )				return 33;
	else if ( L"taiwaneseCountingThousand" == sFormat )		return 35;
	else if ( L"taiwaneseDigital" == sFormat )				return 36;
	else if ( L"thaiCounting" == sFormat )					return 55;
	else if ( L"thaiLetters" == sFormat )					return 53;
	else if ( L"thaiNumbers" == sFormat )					return 54;
	else if ( L"upperLetter" == sFormat )					return 3;
	else if ( L"upperRoman" == sFormat )					return 1;
	else if ( L"vietnameseCounting" == sFormat )			return 56;
	else if ( L"chicago" == sFormat )						return 70;

	return 0; //decimal
}
std::wstring RtfListLevelProperty::RenderToRtf(RenderParameter oRenderParameter)
{
	RtfDocument	* poRtfDocument = static_cast<RtfDocument*>	( oRenderParameter.poDocument);
	RtfWriter	* poRtfWriter	= static_cast<RtfWriter*>	( oRenderParameter.poWriter);
	
    std::wstring sResult;
	sResult += L"{\\listlevel";

	RENDER_RTF_INT( m_nNumberType, sResult, L"levelnfc" )
	RENDER_RTF_INT( m_nNumberType, sResult, L"levelnfcn" )
	RENDER_RTF_BOOL( m_bTentative, sResult, L"lvltentative" )
	RENDER_RTF_INT( m_nJustification, sResult, L"leveljc" )
	RENDER_RTF_INT( m_nJustification, sResult, L"leveljcn" )
	RENDER_RTF_INT( m_nFollow, sResult, L"levelfollow" )
	RENDER_RTF_INT( m_nStart, sResult, L"levelstartat" )
	RENDER_RTF_INT( m_nNoRestart, sResult, L"levelnorestart" )
	RENDER_RTF_INT( m_nPictureIndex, sResult, L"levelpicture" )
	//чтобы при последующем чтении из rtf не потерялась информация о шрифте
	sResult +=  m_oCharProp.RenderToRtf( oRenderParameter );

    sResult += L"{\\leveltext " + RtfChar::renderRtfText( m_sText, oRenderParameter.poDocument, &m_oCharProp ) + L";}";
    sResult += L"{\\levelnumbers " + RtfChar::renderRtfText( m_sNumber, oRenderParameter.poDocument, &m_oCharProp ) + L";}";

	RENDER_RTF_INT( m_nFirstIndent, sResult, L"fi" )
	RENDER_RTF_INT( m_nIndent, sResult, L"li" )
	RENDER_RTF_INT( m_nIndentStart, sResult, L"lin" )

	for (size_t i = 0 ; i < m_oTabs.m_aTabs.size(); i++ )
	{
		sResult += L"\\jclisttab";
		sResult +=  m_oTabs.m_aTabs[i].RenderToRtf( oRenderParameter );
	}


	sResult += L"}";
	return sResult;
}
std::wstring RtfListLevelProperty::GetLevelTextOOX()
{
    std::wstring sResult = m_sText;
    if ( sResult.length() > 0 )
	{
		size_t nLevelTextLength = sResult[0];
		nLevelTextLength--;
       
		for (int i = (int)m_sNumber.length() - 1; i >= 0; i-- )
		{
			int nReplaceNumber = m_sNumber[i];
			nReplaceNumber--;
            if ( nReplaceNumber >= 0 && nReplaceNumber < (int)sResult.length() )
			{
				int nLevel = sResult[ nReplaceNumber ];

				if (nLevel > 11)
				{//001.rtf 
					nReplaceNumber++;
					nLevel = sResult[ nReplaceNumber];
				}
                std::wstring sExt = L"%" + std::to_wstring(nLevel);

                sResult.erase( nReplaceNumber, 1 );
                sResult.insert(sResult.begin() + nReplaceNumber, sExt.begin(), sExt.end());
                nLevelTextLength += sExt.length() - 1;
			}
		}
        sResult = sResult.substr(1);
        if ( nLevelTextLength < sResult.length() )
            sResult = sResult.substr(0,  nLevelTextLength );

	}
    return XmlUtils::EncodeXmlString( sResult );
}

void RtfListLevelProperty::SetLevelTextOOX(std::wstring sText)
{
	m_sText		= L"";
	m_sNumber	= L"";

	int nLevelOffsets = 0;

	 for (size_t i = 0; i < sText.length() ; i++ )
	 {
		if ( (sText[i] == '%') && (i + 1 < sText.length()) && (isdigit( sText[ i + 1 ] )))
		{
			int nLevel = RtfUtility::ToByte( sText[ i + 1 ] );

			wchar_t ch1 = nLevel - 1;
			m_sText += ch1;
			i++; //т.к. следующий симовл уже учли
			wchar_t ch2 = nLevelOffsets + 1;
			m_sNumber += ch2;
		}
		else
			m_sText += sText[i];
		 nLevelOffsets++;
	 }
	 wchar_t ch = (wchar_t)m_sText.length(); 
     m_sText.insert(m_sText.begin() + 0, ch );
}
std::wstring RtfListLevelProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	return RenderToOOX2(oRenderParameter);
}
std::wstring RtfListLevelProperty::RenderToOOX2(RenderParameter oRenderParameter, int nLvl)
{
    std::wstring sResult;
	int nLevel = PROP_DEF;
	
	if		(PROP_DEF != nLvl)		nLevel = nLvl;
	else if (PROP_DEF != m_nLevel)	nLevel = m_nLevel;
	
	if(PROP_DEF != nLevel)
	{
        sResult += L"<w:lvl w:ilvl=\"" + std::to_wstring(nLevel) + L"\"";
		RENDER_OOX_BOOL_ATTRIBUTE( m_bTentative, sResult, L"w:tentative")
		sResult += L">"; 

		if( PROP_DEF != m_nJustification )
		{
			switch( m_nJustification )
			{
				case 0:	sResult += L"<w:lvlJc w:val=\"left\"/>";		break;
				case 1:	sResult += L"<w:lvlJc w:val=\"center\"/>";	break;
				case 2:	sResult += L"<w:lvlJc w:val=\"right\"/>";	break;
			}
		}
		if( 1 == m_nNoRestart)
			sResult += L"<w:lvlRestart w:val=\"0\"/>";
		if( 1 ==  m_nLegal)
			sResult += L"<w:isLgl/>"; 

		std::wstring sText = GetLevelTextOOX();
		if (false == sText.empty())
		{
			sResult += L"<w:lvlText w:val=\"" + sText + L"\"/>";
		}
        sResult += L"<w:numFmt w:val=\"" + GetFormat(m_nNumberType) + L"\"/>";
		
		RENDER_OOX_INT( m_nPictureIndex, sResult, L"w:lvlPicBulletId" )
		RENDER_OOX_INT( m_nStart, sResult, L"w:start" )
		
		if( PROP_DEF != m_nFollow )
		{
			switch( m_nFollow )
			{
				case 0: sResult += L"<w:suff w:val=\"tab\"/>";		break;
				case 1: sResult += L"<w:suff w:val=\"space\"/>";		break;
				case 2: sResult += L"<w:suff w:val=\"nothing\"/>";	break;
				default:
					break;
			}
		}

        std::wstring spPr;
        std::wstring sIndent;
		if( PROP_DEF != m_nFirstIndent )
		{
			if( m_nFirstIndent >= 0 )
                sIndent += L" w:firstLine=\"" + std::to_wstring(m_nFirstIndent) + L"\"";
			else
                sIndent += L" w:hanging=\"" + std::to_wstring(-m_nFirstIndent) + L"\"";
		}
		RENDER_OOX_INT_ATTRIBUTE(m_nIndent, sIndent, L"w:left")
		if (sIndent.empty())
		{
			RENDER_OOX_INT_ATTRIBUTE(m_nIndentStart, sIndent, L"w:start")
		}

        if( !sIndent.empty() )
            spPr += L"<w:ind " + sIndent + L"/>";

		spPr += m_oTabs.RenderToOOX( oRenderParameter );

        if( !spPr.empty() )
            sResult += L"<w:pPr>" + spPr + L"</w:pPr>";

		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		
        std::wstring srPr = m_oCharProp.RenderToOOX(oNewParam);

        if( !srPr.empty() )
            sResult += L"<w:rPr>" + srPr + L"</w:rPr>";

		sResult += L"</w:lvl>"; 
	}
	return sResult;		
}

RtfListProperty::RtfListProperty()
{
	SetDefault();
}
void RtfListProperty::SetDefault()
{
	DEFAULT_PROPERTY	( m_nID )
	DEFAULT_PROPERTY	( m_nTemplateId )
	DEFAULT_PROPERTY	( m_nListSimple )
	DEFAULT_PROPERTY	( m_bListHybrid )
	DEFAULT_PROPERTY_DEF( m_sName, L"" )

	m_aArray.clear();
}
bool RtfListProperty::IsValid()
{
	return (PROP_DEF != m_nID) ;		//&& (L"" != m_sName);
}
std::wstring RtfListProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	sResult += L"\\list";

	RENDER_RTF_INT( m_nTemplateId, sResult, L"listtemplateid" )
	//RENDER_RTF_INT( m_nListSimple, sResult, L"listsimple" )
	//RENDER_RTF_BOOL( m_bListHybrid, sResult, L"listhybrid" )
	if( 1 == m_bListHybrid )
		sResult += L"\\listhybrid";
	else if( 1 == m_nListSimple )
        sResult += L"\\listsimple" + std::to_wstring(m_nListSimple);

	for (size_t i = 0; i < m_aArray.size(); i++ )
	{
		sResult +=  m_aArray[i].RenderToRtf( oRenderParameter );
	}
    sResult += L"{\\listname " + RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument ) + L";}";
	
	RENDER_RTF_INT( m_nID, sResult, L"listid" )
	return sResult;
}
std::wstring RtfListProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument* poRtfDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	OOXWriter* poOOXWriter = static_cast<OOXWriter*>(oRenderParameter.poWriter);
	
    std::wstring sResult;
	
    sResult += L"<w:abstractNum w:abstractNumId=\"" + std::to_wstring(m_nID) + L"\">";
	
	if( 1 == m_bListHybrid )
		sResult += L"<w:multiLevelType w:val=\"hybridMultilevel\"/>";
	else if( 1 == m_nListSimple )
		sResult += L"<w:multiLevelType w:val=\"singleLevel\"/>";
	else
		sResult += L"<w:multiLevelType w:val=\"multilevel\"/>";

    if( !m_sName.empty() )
        sResult += L"<w:name w:val=\"" + XmlUtils::EncodeXmlString( m_sName ) + L"\"/>";
	//if( false == m_nStyleName )
	//{
    //	sResult += L"<w:name w:val=\"" + XmlUtils::EncodeXmlString( m_sName ) + L"\"/>";
	//	if( PROP_DEF !=  m_nStyleID)
    //		sResult += L"<w:styleLink w:val=\"" + XmlUtils::EncodeXmlString( m_sName ) + L"\"/>";
	//}
	//else
    //	sResult += L"<w:numStyleLink w:val=\"" + XmlUtils::EncodeXmlString( m_sName) + L"\"/>";

	if( PROP_DEF != m_nTemplateId )
		sResult += L"<w:tmpl w:val=\"" + XmlUtils::ToString(m_nTemplateId, L"%x") + L"\"/>";

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	
	for (size_t i = 0; i < m_aArray.size(); i++ )
		sResult +=  m_aArray[i].RenderToOOX(oNewParam);

	sResult += L"</w:abstractNum>";
	return sResult;
}

RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevels()
{
}
RtfListOverrideProperty::ListOverrideLevels::ListOverrideLevels( const ListOverrideLevels& oOverLevel )
{
	(*this) = oOverLevel;
}
RtfListOverrideProperty::ListOverrideLevels& RtfListOverrideProperty::ListOverrideLevels::operator=( const ListOverrideLevels& oOverLevel )
{
	m_aOverrideLevels.clear();
	m_aOverrideLevels = oOverLevel.m_aOverrideLevels;
	return (*this);
}
void RtfListOverrideProperty::ListOverrideLevels::SetDefault()
{
	m_aOverrideLevels.clear();
}
std::wstring RtfListOverrideProperty::ListOverrideLevels::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	int nOverrideCount = (int)m_aOverrideLevels.size();
	for (int i = 0; i < nOverrideCount; i++ )
		if ( PROP_DEF == m_aOverrideLevels[i].m_nLevelIndex )
			nOverrideCount--;
	sResult += L"\\listoverridecount" + std::to_wstring( nOverrideCount );
	for (int i = 0; i < nOverrideCount; i++ )
	{
		if ( PROP_DEF != m_aOverrideLevels[i].m_nLevelIndex )
		{
			sResult += L"{\\lfolevel";

			if ( PROP_DEF != m_aOverrideLevels[i].m_nLevelIndex )
				sResult += L"\\listoverrideformat" + std::to_wstring( m_aOverrideLevels[i].m_nLevelIndex );
			if ( PROP_DEF != m_aOverrideLevels[i].m_nStart )
				sResult += L"\\listoverridestartat" + std::to_wstring( m_aOverrideLevels[i].m_nStart );

			sResult += m_aOverrideLevels[i].m_oLevel.RenderToRtf(oRenderParameter);
			sResult += L"}";
		}
	}
	return sResult;
}
std::wstring RtfListOverrideProperty::ListOverrideLevels::RenderToOOX(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	int index_prev = -1, index;
	for (size_t i = 0; i < m_aOverrideLevels.size(); i++ )
	{
		ListOverrideLevel& OverrideLevel = m_aOverrideLevels[i];
		if ( PROP_DEF != OverrideLevel.m_nLevelIndex )
		{
			index = OverrideLevel.m_nLevelIndex;
		}
		else
		{
			index = ++index_prev;
		}
		sResult += L"<w:lvlOverride w:ilvl=\"" + std::to_wstring(index) + L"\">";
		if ( PROP_DEF != OverrideLevel.m_nStart )
			sResult += L"<w:startOverride w:val=\"" + std::to_wstring(OverrideLevel.m_nStart) + L"\"/>";
		sResult += OverrideLevel.m_oLevel.RenderToOOX2(oRenderParameter, OverrideLevel.m_nLevelIndex);
		sResult += L"</w:lvlOverride>";

		index_prev = index;
	}
	return sResult;
}

RtfListOverrideProperty::RtfListOverrideProperty()
{
	SetDefault();
}
bool RtfListOverrideProperty::IsValid()
{
	return PROP_DEF != m_nListID && PROP_DEF != m_nIndex;
}
void RtfListOverrideProperty::SetDefault()
{
	DEFAULT_PROPERTY( m_nListID )
	DEFAULT_PROPERTY( m_nIndex )
	m_oOverrideLevels.SetDefault();
}
std::wstring RtfListOverrideProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( true == IsValid() )
	{
		sResult += L"\\listoverride";
		RENDER_RTF_INT( m_nListID, sResult, L"listid" )
		RENDER_RTF_INT( m_nIndex, sResult, L"ls" )
		sResult +=  m_oOverrideLevels.RenderToRtf( oRenderParameter );
	}
	return sResult;
}
std::wstring RtfListOverrideProperty::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( true == IsValid() )
	{
        sResult += L"<w:num w:numId=\"" + std::to_wstring(m_nIndex) + L"\">";
        sResult += L"<w:abstractNumId w:val=\"" + std::to_wstring(m_nListID) + L"\" />";
		sResult += m_oOverrideLevels.RenderToOOX( oRenderParameter );
		sResult += L"</w:num>";
	}
	return sResult;		
}

RtfStyle::RtfStyle()
{
	SetDefault();
}
bool RtfStyle::IsValid()
{
	return PROP_DEF != m_nID;
}
int RtfStyle::GetType()
{
	return TYPE_RTF_PROPERTY_STYLE;
}
void RtfStyle::SetDefaultRtf()
{
	SetDefault();
	m_eType = stParagraph;
	m_nID = 0;
}
void RtfStyle::SetDefaultOOX()
{
	SetDefault();
}
void RtfStyle::SetDefault()
{
	DEFAULT_PROPERTY_DEF( m_eType, st_none )
	DEFAULT_PROPERTY_DEF( m_sName, L"" )
	DEFAULT_PROPERTY_DEF( m_sID, L"" )
	DEFAULT_PROPERTY	( m_nID )
	DEFAULT_PROPERTY	( m_bAdditive )
	DEFAULT_PROPERTY	( m_nBasedOn )
	DEFAULT_PROPERTY	( m_nNext )
	DEFAULT_PROPERTY	( m_bHidden )
	DEFAULT_PROPERTY	( m_nLink )
	DEFAULT_PROPERTY	( m_bLocked )
	DEFAULT_PROPERTY	( m_bPersonal )
	DEFAULT_PROPERTY	( m_bCompose )
	DEFAULT_PROPERTY	( m_bReply )
	DEFAULT_PROPERTY	( m_nSemiHidden )
	DEFAULT_PROPERTY	( m_bQFormat )
	DEFAULT_PROPERTY	( m_nPriority )
	DEFAULT_PROPERTY	( m_bUnhiddenWhenUse )
}
void RtfStyle::Merge( RtfStylePtr oStyle )
{
	RtfStyle& oCurStyle = *oStyle;

	MERGE_PROPERTY_DEF	( m_eType, oCurStyle, st_none )
	MERGE_PROPERTY_DEF	( m_sName, oCurStyle, L"" )
	MERGE_PROPERTY		( m_nID, oCurStyle )
	MERGE_PROPERTY_DEF	( m_sID, oCurStyle, L"" )

	MERGE_PROPERTY		( m_bAdditive, oCurStyle )
	MERGE_PROPERTY		( m_nBasedOn, oCurStyle )
	MERGE_PROPERTY		( m_nNext, oCurStyle )
	MERGE_PROPERTY		( m_bHidden, oCurStyle )
	MERGE_PROPERTY		( m_nLink, oCurStyle )
	MERGE_PROPERTY		( m_bLocked, oCurStyle )
	MERGE_PROPERTY		( m_bPersonal, oCurStyle )
	MERGE_PROPERTY		( m_bCompose, oCurStyle )
	MERGE_PROPERTY		( m_bReply, oCurStyle )
	MERGE_PROPERTY		( m_nSemiHidden, oCurStyle )
	MERGE_PROPERTY		( m_bQFormat, oCurStyle )
	MERGE_PROPERTY		( m_nPriority, oCurStyle )
	MERGE_PROPERTY		( m_bUnhiddenWhenUse, oCurStyle )
}
bool RtfStyle::operator==( const RtfStyle& oProperty )
{
	return m_eType == oProperty.m_eType && m_nID == oProperty.m_nID && m_nBasedOn == oProperty.m_nBasedOn &&
		m_nNext == oProperty.m_nNext && m_bHidden == oProperty.m_bHidden && m_nLink == oProperty.m_nLink &&
		m_bLocked == oProperty.m_bLocked && m_bPersonal == oProperty.m_bPersonal && m_bCompose == oProperty.m_bCompose &&
		m_bReply == oProperty.m_bReply && m_nSemiHidden == oProperty.m_nSemiHidden && m_bQFormat == oProperty.m_bQFormat &&
		m_nPriority == oProperty.m_nPriority && m_bUnhiddenWhenUse == oProperty.m_bUnhiddenWhenUse;
}
std::wstring RtfStyle::RenderToRtfBegin( RenderParameter oRenderParameter )
{
	if ( false == IsValid() )
		return L"";

	std::wstring sResult;
	switch( m_eType )
	{
		case stParagraph :	sResult += L"{\\s"      + std::to_wstring( m_nID);                  break;
		case stCharacter :	sResult += L"{\\*\\cs"  + std::to_wstring( m_nID);                  break;
		case stSection :	sResult += L"{\\*\\ds"  + std::to_wstring( m_nID);                  break;
		case stTable :		sResult += L"{\\*\\ts"  + std::to_wstring( m_nID) + L"\\tsrowd";    break;
		default: break;
	}
	return sResult;
}
std::wstring RtfStyle::RenderToRtf(RenderParameter oRenderParameter)
{
	return L"";
}
std::wstring RtfStyle::RenderToOOX(RenderParameter oRenderParameter)
{
	return L"";
}
std::wstring RtfStyle::RenderToOOXBegin(RenderParameter oRenderParameter)
{
	RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
    std::wstring sResult;
    std::wstring sType;
	switch( m_eType )
	{
		case stParagraph :	sType = L"paragraph";	break;
		case stCharacter :	sType = L"character";	break;
		case stSection :	sType = L"numbering";	break;
		case stTable :		sType = L"table";		break;
		default:
			break;
	}
	if (m_sID.empty())
	{
		m_sID = L"Style_" + std::to_wstring(m_nID);
	}
	sResult += L"<w:style w:type=\"" + sType + L"\" w:styleId=\"" + m_sID + L"\"";
	
	if (m_nID == 0 && PROP_DEF == m_nBasedOn)
	{
		sResult += L" w:default=\"1\"";
	}

	sResult += L">";
	sResult += L"<w:name w:val=\"" + XmlUtils::EncodeXmlString( m_sName ) + L"\"/>";

	if( PROP_DEF != m_nBasedOn )
	{
		RtfStylePtr oBaseStyle;
		if( true == poDocument->m_oStyleTable.GetStyle( m_nBasedOn, oBaseStyle ) )
		{
			sResult += L"<w:basedOn w:val=\"Style_" + std::to_wstring(m_nBasedOn) + L"\"/>";
            //sResult += L"<w:basedOn w:val=\"" + XmlUtils::EncodeXmlString( oBaseStyle->m_sName ) + L"\"/>";
		}
	}

	//if( PROP_DEF != m_nNext )
    //	sResult += L"<w:next w:val=\"" + XmlUtils::EncodeXmlString( m_sName ) + L"\"/>", );//Todo
	//if( PROP_DEF != m_nLink )
    //	sResult += L"<w:link w:val=\"" + XmlUtils::EncodeXmlString( m_sName ) + L"\"/>", );//Todo
	if( 1 == m_bHidden )			sResult += L"<w:hidden/>";
	if( 1 == m_bLocked )			sResult += L"<w:locked/>";
	if( 1 == m_bPersonal )			sResult += L"<w:personal w:val=\"true\" />";
	if( 1 == m_bCompose )			sResult += L"<w:personalCompose w:val=\"true\" />";
	if( 1 == m_bReply )				sResult += L"<w:personalReply w:val=\"true\" />";
	if( 1 == m_nSemiHidden )		sResult += L"<w:semiHidden/>";
	if( 1 == m_bQFormat )			sResult += L"<w:qFormat/>";
	if( 1 == m_bUnhiddenWhenUse )	sResult += L"<w:unhideWhenUsed/>";
    if( PROP_DEF != m_nPriority )	sResult += L"<w:uiPriority w:val=\"" + std::to_wstring(m_nPriority) + L"\"/>";

	return sResult;
}
std::wstring RtfStyle::RenderToOOXEnd(RenderParameter oRenderParameter)
{
	//if( false == IsValid() )
	//	return false;
    std::wstring sResult;
	sResult += L"</w:style>";
	return sResult;
}
std::wstring RtfStyle::RenderToRtfEnd( RenderParameter oRenderParameter )
{
	if( false == IsValid() ) return L"";

    std::wstring sResult;
	RENDER_RTF_INT	( m_nBasedOn,	sResult, L"sbasedon" )
	RENDER_RTF_INT	( m_nNext,		sResult, L"snext" )
	RENDER_RTF_BOOL	( m_bHidden,	sResult, L"shidden" )
	RENDER_RTF_INT	( m_nLink,		sResult, L"slink" )
	RENDER_RTF_BOOL	( m_bLocked,	sResult, L"slocked" )
	RENDER_RTF_BOOL	( m_bPersonal,	sResult, L"spersonal" )
	RENDER_RTF_BOOL	( m_bCompose,	sResult, L"scompose" )
	RENDER_RTF_BOOL	( m_bReply,		sResult, L"sreply" )
	RENDER_RTF_BOOL	( m_nSemiHidden, sResult, L"ssemihidden" )
	RENDER_RTF_BOOL	( m_bQFormat,	sResult, L"sqformat" )
	RENDER_RTF_INT	( m_nPriority,	sResult, L"spriority" )
	RENDER_RTF_BOOL	( m_bUnhiddenWhenUse, sResult, L"sunhideused" )

	RtfCharProperty* props = NULL;
	sResult += L" " + RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, props, true) + L";}";
	return sResult;
}

std::wstring RtfCharStyle::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult = RenderToRtfBegin( oRenderParameter ) ;
	sResult +=  m_oCharProp.RenderToRtf(  oRenderParameter );
	sResult +=  RenderToRtfEnd( oRenderParameter );
	return sResult;
}
std::wstring RtfCharStyle::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult = RenderToOOXBegin( oRenderParameter ) ;
	
    std::wstring sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
    if( false == sCharProp.empty() )
	{
        sResult += L"<w:rPr>";
		sResult += sCharProp;
		sResult += L"</w:rPr>";
	}
	sResult += RenderToOOXEnd( oRenderParameter );
	return sResult;
}

std::wstring RtfParagraphStyle::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult = RenderToRtfBegin( oRenderParameter );

	sResult += m_oParProp.RenderToRtf(  oRenderParameter ) ;
	sResult += m_oCharProp.RenderToRtf(  oRenderParameter );
	sResult += RenderToRtfEnd( oRenderParameter );
	return sResult;
}
std::wstring RtfParagraphStyle::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult = RenderToOOXBegin( oRenderParameter );
	
    std::wstring sParProp = m_oParProp.RenderToOOX(oRenderParameter);
    if( false == sParProp.empty() )
	{
        sResult += L"<w:pPr>";
			sResult += sParProp;
		sResult += L"</w:pPr>";
	}
    std::wstring sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
    if( false == sCharProp.empty() )
	{
        sResult += L"<w:rPr>";
			sResult += sCharProp;
		sResult += L"</w:rPr>";
	}
	sResult += RenderToOOXEnd( oRenderParameter ) ;
	return sResult;
}

//std::wstring RtfTableStyleProperty::RenderToRtf(RenderParameter oRenderParameter)
//		{
//			std::wstring sResult;
//			if( m_oShading.IsValid() )
//				sResult +=  m_oShading.RenderToRtf( oRenderParameter ) );
//			RENDER_RTF_INT( m_nPaddingTop, sResult, L"tscellpaddt" )
//			RENDER_RTF_INT( m_nPaddingLeft, sResult, L"tscellpaddl" )
//			RENDER_RTF_INT( m_nPaddingRight, sResult, L"tscellpaddr" )
//			RENDER_RTF_INT( m_nPaddingBottom, sResult, L"tscellpaddb" )

//			RENDER_RTF_INT( m_ePaddingTopUnit, sResult, L"tscellpaddft" )
//			RENDER_RTF_INT( m_ePaddingLeftUnit, sResult, L"tscellpaddfl" )
//			RENDER_RTF_INT( m_ePaddingRightUnit, sResult, L"tscellpaddfr" )
//			RENDER_RTF_INT( m_ePaddingBottomUnit, sResult, L"tscellpaddfb" )
//
//			switch( m_eAlign )
//			{
//				case av_tsvertalt: sResult += L"\\tsvertalt";break;
//				case av_tsvertalc: sResult += L"\\tsvertalc";break;
//				case av_tsvertalb: sResult += L"\\tsvertalb";break;
//
//			}
//			RENDER_RTF_BOOL( m_bNoCellWrap, sResult, L"tsnowrap" )
//
//			if( true == m_oCellTopBorder.IsValid() )
//			{
//				sResult += L"\\tsbrdrt";
//				sResult +=  m_oCellTopBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellBottomBorder.IsValid() )
//			{
//				sResult += L"\\tsbrdrb";
//				sResult +=  m_oCellBottomBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellLeftBorder.IsValid() )
//			{
//				sResult += L"\\tsbrdrl";
//				sResult +=  m_oCellLeftBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellRightBorder.IsValid() )
//			{
//				sResult += L"\\tsbrdrr";
//				sResult +=  m_oCellRightBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellHorBorder.IsValid() )
//			{
//				sResult += L"\\tsbrdrh";
//				sResult +=  m_oCellHorBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			if( true == m_oCellVerBorder.IsValid() )
//			{
//				sResult += L"\\tsbrdrv";
//				sResult +=  m_oCellVerBorder.RenderToRtf(  oRenderParameter ) );
//			}
//			RENDER_RTF_INT( m_nRowBandNumber, sResult, L"tscbandsh" )
//			RENDER_RTF_INT( m_nCellBandNumber, sResult, L"tscbandsv" )
//
//			return sResult;
//		}
//std::wstring RtfTableStyleProperty::RenderToOOX(RenderParameter oRenderParameter)
//		{
//			std::wstring sResult;
//			return sResult;
//		}

std::wstring RtfTableStyle::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult = RenderToRtfBegin( oRenderParameter ) ;

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
std::wstring RtfTableStyle::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( RENDER_TO_OOX_PARAM_NESTED != oRenderParameter.nType )
	{
		sResult += RenderToOOXBegin( oRenderParameter ) ;

        std::wstring sTablProp = m_oTableProp.RenderToOOX(oRenderParameter);
        if( false == sTablProp.empty() )
		{
            sResult += L"<w:tblPr>";
				sResult += sTablProp;
			sResult += L"</w:tblPr>";
		}
        std::wstring sRowProp = m_oRowProp.RenderToOOX(oRenderParameter);
        if( false == sRowProp.empty() )
		{
            sResult += L"<w:trPr>";
				sResult += sRowProp;
			sResult += L"</w:trPr>";
		}
        std::wstring sCellProp = m_oCellProp.RenderToOOX(oRenderParameter);
        if( false == sCellProp.empty() )
		{
            sResult += L"<w:tcPr>";
				sResult += sCellProp;
			sResult += L"</w:tcPr>";
		}
        std::wstring sParProp = m_oParProp.RenderToOOX(oRenderParameter);
        if( false == sParProp.empty() )
		{
            sResult += L"<w:pPr>";
				sResult += sParProp;
			sResult += L"</w:pPr>";
		}
        std::wstring sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
        if( false == sCharProp.empty() )
		{
            sResult += L"<w:rPr>";
				sResult += sCharProp;
			sResult += L"</w:rPr>";
		}

		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_NESTED;

		if( NULL != m_oFirstRow )
		{
			oNewParam.sValue = L"firstRow";
			sResult += m_oFirstRow->RenderToOOX(oNewParam) ;
		}
		if( NULL != m_oLastRow )
		{
			oNewParam.sValue = L"lastRow";
			sResult += m_oLastRow->RenderToOOX(oNewParam);
		}
		if( NULL != m_oFirstCol )
		{
			oNewParam.sValue = L"firstCol";
			sResult += m_oFirstCol->RenderToOOX(oNewParam);
		}
		if( NULL != m_oLastCol )
		{
			oNewParam.sValue = L"lastCol";
			sResult += m_oLastCol->RenderToOOX(oNewParam);
		}
		if( NULL != m_oBandHorEven )
		{
			oNewParam.sValue = L"band2Horz";
			sResult += m_oBandHorEven->RenderToOOX(oNewParam) ;
		}
		if( NULL != m_oBandVerEven )
		{
			oNewParam.sValue = L"band2Vert";
			sResult += m_oBandVerEven->RenderToOOX(oNewParam);
		}
		if( NULL != m_oBandHorOdd )
		{
			oNewParam.sValue = L"band1Horz";
			sResult += m_oBandHorOdd->RenderToOOX(oNewParam);
		}
		if( NULL != m_oBandVerOdd )
		{
			oNewParam.sValue = L"band1Vert";
			sResult += m_oBandVerOdd->RenderToOOX(oNewParam);
		}
		if( NULL != m_oNWCell )
		{
			oNewParam.sValue = L"nwCell";
			sResult += m_oNWCell->RenderToOOX(oNewParam);
		}
		if( NULL != m_oNECell )
		{
			oNewParam.sValue = L"neCell";
			sResult += m_oNECell->RenderToOOX(oNewParam);
		}
		if( NULL != m_oSWCell )
		{
			oNewParam.sValue = L"swCell";
			sResult += m_oSWCell->RenderToOOX(oNewParam);
		}
		if( NULL != m_oSECell )
		{
			oNewParam.sValue = L"seCell";
			sResult += m_oSECell->RenderToOOX(oNewParam);
		}
		sResult += RenderToOOXEnd( oRenderParameter ) ;
	}
	else
	{
		sResult += L"<w:tblStylePr w:type=\"" + oRenderParameter.sValue + L"\">";
		
        std::wstring sTablProp = m_oTableProp.RenderToOOX(oRenderParameter);
        if( false == sTablProp.empty() )
		{
            sResult += L"<w:tblPr>";
			sResult += sTablProp;
			sResult += L"</w:tblPr>";
		}
        std::wstring sRowProp = m_oRowProp.RenderToOOX(oRenderParameter);
        if( false == sRowProp.empty() )
		{
            sResult += L"<w:trPr>";
			sResult += sRowProp;
			sResult += L"</w:trPr>";
		}
        std::wstring sCellProp = m_oCellProp.RenderToOOX(oRenderParameter);
        if( false == sCellProp.empty() )
		{
            sResult += L"<w:tcPr>";
			sResult += sCellProp;
			sResult += L"</w:tcPr>";
		}
        std::wstring sParProp = m_oParProp.RenderToOOX(oRenderParameter);
        if( false == sParProp.empty() )
		{
            sResult += L"<w:pPr>";
				sResult += sParProp;
			sResult += L"</w:pPr>";
		}
        std::wstring sCharProp = m_oCharProp.RenderToOOX(oRenderParameter);
        if( false == sCharProp.empty() )
		{
            sResult += L"<w:rPr>";
				sResult += sCharProp;
			sResult += L"</w:rPr>";
		}
		sResult += L"</w:tblStylePr>" ;
	}
	return sResult;
}

RtfTab::RtfTab()
{
	SetDefault();
}
void RtfTab::SetDefaultRtf()
{
	SetDefault();
}
void RtfTab::SetDefaultOOX()
{
	SetDefault();
}
void RtfTab::SetDefault()
{
	DEFAULT_PROPERTY	( m_nTab );
	DEFAULT_PROPERTY_DEF( m_eLeader,	tl_none );
	DEFAULT_PROPERTY_DEF( m_eKind,		tk_tql );
}
std::wstring RtfTab::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( PROP_DEF != m_nTab )
	{
		if( tk_tqbar == m_eKind )
		{
			switch( m_eLeader )
			{
				case tl_dot:	sResult += L"\\tldot";	break;
				case tl_mdot:	sResult += L"\\tlmdot";	break;
				case tl_hyph:	sResult += L"\\tlhyph";	break;
				case tl_ul:		sResult += L"\\tlul";	break;
				default:
					break;
			}
            sResult += L"\\tb" + std::to_wstring( m_nTab );
		}
		else
		{
			switch( m_eLeader )
			{
				case tl_dot:	sResult += L"\\tldot";	break;
				case tl_mdot:	sResult += L"\\tlmdot";	break;
				case tl_hyph:	sResult += L"\\tlhyph";	break;
				case tl_ul:		sResult += L"\\tlul";	break;
				default:
					break;
			}
			switch( m_eKind )
			{
				case tk_tqr:	sResult += L"\\tqr";	break;
				case tk_tqc:	sResult += L"\\tqc";	break;
				case tk_tqdec:	sResult += L"\\tqdec";	break;
				default:
					break;
			}
            sResult += L"\\tx" + std::to_wstring( m_nTab );
		}
	}
	return sResult;
}
std::wstring RtfTab::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;

    std::wstring sTab;
	switch( m_eLeader )
	{
        case tl_dot:	sTab += L" w:leader=\"dot\"";           break;
        case tl_mdot:	sTab += L" w:leader=\"middleDot\"";     break;
		case tl_hyph:	sTab += L" w:leader=\"hyphen\"";		break;
		case tl_ul:		sTab += L" w:leader=\"underscore\"";	break;
		default:
			break;
	}
	switch( m_eKind )
	{
		case tk_tql:	sTab += L" w:val=\"left\"";		break;
        case tk_tqr:	sTab += L" w:val=\"right\"";	break;
		case tk_tqc:	sTab += L" w:val=\"center\"";	break;
		case tk_tqdec:	sTab += L" w:val=\"decimal\"";	break;
		case tk_tqbar:	sTab += L" w:val=\"bar\"";		break;
		case tk_tqclear:sTab += L" w:val=\"clear\"";	break;
		default:
			break;
	}
	if( PROP_DEF != m_nTab )
        sTab += L" w:pos=\"" + std::to_wstring(m_nTab) + L"\"";
	
    if( false == sTab.empty() )
		sResult = L"<w:tab " + sTab + L"/>";
	return sResult;
}

RtfTabs::RtfTabs()
{
	SetDefault();
}
RtfTabs::RtfTabs( const RtfTabs& oTabs )
{
	SetDefault();
}
const RtfTabs& RtfTabs::operator=( const RtfTabs& oTabs )
{
	Merge( oTabs );
	return (*this);
}
void RtfTabs::Merge( const RtfTabs& oTabs )
{
	m_aTabs.clear();
	m_aTabs = oTabs.m_aTabs ;
}
void RtfTabs::SetDefault()
{
	m_aTabs.clear();
}
std::wstring RtfTabs::RenderToRtf(RenderParameter oRenderParameter)
{
	std::wstring sResult;
	for (size_t i = 0; i < (int)m_aTabs.size(); i++ )
	{
		sResult += m_aTabs[i].RenderToRtf( oRenderParameter );
	}
	return sResult;
}
std::wstring RtfTabs::RenderToOOX(RenderParameter oRenderParameter)
{
	std::wstring sTabs;
	for (size_t i = 0; i < (int)m_aTabs.size(); i++ )
	{
		sTabs += m_aTabs[i].RenderToOOX( oRenderParameter );
	}

	std::wstring sResult;
	if ( !sTabs.empty() )
		sResult += L"<w:tabs>" + sTabs + L"</w:tabs>";

	return sResult;
}

RtfFrame::RtfFrame()
{
	SetDefault();
}
void RtfFrame::SetDefaultRtf()
{
	SetDefault();
}
void RtfFrame::SetDefaultOOX()
{
	SetDefault();
}
void RtfFrame::SetDefault()
{
	DEFAULT_PROPERTY		( m_nWidth )
	DEFAULT_PROPERTY		( m_nHeight )
	DEFAULT_PROPERTY_DEF	( m_eHRef, hr_none )
	DEFAULT_PROPERTY_DEF	( m_eVRef, vr_none )
	DEFAULT_PROPERTY_DEF	( m_eHPos, hp_none )
	DEFAULT_PROPERTY_DEF	( m_eVPos, vp_none )
	DEFAULT_PROPERTY		( m_nHPos )
	DEFAULT_PROPERTY		( m_nVPos )
	DEFAULT_PROPERTY		( m_bLockAnchor )
	DEFAULT_PROPERTY_DEF	( m_eWrap, tw_none )
	DEFAULT_PROPERTY		( m_DropcapType )
	DEFAULT_PROPERTY		( m_DropcapLines )
	DEFAULT_PROPERTY		( m_nHorSpace )
	DEFAULT_PROPERTY		( m_nVerSpace )
	DEFAULT_PROPERTY		( m_nAllSpace )
}
void RtfFrame::Merge( RtfFrame& oFramePr )
{
	MERGE_PROPERTY		( m_nWidth, oFramePr )
	MERGE_PROPERTY		( m_nHeight, oFramePr )
	MERGE_PROPERTY_DEF	( m_eHRef, oFramePr, hr_none )
	MERGE_PROPERTY_DEF	( m_eVRef, oFramePr, vr_none )
	MERGE_PROPERTY_DEF	( m_eHPos, oFramePr, hp_none )
	MERGE_PROPERTY_DEF	( m_eVPos, oFramePr, vp_none )
	MERGE_PROPERTY		( m_bLockAnchor, oFramePr )
	MERGE_PROPERTY_DEF	( m_eWrap, oFramePr, tw_none )
	MERGE_PROPERTY		( m_DropcapType, oFramePr )
	MERGE_PROPERTY		( m_DropcapLines, oFramePr )
	MERGE_PROPERTY		( m_nHorSpace, oFramePr )
	MERGE_PROPERTY		( m_nVerSpace, oFramePr )
	MERGE_PROPERTY		( m_nAllSpace, oFramePr )
}
void RtfFrame::ApplyParagraphProp( RtfTableProperty& oProp )
{
	m_nHPos = oProp.m_nHPos;
	m_nVPos = oProp.m_nVPos;
	switch ( oProp.m_eHRef )
	{
		case RtfTableProperty::hr_phmrg: m_eHRef = hr_phmrg;break;
		case RtfTableProperty::hr_phpg: m_eHRef = hr_phpg;break;
		case RtfTableProperty::hr_phcol: m_eHRef = hr_phcol;break;
		default: break;
	}
	switch ( oProp.m_eVRef )
	{
		case RtfTableProperty::vr_pvmrg: m_eVRef = vr_pvmrg;break;
		case RtfTableProperty::vr_pvpg: m_eVRef = vr_pvpg;break;
		case RtfTableProperty::vr_pvpara: m_eVRef = vr_pvpara;break;
		default: break;
	}
	switch ( oProp.m_eHPos )
	{
		case RtfTableProperty::hp_posxc: m_eHPos = hp_posxc;break;
		case RtfTableProperty::hp_posxi: m_eHPos = hp_posxi;break;
		case RtfTableProperty::hp_posxo: m_eHPos = hp_posxo;break;
		case RtfTableProperty::hp_posxl: m_eHPos = hp_posxl;break;
		case RtfTableProperty::hp_posxr: m_eHPos = hp_posxr;break;
		default: break;
	}
	switch ( oProp.m_eVPos )
	{
		case RtfTableProperty::vp_posyc: m_eVPos = vp_posyc;break;
		case RtfTableProperty::vp_posyin: m_eVPos = vp_posyin;break;
		case RtfTableProperty::vp_posyout: m_eVPos = vp_posyout;break;
		case RtfTableProperty::vp_posyt: m_eVPos = vp_posyt;break;
		case RtfTableProperty::vp_posyb: m_eVPos = vp_posyb;break;
		default: break;
	}
}
std::wstring RtfFrame::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	RENDER_RTF_INT( m_nWidth, sResult, L"absw" )
	RENDER_RTF_INT( m_nHeight, sResult, L"absh" )
	switch ( m_eHRef )
	{
		case hr_phmrg:	sResult += L"\\phmrg";	break;
		case hr_phpg:	sResult += L"\\phpg";	break;
		case hr_phcol:	sResult += L"\\phcol";	break;
		default:
			break;
	}
	if( PROP_DEF != m_nHPos )
	{
		if( m_nHPos > 0 )
            sResult += L"\\posx" + std::to_wstring(m_nHPos);
		else
            sResult += L"\\posnegx" + std::to_wstring(m_nHPos);
	}
	switch ( m_eHPos )
	{
		case hp_posxc: sResult += L"\\posxc";	break;
		case hp_posxi: sResult += L"\\posxi";	break;
		case hp_posxo: sResult += L"\\posxo";	break;
		case hp_posxl: sResult += L"\\posxl";	break;
		case hp_posxr: sResult += L"\\posxr";	break;
		default:
			break;
	}

	switch ( m_eVRef )
	{	
		case vr_pvmrg:	sResult += L"\\pvmrg";	break;
		case vr_pvpg:	sResult += L"\\pvpg";	break;
		case vr_pvpara: sResult += L"\\pvpara";	break;
		default:
			break;
	}
	if( PROP_DEF != m_nVPos )
	{
		if( m_nVPos > 0 )
            sResult += L"\\posy" + std::to_wstring(m_nVPos);
		else 
            sResult += L"\\posnegy" + std::to_wstring(m_nVPos);
	}
	switch ( m_eVPos )
	{
        case vp_posyt:		sResult += L"\\posyt";      break;
        case vp_posyil:		sResult += L"\\posyil";     break;
        case vp_posyb:		sResult += L"\\posyb";      break;
        case vp_posyc:		sResult += L"\\posyc";      break;
        case vp_posyin:		sResult += L"\\posyin";     break;
		case vp_posyout:	sResult += L"\\posyout";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_bLockAnchor, sResult, L"abslock" )

	switch ( m_eWrap )
	{
		case tw_wrapdefault:	sResult += L"\\wrapdefault";	break;
        case tw_wraparound:		sResult += L"\\wraparound";     break;
        case tw_wraptight:		sResult += L"\\wraptight";      break;
		case tw_wrapthrough:	sResult += L"\\wrapthrough";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_DropcapType, sResult, L"dropcapt" )
	RENDER_RTF_INT( m_DropcapLines, sResult, L"dropcapli" )

	RENDER_RTF_INT( m_nHorSpace, sResult, L"dfrmtxtx" )
	RENDER_RTF_INT( m_nVerSpace, sResult, L"dfrmtxty" )

	return sResult;
}
std::wstring RtfFrame::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
    std::wstring sFrame;
	RENDER_OOX_INT_ATTRIBUTE( m_nWidth, sFrame, L"w:w" )
	if(PROP_DEF != m_nHeight)
	{
		if( m_nHeight > 0 )
		{
			RENDER_OOX_INT_ATTRIBUTE( m_nHeight, sFrame, L"w:h" )
			sFrame += L" w:hRule=\"atLeast\"";
		}
		else if( m_nHeight == 0 )
		{
			sFrame += L" w:hRule=\"auto\"";
		}
		else
		{
			m_nHeight *= -1;
			RENDER_OOX_INT_ATTRIBUTE( m_nHeight, sFrame, L"w:h" )
			sFrame += L" w:hRule=\"exact\"";
		}
	}
	
	switch ( m_eHRef )
	{
		case hr_phmrg:	sFrame += L" w:hAnchor=\"margin\"";	break;
		case hr_phpg:	sFrame += L" w:hAnchor=\"page\"";	break;
		case hr_phcol:	sFrame += L" w:hAnchor=\"text\"";	break;
		default:
			break;
	}
	RENDER_OOX_INT_ATTRIBUTE( m_nHPos, sFrame, L"w:x" )
	switch ( m_eHPos )
	{
		case hp_posxc: sFrame += L" w:xAlign=\"center\"";	break;
		case hp_posxi: sFrame += L" w:xAlign=\"inside\"";	break;
		case hp_posxo: sFrame += L" w:xAlign=\"outside\"";	break;
		case hp_posxl: sFrame += L" w:xAlign=\"left\"";		break;
		case hp_posxr: sFrame += L" w:xAlign=\"right\"";	break;
		default:
			break;
	}
	switch ( m_eVRef )
	{	
		case vr_pvmrg:	sFrame += L" w:vAnchor=\"margin\"";	break;
		case vr_pvpg:	sFrame += L" w:vAnchor=\"page\"";	break;
		case vr_pvpara: sFrame += L" w:vAnchor=\"text\"";	break;
		default:
			break;
	}
	RENDER_OOX_INT_ATTRIBUTE( m_nVPos, sFrame, L"w:y" )
	switch ( m_eVPos )
	{
		case vp_posyt:		sFrame += L" w:yAlign=\"top\"";		break;
		case vp_posyil:		sFrame += L" w:yAlign=\"inline\"";	break;
		case vp_posyb:		sFrame += L" w:yAlign=\"bottom\"";	break;
		case vp_posyc:		sFrame += L" w:yAlign=\"center\"";	break;
		case vp_posyin:		sFrame += L" w:yAlign=\"inside\"";	break;
		case vp_posyout:	sFrame += L" w:yAlign=\"outside\"";	break;
		default:
			break;
	}
	RENDER_OOX_BOOL_ATTRIBUTE( m_bLockAnchor, sFrame, L"w:anchorLock" )
	switch ( m_eWrap )
	{
		//case tw_wrapdefault: sFrame += L" w:wrap=\"auto\"");break;
		case tw_wraparound:		sFrame += L" w:wrap=\"around\"";	break;
		case tw_wraptight:		sFrame += L" w:wrap=\"tight\"";		break;
		case tw_wrapthrough:	sFrame += L" w:wrap=\"through\"";	break;
		default:
			break;
	}
	if( 1 == m_DropcapType )		sFrame += L" w:dropCap=\"drop\"";
	else if( 2 == m_DropcapType )	sFrame += L" w:dropCap=\"margin\"";
	
	RENDER_OOX_INT_ATTRIBUTE( m_DropcapLines, sFrame, L"w:lines" )

    if( PROP_DEF != m_nHorSpace )		sFrame += L" w:hSpace=\"" + std::to_wstring(m_nHorSpace) + L"\"";
    else if( PROP_DEF != m_nAllSpace )	sFrame += L" w:hSpace=\"" + std::to_wstring(m_nAllSpace) + L"\"";
	
    if( PROP_DEF != m_nVerSpace )		sFrame += L" w:vSpace=\"" + std::to_wstring(m_nVerSpace) + L"\"";
    else if( PROP_DEF != m_nAllSpace )	sFrame += L" w:vSpace=\"" + std::to_wstring(m_nAllSpace) + L"\"";

    if( !sFrame.empty() )
	{
		sResult = L"<w:framePr " + sFrame + L"/>";

		sResult += L"<w:widowControl w:val=\"0\"/>";
	}
	return sResult;
}

RtfParagraphProperty::RtfParagraphProperty()
{
	SetDefault();
}
bool RtfParagraphProperty::IsValid()
{
	return true;
}
void RtfParagraphProperty::SetDefaultRtf()
{
	SetDefault();

	m_nSpaceBefore = 0; // 137.rtf
	//4.1 Наряд_R7_M133.rtf
	m_nSpaceAfter = 0;
	m_nSpaceBetween = 240;
}
void RtfParagraphProperty::SetDefaultOOX()
{
	SetDefault();
}
void RtfParagraphProperty::SetDefault()
{
	DEFAULT_PROPERTY( m_bAutoHyphenation )
	DEFAULT_PROPERTY( m_bInTable )
	DEFAULT_PROPERTY( m_nItap )
	DEFAULT_PROPERTY( m_bKeep )
	DEFAULT_PROPERTY( m_bKeepNext )
	DEFAULT_PROPERTY( m_bPageBB )
	DEFAULT_PROPERTY( m_nOutlinelevel )
	DEFAULT_PROPERTY( m_nStyle )
	DEFAULT_PROPERTY( m_eAlign )

	m_eFontAlign = fa_none;

	DEFAULT_PROPERTY( m_nIndFirstLine )
	DEFAULT_PROPERTY( m_nIndLeft)
	DEFAULT_PROPERTY( m_nIndRight)
	DEFAULT_PROPERTY( m_nIndStart )
	DEFAULT_PROPERTY( m_nIndEnd )
	DEFAULT_PROPERTY( m_bIndRightAuto )
	DEFAULT_PROPERTY( m_bIndMirror )

	DEFAULT_PROPERTY( m_nSpaceBefore )

	DEFAULT_PROPERTY( m_nSpaceAfter )
	DEFAULT_PROPERTY( m_nSpaceBeforeAuto )
	DEFAULT_PROPERTY( m_nSpaceAfterAuto )
	DEFAULT_PROPERTY( m_nSpaceAfterLine )
	DEFAULT_PROPERTY( m_nSpaceBeforeLine )
	DEFAULT_PROPERTY( m_nSpaceBetween )
	DEFAULT_PROPERTY( m_nSpaceMultiLine )
	DEFAULT_PROPERTY( m_bContextualSpacing )

	DEFAULT_PROPERTY( m_bRtl )
	DEFAULT_PROPERTY( m_bNoWordWrap )
	DEFAULT_PROPERTY( m_bSnapToGrid )

	m_eTextBoxWrap = tbw_none;

	DEFAULT_PROPERTY( m_nListId )
	DEFAULT_PROPERTY( m_nListLevel )

	m_oShading.SetDefault();

	m_oBorderTop.SetDefault();
	m_oBorderLeft.SetDefault();
	m_oBorderBottom.SetDefault();
	m_oBorderRight.SetDefault();
	m_oBorderBox.SetDefault();
	m_oBorderBar.SetDefault();

	m_oFrame.SetDefault();
	DEFAULT_PROPERTY( m_bOverlap )
	m_eTextFollow = tf_none;
	m_oTabs.SetDefault();

	DEFAULT_PROPERTY( m_nTableStyle )
	DEFAULT_PROPERTY( m_bStyleFirstRow )
	DEFAULT_PROPERTY( m_bStyleLastRow )
	DEFAULT_PROPERTY( m_bStyleFirstCollumn )
	DEFAULT_PROPERTY( m_bStyleLastCollumn )
	DEFAULT_PROPERTY( m_bStyleOddRowBand )
	DEFAULT_PROPERTY( m_bStyleOddColBand )
	DEFAULT_PROPERTY( m_bStyleEvenRowBand )
	DEFAULT_PROPERTY( m_bStyleEvenColBand )
	DEFAULT_PROPERTY( m_bStyleNWCell )
	DEFAULT_PROPERTY( m_bStyleNECell )
	DEFAULT_PROPERTY( m_bStyleSWCell )
	DEFAULT_PROPERTY( m_bStyleSECell )

	DEFAULT_PROPERTY( m_nPrAuth)
	DEFAULT_PROPERTY( m_nPrDate)

	m_oCharProperty.SetDefault();

	m_bHidden			= false;
	m_bOldList			= false;
	m_pOldParagraphProp = RtfParagraphPropertyPtr();
}
void RtfParagraphProperty::Merge( RtfParagraphProperty& oParPr )
{
	MERGE_PROPERTY		( m_bAutoHyphenation,	oParPr )
	MERGE_PROPERTY		( m_bInTable,			oParPr )
	MERGE_PROPERTY		( m_nItap,				oParPr )
	MERGE_PROPERTY		( m_bKeep,				oParPr )
	MERGE_PROPERTY		( m_bKeepNext,			oParPr )
	MERGE_PROPERTY		( m_bPageBB,			oParPr )
	MERGE_PROPERTY		( m_nOutlinelevel,		oParPr )
	MERGE_PROPERTY		( m_nStyle,				oParPr )
	MERGE_PROPERTY		( m_eAlign,				oParPr )
	MERGE_PROPERTY_DEF	( m_eFontAlign,			oParPr, fa_none )
	MERGE_PROPERTY		( m_nIndFirstLine,		oParPr )
	MERGE_PROPERTY		( m_nIndLeft,			oParPr )
	MERGE_PROPERTY		( m_nIndRight,			oParPr )
	MERGE_PROPERTY		( m_nIndStart,			oParPr )
	MERGE_PROPERTY		( m_nIndEnd,			oParPr )
	MERGE_PROPERTY		( m_bIndRightAuto,		oParPr )
	MERGE_PROPERTY		( m_bIndMirror,			oParPr )

	MERGE_PROPERTY		( m_nSpaceBefore,		oParPr )
	MERGE_PROPERTY		( m_nSpaceAfter,		oParPr )
	MERGE_PROPERTY		( m_nSpaceBeforeAuto,	oParPr )
	MERGE_PROPERTY		( m_nSpaceAfterAuto,	oParPr )
	MERGE_PROPERTY		( m_nSpaceBeforeLine,	oParPr )
	MERGE_PROPERTY		( m_nSpaceAfterLine,	oParPr )
	MERGE_PROPERTY		( m_nSpaceBetween,		oParPr )
	MERGE_PROPERTY		( m_nSpaceMultiLine,	oParPr )
	MERGE_PROPERTY		( m_bSnapToGrid,		oParPr )
	MERGE_PROPERTY		( m_bContextualSpacing, oParPr )

	MERGE_PROPERTY		( m_bRtl,			oParPr )
	MERGE_PROPERTY		( m_bNoWordWrap,	oParPr )
	MERGE_PROPERTY_DEF	( m_eTextBoxWrap,	oParPr, tbw_none )

	MERGE_PROPERTY		( m_nListId,	oParPr )
	MERGE_PROPERTY		( m_nListLevel, oParPr )

	m_oShading.Merge		( oParPr.m_oShading		);
	m_oBorderTop.Merge		( oParPr.m_oBorderTop	);
	m_oBorderLeft.Merge		( oParPr.m_oBorderLeft	);
	m_oBorderBottom.Merge	( oParPr.m_oBorderBottom );
	m_oBorderRight.Merge	( oParPr.m_oBorderRight );
	m_oBorderBox.Merge		( oParPr.m_oBorderBox	);
	m_oBorderBar.Merge		( oParPr.m_oBorderBar	);
	m_oFrame.Merge			( oParPr.m_oFrame );
	m_oTabs.Merge			( oParPr.m_oTabs );

	MERGE_PROPERTY			( m_bOverlap,		oParPr )
	MERGE_PROPERTY_DEF		( m_eTextFollow,	oParPr, tf_none )

	MERGE_PROPERTY( m_nTableStyle,			oParPr )
	MERGE_PROPERTY( m_bStyleFirstRow,		oParPr )
	MERGE_PROPERTY( m_bStyleLastRow,		oParPr )
	MERGE_PROPERTY( m_bStyleFirstCollumn,	oParPr )
	MERGE_PROPERTY( m_bStyleLastCollumn,	oParPr )
	MERGE_PROPERTY( m_bStyleOddRowBand,		oParPr )
	MERGE_PROPERTY( m_bStyleEvenRowBand,	oParPr )
	MERGE_PROPERTY( m_bStyleOddColBand,		oParPr )
	MERGE_PROPERTY( m_bStyleEvenColBand,	oParPr )
	MERGE_PROPERTY( m_bStyleNWCell,			oParPr )
	MERGE_PROPERTY( m_bStyleNECell,			oParPr )
	MERGE_PROPERTY( m_bStyleSWCell,			oParPr )
	MERGE_PROPERTY( m_bStyleSECell,			oParPr )

	MERGE_PROPERTY( m_nPrAuth,				oParPr )
	MERGE_PROPERTY( m_nPrDate,				oParPr )

	//m_oCharProperty.Merge( oParPr.m_oCharProperty );
}
std::wstring RtfParagraphProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	RENDER_RTF_INT	( m_nStyle				, sResult, L"s" ); //test
	RENDER_RTF_BOOL	( m_bAutoHyphenation	, sResult, L"hyphpar" );
	RENDER_RTF_BOOL	( m_bInTable			, sResult, L"intbl" );
	
	if( PROP_DEF != m_nItap && 1 != m_nItap )
        sResult += L"\\itap" + std::to_wstring(m_nItap );
	
	RENDER_RTF_BOOL	( m_bKeep			, sResult, L"keep" );
	RENDER_RTF_BOOL	( m_bKeepNext		, sResult, L"keepn" );
	RENDER_RTF_INT	( m_nOutlinelevel	, sResult, L"outlinelevel" );
	RENDER_RTF_BOOL	( m_bPageBB			, sResult, L"pagebb" );


	switch(m_eAlign)
	{
		case  pa_qc:	sResult += L"\\qc";		break;  
		case  pa_qj:	sResult += L"\\qj";		break;  
		case  pa_ql:	sResult += L"\\ql";		break;  
		case  pa_qr:	sResult += L"\\qr";		break;  
		case  pa_qd:	sResult += L"\\qd";		break;  
		case  pa_qk0:	sResult += L"\\qk0";	break;  
		case  pa_qk10:	sResult += L"\\qk10";	break;  
		case  pa_qk20:	sResult += L"\\qk20";	break;  
		default:
			break;
	}
	switch( m_eFontAlign )
	{
		case fa_faauto:		sResult += L"\\faauto";		break;
		case fa_fahang:		sResult += L"\\fahang";		break;
		case fa_facenter:	sResult += L"\\facenter";	break;
		case fa_faroman:	sResult += L"\\faroman";	break;
		case fa_favar:		sResult += L"\\favar";		break;
		case fa_fafixed:	sResult += L"\\fafixed";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_nIndFirstLine		, sResult, L"fi" );
	RENDER_RTF_INT( m_nIndLeft			, sResult, L"li" );
	RENDER_RTF_INT( m_nIndStart			, sResult, L"lin" );
	RENDER_RTF_INT( m_nIndRight			, sResult, L"ri" );
	RENDER_RTF_INT( m_nIndEnd			, sResult, L"rin" );
	RENDER_RTF_BOOL( m_bIndRightAuto	, sResult, L"adjustright" );
	RENDER_RTF_BOOL( m_bIndMirror		, sResult, L"indmirror" );
	RENDER_RTF_INT( m_nSpaceBefore		, sResult, L"sb" );
	RENDER_RTF_INT( m_nSpaceAfter		, sResult, L"sa" );
	RENDER_RTF_INT( m_nSpaceBeforeAuto	, sResult, L"sbauto" );
	RENDER_RTF_INT( m_nSpaceAfterAuto	, sResult, L"saauto" );
	RENDER_RTF_INT( m_nSpaceBeforeLine	, sResult, L"lisb" );
	RENDER_RTF_INT( m_nSpaceAfterLine	, sResult, L"lisa" );
	RENDER_RTF_INT( m_nSpaceBetween		, sResult, L"sl" );
	RENDER_RTF_INT( m_nSpaceMultiLine	, sResult, L"slmult" );
	RENDER_RTF_BOOL( m_bContextualSpacing, sResult, L"contextualspace" );
	
	if( 0 == m_bRtl )				sResult += L"\\ltrpar";
	else if( PROP_DEF != m_bRtl )	sResult += L"\\rtlpar";
	
	RENDER_RTF_BOOL( m_bNoWordWrap, sResult, L"nowwrap" );
	
	if( 0 == m_bSnapToGrid )				sResult += L"\\nosnaplinegrid";
	else if(  PROP_DEF != m_bSnapToGrid  )	sResult += L"\\nosnaplinegrid0";
	switch ( m_eTextBoxWrap )
	{
		case tbw_txbxtwno:			sResult += L"\\txbxtwno";		break;
		case tbw_txbxtwalways:		sResult += L"\\txbxtwalways";	break;
		case tbw_txbxtwfirstlast:	sResult += L"\\txbxtwfirstlast";	break;
		case tbw_txbxtwfirst:		sResult += L"\\txbxtwfirst";		break;
		case tbw_txbxtwlast:		sResult += L"\\txbxtwlast";		break;
		default:
			break;
	}
	if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
	{
        sResult += L"\\ls" + std::to_wstring( m_nListId );
        sResult += L"\\ilvl" + std::to_wstring(m_nListLevel );
	}

	if( true == m_oShading.IsValid() )
		sResult +=  m_oShading.RenderToRtf( oRenderParameter );

	bool border_sides = false;
	if( true == m_oBorderTop.IsValid() )
	{
		sResult += L"\\brdrt";
		sResult +=  m_oBorderTop.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderLeft.IsValid() )
	{
		sResult += L"\\brdrl";
		sResult +=  m_oBorderLeft.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		sResult += L"\\brdrb";
		sResult +=  m_oBorderBottom.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderRight.IsValid() )
	{
		sResult += L"\\brdrr";
		sResult +=  m_oBorderRight.RenderToRtf( oRenderParameter );
		border_sides = true;
	}
	if( true == m_oBorderBar.IsValid() )
	{
		sResult += L"\\brdrbar";
		sResult +=  m_oBorderBar.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderBox.IsValid() && !border_sides)
	{
		sResult += L"\\box";
		sResult +=  m_oBorderBox.RenderToRtf( oRenderParameter );
	}

	if( true == m_oFrame.IsValid() )
		sResult +=  m_oFrame.RenderToRtf( oRenderParameter );

	RENDER_RTF_BOOL( m_bOverlap, sResult, L"absnoovrlp" );

	switch ( m_eTextFollow )
	{
		case tf_frmtxlrtb:	sResult += L"\\frmtxlrtb";	break;
		case tf_frmtxtbrl:	sResult += L"\\frmtxtbrl";	break;
		case tf_frmtxbtlr:	sResult += L"\\frmtxbtlr";	break;
		case tf_frmtxlrtbv: sResult += L"\\frmtxlrtbv";	break;
		case tf_frmtxtbrlv: sResult += L"\\frmtxtbrlv";	break;
		default:
			break;
	}
	if( true == m_oTabs.IsValid() )
		sResult +=  m_oTabs.RenderToRtf( oRenderParameter );


	//RENDER_RTF_INT( m_nTableStyle, sResult, L"yts" );
	//RENDER_RTF_BOOL( m_bStyleFirstRow, sResult, L"tscfirstrow" );
	//RENDER_RTF_BOOL( m_bStyleLastRow, sResult, L"tsclastrow" );
	//RENDER_RTF_BOOL( m_bStyleFirstCollumn, sResult, L"tscfirstcol" );
	//RENDER_RTF_BOOL( m_bStyleLastCollumn, sResult, L"tsclastcol" );
	//RENDER_RTF_BOOL( m_bStyleOddRowBand, sResult, L"tscbandhorzodd" );
	//RENDER_RTF_BOOL( m_bStyleEvenRowBand, sResult, L"tscbandhorzeven" );
	//RENDER_RTF_BOOL( m_bStyleOddColBand, sResult, L"tscbandvertodd" );
	//RENDER_RTF_BOOL( m_bStyleEvenColBand, sResult, L"tscbandverteven" );
	//RENDER_RTF_BOOL( m_bStyleNWCell, sResult, L"tscnwcell" );
	//RENDER_RTF_BOOL( m_bStyleNECell, sResult, L"tscnecell" );
	//RENDER_RTF_BOOL( m_bStyleSWCell, sResult, L"tscswcell" );
	//RENDER_RTF_BOOL( m_bStyleSECell, sResult, L"tscsecell" );


	//дописываем текст списка (для старых reader например  wordPad)
	if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
	{
		RtfListProperty oListProperty;
		RtfDocument* poRtfDocument = static_cast<RtfDocument*>(  oRenderParameter.poDocument );
		RtfListOverrideProperty oListOverrideProperty;
		//ищем по override table
		if( true == poRtfDocument->m_oListOverrideTable.GetList( m_nListId, oListOverrideProperty ) )
		{
			//Ищем по List Table
			if( true == poRtfDocument->m_oListTable.GetList( oListOverrideProperty.m_nListID, oListProperty) )
			{
				//дописываем свойства параграфа firstIndent Indent
				RtfListLevelProperty poLevelProp ;
				if( true == oListProperty.GetItem( poLevelProp , m_nListLevel ) )
				{
					sResult += L"{\\listtext\\pard\\plain";
					sResult +=  poLevelProp.m_oCharProp.RenderToRtf( oRenderParameter );
					//пишем текст 
                    std::wstring strLevelProp = poLevelProp.GenerateListText();

                    RtfCharProperty* pCharProperty = NULL;
                    sResult +=   RtfChar::renderRtfText( strLevelProp, oRenderParameter.poDocument, pCharProperty );

					//или картинку
					if( PROP_DEF != poLevelProp.m_nPictureIndex )
					{
						int nIndex = poLevelProp.m_nPictureIndex;

						if( 0 < nIndex && nIndex < poRtfDocument->m_oListTable.m_aPictureList.GetCount() )
							sResult +=  poRtfDocument->m_oListTable.m_aPictureList[nIndex]->RenderToRtf( oRenderParameter );
					}
					//ставим tab
					if( PROP_DEF != poLevelProp.m_nFollow )
					{
						switch( poLevelProp.m_nFollow )
						{
							case 0: sResult += L"\\tab";	break; // tab
							case 1: sResult += L" ";		break; // space
							case 2: 
								break; // nothing
						}
					}
					sResult += L"}";
				}
			}
		}
	}

	m_oCharProperty.RenderToRtf(oRenderParameter);

	if (m_pOldParagraphProp)
	{
		RENDER_RTF_INT( m_nPrAuth,		sResult, L"prauth" )
		RENDER_RTF_INT( m_nPrDate,		sResult, L"prdate" )

        sResult += L"{\\*\\oldpprops\\pard";
		sResult += m_pOldParagraphProp->RenderToRtf(oRenderParameter);
		sResult += L"}";
	}

	return sResult;
}
std::wstring RtfParagraphProperty::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
    std::wstring	sResult;
	RtfTabs			styleTabs;

	if( PROP_DEF != m_nStyle )
	{
		RtfStylePtr oCurStyle;
		if( true == poRtfDocument->m_oStyleTable.GetStyle( m_nStyle, oCurStyle ) )
		{
			RtfParagraphStyle* oCurParaStyle = dynamic_cast<RtfParagraphStyle*>(oCurStyle.get());

			if (oCurParaStyle)
			{
				if (oCurParaStyle->m_oParProp.m_nListId != PROP_DEF && 
					oCurParaStyle->m_oParProp.m_nListLevel != PROP_DEF)
				{
					if( PROP_DEF == m_nListId)		m_nListId		= 0;
					if ( PROP_DEF == m_nListLevel )	m_nListLevel	= 0;
				}
				styleTabs = oCurParaStyle->m_oParProp.m_oTabs;

				if (oCurParaStyle->m_oParProp.m_nSpaceBeforeAuto == 1 && m_nSpaceBefore != PROP_DEF)
					m_nSpaceBeforeAuto = 0;
				
				if (oCurParaStyle->m_oParProp.m_nSpaceAfterAuto == 1 && m_nSpaceAfter != PROP_DEF)
					m_nSpaceAfterAuto = 0;

				m_bHidden = (oCurParaStyle->m_bHidden == 1);
			}
			sResult += L"<w:pStyle w:val=\"Style_" + std::to_wstring(m_nStyle) + L"\"/>";
		}
	}

	if		( 0 == m_bAutoHyphenation )	sResult += L"<w:suppressAutoHyphens/>";
	else if	( 1 == m_bAutoHyphenation )	sResult += L"<w:suppressAutoHyphens w:val=\"false\"/>";

	RENDER_OOX_BOOL	( m_bKeep			, sResult, L"w:keepLines" );
	RENDER_OOX_BOOL	( m_bKeepNext		, sResult, L"w:keepNext" );
	RENDER_OOX_INT	( m_nOutlinelevel	, sResult, L"w:outlineLvl" );
	RENDER_OOX_BOOL	( m_bPageBB			, sResult, L"w:pageBreakBefore" );

	if (PROP_DEF == m_nStyle && PROP_DEF == m_eAlign)
		m_eAlign = pa_ql;

	if (m_bRtl == 1)
	{
		if (m_eAlign == pa_ql)		m_eAlign = pa_qr;
		else if (m_eAlign == pa_qr) m_eAlign = pa_ql;
	}

	switch(m_eAlign)
	{
		case pa_qc:		sResult += L"<w:jc w:val=\"center\"/>" ;		break;
		case pa_qj:		sResult += L"<w:jc w:val=\"both\"/>";			break;
		case pa_ql:		sResult += L"<w:jc w:val=\"left\"/>";			break;
		case pa_qr:		sResult += L"<w:jc w:val=\"right\"/>";			break;
		case pa_qd:		sResult += L"<w:jc w:val=\"distribute\"/>";		break;
		case pa_qk0:	sResult += L"<w:jc w:val=\"lowKashida\"/>";		break;
		case pa_qk10:	sResult += L"<w:jc w:val=\"mediumKashida\"/>";	break;
		case pa_qk20:	sResult += L"<w:jc w:val=\"highKashida\"/>";	break;
		default:
			break;
	}
	switch( m_eFontAlign )
	{
		case fa_faauto:		sResult += L"<w:textAlignment w:val=\"auto\"/>";	break;
		case fa_fahang:		sResult += L"<w:textAlignment w:val=\"top\"/>";		break;
		case fa_facenter:	sResult += L"<w:textAlignment w:val=\"center\"/>";	break;
		case fa_faroman:	sResult += L"<w:textAlignment w:val=\"baseline\"/>";break;
		case fa_favar:		sResult += L"<w:textAlignment w:val=\"bottom\"/>";	break;
		case fa_fafixed:	sResult += L"<w:textAlignment w:val=\"bottom\"/>";	break;
		default:
			break;
	}
    std::wstring sIndent;

	if ( PROP_DEF != m_nIndRight )
	{
		if (PROP_DEF == m_nIndLeft)
			m_nIndLeft = 0;
		sIndent += L" w:right=\"" + std::to_wstring(m_nIndRight) + L"\"";
	}
	RENDER_OOX_INT_ATTRIBUTE	( m_nIndLeft,	sIndent, L"w:left" );
	RENDER_OOX_INT_ATTRIBUTE	(m_nIndStart,	sIndent, L"w:start")

	if( PROP_DEF != m_nIndFirstLine )
	{
        if( m_nIndFirstLine >= 0 )	sIndent += L" w:firstLine=\"" + std::to_wstring(m_nIndFirstLine) + L"\"";
        else						sIndent += L" w:hanging=\"" + std::to_wstring(-m_nIndFirstLine) + L"\"";
	}
	else if (m_bOldList && PROP_DEF != m_nIndLeft)
        sIndent += L" w:firstLine=\"0\"";	

    if( !sIndent.empty() )
	{
        sResult += L"<w:ind " + sIndent + L"/>";
	}

	RENDER_OOX_BOOL( m_bIndRightAuto, sResult, L"w:adjustRightInd" );
	RENDER_OOX_BOOL( m_bIndMirror, sResult, L"w:mirrorIndents" );

    std::wstring sSpacing;

	if (m_bKeepNext == 1)//nabidnik.rtf
	{
		if( PROP_DEF == m_nSpaceAfter )		m_nSpaceAfter = 0;
		if( PROP_DEF == m_nSpaceBefore )	m_nSpaceBefore = 0;
	}

	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBefore,		sSpacing, L"w:before" );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfter,		sSpacing, L"w:after" );

	if (m_nSpaceBeforeAuto == 1)
	{
		sSpacing += L" w:beforeAutospacing=\"on\"";
	}
	if (m_nSpaceAfterAuto == 1)
	{
		sSpacing += L" w:afterAutospacing=\"on\"";
	}
	//RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBeforeAuto,	sSpacing, L"w:beforeAutospacing" );
	//RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfterAuto,	sSpacing, L"w:afterAutospacing" );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceBeforeLine,	sSpacing, L"w:beforeLines" );
	RENDER_OOX_INT_ATTRIBUTE( m_nSpaceAfterLine,	sSpacing, L"w:afterLines" );

	if( PROP_DEF != m_nSpaceBetween && 0 != m_nSpaceBetween )
	{
		if( m_nSpaceBetween > 0 )
            sSpacing += L" w:line=\"" + std::to_wstring(m_nSpaceBetween) + L"\"";
		else
            sSpacing += L" w:line=\"" + std::to_wstring(-m_nSpaceBetween) + L"\"";
		if( 0 == m_nSpaceMultiLine )
		{
			if( m_nSpaceBetween > 0 )	sSpacing += L" w:lineRule=\"atLeast\"";
			else						sSpacing += L" w:lineRule=\"exact\"";
		}
		else sSpacing += L" w:lineRule=\"auto\"";
	}
	//else
    //	sSpacing += L" w:line=\"240\""); //по умолчанию - единичный
    if( !sSpacing.empty() )
	{
		sResult += L"<w:spacing " + sSpacing + L"/>";
	}

	RENDER_OOX_BOOL( m_bSnapToGrid, sResult, L"w:snapToGrid" );
	RENDER_OOX_BOOL( m_bContextualSpacing, sResult, L"w:contextualSpacing" );
	RENDER_OOX_BOOL( m_bRtl, sResult, L"w:bidi" );
	
	if( 0 == m_bNoWordWrap )				sResult += L"<w:wordWrap w:val=\"false\"/>";
	else if(  PROP_DEF != m_bNoWordWrap  )	sResult += L"<w:wordWrap w:val=\"true\"/>";

	switch ( m_eTextBoxWrap )
	{
		case tbw_txbxtwno:			sResult += L"<w:textboxTightWrap w:val=\"none\"/>";				break;
		case tbw_txbxtwalways:		sResult += L"<w:textboxTightWrap w:val=\"allLines\"/>";			break;
		case tbw_txbxtwfirstlast:	sResult += L"<w:textboxTightWrap w:val=\"firstAndLastLine\"/>";	break;
		case tbw_txbxtwfirst:		sResult += L"<w:textboxTightWrap w:val=\"firstLineOnly\"/>";	break;
		case tbw_txbxtwlast:		sResult += L"<w:textboxTightWrap w:val=\"lastLineOnly\"/>";		break;
		default:
			break;
	}
	if( PROP_DEF != m_nListId && PROP_DEF != m_nListLevel )
	{
        sResult += L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(m_nListLevel) + L"\"/><w:numId w:val=\"" + std::to_wstring(m_nListId) + L"\"/></w:numPr>";
	}

	if( true == m_oShading.IsValid() )
		sResult +=  m_oShading.RenderToOOX(oRenderParameter);

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE;
    std::wstring sBorder;
	if( true == m_oBorderBox.IsValid() )
	{
        std::wstring sBorderContent = m_oBorderBox.RenderToOOX(oNewParam);
        sBorder += L"<w:left ";		sBorder += sBorderContent;	sBorder += L" />";
        sBorder += L"<w:top ";		sBorder += sBorderContent;	sBorder += L" />";
        sBorder += L"<w:right ";	sBorder += sBorderContent;	sBorder += L" />";
        sBorder += L"<w:bottom ";	sBorder += sBorderContent;	sBorder += L" />";
	}
	else
	{
		if( true == m_oBorderTop.IsValid() )
		{
			sBorder += L"<w:top ";		sBorder += m_oBorderTop.RenderToOOX(oNewParam);		sBorder += L" />";
		}
		if( true == m_oBorderLeft.IsValid() )
		{
			sBorder += L"<w:left ";		sBorder += m_oBorderLeft.RenderToOOX(oNewParam);	sBorder += L" />";
		}
		if( true == m_oBorderBottom.IsValid() )
		{
			sBorder += L"<w:bottom ";	sBorder += m_oBorderBottom.RenderToOOX(oNewParam);	sBorder += L" />";
		}
		if( true == m_oBorderRight.IsValid() )
		{
			sBorder += L"<w:right ";		sBorder += m_oBorderRight.RenderToOOX(oNewParam);	sBorder += L" />";
		}
	}

	if( true == m_oBorderBar.IsValid() )
	{
        sBorder += L"<w:between ";
		sBorder += m_oBorderBar.RenderToOOX(oNewParam);
		sBorder += L"/>";
	}
    if( false == sBorder.empty() )
	{
        sResult += L"<w:pBdr>";
		sResult += sBorder;
		sResult += L"</w:pBdr>";
	}

	if( true == m_oFrame.IsValid() )
		sResult +=  m_oFrame.RenderToOOX( oRenderParameter );

	RENDER_OOX_BOOL( m_bOverlap, sResult, L"w:suppressOverlap" );

	switch ( m_eTextFollow )
	{
		case tf_frmtxlrtb:	sResult += L"<w:textFlow w:val=\"lrtb\"/>";	break;
		case tf_frmtxtbrl:	sResult += L"<w:textFlow w:val=\"tbrl\"/>";	break;
		case tf_frmtxbtlr:	sResult += L"<w:textFlow w:val=\"btlr\"/>";	break;
		case tf_frmtxlrtbv: sResult += L"<w:textFlow w:val=\"lrtbV\"/>";break;
		case tf_frmtxtbrlv: sResult += L"<w:textFlow w:val=\"tbrlV\"/>";break;
		default:
			break;
	}

	if (styleTabs.m_aTabs.size() != m_oTabs.m_aTabs.size() && m_oTabs.m_aTabs.size() > 0)
	{//зачистка от стилевых табов (позиции по возрастанию)
		for (int i = (int)styleTabs.m_aTabs.size() - 1; i >= 0; i--)
		{
			bool bPreset = false;
			for (size_t j = 0; j < m_oTabs.m_aTabs.size(); j++)
			{
				if (m_oTabs.m_aTabs[j].m_nTab == styleTabs.m_aTabs[i].m_nTab)
				{
					bPreset = true;
					break;
				}
			}
			if (!bPreset)
			{
				RtfTab tab_new = styleTabs.m_aTabs[i];

				tab_new.m_eKind = RtfTab::tk_tqclear;

				m_oTabs.m_aTabs.insert(m_oTabs.m_aTabs.begin(), tab_new);
			}
		}
	}

	sResult += m_oTabs.RenderToOOX( oRenderParameter );

    std::wstring sCharProp = m_oCharProperty.RenderToOOX( oRenderParameter );
	
    if( !sCharProp.empty() )
	{
        sResult += L"<w:rPr>";
			sResult += sCharProp;
		sResult += L"</w:rPr>";
	}
	if (m_pOldParagraphProp)
	{
        std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nPrAuth);
        std::wstring sDate(RtfUtility::convertDateTime(m_nPrDate).c_str());

		RenderParameter oRenderParameterNew = oRenderParameter;
		oRenderParameterNew.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		
		sResult += L"<w:pPrChange w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			sResult += L"<w:pPr>";
				sResult += m_pOldParagraphProp->RenderToOOX(oRenderParameterNew);
			sResult += L"</w:pPr>";
		sResult += L"</w:pPrChange>";
	}
	return sResult;
}

RtfCellProperty::RtfCellProperty()
{
	SetDefault();
}
void RtfCellProperty::SetDefaultRtf()
{
	SetDefault();
}
void RtfCellProperty::SetDefaultOOX()
{
	SetDefault();
	m_nSpan = 1;
}
void RtfCellProperty::SetDefault()
{
	DEFAULT_PROPERTY( m_bMergeFirst )
	DEFAULT_PROPERTY( m_bMerge )
	DEFAULT_PROPERTY( m_bMergeFirstVertical )
	DEFAULT_PROPERTY( m_bMergeVertical )
	DEFAULT_PROPERTY( m_bFitText )
	DEFAULT_PROPERTY( m_bNoWrap )

	DEFAULT_PROPERTY( m_nPaddingLeft )
	DEFAULT_PROPERTY( m_ePaddingLeftUnit )
	DEFAULT_PROPERTY( m_nPaddingRight )
	DEFAULT_PROPERTY( m_ePaddingRightUnit )
	DEFAULT_PROPERTY( m_nPaddingTop )
	DEFAULT_PROPERTY( m_ePaddingTopUnit )
	DEFAULT_PROPERTY( m_nPaddingBottom )
	DEFAULT_PROPERTY( m_ePaddingBottomUnit )

	DEFAULT_PROPERTY( m_nSpacingLeft )
	DEFAULT_PROPERTY( m_eSpacingLeftUnit )
	DEFAULT_PROPERTY( m_nSpacingRight )
	DEFAULT_PROPERTY( m_eSpacingRightUnit )
	DEFAULT_PROPERTY( m_nSpacingTop )
	DEFAULT_PROPERTY( m_eSpacingTopUnit )
	DEFAULT_PROPERTY( m_nSpacingBottom )
	DEFAULT_PROPERTY( m_eSpacingBottomUnit )

	m_eWidthUnit = mu_none;
	DEFAULT_PROPERTY( m_nWidth )
	DEFAULT_PROPERTY( m_bHideMark )

	m_oBorderDiagonalLR.SetDefault();
	m_oBorderDiagonalRL.SetDefault();
	m_oBorderLeft.SetDefault();
	m_oBorderTop.SetDefault();
	m_oBorderRight.SetDefault();
	m_oBorderBottom.SetDefault();
	m_oBorderInsideH.SetDefault();
	m_oBorderInsideV.SetDefault();

	m_oShading.SetDefault();
	DEFAULT_PROPERTY( m_nShadingPctFrom )
	DEFAULT_PROPERTY( m_eAlign )

	m_oCellFlow = cf_none;

	DEFAULT_PROPERTY( m_nCellx )
	DEFAULT_PROPERTY( m_nSpan )

	DEFAULT_PROPERTY( m_bStyleFirstRow )
	DEFAULT_PROPERTY( m_bStyleLastRow )
	DEFAULT_PROPERTY( m_bStyleFirstCol )
	DEFAULT_PROPERTY( m_bStyleLastCol )
	DEFAULT_PROPERTY( m_bStyleOddRowBand )
	DEFAULT_PROPERTY( m_bStyleEvenRowBand )
	DEFAULT_PROPERTY( m_bStyleOddColBand )
	DEFAULT_PROPERTY( m_bStyleEvenColBand )
	DEFAULT_PROPERTY( m_bStyleNWCell )
	DEFAULT_PROPERTY( m_bStyleNECell )
	DEFAULT_PROPERTY( m_bStyleSWCell )
	DEFAULT_PROPERTY( m_bStyleSECell )
}
bool RtfCellProperty::IsValid()
{
	return PROP_DEF != m_nCellx;
}
void RtfCellProperty::Merge( RtfCellProperty& oCellPr )
{
	MERGE_PROPERTY( m_bMergeFirst,			oCellPr )
	MERGE_PROPERTY( m_bMerge,				oCellPr )
	MERGE_PROPERTY( m_bMergeFirstVertical,	oCellPr )
	MERGE_PROPERTY( m_bMergeVertical,		oCellPr )

	MERGE_PROPERTY( m_bFitText, oCellPr )
	MERGE_PROPERTY( m_bNoWrap,	oCellPr )

	MERGE_PROPERTY( m_nPaddingLeft,			oCellPr )
	MERGE_PROPERTY( m_ePaddingLeftUnit,		oCellPr )
	MERGE_PROPERTY( m_nPaddingRight,		oCellPr )
	MERGE_PROPERTY( m_ePaddingRightUnit,	oCellPr )
	MERGE_PROPERTY( m_nPaddingTop,			oCellPr )
	MERGE_PROPERTY( m_ePaddingTopUnit,		oCellPr )
	MERGE_PROPERTY( m_nPaddingBottom,		oCellPr )
	MERGE_PROPERTY( m_ePaddingBottomUnit,	oCellPr )

	MERGE_PROPERTY( m_nSpacingLeft,			oCellPr )
	MERGE_PROPERTY( m_eSpacingLeftUnit,		oCellPr )
	MERGE_PROPERTY( m_nSpacingRight,		oCellPr )
	MERGE_PROPERTY( m_eSpacingRightUnit,	oCellPr )
	MERGE_PROPERTY( m_nSpacingTop,			oCellPr )
	MERGE_PROPERTY( m_eSpacingTopUnit,		oCellPr )
	MERGE_PROPERTY( m_nSpacingBottom,		oCellPr )
	MERGE_PROPERTY( m_eSpacingBottomUnit,	oCellPr )

	MERGE_PROPERTY( m_eWidthUnit,	oCellPr )
	MERGE_PROPERTY( m_nWidth,		oCellPr )
	MERGE_PROPERTY( m_bHideMark,	oCellPr )

	m_oBorderDiagonalLR.Merge( oCellPr.m_oBorderDiagonalLR );
	m_oBorderDiagonalRL.Merge( oCellPr.m_oBorderDiagonalRL );
	m_oBorderLeft.Merge( oCellPr.m_oBorderLeft );
	m_oBorderTop.Merge( oCellPr.m_oBorderTop );
	m_oBorderRight.Merge( oCellPr.m_oBorderRight );
	m_oBorderBottom.Merge( oCellPr.m_oBorderBottom );
	m_oBorderInsideH.Merge( oCellPr.m_oBorderInsideH );
	m_oBorderInsideV.Merge( oCellPr.m_oBorderInsideV );

	m_oShading.Merge( oCellPr.m_oShading );
	MERGE_PROPERTY	( m_nShadingPctFrom, oCellPr)

	MERGE_PROPERTY( m_eAlign, oCellPr )
	MERGE_PROPERTY( m_oCellFlow, oCellPr )

	MERGE_PROPERTY( m_nCellx, oCellPr )
	MERGE_PROPERTY( m_nSpan, oCellPr )

	MERGE_PROPERTY( m_bStyleFirstRow, oCellPr )
	MERGE_PROPERTY( m_bStyleLastRow, oCellPr )
	MERGE_PROPERTY( m_bStyleFirstCol, oCellPr )
	MERGE_PROPERTY( m_bStyleLastCol, oCellPr )
	MERGE_PROPERTY( m_bStyleOddRowBand, oCellPr )
	MERGE_PROPERTY( m_bStyleEvenRowBand, oCellPr )
	MERGE_PROPERTY( m_bStyleOddColBand, oCellPr )
	MERGE_PROPERTY( m_bStyleEvenColBand, oCellPr )
	MERGE_PROPERTY( m_bStyleNWCell, oCellPr )
	MERGE_PROPERTY( m_bStyleNECell, oCellPr )
	MERGE_PROPERTY( m_bStyleSWCell, oCellPr )
	MERGE_PROPERTY( m_bStyleSECell, oCellPr )
}
bool RtfCellProperty::GetAlignFromStringRtf(  std::wstring & oAlign, CellAlign& oOutput )
{
	if ( L"clvertalt" == oAlign ) { oOutput = ca_Top; return true;}
	else if ( L"clvertalc" == oAlign ){ oOutput = ca_Center; return true;}
	else if ( L"clvertalb" == oAlign ){ oOutput = ca_Bottom; return true;}
	return false;
}
bool RtfCellProperty::GetAlignFromStringOOX(  std::wstring & oAlign, CellAlign& oOutput )
{
	if ( L"top" == oAlign ) { oOutput = ca_Top; return true;}
	else if ( L"center" == oAlign ){ oOutput = ca_Center; return true;}
	else if ( L"bottom" == oAlign ){ oOutput = ca_Bottom; return true;}
	return false;
}
std::wstring RtfCellProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	RENDER_RTF_BOOL( m_bMergeFirst,			sResult, L"clmgf" )
	RENDER_RTF_BOOL( m_bMerge,				sResult, L"clmrg" )
	RENDER_RTF_BOOL( m_bMergeFirstVertical, sResult, L"clvmgf" )
	RENDER_RTF_BOOL( m_bMergeVertical,		sResult, L"clvmrg" )

	RENDER_RTF_BOOL( m_bFitText,			sResult, L"clFitText" )
	RENDER_RTF_BOOL( m_bNoWrap,				sResult, L"clNoWrap" )

//https://www.office-forums.com/threads/rtf-file-weirdness-clpadt-vs-clpadl.2163500/
	RENDER_RTF_INT( m_nPaddingLeft,			sResult, L"clpadt" )	//перепутаны top & left
	RENDER_RTF_INT( m_ePaddingLeftUnit,		sResult, L"clpadft" )	//перепутаны top & left
	RENDER_RTF_INT( m_nPaddingRight,		sResult, L"clpadr" )
	RENDER_RTF_INT( m_ePaddingRightUnit,	sResult, L"clpadfr" )
	RENDER_RTF_INT( m_nPaddingTop,			sResult, L"clpadl" )	//перепутаны top & left
	RENDER_RTF_INT( m_ePaddingTopUnit,		sResult, L"clpadfl" )	//перепутаны top & left
	RENDER_RTF_INT( m_nPaddingBottom,		sResult, L"clpadb" )
	RENDER_RTF_INT( m_ePaddingBottomUnit,	sResult, L"clpadfb" )

	RENDER_RTF_INT( m_eSpacingLeftUnit,		sResult, L"clspfl" )
	RENDER_RTF_INT( m_nSpacingLeft,			sResult, L"clspl" )
	RENDER_RTF_INT( m_eSpacingRightUnit,	sResult, L"clspl" )
	RENDER_RTF_INT( m_nSpacingRight,		sResult, L"clspr" )

	RENDER_RTF_INT( m_eSpacingTopUnit,		sResult, L"clspft" )
	RENDER_RTF_INT( m_nSpacingTop,			sResult, L"clspt" )
	RENDER_RTF_INT( m_eSpacingBottomUnit,	sResult, L"clspfb" )
	RENDER_RTF_INT( m_nSpacingBottom,		sResult, L"clspb" )
	
	RENDER_RTF_BOOL( m_bHideMark,			sResult, L"clhidemark" )

	if( true == m_oBorderDiagonalLR.IsValid() )
	{
		sResult += L"\\cldglu";
		sResult +=  m_oBorderDiagonalLR.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderDiagonalRL.IsValid() )
	{
		sResult += L"\\cldgll";
		sResult +=  m_oBorderDiagonalRL.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderLeft.IsValid() )
	{
		sResult += L"\\clbrdrl";
		sResult +=  m_oBorderLeft.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderTop.IsValid() )
	{
		sResult += L"\\clbrdrt";
		sResult +=  m_oBorderTop.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderRight.IsValid() )
	{
		sResult += L"\\clbrdrr";
		sResult +=  m_oBorderRight.RenderToRtf( oRenderParameter );
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		sResult += L"\\clbrdrb";
		sResult +=  m_oBorderBottom.RenderToRtf( oRenderParameter );
	}
	if( true == m_oShading.IsValid() )
		sResult +=  m_oShading.RenderToRtf( oRenderParameter );

	switch( m_eAlign )
	{
		case ca_Top:	sResult += L"\\clvertalt";	break;
		case ca_Center: sResult += L"\\clvertalc";	break;
		case ca_Bottom: sResult += L"\\clvertalb";	break;
		default:
			break;
	}
	switch( m_oCellFlow )
	{
		case cf_lrtb:	sResult += L"\\cltxlrtb";	break;
		case cf_tbrl:	sResult += L"\\cltxtbrl";	break;
		case cf_btlr:	sResult += L"\\cltxbtlr";	break;
		case cf_lrtbv:	sResult += L"\\cltxlrtbv";	break;
		case cf_tbrlv:	sResult += L"\\cltxtbrlv";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_nWidth, sResult, L"clwWidth" )
	switch( m_eWidthUnit )
	{
		case mu_none:		sResult += L"\\clftsWidth0";	break;
		case mu_Auto:		sResult += L"\\clftsWidth1";	break;
		case mu_Percent:	sResult += L"\\clftsWidth2";	break;
		case mu_Twips:		sResult += L"\\clftsWidth3";	break;
		default:
			break;
	}

    sResult += L"\\cellx" + std::to_wstring(m_nCellx);

	return sResult;
}
std::wstring RtfCellProperty::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( 1 == m_bMergeFirst )			sResult += L"<w:hMerge w:val=\"restart\"/>";
	if( 1 == m_bMerge )					sResult += L"<w:hMerge w:val=\"continue\"/>";
	if( 1 == m_bMergeFirstVertical )	sResult += L"<w:vMerge w:val=\"restart\"/>";
	if( 1 == m_bMergeVertical)			sResult += L"<w:vMerge w:val=\"continue\"/>";

	RENDER_OOX_BOOL( m_bFitText, sResult, L"w:tcFitText" )
	RENDER_OOX_BOOL( m_bNoWrap, sResult, L"w:noWrap" )

    std::wstring sMargin;
	if( 3 == m_ePaddingLeftUnit && PROP_DEF != m_nPaddingLeft)
        sMargin += L"<w:left w:w=\"" + std::to_wstring(m_nPaddingLeft) + L"\" w:type=\"dxa\"/>";
	if( 3 == m_ePaddingRightUnit && PROP_DEF != m_nPaddingRight)
        sMargin += L"<w:right w:w=\"" + std::to_wstring(m_nPaddingRight) + L"\" w:type=\"dxa\"/>";
	if( 3 == m_ePaddingTopUnit && PROP_DEF != m_nPaddingTop)
        sMargin += L"<w:top w:w=\"" + std::to_wstring(m_nPaddingTop) + L"\" w:type=\"dxa\"/>";
	if( 3 == m_ePaddingBottomUnit && PROP_DEF != m_nPaddingBottom)
        sMargin += L"<w:bottom w:w=\"" + std::to_wstring(m_nPaddingBottom) + L"\" w:type=\"dxa\"/>";
	
    if( !sMargin.empty() )
	{
        sResult += L"<w:tcMar>";
		sResult += sMargin;
		sResult += L"</w:tcMar>";
	}

	//if( true == m_bIsSpacingLeft )
	//	sResult += L"trspdl3");
	//else	
	//	sResult += L"trspdl0");
	//if( PROP_DEF != m_nSpacingLeft )
    //	sResult += L"trspdfl" + std::to_wstring(m_nSpacingLeft) + L"",);
	//if( true == m_bIsSpacingRight )
	//	sResult += L"trspdr3");
	//else	
	//	sResult += L"trspdr0");
	//if( PROP_DEF != m_nSpacingRight )
    //	sResult += L"trspdfr" + std::to_wstring(m_nSpacingRight) + L"",);
	//if( true == m_bIsSpacingTop )
	//	sResult += L"trspdt3");
	//else	
	//	sResult += L"trspdt0");
	//if( PROP_DEF != m_nSpacingTop )
    //	sResult += L"trspdft" + std::to_wstring(m_nSpacingTop) + L"",);
	//if( true == m_bIsSpacingBottom )
	//	sResult += L"trspdb3");
	//else	
	//	sResult += L"trspdb0");
	//if( PROP_DEF != m_nSpacingBottom )
    //	sResult += L"trspdfb" + std::to_wstring(m_nSpacingBottom) + L"",);

	if( PROP_DEF != m_nWidth )
	{
		if( mu_Percent ==  m_eWidthUnit )
            sResult += L"<w:tcW w:type=\"pct\" w:w=\"" + std::to_wstring(m_nWidth) + L"%\"/>";
		else if( mu_Twips ==  m_eWidthUnit )
            sResult += L"<w:tcW w:type=\"dxa\" w:w=\"" + std::to_wstring(m_nWidth) + L"\"/>";
	}
	RENDER_OOX_BOOL( m_bHideMark, sResult, L"w:hideMark" )

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_TAG;
	
    std::wstring sBorder;
	if( true == m_oBorderDiagonalLR.IsValid() )
	{
		oNewParam.sValue = L"w:tl2br";
		sBorder +=  m_oBorderDiagonalLR.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderDiagonalRL.IsValid() )
	{
		oNewParam.sValue = L"w:tr2bl";
		sBorder +=  m_oBorderDiagonalRL.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderLeft.IsValid() )
	{
		oNewParam.sValue = L"w:left";
		sBorder +=  m_oBorderLeft.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderTop.IsValid() )
	{
		oNewParam.sValue = L"w:top";
		sBorder +=  m_oBorderTop.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderRight.IsValid() )
	{
		oNewParam.sValue = L"w:right";
		sBorder +=  m_oBorderRight.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderBottom.IsValid() )
	{
		oNewParam.sValue = L"w:bottom";
		sBorder +=  m_oBorderBottom.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderInsideH.IsValid() )
	{
		oNewParam.sValue = L"w:insideH";
		sResult += m_oBorderInsideH.RenderToOOX(oNewParam);
	}
	if( true == m_oBorderInsideV.IsValid() )
	{
		oNewParam.sValue = L"w:insideV";
		sResult += m_oBorderInsideV.RenderToOOX(oNewParam);
	}
    if( false == sBorder.empty() )
	{
        sResult += L"<w:tcBorders>";
		sResult += sBorder;
		sResult += L"</w:tcBorders>";
	}

	if( true == m_oShading.IsValid() )
	{
		if (PROP_DEF != m_nShadingPctFrom && PROP_DEF != m_oShading.m_nValue && m_oShading.m_nValue != 0)
		{
			// todooo тут цвет подложки подкладывается от ячейки, таблицы или еще какой хрени
			// пока берем второй цвет паттерна Romanization_Armenian.rtf
			m_oShading.m_nBackColor = (int)(m_oShading.m_nForeColor * (m_oShading.m_nValue / 10000.));
		}
		sResult +=  m_oShading.RenderToOOX(oRenderParameter);
	}
	
	switch( m_eAlign )
	{
		case ca_Top:	sResult += L"<w:vAlign w:val=\"top\"/>";		break;
		case ca_Center: sResult += L"<w:vAlign w:val=\"center\"/>";	break;
		case ca_Bottom: sResult += L"<w:vAlign w:val=\"bottom\"/>";	break;
		default:
			break;
	}
	switch( m_oCellFlow )
	{
		case cf_lrtb:	sResult += L"<w:textDirection w:val=\"lrTb\"/>";		break;
		case cf_tbrl:	sResult += L"<w:textDirection w:val=\"tbRl\"/>";		break;
		case cf_btlr:	sResult += L"<w:textDirection w:val=\"btLr\"/>";		break;
		case cf_lrtbv:	sResult += L"<w:textDirection w:val=\"lrTbV\"/>";	break;
		case cf_tbrlv:	sResult += L"<w:textDirection w:val=\"tbRlV\"/>";	break;
		default:
			break;
	}

	RENDER_OOX_INT( m_nSpan, sResult, L"w:gridSpan" );

    //std::wstring scnfStyle;
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstRow, scnfStyle, L"w:firstRow" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastRow, scnfStyle, L"w:lastRow" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstCol, scnfStyle, L"w:firstColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastCol, scnfStyle, L"w:lastColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddRowBand, scnfStyle, L"w:oddHBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenRowBand, scnfStyle, L"w:evenHBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddColBand, scnfStyle, L"w:oddVBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenColBand, scnfStyle, L"w:evenvBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNWCell, scnfStyle, L"w:firstRowFirstColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNECell, scnfStyle, L"w:firstRowLastColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSWCell, scnfStyle, L"w:lastRowFirstColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSECell, scnfStyle, L"w:lastRowLastColumn" )
    //if( !scnfStyle.empty() )
	//	sResult += L"<w:cnfStyle " + scnfStyle + T("(/>";

	//cellx

	return sResult;
}

RtfTableProperty::RtfTableProperty()
{
	SetDefault();
}
void RtfTableProperty::SetDefaultRtf()
{
	SetDefault();
}
void RtfTableProperty::SetDefaultOOX()
{
	SetDefault();
}
void RtfTableProperty::SetDefault()
{
	DEFAULT_PROPERTY( m_bBidi )
	DEFAULT_PROPERTY( m_nAutoFit )
	DEFAULT_PROPERTY( m_nGraph )

	DEFAULT_PROPERTY( nTableIndent )
	DEFAULT_PROPERTY( eTableIndentUnit )

	m_eJust = rj_none;

	DEFAULT_PROPERTY( m_nWrapLeft )
	DEFAULT_PROPERTY( m_nWrapRight )
	DEFAULT_PROPERTY( m_nWrapTop )
	DEFAULT_PROPERTY( m_nWrapBottom )
	DEFAULT_PROPERTY( m_bOverlap )

	DEFAULT_PROPERTY_DEF( m_eHRef, hr_none )
	DEFAULT_PROPERTY_DEF( m_eVRef, vr_none )
	DEFAULT_PROPERTY_DEF( m_eHPos, hp_none )
	DEFAULT_PROPERTY_DEF( m_eVPos, vp_none )
	DEFAULT_PROPERTY( m_nHPos )
	DEFAULT_PROPERTY( m_nVPos )

	DEFAULT_PROPERTY( m_nLeft )
	DEFAULT_PROPERTY( m_nWidth )

	m_eWidthUnit = mu_none;

	//m_nDefCellMarBottom = 0;
	//m_nDefCellMarRight = 108;
	//m_nDefCellMarLeft = 108;
	//m_nDefCellMarTop = 0;
	DEFAULT_PROPERTY( m_nDefCellMarBottom )
	DEFAULT_PROPERTY( m_nDefCellMarRight )
	DEFAULT_PROPERTY( m_nDefCellMarLeft )
	DEFAULT_PROPERTY( m_nDefCellMarTop )

	//m_eDefCellMarBottomUnit = 3;
	//m_eDefCellMarRightUnit = 3;
	//m_eDefCellMarLeftUnit = 3;
	//m_eDefCellMarTopUnit = 3;
	DEFAULT_PROPERTY( m_eDefCellMarBottomUnit )
	DEFAULT_PROPERTY( m_eDefCellMarRightUnit )
	DEFAULT_PROPERTY( m_eDefCellMarLeftUnit )
	DEFAULT_PROPERTY( m_eDefCellMarTopUnit )

	DEFAULT_PROPERTY( m_nDefCellSpBottom )
	DEFAULT_PROPERTY( m_nDefCellSpLeft )
	DEFAULT_PROPERTY( m_nDefCellSpRight )
	DEFAULT_PROPERTY( m_nDefCellSpTop )

	DEFAULT_PROPERTY( m_eDefCellSpBottomUnit )
	DEFAULT_PROPERTY( m_eDefCellSpLeftUnit )
	DEFAULT_PROPERTY( m_eDefCellSpRightUnit )
	DEFAULT_PROPERTY( m_eDefCellSpTopUnit )

	m_oBorderLeft.SetEmpty();
	m_oBorderRight.SetEmpty();
	m_oBorderTop.SetEmpty();
	m_oBorderBottom.SetEmpty();
	m_oBorderVert.SetEmpty();
	m_oBorderHor.SetEmpty();

	m_oShading.SetDefault();

	DEFAULT_PROPERTY( m_nStyle )

	DEFAULT_PROPERTY( m_bAutoFirstRow )
	DEFAULT_PROPERTY( m_bAutoLastRow )
	DEFAULT_PROPERTY( m_bAutoFirstCol )
	DEFAULT_PROPERTY( m_bAutoLastCol )
	DEFAULT_PROPERTY( m_bAutoNoRowBand )
	DEFAULT_PROPERTY( m_bAutoNoColBand )

	DEFAULT_PROPERTY( m_nRowBandSize )
	DEFAULT_PROPERTY( m_nColBandSize )
}
bool RtfTableProperty::IsValid()
{
	return true;
}
void RtfTableProperty::Merge( RtfTableProperty& oTablePr )
{
	MERGE_PROPERTY( m_bBidi,			oTablePr )
	MERGE_PROPERTY( m_nAutoFit,			oTablePr )
	MERGE_PROPERTY( m_nGraph,			oTablePr )
	MERGE_PROPERTY( nTableIndent,		oTablePr )
	MERGE_PROPERTY( eTableIndentUnit,	oTablePr )

	MERGE_PROPERTY_DEF( m_eJust,	oTablePr, rj_none )

	MERGE_PROPERTY( m_nWrapLeft,	oTablePr )
	MERGE_PROPERTY( m_nWrapRight,	oTablePr )
	MERGE_PROPERTY( m_nWrapTop,		oTablePr )
	MERGE_PROPERTY( m_nWrapBottom,	oTablePr )
	MERGE_PROPERTY( m_bOverlap,		oTablePr )

	MERGE_PROPERTY_DEF( m_eHRef,	oTablePr, hr_none )
	MERGE_PROPERTY_DEF( m_eVRef,	oTablePr, vr_none )
	MERGE_PROPERTY_DEF( m_eHPos,	oTablePr, hp_none )
	MERGE_PROPERTY_DEF( m_eVPos,	oTablePr, vp_none )
	MERGE_PROPERTY( m_nHPos,		oTablePr )
	MERGE_PROPERTY( m_nVPos,		oTablePr )

	MERGE_PROPERTY( m_nLeft,					oTablePr )
	MERGE_PROPERTY( m_nWidth,					oTablePr )

	MERGE_PROPERTY_DEF( m_eWidthUnit,			oTablePr, mu_none )

	MERGE_PROPERTY( m_nDefCellMarBottom,		oTablePr )
	MERGE_PROPERTY( m_nDefCellMarRight,			oTablePr )
	MERGE_PROPERTY( m_nDefCellMarLeft,			oTablePr )
	MERGE_PROPERTY( m_nDefCellMarTop,			oTablePr )
	MERGE_PROPERTY( m_eDefCellMarBottomUnit,	oTablePr )
	MERGE_PROPERTY( m_eDefCellMarRightUnit,		oTablePr )
	MERGE_PROPERTY( m_eDefCellMarLeftUnit,		oTablePr )
	MERGE_PROPERTY( m_eDefCellMarTopUnit,		oTablePr )

	MERGE_PROPERTY( m_nDefCellSpBottom,			oTablePr )
	MERGE_PROPERTY( m_nDefCellSpLeft,			oTablePr )
	MERGE_PROPERTY( m_nDefCellSpRight,			oTablePr )
	MERGE_PROPERTY( m_nDefCellSpTop,			oTablePr )
	MERGE_PROPERTY( m_eDefCellSpBottomUnit,		oTablePr )
	MERGE_PROPERTY( m_eDefCellSpLeftUnit,		oTablePr )
	MERGE_PROPERTY( m_eDefCellSpRightUnit,		oTablePr )
	MERGE_PROPERTY( m_eDefCellSpTopUnit,		oTablePr )

	m_oBorderLeft.Merge	( oTablePr.m_oBorderLeft );
	m_oBorderRight.Merge( oTablePr.m_oBorderRight );
	m_oBorderTop.Merge	( oTablePr.m_oBorderTop );
	m_oBorderBottom.Merge( oTablePr.m_oBorderBottom );
	m_oBorderVert.Merge	( oTablePr.m_oBorderVert );
	m_oBorderHor.Merge	( oTablePr.m_oBorderHor );
	m_oShading.Merge	( oTablePr.m_oShading );

	MERGE_PROPERTY( m_nStyle, oTablePr )

	MERGE_PROPERTY( m_bAutoFirstRow, oTablePr )
	MERGE_PROPERTY( m_bAutoLastRow, oTablePr )
	MERGE_PROPERTY( m_bAutoFirstCol, oTablePr )
	MERGE_PROPERTY( m_bAutoLastCol, oTablePr )
	MERGE_PROPERTY( m_bAutoNoRowBand, oTablePr )
	MERGE_PROPERTY( m_bAutoNoColBand, oTablePr )

	MERGE_PROPERTY( m_nRowBandSize, oTablePr )
	MERGE_PROPERTY( m_nColBandSize, oTablePr )
}
std::wstring RtfTableProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	
	RENDER_RTF_BOOL( m_bBidi, sResult, L"taprtl" )
	
		if( PROP_DEF == m_nAutoFit || 1 == m_nAutoFit )
			sResult += L"\\trautofit1";
	else if( 0 == m_nAutoFit )
			sResult += L"\\trautofit0";
	
	RENDER_RTF_INT( m_nAutoFit,			sResult, L"trautofit" )
	RENDER_RTF_INT( m_nGraph,			sResult, L"trgaph" )
	RENDER_RTF_INT( nTableIndent,		sResult, L"tblind" )
	RENDER_RTF_INT( eTableIndentUnit,	sResult, L"tblindtype" )

	RENDER_RTF_INT( m_nWrapLeft,		sResult, L"tdfrmtxtLeft" )
	RENDER_RTF_INT( m_nWrapRight,		sResult, L"tdfrmtxtRight" )
	RENDER_RTF_INT( m_nWrapTop,			sResult, L"tdfrmtxtTop" )
	RENDER_RTF_INT( m_nWrapBottom,		sResult, L"tdfrmtxtBottom" )
	RENDER_RTF_BOOL( m_bOverlap,		sResult, L"tabsnoovrlp" )

	switch ( m_eHRef )
	{
		case hr_phmrg:	sResult += L"\\tphmrg";	break;
		case hr_phpg:	sResult += L"\\tphpg";	break;
		case hr_phcol:	sResult += L"\\tphcol";	break;
	}
	if( PROP_DEF != m_nHPos )
	{
		if( m_nHPos > 0 )
            sResult += L"\\tposx" + std::to_wstring(m_nHPos);
		else
            sResult += L"\\tposnegx" + std::to_wstring(m_nHPos);
	}
	switch ( m_eHPos )
	{
		case hp_posxc: sResult += L"\\tposxc";	break;
		case hp_posxi: sResult += L"\\tposxi";	break;
		case hp_posxo: sResult += L"\\tposxo";	break;
		case hp_posxl: sResult += L"\\tposxl";	break;
		case hp_posxr: sResult += L"\\tposxr";	break;
	}

	switch ( m_eVRef )
	{	
		case vr_pvmrg:	sResult += L"\\tpvmrg";	break;
		case vr_pvpg:	sResult += L"\\tpvpg";	break;
		case vr_pvpara: sResult += L"\\tpvpara";	break;
	}
	if( PROP_DEF != m_nVPos )
	{
		if( m_nVPos > 0 )
            sResult += L"\\tposy" + std::to_wstring(m_nVPos);
		else 
            sResult += L"\\tposnegy" + std::to_wstring(m_nVPos);
	}
	switch ( m_eVPos )
	{
		case vp_posyt:		sResult += L"\\tposyt";		break;
		case vp_posyil:		sResult += L"\\tposyil";		break;
		case vp_posyb:		sResult += L"\\tposyb";		break;
		case vp_posyc:		sResult += L"\\tposyc";		break;
		case vp_posyin:		sResult += L"\\tposyin";		break;
		case vp_posyout:	sResult += L"\\tposyout";	break;
	}
	RENDER_RTF_INT( m_nLeft, sResult, L"trleft" )

	RENDER_RTF_INT( m_nDefCellMarLeft,			sResult, L"trpaddl" );
	RENDER_RTF_INT( m_nDefCellMarRight,			sResult, L"trpaddr" );
	RENDER_RTF_INT( m_nDefCellMarTop,			sResult, L"trpaddt" );
	RENDER_RTF_INT( m_nDefCellMarBottom,		sResult, L"trpaddb" );
	RENDER_RTF_INT( m_eDefCellMarLeftUnit,		sResult, L"trpaddfl" );
	RENDER_RTF_INT( m_eDefCellMarRightUnit,		sResult, L"trpaddfr" );
	RENDER_RTF_INT( m_eDefCellMarTopUnit,		sResult, L"trpaddft" );
	RENDER_RTF_INT( m_eDefCellMarBottomUnit,	sResult, L"trpaddfb" );

	RENDER_RTF_INT( m_nDefCellSpBottom,			sResult, L"trspdb" );
	RENDER_RTF_INT( m_nDefCellSpLeft,			sResult, L"trspdl" );
	RENDER_RTF_INT( m_nDefCellSpRight,			sResult, L"trspdr" );
	RENDER_RTF_INT( m_nDefCellSpTop,			sResult, L"trspdt" );
	RENDER_RTF_INT( m_eDefCellSpBottomUnit,		sResult, L"trspdfb" );
	RENDER_RTF_INT( m_eDefCellSpLeftUnit,		sResult, L"trspdfl" );
	RENDER_RTF_INT( m_eDefCellSpRightUnit,		sResult, L"trspdfr" );
	RENDER_RTF_INT( m_eDefCellSpTopUnit,		sResult, L"trspdft" );

	if( m_oBorderTop.IsValid() == true )
	{
		sResult += L"\\trbrdrt";
		sResult += m_oBorderTop.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderLeft.IsValid() == true )
	{
		sResult += L"\\trbrdrl";
		sResult += m_oBorderLeft.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderBottom.IsValid() == true )
	{
		sResult += L"\\trbrdrb";
		sResult += m_oBorderBottom.RenderToRtf( oRenderParameter );
	}	
	if( m_oBorderRight.IsValid() == true )
	{
		sResult += L"\\trbrdrr";
		sResult += m_oBorderRight.RenderToRtf( oRenderParameter );
	}
	if( m_oBorderHor.IsValid() == true/*  && m_bAutoNoRowBand != 1*/)
	{
		sResult += L"\\trbrdrh";
		sResult += m_oBorderHor.RenderToRtf( oRenderParameter );
	}	
	if( m_oBorderVert.IsValid() == true/* && m_bAutoNoColBand != 1*/)
	{
		sResult += L"\\trbrdrv";
		sResult += m_oBorderVert.RenderToRtf( oRenderParameter );
	}

	if( m_oShading.IsValid() == true )
		sResult += m_oShading.RenderToRtf( oRenderParameter );

	//RENDER_RTF_INT( m_nStyle, sResult, L"ts" )

	RENDER_RTF_BOOL( m_bAutoFirstRow,	sResult, L"tbllkhdrrows" )
	RENDER_RTF_BOOL( m_bAutoLastRow,	sResult, L"tbllklastrow" )
	RENDER_RTF_BOOL( m_bAutoFirstCol,	sResult, L"tbllkhdrcols" )
	RENDER_RTF_BOOL( m_bAutoLastCol,	sResult, L"tbllklastcol" )
	RENDER_RTF_BOOL( m_bAutoNoRowBand,	sResult, L"tbllknorowband" )
	RENDER_RTF_BOOL( m_bAutoNoColBand,	sResult, L"tbllknocolband" )

	RENDER_RTF_INT( m_nRowBandSize,		sResult, L"tscbandsh" )
	RENDER_RTF_INT( m_nColBandSize,		sResult, L"tscbandsv" )

	return sResult;
}
std::wstring RtfTableProperty::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( PROP_DEF != m_nStyle )
	{
		RtfDocument* poDocument = static_cast<RtfDocument*>( oRenderParameter.poDocument );
		RtfStylePtr oCurStyle;
		if( true == poDocument->m_oStyleTable.GetStyle( m_nStyle, oCurStyle) )
		{
			sResult += L"<w:tblStyle w:val=\"";
			sResult += L"Style_" + std::to_wstring(m_nStyle); //oCurStyle->m_sName;
			sResult += L"\"/>";
		}
	}
	RENDER_OOX_BOOL( m_bBidi, sResult, L"w:bidiVisual" );

	if( PROP_DEF == m_nAutoFit || 0 == m_nAutoFit )
		sResult += L"<w:tblLayout w:type=\"fixed\"/>";
	else if( 1 == m_nAutoFit )
		sResult += L"<w:tblLayout w:type=\"autofit\"/>";

	//сделаем не по документации, а как все остальные юниты !!!
	if( PROP_DEF != nTableIndent && 3 == eTableIndentUnit  )
	{
		sResult += L"<w:tblInd w:w=\"" + std::to_wstring(nTableIndent) + L"\" w:type=\"dxa\"/>";
	}

	if( 1 == m_bOverlap )
		sResult += L"<w:tblOverlap w:val=\"overlap\"/>";

	switch( m_eJust )
	{
		case rj_trql : sResult += L"<w:jc w:val=\"left\"/>";		break;
		case rj_trqr : sResult += L"<w:jc w:val=\"right\"/>";	break;
		case rj_trqc : sResult += L"<w:jc w:val=\"center\"/>";	break;
		default:
			break;
	}

    std::wstring sFloatingPosition;
	if( PROP_DEF != m_nWrapLeft )
        sFloatingPosition += L" w:leftFromText=\"" + std::to_wstring(m_nWrapLeft) + L"\"";
	if( PROP_DEF != m_nWrapRight )
        sFloatingPosition += L" w:rightFromText=\"" + std::to_wstring(m_nWrapRight) + L"\"";
	if( PROP_DEF != m_nWrapTop )
        sFloatingPosition += L" w:topFromText=\"" + std::to_wstring(m_nWrapTop) + L"\"";
	if( PROP_DEF != m_nWrapBottom )
        sFloatingPosition += L" w:bottomFromText=\"" + std::to_wstring(m_nWrapBottom) + L"\"";

	switch ( m_eHRef )
	{
        case hr_phmrg:	sFloatingPosition += L" w:horzAnchor=\"margin\"";   break;
        case hr_phpg:	sFloatingPosition += L" w:horzAnchor=\"page\"";     break;
        case hr_phcol:	sFloatingPosition += L" w:horzAnchor=\"text\"";     break;
	}
	if( PROP_DEF != m_nHPos )
        sFloatingPosition += L" w:tblpX=\"" + std::to_wstring(m_nHPos) + L"\"";
	//else
	switch ( m_eHPos )
	{
        case hp_posxc: sFloatingPosition += L" w:tblpXSpec=\"center\"";     break;
        case hp_posxi: sFloatingPosition += L" w:tblpXSpec=\"inside\"";     break;
		case hp_posxo: sFloatingPosition += L" w:tblpXSpec=\"outside\"";	break;
        case hp_posxl: sFloatingPosition += L" w:tblpXSpec=\"left\"";       break;
        case hp_posxr: sFloatingPosition += L" w:tblpXSpec=\"right\"";      break;
	}

	switch ( m_eVRef )
	{	
        case vr_pvmrg:	sFloatingPosition += L" w:vertAnchor=\"margin\"";   break;
        case vr_pvpg:	sFloatingPosition += L" w:vertAnchor=\"page\"";     break;
        case vr_pvpara:	sFloatingPosition += L" w:vertAnchor=\"text\"";     break;
	}

	if( PROP_DEF != m_nVPos )
        sFloatingPosition += L" w:tblpY=\"" + std::to_wstring(m_nVPos) + L"\"";
	//else
	switch ( m_eVPos )
	{
		case vp_posyt:		sFloatingPosition += L" w:tblpYSpec=\"top\"";		break;
        case vp_posyil:		sFloatingPosition += L" w:tblpYSpec=\"inline\"";	break;
        case vp_posyb:		sFloatingPosition += L" w:tblpYSpec=\"bottom\"";	break;
        case vp_posyc:		sFloatingPosition += L" w:tblpYSpec=\"center\"";	break;
        case vp_posyin:		sFloatingPosition += L" w:tblpYSpec=\"inside\"";	break;
		case vp_posyout:	sFloatingPosition += L" w:tblpYSpec=\"outside\"";	break;
	}
    if( false == sFloatingPosition.empty() )
	{
        sResult += L"<w:tblpPr ";
		sResult += sFloatingPosition;
		sResult += L"/>";
	}

	if( PROP_DEF != m_nWidth && ( 2 == m_eWidthUnit || 3 == m_eWidthUnit ))
	{
		if( 2 == m_eWidthUnit )
            sResult += L"<w:tblW w:w=\"" + std::to_wstring(m_nWidth) + L"\" w:type=\"pct\"/>";
		else
            sResult += L"<w:tblW w:w=\"" + std::to_wstring(m_nWidth) + L"\" w:type=\"dxa\"/>";
	}
	else
		sResult += L"<w:tblW w:w=\"0\" w:type=\"auto\"/>";

    std::wstring sDefCellMargins;
	if( PROP_DEF != m_nDefCellMarBottom && 3 == m_eDefCellMarBottomUnit )
        sDefCellMargins += L"<w:bottom w:w=\"" + std::to_wstring(m_nDefCellMarBottom) + L"\" w:type=\"dxa\"/>";
	if( PROP_DEF != m_nDefCellMarLeft && 3 == m_eDefCellMarLeftUnit )
        sDefCellMargins += L"<w:left w:w=\"" + std::to_wstring(m_nDefCellMarLeft) + L"\" w:type=\"dxa\"/>";
	else
        sDefCellMargins += L"<w:left w:w=\"0\" w:type=\"dxa\"/>";

	if( PROP_DEF != m_nDefCellMarRight && 3 == m_eDefCellMarRightUnit )
        sDefCellMargins += L"<w:right w:w=\"" + std::to_wstring(m_nDefCellMarRight) + L"\" w:type=\"dxa\"/>";
	else
        sDefCellMargins += L"<w:right w:w=\"0\" w:type=\"dxa\"/>";

	if( PROP_DEF != m_nDefCellMarTop && 3 == m_eDefCellMarTopUnit )
        sDefCellMargins += L"<w:top w:w=\"" + std::to_wstring(m_nDefCellMarTop) + L"\" w:type=\"dxa\"/>";
	
    if( !sDefCellMargins.empty() )
        sResult += L"<w:tblCellMar>" + sDefCellMargins + L"</w:tblCellMar>";
	
	if( PROP_DEF != m_nDefCellSpBottom && 3 == m_eDefCellSpBottomUnit ) 
        sResult += L"<w:tblCellSpacing w:w=\"" + std::to_wstring(m_nDefCellSpBottom) + L"\" w:type=\"dxa\"/>";

	//if( PROP_DEF != m_nLeft )
    //	sResult += L"<w:tblInd w:w=\"" + std::to_wstring(m_nLeft) + L"\" w:type=\"dxa\"/>";

	RenderParameter oNewParam = oRenderParameter;
	oNewParam.nType = RENDER_TO_OOX_PARAM_BORDER_ATTRIBUTE;
    std::wstring sBorders;
	if( m_oBorderLeft.IsValid() )	sBorders += L"<w:left "		+ m_oBorderLeft.RenderToOOX(oNewParam)	+ L"/>";
    if( m_oBorderRight.IsValid() )	sBorders += L"<w:right "	+ m_oBorderRight.RenderToOOX(oNewParam)	+ L"/>";
	if( m_oBorderTop.IsValid() )	sBorders += L"<w:top "		+ m_oBorderTop.RenderToOOX(oNewParam)	+ L"/>";
	if( m_oBorderBottom.IsValid() )	sBorders += L"<w:bottom "	+ m_oBorderBottom.RenderToOOX(oNewParam) + L"/>";
	if( m_oBorderVert.IsValid() )	sBorders += L"<w:insideV "	+ m_oBorderVert.RenderToOOX(oNewParam)	+ L"/>";
	if( m_oBorderHor.IsValid() )	sBorders += L"<w:insideH "	+ m_oBorderHor.RenderToOOX(oNewParam)	+ L"/>";
	
    if( !sBorders.empty() )
		sResult += L"<w:tblBorders>" + sBorders+ L"</w:tblBorders>";

	if( m_oShading.IsValid() == true )
		sResult += m_oShading.RenderToOOX(oRenderParameter);


	//if( RENDER_TO_OOX_PARAM_STYLES == oRenderParameter.nType )
	//{
	//	RENDER_OOX_INT( m_nRowBandSize, sResult, L"w:tblStyleRowBandSize" )
	//	RENDER_OOX_INT( m_nColBandSize, sResult, L"w:tblStyleColBandSize " )
	//}
	//else
	//{
    //	std::wstring sTableLook;
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoFirstRow, sTableLook, L"w:firstRow" )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoLastRow, sTableLook, L"w:lastRow" )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoFirstCol, sTableLook, L"w:firstColumn" )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoLastCol, sTableLook, L"w:lastColumn" )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoNoRowBand, sTableLook, L"w:noHBand" )
	//	RENDER_OOX_BOOL_ATTRIBUTE( m_bAutoNoColBand, sTableLook, L"w:noVBand" )

    //	if( false == sTableLook.empty() )
	//		sResult += L"<w:tblLook " + sTableLook + L"/>");
	//}

    if( !sResult.empty() )
		sResult = L"<w:tblPr>" + sResult + L"</w:tblPr>";
	return sResult;
}

RtfRowProperty::RtfRowProperty()
{
	SetDefault();
}
void RtfRowProperty::SetDefaultRtf()
{
	SetDefault();
}
void RtfRowProperty::SetDefaultOOX()
{
	SetDefault();
	//не SetEmpty() !!!
	m_oBorderLeft.SetDefault();
	m_oBorderRight.SetDefault();
	m_oBorderTop.SetDefault();
	m_oBorderBottom.SetDefault();
	m_oBorderVert.SetDefault();
	m_oBorderHor.SetDefault();
}
void RtfRowProperty::SetDefault()
{
	RtfTableProperty::SetDefault();
	DEFAULT_PROPERTY( m_nIndex )
	DEFAULT_PROPERTY( m_nBandIndex )
	DEFAULT_PROPERTY( m_bLastRow )
	DEFAULT_PROPERTY( m_nAutoFit )
	DEFAULT_PROPERTY( m_bIsHeader )
	DEFAULT_PROPERTY( m_bKeep )
	DEFAULT_PROPERTY( m_nRightToLeft )

	DEFAULT_PROPERTY( m_nHeight )

	DEFAULT_PROPERTY( m_nWidthStartInvCell )
	DEFAULT_PROPERTY( m_nWidthEndInvCell )

	m_eWidthStartInvCellUnit	= mu_none;
	m_eWidthEndInvCellUnit		= mu_none;

	DEFAULT_PROPERTY( m_nGridBefore )
	DEFAULT_PROPERTY( m_nGridAfter )
	m_aArray.clear();

	DEFAULT_PROPERTY( m_bStyleFirstRow )
	DEFAULT_PROPERTY( m_bStyleLastRow )
	DEFAULT_PROPERTY( m_bStyleFirstCol )
	DEFAULT_PROPERTY( m_bStyleLastCol )
	DEFAULT_PROPERTY( m_bStyleOddRowBand )
	DEFAULT_PROPERTY( m_bStyleEvenRowBand )
	DEFAULT_PROPERTY( m_bStyleOddColBand )
	DEFAULT_PROPERTY( m_bStyleEvenColBand )
	DEFAULT_PROPERTY( m_bStyleNWCell )
	DEFAULT_PROPERTY( m_bStyleNECell )
	DEFAULT_PROPERTY( m_bStyleSWCell )
	DEFAULT_PROPERTY( m_bStyleSECell )

	DEFAULT_PROPERTY( m_nTrAuth)
	DEFAULT_PROPERTY( m_nTrDate)

	m_pOldRowProperty = RtfRowPropertyPtr();
}
bool RtfRowProperty::IsValid()
{
	return true;
}
void RtfRowProperty::Merge( RtfRowProperty& oRowPr )
{
	RtfTableProperty::Merge( oRowPr );

	MERGE_PROPERTY( m_nIndex,		oRowPr )
	MERGE_PROPERTY( m_nBandIndex,	oRowPr )
	MERGE_PROPERTY( m_bLastRow,		oRowPr )
	MERGE_PROPERTY( m_nRightToLeft,	oRowPr )

	MERGE_PROPERTY( m_nAutoFit,		oRowPr )
	MERGE_PROPERTY( m_bIsHeader,	oRowPr )
	MERGE_PROPERTY( m_bKeep,		oRowPr )

	MERGE_PROPERTY( m_nHeight,		oRowPr )

	MERGE_PROPERTY		( m_nWidthStartInvCell,		oRowPr )
	MERGE_PROPERTY_DEF	( m_eWidthStartInvCellUnit,	oRowPr, mu_none )
	MERGE_PROPERTY		( m_nWidthEndInvCell,		oRowPr )
	MERGE_PROPERTY_DEF	( m_eWidthEndInvCellUnit,	oRowPr, mu_none )

	MERGE_PROPERTY_DEF	( m_nGridBefore,		oRowPr, mu_none )
	MERGE_PROPERTY_DEF	( m_nGridAfter,			oRowPr, mu_none )

	MERGE_PROPERTY( m_bStyleFirstRow,		oRowPr )
	MERGE_PROPERTY( m_bStyleLastRow,		oRowPr )
	MERGE_PROPERTY( m_bStyleFirstCol,		oRowPr )
	MERGE_PROPERTY( m_bStyleLastCol,		oRowPr )
	MERGE_PROPERTY( m_bStyleOddRowBand,		oRowPr )
	MERGE_PROPERTY( m_bStyleEvenRowBand,	oRowPr )
	MERGE_PROPERTY( m_bStyleOddColBand,		oRowPr )
	MERGE_PROPERTY( m_bStyleEvenColBand,	oRowPr )
	MERGE_PROPERTY( m_bStyleNWCell,			oRowPr )
	MERGE_PROPERTY( m_bStyleNECell,			oRowPr )
	MERGE_PROPERTY( m_bStyleSWCell,			oRowPr )
	MERGE_PROPERTY( m_bStyleSECell,			oRowPr )

	MERGE_PROPERTY( m_nTrAuth,				oRowPr )
	MERGE_PROPERTY( m_nTrDate,				oRowPr )
}
std::wstring RtfRowProperty::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	
	if( RENDER_TO_RTF_PARAM_NO_WROWD != oRenderParameter.nType )
		sResult += L"\\trowd";

	RENDER_RTF_INT	( m_nIndex,		sResult, L"irow" );
	RENDER_RTF_INT	( m_nBandIndex, sResult, L"irowband" );
	RENDER_RTF_BOOL	( m_bLastRow,	sResult, L"lastrow" );
	RENDER_RTF_INT	( m_nAutoFit,	sResult, L"trautofit" );
	RENDER_RTF_BOOL	( m_bIsHeader,	sResult, L"trhdr" );
	RENDER_RTF_BOOL	( m_bKeep,		sResult, L"trkeep" );

	if (m_nRightToLeft == 1) sResult += L"\\rtlrow";
	if (m_nRightToLeft == 0) sResult += L"\\ltrrow";

	switch( m_eJust )
	{
		case rj_trql : sResult += L"\\trql";	break;
		case rj_trqr : sResult += L"\\trqr";	break;
		case rj_trqc : sResult += L"\\trqc";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_nHeight, sResult, L"trrh" );

	switch( m_eWidthUnit )
	{
		case mu_Auto:		sResult += L"\\trftsWidth1";	break;
		case mu_Percent:	sResult += L"\\trftsWidth2";	break;
		case mu_Twips:		sResult += L"\\trftsWidth3";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_nWidth, sResult, L"trwWidth" );
	switch( m_eWidthStartInvCellUnit )
	{
		case mu_Auto:		sResult += L"\\trftsWidthB1";	break;
		case mu_Percent:	sResult += L"\\trftsWidthB2";	break;
		case mu_Twips:		sResult += L"\\trftsWidthB3";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_nWidthStartInvCell, sResult, L"trwWidthB" );
	switch( m_eWidthEndInvCellUnit )
	{
		case mu_Auto:		sResult += L"\\trftsWidthA1";	break;
		case mu_Percent:	sResult += L"\\trftsWidthA2";	break;
		case mu_Twips:		sResult += L"\\trftsWidthA3";	break;
		default:
			break;
	}
	RENDER_RTF_INT( m_nWidthEndInvCell, sResult, L"trwWidthA" );
	sResult +=  RtfTableProperty::RenderToRtf(  oRenderParameter  );

	for (size_t i = 0; i < m_aArray.size(); i++ )
		sResult +=  m_aArray[i].RenderToRtf(  oRenderParameter  );

	if (m_pOldRowProperty)
	{
        sResult += L"{\\*\\oldtprops";
		sResult += m_pOldRowProperty->RenderToRtf(oRenderParameter);
		sResult += L"}";
	}

	return sResult;
}
std::wstring RtfRowProperty::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	
	RtfDocument	* poRtfDocument = static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter	* poOOXWriter	= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
	
	RENDER_OOX_BOOL( m_bIsHeader, sResult, L"w:tblHeader" )
	RENDER_OOX_BOOL( m_bKeep, sResult, L"w:cantSplit" )

	switch( m_eJust )
	{
		case rj_trql : sResult += L"<w:jc w:val=\"left\"/>";	break;
		case rj_trqr : sResult += L"<w:jc w:val=\"right\"/>";	break;
		case rj_trqc : sResult += L"<w:jc w:val=\"center\"/>";	break;
		default:
			break;
	}
	if( PROP_DEF != m_nHeight )
	{
		if( m_nHeight > 0 )
            sResult += L"<w:trHeight w:val=\"" + std::to_wstring(m_nHeight) +L"\" w:hRule=\"atLeast\"/>";
		else if( m_nHeight < 0 )
            sResult += L"<w:trHeight w:val=\"" + std::to_wstring(-m_nHeight) +L"\" w:hRule=\"exact\"/>";
	}

	if( PROP_DEF != m_nWidthStartInvCell )
	{
		switch( m_eWidthStartInvCellUnit )
		{
        case mu_Percent:	sResult += L"<w:wBefore w:type=\"pct\" w:w=\"" + std::to_wstring(m_nWidthStartInvCell) + L"%\"/>";  break;
        case mu_Twips:		sResult += L"<w:wBefore w:type=\"dxa\" w:w=\"" + std::to_wstring(m_nWidthStartInvCell) + L"\"/>";	break;
		default:
			break;
		}
	}
	if( PROP_DEF != m_nWidthEndInvCell )
	{
		switch( m_eWidthEndInvCellUnit )
		{
        case mu_Percent:	sResult += L"<w:wAfter w:type=\"pct\" w:w=\"" + std::to_wstring(m_nWidthEndInvCell) + L"\"/>"; break;
        case mu_Twips:		sResult += L"<w:wAfter w:type=\"dxa\" w:w=\"" + std::to_wstring(m_nWidthEndInvCell) + L"\"/>";	break;
		default:
			break;
		}
	}
	RENDER_OOX_INT( m_nGridBefore,	sResult, L"w:gridBefore" )
	RENDER_OOX_INT( m_nGridAfter,	sResult, L"w:gridAfter" )

    //std::wstring scnfStyle;
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstRow, scnfStyle, L"w:firstRow" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastRow, scnfStyle, L"w:lastRow" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleFirstCol, scnfStyle, L"w:firstColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleLastCol, scnfStyle, L"w:lastColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddRowBand, scnfStyle, L"w:oddHBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenRowBand, scnfStyle, L"w:evenHBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleOddColBand, scnfStyle, L"w:oddVBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleEvenColBand, scnfStyle, L"w:evenvBand" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNWCell, scnfStyle, L"w:firstRowFirstColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleNECell, scnfStyle, L"w:firstRowLastColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSWCell, scnfStyle, L"w:lastRowFirstColumn" )
	//RENDER_OOX_BOOL_ATTRIBUTE( m_bStyleSECell, scnfStyle, L"w:lastRowLastColumn" )
    //if(!scnfStyle.empty() )
	//	sResult += L"<w:cnfStyle " + scnfStyle + L"/>" );

//	_bstr_t hk;

	if (m_pOldRowProperty)
	{
		m_pOldRowProperty;
		//if ( PROP_DEF != oReader.m_oState->m_oCharProp.m_nDeleted )
		//{
        //	std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(oReader.m_oState->m_oCharProp.m_nRevauthDel);
        //	std::wstring sDate(RtfUtility::convertDateTime(oReader.m_oState->m_oCharProp.m_nRevdttmDel).c_str());

		//	sResult += L"<w:del w:date=\""	+ sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\"/>";
		//}
		//if ( PROP_DEF != oReader.m_oState->m_oCharProp.m_nRevised )
		//{
        //	std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(oReader.m_oState->m_oCharProp.m_nRevauth);
        //	std::wstring sDate(RtfUtility::convertDateTime(oReader.m_oState->m_oCharProp.m_nRevdttm).c_str());
		//	
		//	sResult += L"<w:ins w:date=\""	+ sDate +  L"\" w:author=\""	+ sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\"/>";
		//}
        std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_nTrAuth);
        std::wstring sDate(RtfUtility::convertDateTime(m_nTrDate).c_str());
		
		RenderParameter oRenderParameterNew = oRenderParameter;
		oRenderParameterNew.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
        std::wstring rowChangeProps = m_pOldRowProperty->RenderToOOX(oRenderParameterNew);

        if (rowChangeProps.empty())
		{
			sResult += L"<w:ins w:date=\""	+ sDate +  L"\" w:author=\""	+ sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\"/>";
		}
		else
		{
			sResult += L"<w:trPrChange w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
				sResult += L"<w:trPr>";
					sResult += rowChangeProps;
				sResult += L"</w:trPr>";
			sResult += L"</w:trPrChange>";
		}
	}
	return sResult;
}

RtfInformation::RtfInformation()
{
	SetDefault();
}
bool RtfInformation::IsValid()
{
	return true;
}
void RtfInformation::SetDefaultRtf()
{
	SetDefault();
}
void RtfInformation::SetDefaultOOX()
{
	SetDefault();
}
void RtfInformation::SetDefault()
{
	m_sTitle = L"";
	m_sSubject = L"";
	m_sAuthor = L"";
	m_sManager = L"";
	m_sCompany = L"";
	m_sOperator = L"";
	m_sCategory = L"";
	m_sKeywords = L"";
	m_sComment = L"";
	m_sDocCom = L"";
	m_sLinkBase = L"";
	m_oCreateTime.SetDefault();
	m_oRevTime.SetDefault();
	m_oPrintTime.SetDefault();
	m_oBackupTime.SetDefault();

	DEFAULT_PROPERTY( m_nVersion )
	DEFAULT_PROPERTY( m_nInternalVersion )
	DEFAULT_PROPERTY( m_nEndingTime )

	DEFAULT_PROPERTY( m_nNumberOfPages )
	DEFAULT_PROPERTY( m_nNumberOfWords )
	DEFAULT_PROPERTY( m_nNumberOfCharactersWithSpace )
	DEFAULT_PROPERTY( m_nNumberOfCharactersWithoutSpace )
	DEFAULT_PROPERTY( m_nInternalId )
}
std::wstring RtfInformation::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;

    if( !m_sTitle.empty() )
	{
        sResult += L"{\\title ";	sResult += RtfChar::renderRtfText( m_sTitle, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sSubject.empty() )
	{
        sResult += L"{\\subject ";	sResult += RtfChar::renderRtfText( m_sSubject, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sAuthor.empty() )
	{
        sResult += L"{\\author ";	sResult += RtfChar::renderRtfText( m_sAuthor, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sManager.empty() )
	{
        sResult += L"{\\manager ";	sResult += RtfChar::renderRtfText( m_sManager, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sCompany.empty() )
	{
        sResult += L"{\\company ";	sResult += RtfChar::renderRtfText( m_sCompany, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sOperator.empty() )
	{
        sResult += L"{\\operator ";	sResult += RtfChar::renderRtfText( m_sOperator, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sCategory.empty() )
	{
        sResult += L"{\\category ";	sResult += RtfChar::renderRtfText( m_sCategory, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sKeywords.empty() )
	{
        sResult += L"{\\keywords ";	sResult += RtfChar::renderRtfText( m_sKeywords, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sComment.empty() )
	{
        sResult += L"{\\comment ";	sResult += RtfChar::renderRtfText( m_sComment, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sDocCom.empty() )
	{
        sResult += L"{\\doccomm ";	sResult += RtfChar::renderRtfText( m_sDocCom, oRenderParameter.poDocument );	sResult += L"}";
	}
    if( !m_sDocCom.empty() )
	{
        sResult += L"{\\doccomm ";	sResult += RtfChar::renderRtfText( m_sDocCom, oRenderParameter.poDocument );	sResult += L"}";
	}
	if( PROP_DEF != m_nInternalVersion )
	{
        sResult += L"{\\vern" + std::to_wstring(m_nInternalVersion) + L"}";
	}
    if( !m_sLinkBase.empty() )
	{
        sResult += L"{\\hlinkbase ";	sResult += RtfChar::renderRtfText( m_sLinkBase, oRenderParameter.poDocument );	sResult += L"}";
	}

    std::wstring sCreateTime = m_oCreateTime.RenderToRtf( oRenderParameter );
    if( !sCreateTime.empty() )
	{
        sResult += L"{\\creatim";	sResult += sCreateTime;	sResult += L"}";
	}
    std::wstring sRevTime = m_oRevTime.RenderToRtf( oRenderParameter );
    if( !sRevTime.empty() )
	{
		sResult += L"{\\revtim";	sResult += sRevTime;	sResult += L"}";
	}
    std::wstring sPrintTime = m_oPrintTime.RenderToRtf( oRenderParameter );
    if( !sPrintTime.empty() )
	{
        sResult += L"{\\printim";	sResult += sPrintTime;	sResult += L"}";
	}
    std::wstring sBackupTime = m_oBackupTime.RenderToRtf( oRenderParameter );
    if( !sBackupTime.empty() )
	{
        sResult += L"{\\buptim";	sResult += sBackupTime;	sResult += L"}";
	}
	if( PROP_DEF != m_nEndingTime )
	{
        sResult += L"{\\edmins" + std::to_wstring(m_nEndingTime) + L"}";
	}
	if( PROP_DEF != m_nNumberOfPages )
	{
        sResult += L"{\\nofpages" + std::to_wstring(m_nNumberOfPages) + L"}";
	}
	if( PROP_DEF != m_nNumberOfWords )
	{
        sResult += L"{\\nofwords" + std::to_wstring(m_nNumberOfWords) + L"}";
	}
	if( PROP_DEF != m_nNumberOfCharactersWithSpace )
	{
        sResult += L"{\\nofchars" + std::to_wstring(m_nNumberOfCharactersWithSpace) + L"}";
	}
	if( PROP_DEF != m_nNumberOfCharactersWithoutSpace )
	{
        sResult += L"{\\nofcharsws" + std::to_wstring(m_nNumberOfCharactersWithoutSpace) + L"}";
	}

	if( PROP_DEF != m_nInternalId )
        sResult += L"{\\id" + std::to_wstring(m_nInternalId) + L"}";

	std::wstring m_sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
	if (m_sApplication.empty())
		m_sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;

#if defined(INTVER)
	std::string s = VALUE2STR(INTVER);
    m_sApplication += L"/" + std::wstring(s.begin(), s.end());
#endif	
	if( !m_sApplication.empty() )
	{
		sResult += L"{\\*\\userprops ";
			sResult += L"{\\propname Application}";
			sResult += L"\\proptype30";
			sResult += L"{\\staticval " + RtfChar::renderRtfText( m_sApplication, oRenderParameter.poDocument ) + L"}";
		sResult += L"}";	
	}

    if( !sResult.empty() )
		sResult = L"{\\info" + sResult + L"}";
	return sResult;
}
std::wstring RtfInformation::RenderToOOX(RenderParameter oRenderParameter)
{
	if( RENDER_TO_OOX_PARAM_CORE == oRenderParameter.nType )
	{
		OOX::CCore *pCore = static_cast<OOX::CCore*>( oRenderParameter.poWriter );
		
        if( !m_sTitle.empty() )		pCore->m_sTitle			= XmlUtils::EncodeXmlString(m_sTitle);
        if( !m_sSubject.empty() )		pCore->m_sSubject		= XmlUtils::EncodeXmlString(m_sSubject);
        if( !m_sAuthor.empty() )		pCore->m_sCreator		= m_sAuthor;
        if( !m_sKeywords.empty() )	pCore->m_sKeywords		= m_sKeywords;
        if( !m_sComment.empty() )		pCore->m_sDescription	= XmlUtils::EncodeXmlString(m_sComment);

        //std::wstring sCreateTime = m_oCreateTime.RenderToOOX( oRenderParameter );
        //if( false == sCreateTime.empty() ) pCore->m_sCreated		= sCreateTime;
		//
        //std::wstring sRevTime = m_oRevTime.RenderToOOX( oRenderParameter );
        //if( false == sRevTime.empty() )	pCore->m_sModified		= sRevTime;

        //std::wstring sPrintTime = m_oPrintTime.RenderToOOX( oRenderParameter );
		//if( L"" != sPrintTime )
		//	sResult += L"<dcterms:created xsi:type=\"dcterms:W3CDTF\">" + sCreateTime + L"</dcterms:created>";
        //std::wstring sBackupTime = m_oBackupTime.RenderToOOX( oRenderParameter );
		//if( L"" != sBackupTime )
		//	sResult += L"<dcterms:created xsi:type=\"dcterms:W3CDTF\">" + sCreateTime + L"</dcterms:created>";
	}
	else if( RENDER_TO_OOX_PARAM_APP == oRenderParameter.nType )
	{
		OOX::CApp *pApp = static_cast<OOX::CApp*>( oRenderParameter.poWriter );
		
		if( PROP_DEF != m_nEndingTime)						pApp->m_nTotalTime				= m_nEndingTime;
		if( PROP_DEF != m_nNumberOfPages )					pApp->m_nPages					= m_nNumberOfPages;
		if( PROP_DEF != m_nNumberOfWords )					pApp->m_nWords					= m_nNumberOfWords;
		if( PROP_DEF != m_nNumberOfCharactersWithSpace )	pApp->m_nCharactersWithSpaces	= m_nNumberOfCharactersWithSpace;
		
		//if( PROP_DEF != m_nNumberOfCharactersWithoutSpace )	
        //	sResult += L"<CharactersWithSpaces>" + std::to_wstring() + L"</CharactersWithSpaces>",m_nNumberOfCharactersWithoutSpace );
	}
	return L"";
}

RtfTime::RtfTime()
{
	SetDefault();
}
bool RtfTime::IsValid()
{
	return true;
}
void RtfTime::SetDefaultRtf()
{
	SetDefault();
}
void RtfTime::SetDefaultOOX()
{
	SetDefault();
}
void RtfTime::SetDefault()
{
	DEFAULT_PROPERTY( m_nYear )
	DEFAULT_PROPERTY( m_nMonth )
	DEFAULT_PROPERTY( m_nDay )
	DEFAULT_PROPERTY( m_nHour )
	DEFAULT_PROPERTY( m_nMin )
	DEFAULT_PROPERTY( m_nSecond )
}
std::wstring RtfTime::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
    if( PROP_DEF != m_nYear )		sResult += L"\\yr" + std::to_wstring(m_nYear);
    if( PROP_DEF != m_nMonth )		sResult += L"\\mo" + std::to_wstring(m_nMonth);
    if( PROP_DEF != m_nDay )		sResult += L"\\dy" + std::to_wstring(m_nDay);
    if( PROP_DEF != m_nHour )		sResult += L"\\hr" + std::to_wstring(m_nHour);
    if( PROP_DEF != m_nMin )		sResult += L"\\min" + std::to_wstring(m_nMin);
    if( PROP_DEF != m_nSecond )		sResult += L"\\sec" + std::to_wstring(m_nSecond);
	return sResult;
}
std::wstring RtfTime::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( PROP_DEF != m_nYear || PROP_DEF != m_nMonth || PROP_DEF != m_nDay )
	{
		int nYear = 0;
		if( PROP_DEF != m_nYear )			nYear = m_nYear;
		int nMonth = 0;
		if( PROP_DEF != m_nMonth )			nMonth = m_nMonth;
		int nDay = 0;
		if( PROP_DEF != m_nDay )			nDay = m_nDay;

        sResult = std::to_wstring(nYear) + L"-" + std::to_wstring(nMonth) + L"-" + std::to_wstring(nDay) + L"T";
	}
	if( PROP_DEF != m_nHour || PROP_DEF != m_nMin || PROP_DEF != m_nSecond )
	{
		int nHour = 0;
		if( PROP_DEF != m_nHour )			nHour = m_nHour;
		int nMin = 0;
		if( PROP_DEF != m_nMin )			nMin = m_nMin;
		int nSecond = 0;
		if( PROP_DEF != m_nSecond )			nSecond = m_nSecond;
		
        sResult = std::to_wstring(nHour) + L":" + std::to_wstring(nMin) + L":" + std::to_wstring(nSecond) + L"Z";
	}
	return sResult;
}

RtfCharStyle::RtfCharStyle()
{
   SetDefault();
   m_eType = stCharacter;
}
int RtfCharStyle::GetType()
{
	return TYPE_RTF_PROPERTY_STYLE_CHAR;
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

RtfParagraphStyle::RtfParagraphStyle()
{
   SetDefault();
   m_eType = stParagraph;
}
int RtfParagraphStyle::GetType()
{
	return TYPE_RTF_PROPERTY_STYLE_PARAGRAPH;
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

RtfTableStyle::RtfTableStyle()
{
	SetDefault();
	m_eType = stTable;
}
void RtfTableStyle::SetDefault()
{
   RtfStyle::SetDefault();
	m_oFirstRow = RtfTableStylePtr();
	m_oLastRow = RtfTableStylePtr();
	m_oFirstCol = RtfTableStylePtr();
	m_oLastCol = RtfTableStylePtr();
	m_oBandHorEven = RtfTableStylePtr();
	m_oBandVerEven = RtfTableStylePtr();
	m_oBandHorOdd = RtfTableStylePtr();
	m_oBandVerOdd = RtfTableStylePtr();
	m_oNWCell = RtfTableStylePtr();
	m_oNECell = RtfTableStylePtr();
	m_oSWCell = RtfTableStylePtr();
	m_oSECell = RtfTableStylePtr();
}
int RtfTableStyle::GetType()
{
	return TYPE_RTF_PROPERTY_STYLE_TABLE;
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
		
		m_oTableProp.Merge	( oTableStyle->m_oTableProp );
		m_oRowProp.Merge	( oTableStyle->m_oRowProp );
		m_oCellProp.Merge	( oTableStyle->m_oCellProp );
		m_oParProp.Merge	( oTableStyle->m_oParProp );
		m_oCharProp.Merge	( oTableStyle->m_oCharProp );
		
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
