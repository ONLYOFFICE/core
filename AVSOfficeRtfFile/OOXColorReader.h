#pragma once
#include "RtfProperty.h"
#include "OOXReader.h"
#include "OOXReaderBasic.h"

	class OOXColorReader
	{
	public: OOXColorReader()
			{
			}
	public: bool Parse( ReaderParameter oParam, RtfColor& oOutputColor )
			{	
				XmlUtils::CXmlReader oXmlReader;
				if( FALSE == oXmlReader.OpenFromXmlNode( oParam.oNode ) )
					return false;
				oXmlReader.ReadNode( _T("*") );
				CString sNodeName = oXmlReader.ReadNodeName();
				if( _T("a:hslClr") == sNodeName )
				{
					CString sHue = oXmlReader.ReadNodeAttribute( _T("hue"), _T("") );
					CString sLum = oXmlReader.ReadNodeAttribute( _T("lum"), _T("") );
					CString sSat = oXmlReader.ReadNodeAttribute( _T("sat"), _T("") );
					double dHue = Strings::ToInteger(sHue) / 21600000;
					double dSat = Strings::ToInteger(sLum) / 100;
					double dLum = Strings::ToInteger(sSat) / 100;
					oOutputColor.SetHSL( dHue, dSat, dLum );
					return true;
				}
				else if( _T("a:prstClr") == sNodeName)
				{
					CString sPreset = oXmlReader.ReadNodeAttribute( _T("val"), _T("") );
					oOutputColor = RtfColor::GetColorByPreset( sPreset );
					return true;
				}
				else if( _T("a:schemeClr") == sNodeName)
				{
					CString sTheme = oXmlReader.ReadNodeAttribute( _T("val"), _T("") );
					RtfColor::ThemeColor oTheme;
					if( true == RtfColor::GetThemeByString( sTheme, oTheme ))
						return oParam.oDocument->m_oColorTable.GetColor( oTheme, oOutputColor );
					return false;
				}
				else if( _T("a:srgbClr") == sNodeName)
				{
					CString sHex = oXmlReader.ReadNodeAttribute( _T("val"), _T("") );
					oOutputColor.SetHEXString( sHex );
					return true;
				}
				else if( _T("a:scrgbClr") == sNodeName)
				{
					int nRed = Strings::ToInteger( oXmlReader.ReadNodeAttribute( _T("r"), _T("") ) );
					int nGreen = Strings::ToInteger( oXmlReader.ReadNodeAttribute( _T("g"), _T("") ) );
					int nBlue = Strings::ToInteger( oXmlReader.ReadNodeAttribute( _T("b"), _T("") ) );
					oOutputColor.SetRGBPercent(nRed,nGreen,nBlue);
					return true;
				}
				else if( _T("a:sysClr") == sNodeName)
				{
					CString sHex = oXmlReader.ReadNodeAttribute( _T("lastClr"), _T("") );
					if(  _T("") != sHex )
					{
						oOutputColor.SetHEXString(sHex);
						return true;
					}
					else
						return false;
					
				}
				else
				{
					oXmlReader.OpenFromXmlNode(oParam.oNode);
					CString sThemeColor = oXmlReader.ReadNodeAttribute( _T("w:themeColor"), _T("") );
					CString themeShade = oXmlReader.ReadNodeAttribute( _T("w:themeShade"), _T("255") );
					CString themeTint = oXmlReader.ReadNodeAttribute( _T("w:themeTint"), _T("0") );
					CString val = oXmlReader.ReadNodeAttribute( _T("w:val"), _T("") );
					if( "" != sThemeColor && true == oParam.oDocument->m_oColorTable.GetColor( sThemeColor , oOutputColor ))
					{
						//из themeColor
						int nShade = Strings::ToInteger( themeShade );
						if( nShade < 0 )
							nShade = 0;
						if( nShade > 255 )
							nShade = 255;

						int nTint = Strings::ToInteger( themeTint );
						if( nTint < 0 )
							nTint = 0;
						if( nTint > 255 )
							nTint = 255;

						oOutputColor.SetTint( nTint );
						oOutputColor.SetShade( nShade );
						return true;
					}
					else if( _T("") != val )
					{
						oOutputColor.SetHEXString(val);
						return true;
					}
				}
				return false;

			}
	};
