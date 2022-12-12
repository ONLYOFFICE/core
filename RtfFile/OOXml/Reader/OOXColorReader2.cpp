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

#include "OOXColorReader2.h"

OOXColorReader2::OOXColorReader2( )
{
}
bool OOXColorReader2::Parse( ReaderParameter oParam, RtfColor& oOutputColor,std::wstring sColor, std::wstring sTheme,std::wstring sThemeShade, std::wstring sThemeTint )
{
	//XmlUtils::CXmlLiteReader oXmlReader;
	//if( TRUE == oXmlReader.OpenFromXmlNode( oParam.oNode ) )
	//{

	//	std::wstring sThemeColor = oXmlReader.ReadNodeAttribute(sTheme, L"" );
	//	std::wstring themeShade = oXmlReader.ReadNodeAttribute(sThemeShade, L"0" );
	//	std::wstring themeTint = oXmlReader.ReadNodeAttribute(sThemeTint, L"255" );
	//	std::wstring val = oXmlReader.ReadNodeAttribute(sColor, L"" );
	//	if( !sThemeColor.empty() )
	//	{
	//		//Todo  tx1 - моджет приходить text1
	//		RtfColor::ThemeColor oTheme = RtfColor::TC_NONE;
	//		if( sThemeColor == L"accent1" )
	//			oTheme = RtfColor::caccentone;
	//		else if( sThemeColor ==  L"accent2" )
	//			oTheme = RtfColor::caccenttwo;
	//		else if( sThemeColor == L"accent3" )
	//			oTheme = RtfColor::caccentthree;
	//		else if( sThemeColor == L"accent4" )
	//			oTheme = RtfColor::caccentfour;
	//		else if( sThemeColor == L"accent5" )
	//			oTheme = RtfColor::caccentfive;
	//		else if( sThemeColor == L"accent6" )
	//			oTheme = RtfColor::caccentsix;
	//		else if( sThemeColor == L"bg1" )
	//			oTheme = RtfColor::cbackgroundone;
	//		else if( sThemeColor == L"bg2" )
	//			oTheme = RtfColor::cbackgroundtwo;
	//		else if( sThemeColor == L"dk1" )
	//			oTheme = RtfColor::cmaindarkone;
	//		else if( sThemeColor == L"dk2" )
	//			oTheme = RtfColor::cmaindarktwo;
	//		else if( sThemeColor == L"folHlink" )
	//			oTheme = RtfColor::cfollowedhyperlink;
	//		else if( sThemeColor == L"hlink" )
	//			oTheme = RtfColor::chyperlink;
	//		else if( sThemeColor == L"lt1" )
	//			oTheme = RtfColor::cmainlightone;
	//		else if( sThemeColor == L"lt2" )
	//			oTheme = RtfColor::cmainlighttwo;
	//		else if( sThemeColor == L"phClr" )
	//			oTheme = RtfColor::cmainlighttwo;
	//		else if( sThemeColor == L"tx1" )
	//			oTheme = RtfColor::ctextone;
	//		else if( sThemeColor =  L"tx2" )
	//			oTheme = RtfColor::ctexttwo;

	//		if( true == oParam.oRtf->m_oColorTable.GetColor( oTheme , oOutputColor ) )
	//		{
	//			//из themeColor
	//			int nShade = Strings::ToInteger( themeShade );
	//			if( nShade < 0 )
	//				nShade = 0;
	//			if( nShade > 255 )
	//				nShade = 255;

	//			int nTint = Strings::ToInteger( themeTint );
	//			if( nTint < 0 )
	//				nTint = 0;
	//			if( nTint > 255 )
	//				nTint = 255;

	//			oOutputColor.SetTint( nTint );
	//			oOutputColor.SetShade( nShade );
	//			return true;
	//		}
	//		else if( false == val.empty() )
	//		{
	//			oOutputColor.SetHEXString( val );
	//			return true;
	//		}
	//		else
	//			return false;

	//	}
	//	else if( L"auto" != val )
	//	{
	//		oOutputColor.SetHEXString( val );
	//		return true;
	//	}
	//	else
	//		return false;

	//}
	return false;

}

