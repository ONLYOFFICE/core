
#pragma once
#include "RtfDocument.h"
#include "RtfProperty.h"


class OOXColorReader2
{
public: 
	OOXColorReader2( )
	{
	}
	bool Parse( ReaderParameter oParam, RtfColor& oOutputColor,CString sColor, CString sTheme,CString sThemeShade, CString sThemeTint )
	{	
		//XmlUtils::CXmlLiteReader oXmlReader;
		//if( TRUE == oXmlReader.OpenFromXmlNode( oParam.oNode ) )
		//{

		//	CString sThemeColor = oXmlReader.ReadNodeAttribute(sTheme, _T("") );
		//	CString themeShade = oXmlReader.ReadNodeAttribute(sThemeShade, _T("0") );
		//	CString themeTint = oXmlReader.ReadNodeAttribute(sThemeTint, _T("255") );
		//	CString val = oXmlReader.ReadNodeAttribute(sColor, _T("") );
		//	if( _T("") != sThemeColor )
		//	{
		//		//Todo  tx1 - моджет приходить text1
		//		RtfColor::ThemeColor oTheme = RtfColor::TC_NONE;
		//		if( sThemeColor == _T("accent1") )
		//			oTheme = RtfColor::caccentone;
		//		else if( sThemeColor ==  _T("accent2") )
		//			oTheme = RtfColor::caccenttwo;
		//		else if( sThemeColor == _T("accent3") )
		//			oTheme = RtfColor::caccentthree;
		//		else if( sThemeColor == _T("accent4") )
		//			oTheme = RtfColor::caccentfour;
		//		else if( sThemeColor == _T("accent5") )
		//			oTheme = RtfColor::caccentfive;
		//		else if( sThemeColor == _T("accent6") )
		//			oTheme = RtfColor::caccentsix;
		//		else if( sThemeColor == _T("bg1") )
		//			oTheme = RtfColor::cbackgroundone;
		//		else if( sThemeColor == _T("bg2") )
		//			oTheme = RtfColor::cbackgroundtwo;
		//		else if( sThemeColor == _T("dk1") )
		//			oTheme = RtfColor::cmaindarkone;
		//		else if( sThemeColor == _T("dk2") )
		//			oTheme = RtfColor::cmaindarktwo;
		//		else if( sThemeColor == _T("folHlink") )
		//			oTheme = RtfColor::cfollowedhyperlink;
		//		else if( sThemeColor == _T("hlink") )
		//			oTheme = RtfColor::chyperlink;
		//		else if( sThemeColor == _T("lt1") )
		//			oTheme = RtfColor::cmainlightone;
		//		else if( sThemeColor == _T("lt2") )
		//			oTheme = RtfColor::cmainlighttwo;
		//		else if( sThemeColor == _T("phClr") )
		//			oTheme = RtfColor::cmainlighttwo;
		//		else if( sThemeColor == _T("tx1") )
		//			oTheme = RtfColor::ctextone;
		//		else if( sThemeColor =  _T("tx2") )
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
		//		else if( false == val.IsEmpty() )
		//		{
		//			oOutputColor.SetHEXString( val );
		//			return true;
		//		}
		//		else
		//			return false;

		//	}
		//	else if( _T("auto") != val )
		//	{
		//		oOutputColor.SetHEXString( val );
		//		return true;
		//	}
		//	else 
		//		return false;

		//}
		return false;

	}
};
