/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
	//		//Todo  tx1 - may come as text1
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
	//			//from themeColor
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

