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
#pragma once

#include "Basic.h"
#include "Utils.h"

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Drawing.h"

#if defined (_WIN32) || defined (_WIN64)
    #define MERGE_PROPERTY(propName, propObj)\
        if ( PROP_DEF != ##propObj##.##propName## )	propName = ##propObj##.##propName##;

    #define MERGE_PROPERTY_DEF(propName, propObj, defval)\
        if ( defval != ##propObj##.##propName## )	propName = ##propObj##.##propName##;
#else
    #define MERGE_PROPERTY(propName, propObj)\
        if ( PROP_DEF != propObj.propName )	propName = propObj.propName;

    #define MERGE_PROPERTY_DEF(propName, propObj, defval)\
        if ( defval != propObj.propName )	propName = propObj.propName;
#endif

#define DEFAULT_PROPERTY(propName)\
		propName= PROP_DEF;

#define DEFAULT_PROPERTY_DEF(propName, defval)\
		propName = defval;

#define RENDER_OOX_BOOL( prop, sResult, sName)\
		if ( 0 == prop )\
		{\
			sResult += L"<";\
			sResult += sName;\
            sResult += L" w:val=\"false\"/>";\
		}\
		else if (  PROP_DEF != prop  )\
		{\
			sResult += L"<";\
			sResult += sName;\
            sResult += L" w:val=\"true\"/>";\
		}

#define RENDER_OOX_INT( prop, sResult, sName)\
		if ( PROP_DEF != prop )\
		{\
			sResult += L"<";\
			sResult += sName;\
            sResult += L" w:val=\"" + std::to_wstring(prop) + L"\"/>";\
		}
#define RENDER_OOX_INT_ATTRIBUTE( prop, sResult, sName)\
		if ( PROP_DEF != prop )\
		{\
			sResult += L" ";\
			sResult += sName;\
            sResult += L"=\"" + std::to_wstring(prop) + L"\"";\
		}
#define RENDER_OOX_BOOL_ATTRIBUTE( prop, sResult, sName)\
		if ( 0 == prop )\
		{\
			sResult += L" ";\
			sResult += sName;\
			sResult += L"=\"0\"";\
		}\
		else if (  PROP_DEF != prop  )\
		{\
			sResult += L" ";\
			sResult += sName;\
			sResult += L"=\"1\"";\
		}

#define RENDER_RTF_BOOL( prop, sResult, sName)\
		if ( 0 == prop )\
		{\
			sResult += L"\\";\
			sResult += sName;\
			sResult += L"0";\
		}\
		else if (  PROP_DEF != prop  )\
		{\
			sResult += L"\\";\
			sResult += sName;\
		}

#define RENDER_RTF_INT( prop, sResult, sName)\
		if ( PROP_DEF != prop )\
		{\
			sResult += L"\\";\
			sResult += sName;\
            sResult += std::to_wstring( prop  );\
		}
#define RENDER_RTF_SHAPE_PROP( sName, sResult, prop)\
        if ( PROP_DEF != prop )\
            sResult += L"{\\sp{\\sn " + std::wstring(sName) + L"}{\\sv " + std::to_wstring( prop ) + L"}}";

class RtfSection;
typedef boost::shared_ptr<RtfSection> RtfSectionPtr;

class RtfFont : public IRenderableProperty
{
public: 
	typedef enum {ft_none,ft_flomajor , ft_fhimajor ,ft_fdbmajor ,ft_fbimajor ,ft_flominor ,ft_fhiminor ,ft_fdbminor ,ft_fbiminor}FontTheme;
		
	typedef enum {ff_none, ff_fnil ,ff_froman ,ff_fswiss ,ff_fmodern ,ff_fscript ,ff_fdecor ,ff_ftech ,ff_fbidi} FontFamily;
	
    FontTheme       m_eFontTheme;
    FontFamily      m_eFontFamily;
    std::wstring    m_sPanose;
    int             m_nID;
    std::wstring	m_sName;
    std::wstring	m_sAltName;
    int             m_nCharset;
    int             m_nCodePage;
    int             m_nPitch;

	RtfFont()
	{
		SetDefault();
	}
	int GetType()
	{
		return TYPE_RTF_FONT;
	}
	bool operator==( const RtfFont& oFont)
	{
		return m_eFontTheme == oFont.m_eFontTheme	&& m_eFontFamily	== oFont.m_eFontFamily	&& 
				m_sPanose	== oFont.m_sPanose		&& m_nID			== oFont.m_nID			&& 
				m_sName		== oFont.m_sName		&& m_sAltName		== oFont.m_sAltName		&& 
				m_nCharset	== oFont.m_nCharset		&& m_nCodePage		== oFont.m_nCodePage	&& 
				m_nPitch	== oFont.m_nPitch;
	}
	bool IsValid()
	{
		return PROP_DEF != m_nID;
	}
	void SetDefaultRtf()
	{
		SetDefault();
		m_nPitch = 0;
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfColor : public IRenderableProperty
{
public: 
	enum _ThemeColor {TC_NONE,cmaindarkone ,cmainlightone ,cmaindarktwo ,cmainlighttwo ,caccentone ,caccenttwo ,caccentthree ,caccentfour ,caccentfive ,caccentsix ,chyperlink ,cfollowedhyperlink ,cbackgroundone ,ctextone ,cbackgroundtwo ,ctexttwo};
	
	bool		m_bAuto;

    BYTE		m_byteRed;
    BYTE		m_byteGreen;
    BYTE		m_byteBlue;
//for rtf read only
	BYTE		m_byteShade;
	BYTE		m_byteTint;
	_ThemeColor	m_eTheme;

	int			m_index;

//--------------------------------------------------------
	RtfColor()
	{
		SetDefault();
	}	
	RtfColor(int nHex)
	{
		SetHEX( nHex );
	}
	RtfColor(BYTE r, BYTE g, BYTE b)
	{
		SetRGB( r, g, b );
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetAuto()
	{
		m_bAuto		= true;
	}
	void SetDefault()
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
    BYTE GetR()
	{
		return m_byteRed;
	}
    BYTE GetG()
	{
		return m_byteGreen;
	}
    BYTE GetB()
	{
 		return m_byteBlue;
	}
	
	bool operator==(const RtfColor& oColor)
	{
		return  m_byteRed == oColor.m_byteRed && m_byteGreen == oColor.m_byteGreen && m_byteBlue == oColor.m_byteBlue &&
				m_eTheme == oColor.m_eTheme && m_byteTint	== oColor.m_byteTint && m_byteShade == oColor.m_byteShade;
	}
	const RtfColor& operator=( const RtfColor& oColor )
	{
		m_byteRed	= oColor.m_byteRed;
		m_byteGreen	= oColor.m_byteGreen;
		m_byteBlue	= oColor.m_byteBlue;

		m_eTheme	= oColor.m_eTheme;
		m_byteShade	= oColor.m_byteShade;
		m_byteTint	= oColor.m_byteTint;

		return (*this);
	}

	void SetHEX(int color)
	{
		SetDefault();
		
		m_byteRed	= (color&0xFF0000) >> 16;
		m_byteGreen = (color&0xFF00) >> 8;
		m_byteBlue	= (color&0xFF);
	}
	int GetRGB()const
	{
		return (m_byteRed << 16) | (m_byteGreen << 8) | m_byteBlue;
	}	
	void SetRGB(unsigned int color)
	{
		SetDefault();

		m_byteRed	= (color & 0xFF);
		m_byteGreen = (color & 0xFF00)		>> 8;
		m_byteBlue	= (color & 0xFF0000)	>> 16;
	}
	void SetRGB(BYTE red, BYTE green, BYTE blue)
	{
		SetDefault();

		m_byteRed	= red;
		m_byteGreen = green;
		m_byteBlue	= blue;
	}
	void GetHSL(double &dH, double &dS,double &dL)
	{
		RGB2HSL(m_byteRed, m_byteGreen, m_byteBlue, dH, dS, dL);
	}
	void SetHSL(double dH, double dS,double dL)
	{
		HSL2RGB(dH, dS, dL, m_byteRed, m_byteGreen, m_byteBlue);
	}
	void SetRGBPercent(int nRedPer, int nGreenPer, int nBluePer)
	{
		m_byteRed	= (BYTE)(nRedPer	* 255 / 100);
		m_byteGreen = (BYTE)(nGreenPer	* 255 / 100);
		m_byteBlue	= (BYTE)(nBluePer	* 255 / 100);
	}
    void SetHEXString(std::wstring hex)
	{
		if ( L"auto" != hex )
		{
			int color	= Strings::ToColor(hex);
		
			SetRGB(color);
		}
		else
			SetDefault();
	}
    std::wstring ToHexColor(bool bBGR = false)
	{
		if (m_bAuto) return L"auto";

        std::wstring sRed	= XmlUtils::IntToString(m_byteRed,		L"%02X");
        std::wstring sGreen = XmlUtils::IntToString(m_byteGreen,	L"%02X");;
        std::wstring sBlue	= XmlUtils::IntToString(m_byteBlue,		L"%02X");

		if (bBGR)	return sBlue + sGreen + sRed ;
		else		return sRed + sGreen + sBlue ;
	}
	
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
    BYTE SetShade(BYTE bShade)
	{
		m_byteShade = bShade;
        //return (BYTE)( ( 1.0 - m_byteShade / 255 ) * bColor );
		return bShade;
	}
	BYTE SetTint(BYTE bTint)
	{
		m_byteTint = bTint;

		double dH,  dS,  dL;
		GetHSL(dH, dS,dL);
		dL = dL * bTint / 255. + (1 - bTint / 255.); 
		SetHSL(dH, dS, dL);
		return bTint;
	 }
    static bool GetHighlightByColor( RtfColor oOutputColor,std::wstring& oStr ) //todo
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
    static RtfColor GetColorByPreset( std::wstring oStr )
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
    static std::wstring GetPresetByColor( RtfColor oCol ) //стр. 3320
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
    static bool GetThemeByString( std::wstring sTheme, _ThemeColor & oOutTheme )
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
	static bool GetThemeByOOX( SimpleTypes::EShemeColorVal val, _ThemeColor & oOutTheme )
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
    static bool GetStringByTheme( std::wstring sTheme , _ThemeColor& oOutTheme )
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

    std::wstring GetHighLight()
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
private:
	inline void RGB2HSL(unsigned char unR, unsigned char unG, unsigned char unB, double& dH, double& dS, double& dL)
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

	inline void HSL2RGB(double dH, double dS, double dL, unsigned char &unR, unsigned char& unG, unsigned char& unB)
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
	inline double Hue2RGB(double dV1, double dV2, double dH)
	{
		if ( dH < 0.0 ) dH += 1.0;
		if ( dH > 1.0 ) dH -= 1.0;
		if ( dH < c_d_1_6 ) return dV1 + ( dV2 - dV1 ) * 6.0 * dH;
		if ( dH < 0.5     ) return dV2;
		if ( dH < c_d_2_3 ) return dV1 + ( dV2 - dV1 ) * ( c_d_2_3 - dH ) * 6.0;
		return dV1;
	}
	const double c_d_1_6 = 1.0 / 6.0;
	const double c_d_1_3 = 1.0 / 3.0;
	const double c_d_2_3 = 2.0 / 3.0;
				
	std::wstring WriteOOXAttribute( std::wstring sParam )
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

};
class RtfShading
{
public: 
	enum _ShadingType
	{	st_none, 
		st_chbghoriz,	//bghoriz	Specifies a horizontal background pattern for the paragraph.
		st_chbgvert,	//bgvert	Specifies a vertical background pattern for the paragraph.
		st_chbgfdiag,	//bgfdiag	Specifies a forward diagonal background pattern for the paragraph (\\\\).
		st_chbgbdiag,	//bgbdiag	Specifies a backward diagonal background pattern for the paragraph (////).
		st_chbgcross,	//bgcross	Specifies a cross background pattern for the paragraph.
		st_chbgdcross,	//bgdcross	Specifies a diagonal cross background pattern for the paragraph.
		st_chbgdkhoriz,	//bgdkhoriz	Specifies a dark horizontal background pattern for the paragraph.
		st_chbgdkvert,	//bgdkvert	Specifies a dark vertical background pattern for the paragraph.
		st_chbgdkfdiag,	//bgdkfdiag	Specifies a dark forward diagonal background pattern for the paragraph (\\\\).
		st_chbgdkbdiag,	//bgdkbdiag	Specifies a dark backward diagonal background pattern for the paragraph (////).
		st_chbgdkcross,	//bgdkcross	Specifies a dark cross background pattern for the paragraph.
		st_chbgdkdcross,	//bgdkdcross	Specifies a dark diagonal cross background pattern for the paragraph.
		st_clshdrawnil,	//clshdrawnil	Specifies a dark diagonal cross background pattern for the paragraph.
	}	m_eType;

	int m_nValue;
	int m_nForeColor;
	int m_nBackColor;

	RtfShading()
	{
		SetDefault();
	}
	bool operator==( const RtfShading& oChar )
	{
		return m_eType == oChar.m_eType && m_nValue == oChar.m_nValue && m_nForeColor == oChar.m_nForeColor && m_nBackColor == oChar.m_nBackColor;
	}
	bool IsValid()
	{
		return true;
	}
	int GetType()
	{
		return TYPE_RTF_PROPERTY_SHADING;
	}
	void SetDefaultRtf( )
	{
		SetDefault();
	}
	void SetDefaultOOX( )
	{
		SetDefault();
	}
	void SetDefault( )
	{
		DEFAULT_PROPERTY_DEF( m_eType, st_none )
		DEFAULT_PROPERTY( m_nValue )
		DEFAULT_PROPERTY( m_nForeColor )
		DEFAULT_PROPERTY( m_nBackColor )
	}
	void Merge( RtfShading& oParPr )
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
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfShadingPar : public RtfShading
{
public: 
	int GetType()
	{
		return TYPE_RTF_PROPERTY_SHADING_PARAGRAPH;
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
};
class RtfShadingChar : public RtfShading
{
public: 
	int GetType()
	{
		return TYPE_RTF_PROPERTY_SHADING_CHAR;
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
};

class RtfShadingCell : public RtfShading
{
public: 
	int GetType()
	{
		return TYPE_RTF_PROPERTY_SHADING_CELL;
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
};

class RtfShadingRow : public RtfShading
{
public:
	int GetType()
	{
		return TYPE_RTF_PROPERTY_SHADING_CELL;
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
};

class RtfShadingTableStyle : public RtfShading
{
public: 
	int GetType()
	{
		return TYPE_RTF_PROPERTY_SHADING_TABLESTYLE;
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
};

class RtfBorder
{
public: 
	enum _BorderType
	{ 
		bt_none,
		bt_brdrs,			//brdrs	Single-thickness border.
		bt_brdrth,			//brdrth	Double-thickness border.
		bt_brdrsh,			//brdrsh	Shadowed border.
		bt_brdrdb,			//brdrdb	Double border.
		bt_brdrdot,			//brdrdot	Dotted border.
		bt_brdrdash,		//brdrdash	Dashed border.
		bt_brdrhair,		//brdrhair	Hairline border.
		bt_brdrdashsm,		//brdrdashsm	Dashed border (small).
		bt_brdrdashd,		//brdrdashd	Dot-dashed border.
		bt_brdrdashdd,		//brdrdashdd	Dot-dot-dashed border.
		bt_brdrdashdot,		//brdrdashdot	Dot-dashed border (alias for \brdrdashd read but not written by Word)
		bt_brdrinset,		//brdrinset	Inset border.
		bt_brdrnone,		//brdrnone	No border.
		bt_brdroutset,		//brdroutset	Outset border.
		bt_brdrtriple,		//brdrtriple	Triple border.
		bt_brdrtnthsg,		//brdrtnthsg	Thick-thin border (small).
		bt_brdrthtnsg,		//brdrthtnsg	Thin-thick border (small).
		bt_brdrtnthtnsg,	//brdrtnthtnsg	Thin-thick thin border (small).
		bt_brdrtnthmg,		//brdrtnthmg	Thick-thin border (medium).
		bt_brdrthtnmg,		//brdrthtnmg	Thin-thick border (medium).
		bt_brdrtnthtnmg,	//brdrtnthtnmg	Thin-thick thin border (medium).
		bt_brdrtnthlg,		//brdrtnthlg	Thick-thin border (large).
		bt_brdrthtnlg,		//brdrthtnlg	Thin-thick border (large).
		bt_brdrtnthtnlg,	//brdrtnthtnlg	Thin-thick-thin border (large).
		bt_brdrwavy,		//brdrwavy	Wavy border.
		bt_brdrwavydb,		//brdrwavydb	Double wavy border.
		bt_brdrdashdotstr,	//brdrdashdotstr	Striped border.
		bt_brdremboss,		//brdremboss	Embossed border.
		bt_brdrengrave,		//brdrengrave	Engraved border.
	}	m_eType;
	
	int m_nWidth;
	int m_nSpace;
	int m_nColor;

	RtfBorder()
	{
		SetDefault();
	}
	bool operator==( const RtfBorder& oChar )
	{
		return m_eType == oChar.m_eType && m_nWidth == oChar.m_nWidth && m_nSpace == oChar.m_nSpace && m_nColor == oChar.m_nColor;
	}
	bool IsValid()
	{
		return bt_none != m_eType;
	}
	int GetType()
	{
		return TYPE_RTF_PROPERTY_BORDER;
	}
	void SetDefaultRtf( )
	{
		SetDefault();
	}
	void SetDefaultOOX( )
	{
		SetDefault();
	}
	void SetDefault( )
	{
		DEFAULT_PROPERTY_DEF( m_eType, bt_none )
		DEFAULT_PROPERTY	( m_nWidth )
		DEFAULT_PROPERTY	( m_nSpace )
		DEFAULT_PROPERTY	( m_nColor )
	}
	void SetEmpty( )
	{
		m_eType		= bt_brdrnone;
		m_nWidth	= 0;
		m_nSpace	= 0;
		m_nColor	= -1; //auto
	}
	void Merge( RtfBorder& oBorPr )
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
	
    static bool GetStringRtfByType( _BorderType nValue, std::wstring& sValue )
	{
		sValue = L"\\brdrs";
		//switch( nValue )
		//{
		//default: sResult = L"\\brdrs"; break;
		////}
		return false;
	}
    static std::wstring GetStringOOXByType( _BorderType nValue, std::wstring& sValue )
	{
        std::wstring sResult;
		sResult = L"single";
		//switch( nValue )
		//{
		//default: sResult = L"single"; break;
		//}
		return sResult;
	}
};

class RtfTab : public IRenderableProperty
{
public:
	enum _TabLeader
	{	tl_none,
		tl_dot ,	//tldot 	Absolute position tab with a leading that uses period symbols (.....).
		tl_mdot ,	//tlmdot 	Absolute position tab with a leading that uses middle dot symbols (•••••).
		tl_hyph ,	//tlhyph 	Absolute position tab with a leading that uses minus symbols (-----).
		tl_ul ,		//tlul 		Absolute position tab with a leading that uses underscore symbols (_____).
	}m_eLeader;

	enum _TabKind
	{	tk_none,
		tk_tql,
		tk_tqr,		//tqr	Flush-right tab.
		tk_tqc,		//tqc	Centered tab.
		tk_tqdec,	//tqdec	Decimal tab.
		tk_tqbar,	//tbN	Bar tab position in twips from the left margin.
		tk_tqclear
	}	m_eKind;
	
	int m_nTab;		//tbN or \txN	Tab position in twips from the left margin.

	RtfTab()
	{
		SetDefault();
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
	{
		DEFAULT_PROPERTY	( m_nTab );
		DEFAULT_PROPERTY_DEF( m_eLeader,	tl_none );
		DEFAULT_PROPERTY_DEF( m_eKind,		tk_tql );
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfTabs: public IRenderableProperty
{
public: 
	std::vector<RtfTab> m_aTabs;
	RtfTabs()
	{
		SetDefault();
	}
	RtfTabs( const RtfTabs& oTabs )
	{
		SetDefault();
	}
	const RtfTabs& operator=( const RtfTabs& oTabs )
	{
		Merge( oTabs );
		return (*this);
	}
	void Merge( const RtfTabs& oTabs )
	{
		m_aTabs.clear();
		m_aTabs = oTabs.m_aTabs ;
	}
	void SetDefault()
	{
		m_aTabs.clear();
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter)
	{
        std::wstring sResult;
		for (size_t i = 0; i < (int)m_aTabs.size(); i++ )
		{
			sResult += m_aTabs[i].RenderToRtf( oRenderParameter );
		}
		return sResult;
	}
    std::wstring RenderToOOX(RenderParameter oRenderParameter)
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
};
//----------------------------------------------------------------------------------------------------------
class RtfCharProperty;
typedef boost::shared_ptr<RtfCharProperty> RtfCharPropertyPtr;

class RtfCharProperty: public IRenderableProperty
{
public: 
	bool m_bListLevel;

	enum _UnderlineStyle {uls_none, uls_Single, uls_Dotted,uls_Dashed ,uls_Dash_dotted,uls_Dash_dot_dotted,uls_Double,uls_Heavy_wave,uls_Long_dashe,uls_Stops_all,uls_Thick,uls_Thick_dotted,uls_Thick_dashed,uls_Thick_dash_dotted,uls_Thick_dash_dot_dotted,uls_Thick_long_dashed,uls_Double_wave,uls_Word,uls_Wave};

	int m_nAnimated;		//animtextN	Animated text properties (note: Word 2007 ignores this control word):

	int m_bBold;			//b*	Bold.
	int m_bCaps;			//caps*	All capitals.
	int m_nScalex;			//charscalexN 	Character scaling value. The N argument is a value representing a percentage (default is 100).
	int m_nCharStyle;		//csN	Designates character style. If a character style is specified, style properties must be specified with the character run. N refers to an entry in the style table.
	int m_nDown;			//dnN	Move down N half-points (default is 6).
	int m_bEmbo;			//embo*	Emboss.
	int m_nCharacterSpacing;//expndtwN	Expansion or compression of the space between characters in twips; a negative value compresses. For backward compatibility, both \expndtwN and \expndN should be emitted.		
	int m_nFitText;			//fittextN	Fit the text in the current group in N twips. When N is set to -1 (\fittext-1), it indicates a continuation of the previous \fittextN run. In other words, {\fittext1000 Fit this} {\fittext-1 text} fits the string “Fit this text” in 1000 twips.
	int m_nFont;			//fN	Font number. N refers to an entry in the font table.
	int m_nFont2; 
	int m_nFont3; 
	int m_nFontSize;		//fsN	Font size in half-points (default is 24).
	int m_bItalic;			//i*	Italic.
	int m_bImprint;			//impr*	Engrave (imprint).
	int m_nKerning;			//kerningN	Point size (in half-points) above which to kern character pairs. \kerning0 turns off kerning.
	int m_bRightToLeft;		//rtlch	Character data following this control word is treated as a right-to-left run.
	int m_nLanguage;
	int m_nLanguageAsian;	// for East Asian text
	int m_nComplexScript;	// 1 - means South East Asian complex script; 0 - means not South East Asian script
	int m_bOutline;			//outl*	Outline.
	int m_bScaps;			//scaps*	Small capitals.
	int m_bShadow;			//shad*	Shadow.
	int m_bStrike;			//strike*	Strikethrough.
	int m_nStriked;			//striked1	Double strikethrough. \striked0 turns it off.
	int m_bSub;				//sub	Subscripts text and shrinks point size according to font information.
	int m_bSuper;			//super	Superscripts text and shrinks point size according to font information.
	int m_bHidden;			//v*	Hidden text.
	int m_nHightlited; 

	int				m_nForeColor; 

	int				m_nCrAuth;
	int				m_nCrDate;

	int				m_nDeleted;
	int				m_nRevised;
	int				m_nRevauth;
	int				m_nRevdttm;
	int				m_nRevauthDel;
	int				m_nRevdttmDel;

	int				m_nInsrsid;

//	int				m_bUnderline;					//ul*	Continuous underline. \ul0 turns off all underlining.
	_UnderlineStyle	m_eUnderStyle;		//
	int				m_nUnderlineColor;	//
	int				m_nUp;				//upN	Move up N half-points (default is 6).

	RtfCharPropertyPtr	m_pOldCharProp;
	RtfBorder			m_poBorder;
	RtfShadingChar		m_poShading;
	
	bool operator==( const RtfCharProperty& oChar )
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

	RtfCharProperty()
	{
		m_bListLevel = false;
		SetDefault();
	}
	int GetType()
	{
		return TYPE_RTF_PROPERTY_CHAR;
	}
	void SetDefaultRtf()
	{
		SetDefault();
		
		m_poShading.SetDefaultRtf();
		m_poBorder.SetDefaultRtf();
		
		if (false == m_bListLevel)
			m_nFontSize = 24;
	}
	void SetDefaultOOX()
	{
		SetDefault();
		
		m_poShading.SetDefaultOOX();
		m_poBorder.SetDefaultOOX();
		
		if (false == m_bListLevel)
			m_nFontSize = 20;
	}
	void SetDefault()
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
	void Merge( RtfCharProperty& oCharPr, bool bAll = true )
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfListLevelProperty : IRenderableProperty
{
public: 
	int		m_nLevel;			///OOX
	int		m_nNumberType;		//levelnfcN \levelnfcnN Specifies the number type for the level
	int		m_bTentative;		//lvltentative 	Specifies that a given numbering level was been saved by a producer but was not used in the parent document. This means that this numbering level may be redefined by a future consumer without changing the actual content of the document.
	int		m_nJustification;	//leveljcN \leveljcnN	0	Left justified 1	Center justified 2	Right justified
	int		m_nFollow;			//levelfollowN
	int		m_nStart;			//levelstartatN	N specifies the start-at value for the level.
    std::wstring m_sText;			//как в rtf текст, но сдвинут от нуля на 1 // \'03\'00.\'01 -> ("%d%d%d",4,1,2)
    std::wstring m_sNumber;
	int		m_nNoRestart;		//levelnorestartN	1 if this level does not restart its count each time a super ordinate level is incremented; 0 if this level does restart its count each time a super ordinate level is incremented.
	int		m_nLegal;			//levellegalN	1 if any list numbers from previous levels should be converted to Arabic numbers; 0 if they should be left with the format specified by their own level’s definition.
	int		m_nPictureIndex;	//levelpictureN	Determines which picture bullet from the \listpicture destination should be applied.

	RtfTabs m_oTabs;			//ParagraphProp
	int		m_nFirstIndent;
	int		m_nIndent;
	int		m_nIndentStart;
	int		m_nSpace;
	
	RtfCharProperty m_oCharProp; //Char

	RtfListLevelProperty()
	{
		m_oCharProp.m_bListLevel = true;

		SetDefault();
	}
	bool IsValid()
	{
		//return -1 != m_nFollow && -1 != m_nStart && -1 != m_nNumberType && -1 != m_nJustification &&
        //	false == m_sText.empty() && false == m_sNumber.empty();
        return  PROP_DEF != m_nNumberType && false == m_sText.empty();
	}
    std::wstring GenerateListText()
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
	void SetDefault()
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
    std::wstring RenderToOOX2(RenderParameter oRenderParameter, int lvl = PROP_DEF);

    static std::wstring GetFormat( int nNumFormat)
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
    static int GetFormat( std::wstring sFormat)
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
    std::wstring GetLevelTextOOX();
    void SetLevelTextOOX(std::wstring sText);
};

class RtfListProperty : public IRenderableProperty, public ItemContainer<RtfListLevelProperty>
{
public: 
	int		m_nID;		//listidN	Each list must have a unique list ID that should be randomly generated. N is a long integer. The list ID cannot be between –1 and –5. 
	int		m_nTemplateId;		//listtemplateidN	Each list should have a unique template ID as well, which also should be randomly generated. The template ID –1 means the template ID is undefined. N is a long integer. 
	int		m_nListSimple;		//listsimpleN	1 if the list has one level; 0 (default) if the list has nine levels.
	int		m_bListHybrid;		//listhybrid	Present if the list has 9 levels, each of which is the equivalent of a simple list. Only one of \listsimpleN and \listhybrid should be present. Word 2000 and newer versions will write lists with the \listhybrid property.
    std::wstring m_sName;		//listname	The argument for \listname is a string that is the name of this list. Names allow ListNum fields to specify the list to which they belong. This is a destination control word.

	RtfListProperty()
	{
		SetDefault();
	}
	void SetDefault()
	{
		DEFAULT_PROPERTY	( m_nID )
		DEFAULT_PROPERTY	( m_nTemplateId )
		DEFAULT_PROPERTY	( m_nListSimple )
		DEFAULT_PROPERTY	( m_bListHybrid )
		DEFAULT_PROPERTY_DEF( m_sName, L"" )

		m_aArray.clear();
	}
	bool IsValid()
	{
		return (PROP_DEF != m_nID) ;		//&& (L"" != m_sName);
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
class RtfListOverrideProperty : IRenderableProperty
{
public: 
	class ListOverrideLevels : IRenderableProperty
	{
	public: 
		class ListOverrideLevel
		{
			public:
				int m_nLevelIndex;	//listoverrideformat	Number of list format override levels within this list override (should be either 1, 9, or missing, which means 0).
				int m_nStart;		//listoverridestartat	Indicates an override of the start-at value.
				RtfListLevelProperty m_oLevel;
				ListOverrideLevel()
				{
					m_nLevelIndex = PROP_DEF;
					m_nStart = PROP_DEF;
				}
		};
		std::vector<ListOverrideLevel> m_aOverrideLevels;
		ListOverrideLevels()
		{
		}
		ListOverrideLevels( const ListOverrideLevels& oOverLevel )
		{
			(*this) = oOverLevel;
		}
		ListOverrideLevels& operator=( const ListOverrideLevels& oOverLevel )
		{
			m_aOverrideLevels.clear();
			m_aOverrideLevels = oOverLevel.m_aOverrideLevels;
			return (*this);
		}
		void SetDefault()
		{
			m_aOverrideLevels.clear();
		}
        std::wstring RenderToRtf(RenderParameter oRenderParameter)
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
        std::wstring RenderToOOX(RenderParameter oRenderParameter)
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
	};
	int m_nListID;		//listidN	Should exactly match the \listid of one of the lists in the List table. The value N is a long integer.
	int m_nIndex;		//lsN	The (1-based) index of this \listoverride in the \listoverride table. This value should never be zero inside a \listoverride and must be unique for all \listoverride’s within a document. The valid values are from 1 to 2000. The value 0 means no list.
	
	ListOverrideLevels m_oOverrideLevels;

	RtfListOverrideProperty()
	{
		SetDefault();
	}
	bool IsValid()
	{
		return PROP_DEF != m_nListID && PROP_DEF != m_nIndex;
	}
	void SetDefault()
	{
		DEFAULT_PROPERTY( m_nListID )
		DEFAULT_PROPERTY( m_nIndex )
		m_oOverrideLevels.SetDefault();
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};


class RtfStyle;
typedef boost::shared_ptr<RtfStyle> RtfStylePtr;

class RtfStyle: public IRenderableProperty
{
public: 
	enum _StyleType { st_none, stParagraph, stCharacter, stSection, stTable, stNumbering} ;

	_StyleType		m_eType;
    std::wstring	m_sName;
	int				m_nID;
    std::wstring	m_sID;

	int m_bAdditive;
	int m_nBasedOn;
	int m_nNext;
	int m_bHidden;
	int m_nLink;
	int m_bLocked;
	int m_bPersonal;
	int m_bCompose;
	int m_bReply;
	int m_nSemiHidden;
	int m_bQFormat;
	int m_nPriority;
	int m_bUnhiddenWhenUse;

	RtfStyle()
	{
		SetDefault();
	}
	bool IsValid()
	{
		return PROP_DEF != m_nID;
	}
	int GetType()
	{
		return TYPE_RTF_PROPERTY_STYLE;
	}
	void SetDefaultRtf()
	{
		SetDefault();
		m_eType = stParagraph;
		m_nID = 0;
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
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

	virtual void Merge( RtfStylePtr oStyle )
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

	bool operator==( const RtfStyle& oProperty )
	{
		return m_eType == oProperty.m_eType && m_nID == oProperty.m_nID && m_nBasedOn == oProperty.m_nBasedOn && 
			m_nNext == oProperty.m_nNext && m_bHidden == oProperty.m_bHidden && m_nLink == oProperty.m_nLink && 
			m_bLocked == oProperty.m_bLocked && m_bPersonal == oProperty.m_bPersonal && m_bCompose == oProperty.m_bCompose && 
			m_bReply == oProperty.m_bReply && m_nSemiHidden == oProperty.m_nSemiHidden && m_bQFormat == oProperty.m_bQFormat && 
			m_nPriority == oProperty.m_nPriority && m_bUnhiddenWhenUse == oProperty.m_bUnhiddenWhenUse;
	}
    std::wstring RenderToRtfBegin( RenderParameter oRenderParameter )
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
    std::wstring RenderToRtfEnd( RenderParameter oRenderParameter );
    std::wstring RenderToOOXBegin(RenderParameter oRenderParameter);
    std::wstring RenderToOOXEnd(RenderParameter oRenderParameter);
    std::wstring RenderToRtf(RenderParameter oRenderParameter)
	{
		return L"";
	}
    std::wstring RenderToOOX(RenderParameter oRenderParameter)
	{
		return L"";
	}
};

class RtfTableProperty: public IRenderableProperty
{
public: 
	enum _HRef
	{
		hr_none,
		hr_phmrg,	// tphmrg	Use margin as horizontal reference frame.
		hr_phpg,	//tphpg	Use page as horizontal reference frame.
		hr_phcol	//tphcol	Use column as horizontal reference frame. This is the default if no horizontal table positioning information is given.
	} ;
	enum _VRef
	{
		vr_none,
		vr_pvmrg,	//tpvmrg	Position table vertically relative to the top margin. This is the default if no vertical table positioning information is given.
		vr_pvpg,	//tpvpg	Position table vertically relative to the top of the page.
		vr_pvpara	//tpvpara	Position table vertically relative to the upper left corner of the next unframed paragraph in the stream.
	};
	enum _HPos
	{
		hp_none, 
		hp_posxc,	//tposxc	Center table within the horizontal reference frame.
		hp_posxi,	//tposxi	Position table inside the horizontal reference frame.
		hp_posxo,	//tposxo	Position table outside the horizontal reference frame.
		hp_posxl,	//tposxl	Position table at the left of the horizontal reference frame.
		hp_posxr	//tposxr	Position table at the right of the horizontal reference frame.
	};
	enum _VPos
	{
		vp_none,
		vp_posyt,	//tposyt	Position table at the top of the vertical reference frame.
		vp_posyil,	//tposyil	Position table to be inline.
		vp_posyb,	//tposyb	Position table at the bottom of the vertical reference frame.
		vp_posyc,	//tposyc	Center table within the vertical reference frame
		vp_posyin,	//tposyin	Position table inside within the vertical reference frame.
		vp_posyout	//tposyout	Position table outside within the vertical reference frame.
	};

	int m_bBidi;			//taprtl	Table direction is right to left.
	int m_nAutoFit;			//trautofitN	AutoFit:0	No AutoFit (default).1	AutoFit is on for the row. Overridden by \clwWidthN and \trwWidthN in any table row
	int m_nGraph;			//trgaphN	Half the space between the cells of a table row in twips.

	int nTableIndent;		//tblindN 
	int eTableIndentUnit;	// tblindtypeN  

	enum _RowJust
	{
		rj_none,
		rj_trql,		//trql	Left-justifies a table row with respect to its containing column.
		rj_trqr,		//trqr	Right-justifies a table row with respect to its containing column.
		rj_trqc			//trqc	Centers a table row with respect to its containing column.
	}	m_eJust; 

	int m_nWrapLeft;		//tdfrmtxtLeftN	Distance in twips, between the left of the table and surrounding text (default is 0).
	int m_nWrapRight;		//tdfrmtxtRightN	Distance in twips, between the right of the table and surrounding text (default is 0).
	int m_nWrapTop;			//tdfrmtxtTopN	Distance in twips, between the top of the table and surrounding text (default is 0).
	int m_nWrapBottom;		//tdfrmtxtBottomN	Distance in twips, between the bottom of the table and surrounding text (default is 0).
	int m_bOverlap;			//tabsnoovrlp	Do not allow table to overlap with other tables or shapes with similar wrapping not contained within it.

	_HRef			m_eHRef;
	_VRef			m_eVRef;
	_HPos			m_eHPos;
	int				m_nHPos;				//tposxN	Position table N twips from the left edge of the horizontal reference frame.
	_VPos			m_eVPos;
	int				m_nVPos;				//tposyN	Position table N twips from the top edge of the vertical reference frame.

	int				m_nLeft;
	
	int				m_nWidth;				//trwWidthN	Width of invisible cell at the end of the row. Used only when rows have different widths.
	int				m_eWidthUnit;			//trftsWidthN	Units for \trwWidthN:

	int m_nDefCellMarBottom;	//trpaddbN	Default bottom cell margin or padding for the row.
	int m_nDefCellMarLeft;		//trpaddlN	Default left cell margin or padding for the row.
	int m_nDefCellMarRight;		//trpaddrN	Default right cell margin or padding for the row.
	int m_nDefCellMarTop;		//trpaddtN	Default top cell margin or padding for the row.

	int m_eDefCellMarBottomUnit; //Units for \trpaddbN: 0	Null. Ignore \trpaddbN in favor of \trgaphN (Word 97 style padding). 3	Twips.
	int m_eDefCellMarLeftUnit;
	int m_eDefCellMarRightUnit;
	int m_eDefCellMarTopUnit;

	int m_nDefCellSpBottom;		//trspdbN	Default bottom cell spacing for the row. The total vertical spacing between adjacent cells is equal to the sum of \trspdtN from the bottom cell and \trspdbN from the top cell, both of which will have the same value when written by Word.
	int m_nDefCellSpLeft;		//trspdlN
	int m_nDefCellSpRight;		//trspdrN
	int m_nDefCellSpTop;		//trspdtN

	int m_eDefCellSpBottomUnit; //trspdfbN Units for \trspdbN: 0	Null. Ignore \trspdbN. 3	Twips.
	int m_eDefCellSpLeftUnit;
	int m_eDefCellSpRightUnit;
	int m_eDefCellSpTopUnit;

	RtfBorder m_oBorderLeft;
	RtfBorder m_oBorderRight;
	RtfBorder m_oBorderTop;
	RtfBorder m_oBorderBottom;
	RtfBorder m_oBorderVert;
	RtfBorder m_oBorderHor;

	RtfShadingRow m_oShading;
	int m_nStyle;

//Autoformatting Flags
	int m_bAutoFirstRow;	//tbllkhdrrows	Flag sets table autoformat to format the first (header) row.
	int m_bAutoLastRow;		//tbllklastrow	Flag sets table autoformat to format the last row.
	int m_bAutoFirstCol;	//tbllkhdrcols	Flag sets table autoformat to format the first (header) column.
	int m_bAutoLastCol;		//tbllklastcol	Flag sets table autoformat to format the last column.
	int m_bAutoNoRowBand;	//tbllknorowband	Specifies row banding conditional formatting shall not be applied.
	int m_bAutoNoColBand;	//tbllknocolband	Specifies column banding conditional formatting shall not be applied.

	int m_nRowBandSize;		//tscbandshN	Count of rows in a row band
	int m_nColBandSize;		//tscbandsvN	Count of cells in a cell band


	RtfTableProperty()
	{
		SetDefault();
	}

	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
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
	bool IsValid()
	{
		return true;
	}
	void Merge( RtfTableProperty& oTablePr )
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfFrame : public IRenderableProperty
{
public:
	enum _HRef
	{
		hr_none,
		hr_phmrg,
		hr_phpg,
		hr_phcol
	};
	enum _VRef
	{
		vr_none,
		vr_pvmrg,
		vr_pvpg,
		vr_pvpara
	};
	enum _HPos
	{
		hp_none, 
		hp_posxc,
		hp_posxi,
		hp_posxo,
		hp_posxl,
		hp_posxr 
	};
	enum _VPos
	{
		vp_none,
		vp_posyt,
		vp_posyil,
		vp_posyb,
		vp_posyc,
		vp_posyin,
		vp_posyout
	};
	enum _TextWrap
	{
		tw_none,
		tw_wrapdefault,
		tw_wraparound,
		tw_wraptight,
		tw_wrapthrough
	};


	int			m_nWidth;
	int			m_nHeight;
	_HRef		m_eHRef;
	_VRef		m_eVRef;
	_HPos		m_eHPos;
	int			m_nHPos;
	_VPos		m_eVPos;
	int			m_nVPos;
	int			m_bLockAnchor;
	_TextWrap	m_eWrap;
	int			m_DropcapType;
	int			m_DropcapLines;
	int			m_nHorSpace;		// dxfrtextN	Distance in twips of a positioned paragraph from text in the main text flow in all directions.
	int			m_nVerSpace;		// dfrmtxtxN	N is the horizontal distance in twips from text on both sides of the frame.
	int			m_nAllSpace;		// dfrmtxtyN	N is the vertical distance in twips from text on both sides of the frame.

	RtfFrame()
	{
		SetDefault();
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
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
	void Merge( RtfFrame& oFramePr )
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
	
	void ApplyParagraphProp( RtfTableProperty& oProp )
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
};

class RtfCellProperty;
typedef boost::shared_ptr<RtfCellProperty>	RtfCellPropertyPtr;

class RtfCellProperty: public IRenderableProperty
{	
public:
	int m_bMergeFirst;			//clmgf		The first cell in a range of table cells to be merged.
	int m_bMerge;				//clmrg		Contents of the table cell are merged with those of the preceding cell.
	int m_bMergeFirstVertical;	//clvmgf	The first cell in a range of table cells to be vertically merged.
	int m_bMergeVertical;		//clvmrg	Contents of the table cell are vertically merged with those of the preceding cell.

	int m_bFitText;				//clFitText	Fit text in cell, compressing each paragraph to the width of the cell.
	int m_bNoWrap;				//clNoWrap	Do not wrap text for the cell. Only has an effect if the table cell does not have a preferred \clwWidthN, which overrides \trautofitN.

	int m_nPaddingLeft;			//clpadlN	Left cell margin or padding. Overrides \trpaddlN.
	int m_ePaddingLeftUnit;		//clpadflN	Units for \clpadlN:
	int m_nPaddingRight;
	int m_ePaddingRightUnit;	
	int m_nPaddingTop;
	int m_ePaddingTopUnit;	
	int m_nPaddingBottom;
	int m_ePaddingBottomUnit;	

	int m_nSpacingLeft;			//clsplN	Left cell margin or padding. Overrides \trspdlN.
	int m_eSpacingLeftUnit;		//clspflN	Units for \clsplN:
	int m_nSpacingRight;
	int m_eSpacingRightUnit;
	int m_nSpacingTop;
	int m_eSpacingTopUnit;
	int m_nSpacingBottom;
	int m_eSpacingBottomUnit;

	int				m_nWidth;			//clwWidthN	Preferred cell width. Overrides \trautofitN.
	int				m_eWidthUnit;		//clftsWidthN	Units for \clwWidthN:

	int				m_bHideMark;		//clhidemark	This control word specifies whether the end of cell glyph shall influence the height of the given table row in the table. If it is specified, then only printing characters in this cell shall be used to determine the row height.

	RtfBorder m_oBorderDiagonalLR;
	RtfBorder m_oBorderDiagonalRL;
	RtfBorder m_oBorderLeft;
	RtfBorder m_oBorderTop;
	RtfBorder m_oBorderRight;
	RtfBorder m_oBorderBottom;
	RtfBorder m_oBorderInsideH;
	RtfBorder m_oBorderInsideV;

	RtfShadingCell	m_oShading;
	int				m_nShadingPctFrom;

	typedef enum{ ca_none, 
					ca_Top,	//clvertalt	Text is top-aligned in cell (the default).
					ca_Center,	//clvertalc	Text is centered vertically in cell.
					ca_Bottom //clvertalb	Text is bottom-aligned in cell.
				} CellAlign;
	int m_eAlign;
	typedef enum{ cf_none,
					cf_lrtb,	//cltxlrtb	Text in a cell flows from left to right and top to bottom (default).
					cf_tbrl,	//cltxtbrl	Text in a cell flows right to left and top to bottom.
					cf_btlr,	//cltxbtlr	Text in a cell flows left to right and bottom to top.
					cf_lrtbv,	//cltxlrtbv	Text in a cell flows left to right and top to bottom, vertical.
					cf_tbrlv//cltxtbrlv	Text in a cell flows top to bottom and right to left, vertical.
				} CellFlow;
	CellFlow m_oCellFlow;

	int m_nCellx;		//cellxN	Defines the right boundary of a table cell, including its half of the space between cells.
	int m_nSpan;

//Table Style Specific
	int m_bStyleFirstRow;		// tscfirstrow	This cell is in the first row.
	int m_bStyleLastRow;		// tsclastrow	This cell is in the last row.
	int m_bStyleFirstCol;		// tscfirstcol	This cell is in the first column.
	int m_bStyleLastCol;		// tsclastcol	This cell is in the last column.
	int m_bStyleOddRowBand;		// tscbandhorzodd	This cell is in the odd row band.
	int m_bStyleEvenRowBand;		// tscbandhorzeven	This cell is in the even row band.
	int m_bStyleOddColBand;		// tscbandvertodd	This cell is in the odd column band.
	int m_bStyleEvenColBand;		// tscbandverteven	This cell is in the even column band.
	int m_bStyleNWCell;		// tscnwcell	This is the NW (north west) cell in the table (upper left).
	int m_bStyleNECell;		// tscnecell	NE cell
	int m_bStyleSWCell;		// tscswcell	SW cell.
	int m_bStyleSECell;		// tscsecell	SE cell.

	RtfCellProperty()
	{
		SetDefault();
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
		m_nSpan = 1;
	}
	void SetDefault()
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
	bool IsValid()
	{
		return PROP_DEF != m_nCellx;
	}

	void Merge( RtfCellProperty& oCellPr )
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
    bool GetAlignFromStringRtf(  std::wstring & oAlign, CellAlign& oOutput )
	{
		if ( L"clvertalt" == oAlign ) { oOutput = ca_Top; return true;}
		else if ( L"clvertalc" == oAlign ){ oOutput = ca_Center; return true;}
		else if ( L"clvertalb" == oAlign ){ oOutput = ca_Bottom; return true;}
		return false;
	}
    bool GetAlignFromStringOOX(  std::wstring & oAlign, CellAlign& oOutput )
	{
		if ( L"top" == oAlign ) { oOutput = ca_Top; return true;}
		else if ( L"center" == oAlign ){ oOutput = ca_Center; return true;}
		else if ( L"bottom" == oAlign ){ oOutput = ca_Bottom; return true;}
		return false;
	}

};

class RtfRowProperty;
typedef boost::shared_ptr<RtfRowProperty>	RtfRowPropertyPtr;

class RtfRowProperty: public RtfTableProperty, public ItemContainer< RtfCellProperty >
{
public: 
	int m_nRightToLeft; //bi direction 
	int m_nIndex;		//irowN	N is the row index of this row.
	int m_nBandIndex;	//irowbandN	N is the row index of the row, adjusted to account for header rows. A header row has a value of –1.

	int m_bLastRow;		//lastrow	Output if this is the last row in the table.

	int m_bIsHeader;	//trhdr	Table row header. This row should appear at the top of every page on which the current table appears.
	int m_bKeep;		//trkeep	Keep table row together. This row cannot be split by a page break. This property is assumed to be off unless the control word is present.

	int m_nHeight;		//trrhN	Height of a table row in twips. When 0, the height is sufficient for all the text in the line; when positive, the height is guaranteed to be at least the specified height; when negative, the absolute value of the height is used, regardless of the height of the text in the line.

	int	m_nWidthStartInvCell;		//trwWidthBN	Width of invisible cell at the beginning of the row. Used only in cases where rows have different widths.
	int	m_eWidthStartInvCellUnit;	//trftsWidthBN	Units for \trwWidthBN:

	int	m_nWidthEndInvCell;			//trwWidthAN	Width of invisible cell at the end of the row. Used only when rows have different widths.
	int	m_eWidthEndInvCellUnit;		//trftsWidthAN	Units for \trwWidthAN:

	int m_nGridBefore;	//для oox
	int m_nGridAfter;	//для oox

	int m_bStyleFirstRow;		// tscfirstrow	This cell is in the first row.
	int m_bStyleLastRow;		// tsclastrow	This cell is in the last row.
	int m_bStyleFirstCol;		// tscfirstcol	This cell is in the first column.
	int m_bStyleLastCol;		// tsclastcol	This cell is in the last column.
	int m_bStyleOddRowBand;		// tscbandhorzodd	This cell is in the odd row band.
	int m_bStyleEvenRowBand;	// tscbandhorzeven	This cell is in the even row band.
	int m_bStyleOddColBand;		// tscbandvertodd	This cell is in the odd column band.
	int m_bStyleEvenColBand;	// tscbandverteven	This cell is in the even column band.
	int m_bStyleNWCell;			// tscnwcell	This is the NW (north west) cell in the table (upper left).
	int m_bStyleNECell;			// tscnecell	NE cell
	int m_bStyleSWCell;			// tscswcell	SW cell.
	int m_bStyleSECell;			// tscsecell	SE cell.

	int m_nTrAuth;
	int m_nTrDate;

	RtfRowPropertyPtr	m_pOldRowProperty;

	RtfRowProperty()
	{
		SetDefault();
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
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
	//5.5 Доверенность_MO_Q139.rtf
	void SetDefault()
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
	bool IsValid()
	{
		return true;
	}
	void Merge( RtfRowProperty& oRowPr )
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

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

//----------------------------------------------------------------------------------------------------------
class RtfParagraphProperty;
typedef boost::shared_ptr<RtfParagraphProperty>		RtfParagraphPropertyPtr;

class RtfParagraphProperty: public IRenderableProperty
{
public: 
	bool					m_bHidden;
	bool					m_bOldList;
	RtfParagraphPropertyPtr m_pOldParagraphProp;
//-------------
	int		m_bAutoHyphenation;	//hyphpar	Switches automatic hyphenation for the paragraph. Append 1 or nothing to toggle property on; append 0 to turn it off.
	int		m_bInTable;			//intbl	Paragraph is part of a table.
	int		m_nItap;			//itapN	Paragraph nesting level, where 0 is the main document, 1 is a table cell, 2 is a nested table cell, 3 is a doubly nested table cell, and so forth (default is 1).
	int		m_bKeep;			//keep	Keep paragraph intact (completely on one page if possible).
	int		m_bKeepNext;		//keepn	Keep paragraph with the next paragraph.
	int		m_bPageBB;			//pagebb	Break page before the paragraph.
	int		m_nOutlinelevel;	// outlinelevelN	Outline level of paragraph. The N argument is a value from 0 to 8 representing the outline level of the paragraph. In the default case, no outline level is specified (same as body text).
	int		m_nStyle;			//sN	Designates paragraph style. If a paragraph style is specified, style properties must be specified with the paragraph. N references an entry in the style sheet.

	typedef enum 
	{
		pa_none,
		pa_qc,					//qc	Centered.
		pa_qj,					//qj	Justified.
		pa_ql,					//ql	Left-aligned (the default).
		pa_qr,					//qr	Right-aligned.
		pa_qd,					//qd	Distributed.
		pa_qk0,					//qkN	Percentage of line occupied by Kashida justification (0 – low, 10 – medium, 20 – high).
		pa_qk10,
		pa_qk20,
	} ParagraphAlign;
	
	int m_eAlign;
	
	typedef enum 
	{
		fa_none,
		fa_faauto,				//faauto	Font alignment. The default setting for this is "Auto."
		fa_fahang,				//fahang	Font alignment: Hanging.
		fa_facenter,			//facenter	Font alignment: Center.
		fa_faroman,				//faroman	Font alignment: Roman (default).
		fa_favar,				//favar	Font alignment: Upholding variable.
		fa_fafixed,				//fafixed	Font alignment: Upholding fixed.
	} FontAlign;
	
	FontAlign m_eFontAlign;

	int m_nIndFirstLine;		//fiN	First-line indent in twips (default is 0).
	int m_nIndLeft;				//liN	Left indent in twips (default is 0).
	int m_nIndRight;			//riN	Right indent in twips (default is 0).
	int m_nIndStart;			//linN	Left indent for left-to-right paragraphs; right indent for right-to-left paragraphs (default is 0). \linN defines space before the paragraph.
	int m_nIndEnd;				//rinN	Right indent for left-to-right paragraphs; left indent for right-to-left paragraphs (default is 0). \rinN defines space after the paragraph.
	int m_bIndRightAuto;		//adjustright	Automatically adjust right indent when document grid is defined.
	int m_bIndMirror;			//indmirror...

	int m_nSpaceBefore;			//sbN	Space before in twips (default is 0).
	int m_nSpaceAfter;			//saN	Space after in twips (default is 0).
	int m_nSpaceBeforeAuto;		//sbautoN	Auto spacing before:
	int m_nSpaceAfterAuto;		//saautoN	Auto spacing after:
	int m_nSpaceBeforeLine;		//lisbN	Space before in hundredths of a character unit. Overrides \sbN, although they should both be emitted with equivalent values.
	int m_nSpaceAfterLine;		//lisaN	Space after in hundredths of a character unit. Overrides \saN, although they should both be emitted with equivalent values.
	int m_nSpaceBetween;		//slN	Space between lines. If this control word is missing or if \sl0 is used, the line spacing is automatically determined by the tallest character in the line. If N is a positive value, this size is used only if it is taller than the tallest character (otherwise, the tallest character is used); if N is a negative value, the absolute value of N is used, even if it is shorter than the tallest character.
	int m_nSpaceMultiLine;		//slmultN
	int m_bContextualSpacing;	//contextualspace

	int m_bRtl;					//rtlpar	Text in this paragraph will display with right-to-left precedence.
	int m_bNoWordWrap;			//nowwrap
	int m_bSnapToGrid;			//nosnaplinegrid	Disable snap line to grid.

	enum _TextBoxWrap
	{
		tbw_none,
		tbw_txbxtwno,			//txbxtwno
		tbw_txbxtwalways,		//txbxtwalways
		tbw_txbxtwfirstlast,	//txbxtwfirstlast
		tbw_txbxtwfirst,		//txbxtwfirst
		tbw_txbxtwlast,			//txbxtwlast
	}			m_eTextBoxWrap;
	int			m_nListId;					//lsN	Should exactly match the \lsN for one of the list overrides in the List Override table.
	int			m_nListLevel;				//ilvlN	The 0-based level of the list to which the paragraph belongs. For all simple lists, N should always be 0. For multilevel lists, it can be 0 through 8. The value 9 is never used. The values 10 through 12 have the special meanings for documents generated by Word 6: 10 = ilvlBullet (a bulleted paragraph in Word 6), 11 = ilvlList (a numbered paragraph in Word 6), 12 = ilvlContinue (a paragraph that was not itself numbered, but took its indenting scheme from its numbering properties and did not “break” numbering (that in Word 6 required otherwise contiguous paragraphs).

	RtfShadingPar m_oShading;

	RtfBorder m_oBorderTop;
	RtfBorder m_oBorderLeft;
	RtfBorder m_oBorderBottom;
	RtfBorder m_oBorderRight;
	RtfBorder m_oBorderBox;
	RtfBorder m_oBorderBar;

	RtfFrame	m_oFrame;
	int			m_bOverlap;				//1\absnoovrlpN	Allow overlap with other frames or objects with similar wrapping:
	enum _TextFollow
	{
		tf_none,
		tf_frmtxlrtb,		//frmtxlrtb	Frame box flows from left to right and top to bottom (default).
		tf_frmtxtbrl,		//frmtxtbrl	Frame box flows right to left and top to bottom.
		tf_frmtxbtlr,		//frmtxbtlr	Frame box flows left to right and bottom to top.
		tf_frmtxlrtbv,		//frmtxlrtbv	Frame box flows left to right and top to bottom, vertical.
		tf_frmtxtbrlv		//frmtxtbrlv	Frame box flows top to bottom and right to left, vertical.
	}			m_eTextFollow;
	RtfTabs		m_oTabs;

//Table Style Specific
	int m_nTableStyle;			// ytsN	Designates the table style handle that was applied to the row/cell.
	int m_bStyleFirstRow;		// tscfirstrow	This cell is in the first row.
	int m_bStyleLastRow;		// tsclastrow	This cell is in the last row.
	int m_bStyleFirstCollumn;	// tscfirstcol	This cell is in the first column.
	int m_bStyleLastCollumn;	// tsclastcol	This cell is in the last column.
	int m_bStyleOddRowBand;		// tscbandhorzodd	This cell is in the odd row band.
	int m_bStyleEvenRowBand;	// tscbandhorzeven	This cell is in the even row band.
	int m_bStyleOddColBand;		// tscbandvertodd	This cell is in the odd column band.
	int m_bStyleEvenColBand;	// tscbandverteven	This cell is in the even column band.
	int m_bStyleNWCell;			// tscnwcell	This is the NW (north west) cell in the table (upper left).
	int m_bStyleNECell;			// tscnecell	NE cell.
	int m_bStyleSWCell;			// tscswcell	SW cell.
	int m_bStyleSECell;			// tscsecell	SE cell.
	
	int m_nPrAuth;
	int m_nPrDate;
	
	RtfCharProperty		m_oCharProperty;
//--------------------------------------------------------------------------------------------------------------
	RtfParagraphProperty()
	{
		SetDefault();
	}
	bool IsValid()
	{
		return true;
	}
	void SetDefaultRtf()
	{
		SetDefault();
		
		//4.1 Наряд_R7_M133.rtf
		m_nSpaceAfter = 0;
		m_nSpaceBetween = 240;
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
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
	void Merge( RtfParagraphProperty& oParPr )
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
//class RtfParagraphPropertyWithTable: public RtfParagraphProperty
//{
//public: RtfRowProperty m_oRowProperty;
//public: RtfCellProperty m_oCellProperty;
//
//public: RtfParagraphPropertyWithTable()
//		{
//			SetDefault();
//		}
//public: void SetDefaultRtfPar()
//		{
//			RtfParagraphProperty::SetDefaultRtf();
//		}
//public: void SetDefaultRtfTable()
//		{
//			m_oRowProperty.SetDefaultRtf();
//		}
//public: void SetDefault()
//		{
//			RtfParagraphProperty::SetDefault();
//			m_oRowProperty.SetDefault();
//		}
//public: std::wstring RenderToRtf(RenderParameter oRenderParameter)
//		{
//			std::wstring sResult;
//			sResult += m_oRowProperty.RenderToRtf(  oRenderParameter  );
//			sResult += L" {" + RtfParagraphProperty::RenderToRtf(  oRenderParameter  ) + L" }";
//			return sResult;
//		}
//};
class RtfTime: public IRenderableProperty
{
public: 
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nHour;
	int m_nMin;
	int m_nSecond;
	RtfTime()
	{
		SetDefault();
	}
	bool IsValid()
	{
		return true;
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
	{
		DEFAULT_PROPERTY( m_nYear )
		DEFAULT_PROPERTY( m_nMonth )
		DEFAULT_PROPERTY( m_nDay )
		DEFAULT_PROPERTY( m_nHour )
		DEFAULT_PROPERTY( m_nMin )
		DEFAULT_PROPERTY( m_nSecond )
	}
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
class RtfInformation: public IRenderableProperty
{
public:
    std::wstring m_sTitle;
    std::wstring m_sSubject;
    std::wstring m_sAuthor;
    std::wstring m_sManager;
    std::wstring m_sCompany;
    std::wstring m_sOperator;
    std::wstring m_sCategory;
    std::wstring m_sKeywords;
    std::wstring m_sComment;
    std::wstring m_sDocCom;
    std::wstring m_sLinkBase;
	
	std::wstring m_sApplication;

	RtfTime m_oCreateTime;
	RtfTime m_oRevTime;
	RtfTime m_oPrintTime;
	RtfTime m_oBackupTime;

	int m_nVersion;
	int m_nInternalVersion;
	int m_nEndingTime;

	int m_nNumberOfPages;
	int m_nNumberOfWords;
	int m_nNumberOfCharactersWithSpace;
	int m_nNumberOfCharactersWithoutSpace;
	int m_nInternalId;
	RtfInformation()
	{
		SetDefault();
	}
	bool IsValid()
	{
		return true;
	}
	void SetDefaultRtf()
	{
		SetDefault();
	}
	void SetDefaultOOX()
	{
		SetDefault();
	}
	void SetDefault()
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
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfCharStyle: public RtfStyle
{
public: 
	RtfCharProperty m_oCharProp;

	RtfCharStyle()
	{
	   SetDefault();
	   m_eType = stCharacter;
	}
	int GetType()
	{
		return TYPE_RTF_PROPERTY_STYLE_CHAR;
	}
	void Merge( RtfStylePtr oStyle );
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
class RtfParagraphStyle: public RtfCharStyle
{
public: 
	RtfParagraphProperty m_oParProp;

	RtfParagraphStyle()
	{
	   SetDefault();
	   m_eType = stParagraph;
	}
	int GetType()
	{
		return TYPE_RTF_PROPERTY_STYLE_PARAGRAPH;
	}
	void Merge( RtfStylePtr oStyle );
	
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
//---------------------------------------------------------------------------------------
class RtfTableStyle;
typedef boost::shared_ptr<RtfTableStyle> RtfTableStylePtr;

class RtfTableStyle: public RtfParagraphStyle
{
public: 
	RtfTableProperty	m_oTableProp;
	RtfRowProperty		m_oRowProp;
	RtfCellProperty		m_oCellProp;


	RtfTableStylePtr m_oFirstRow;
	RtfTableStylePtr m_oLastRow;
	RtfTableStylePtr m_oFirstCol;
	RtfTableStylePtr m_oLastCol;
	RtfTableStylePtr m_oBandHorEven;
	RtfTableStylePtr m_oBandVerEven;
	RtfTableStylePtr m_oBandHorOdd;
	RtfTableStylePtr m_oBandVerOdd;
	RtfTableStylePtr m_oNWCell;
	RtfTableStylePtr m_oNECell;
	RtfTableStylePtr m_oSWCell;
	RtfTableStylePtr m_oSECell;
	RtfTableStyle()
	{
		SetDefault();
		m_eType = stTable;
	}
	void SetDefault()
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
	int GetType()
	{
		return TYPE_RTF_PROPERTY_STYLE_TABLE;
	}
	void Merge( RtfStylePtr oStyle );
	
    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};


//typedef boost::shared_ptr<RtfTableStyleProperty>	RtfTableStylePropertyPtr;
typedef boost::shared_ptr<RtfCharStyle>				RtfCharStylePtr;
typedef boost::shared_ptr<RtfParagraphStyle>		RtfParagraphStylePtr;
typedef boost::shared_ptr<RtfInformation>			RtfInformationPtr;
typedef boost::shared_ptr<RtfTime>					RtfTimePtr;
typedef boost::shared_ptr<RtfRowProperty>			RtfRowPropertyPtr;
typedef boost::shared_ptr<RtfCellProperty>			RtfCellPropertyPtr;
typedef boost::shared_ptr<RtfFrame>					RtfFramePtr;
typedef boost::shared_ptr<RtfTableProperty>			RtfTablePropertyPtr;
typedef boost::shared_ptr<RtfTab>					RtfTabPtr;

typedef boost::shared_ptr<RtfListOverrideProperty>	RtfListOverridePropertyPtr;
typedef boost::shared_ptr<RtfListProperty>			RtfListPropertyPtr;
typedef boost::shared_ptr<RtfListLevelProperty>		RtfListLevelPropertyPtr;
typedef boost::shared_ptr<RtfBorder>				RtfBorderPtr;
typedef boost::shared_ptr<RtfShadingTableStyle>		RtfShadingTableStylePtr;
typedef boost::shared_ptr<RtfShadingRow>			RtfShadingRowPtr;
typedef boost::shared_ptr<RtfShadingCell>			RtfShadingCellPtr;
typedef boost::shared_ptr<RtfShadingChar>			RtfShadingCharPtr;
typedef boost::shared_ptr<RtfShadingPar>			RtfShadingParPtr;
typedef boost::shared_ptr<RtfShading>				RtfShadingPtr;
typedef boost::shared_ptr<RtfColor>					RtfColorPtr;
typedef boost::shared_ptr<RtfFont>					RtfFontPtr;
