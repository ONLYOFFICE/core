#pragma once
#include "OOXReader.h"
#include "RtfDocument.h"
#include "OOXReaderBasic.h"

class OOXFontReader
{
public: OOXFontReader()
		{
		}
public: bool Parse( ReaderParameter oParam, RtfFont& oOutputFont)
		{
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
			{
				CString sNodeName = oXmlReader.ReadNodeName();
				if( _T("w:font") == sNodeName )
				{
					CString sFontName = oXmlReader.ReadNodeAttribute( _T("w:name"), _T("") );
					if( _T("") != sFontName  )
					{
						oOutputFont.m_sName = sFontName;
						oOutputFont.m_nID = oParam.oDocument->m_oFontTable.GetCount() + 1;
						oXmlReader.ReadNodeList( _T("*") );
						for(int i=0; i<oXmlReader.GetLengthList();i++ )
						{
							CString sNodeName = oXmlReader.ReadNodeName(i);

							if( sNodeName == _T("w:altName") )	
								oOutputFont.m_sAltName = oXmlReader.ReadNodeAttribute( i, _T("w:name"), _T("") );
							else if(sNodeName == _T("w:panose1") )
								oOutputFont.m_sPanose = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
							else if( sNodeName == _T("w:family") )
							{
								CString sFontFamily = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
								if( _T("auto") == sFontFamily )
									oOutputFont.m_eFontFamily =RtfFont::ff_fnil;
								else if( _T("roman") == sFontFamily )
									oOutputFont.m_eFontFamily = RtfFont::ff_froman;
								else if( _T("swiss") == sFontFamily )
									oOutputFont.m_eFontFamily = RtfFont::ff_fswiss;
								else if( _T("modern") == sFontFamily )
									oOutputFont.m_eFontFamily = RtfFont::ff_fmodern;
								else if( _T("script") == sFontFamily )
									oOutputFont.m_eFontFamily = RtfFont::ff_fscript;
								else if( _T("decorative") == sFontFamily )
									oOutputFont.m_eFontFamily = RtfFont::ff_fdecor;
							}
							else if( sNodeName == _T("w:pitch") )
							{
								CString sPitch = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
								if( _T("default") == sPitch )
									oOutputFont.m_nPitch = 0;
								else if( _T("fixed") == sPitch )
									oOutputFont.m_nPitch = 1;
								else if( _T("variable") == sPitch )
									oOutputFont.m_nPitch = 2;
							}
							else if( sNodeName == _T("w:charset") )
							{
								CString sCharset = oXmlReader.ReadNodeAttribute( i, _T("w:val"), _T("") );
								if( _T("") != sCharset )
									oOutputFont.m_nCharset = Convert::ToInt32( sCharset, 16 );
							}
						}
						return true;
					}
				}
			}
			return false;
		}
};

class OOXFontReader2
{
public: OOXFontReader2()
		{
		}
public: bool Parse( ReaderParameter oParam, int& nFont)
		{
			XmlUtils::CXmlReader oXmlReader;
			if( TRUE ==  oXmlReader.OpenFromXmlNode(oParam.oNode) )
			{
				CString sNodeName = oXmlReader.ReadNodeName();
				if( _T("w:oOutputFont") == sNodeName || _T("w:rFonts") == sNodeName)
				{
					CString sAscii = oXmlReader.ReadNodeAttribute( _T("w:ascii"), _T("") );
					CString sAsciiTheme = oXmlReader.ReadNodeAttribute( _T("w:asciiTheme"), _T("") );
					CString sCs = oXmlReader.ReadNodeAttribute( _T("w:cs"), _T("") );
					CString sCsTheme = oXmlReader.ReadNodeAttribute( _T("w:cstheme"), _T("") );
					CString sEastAsia = oXmlReader.ReadNodeAttribute( _T("w:eastAsia"), _T("") );
					CString sEastAsiaTheme = oXmlReader.ReadNodeAttribute( _T("w:eastAsiaTheme"), _T("") );
					CString sHAnsi = oXmlReader.ReadNodeAttribute( _T("w:hAnsi"), _T("") );
					CString sHAnsiTheme = oXmlReader.ReadNodeAttribute( _T("w:hAnsiTheme"), _T("") );

					CString sFont = _T("");	
					CString sTempFont;
					if( _T("") != sAscii )
						sFont = sAscii;
					else if( _T("") != sAsciiTheme && _T("") != GetThemeFont(sAsciiTheme, *oParam.oReader) )
						sFont = GetThemeFont(sAsciiTheme, *oParam.oReader);
					else if( _T("") != sHAnsi )
						sFont = sHAnsi;
					else if( _T("") != sHAnsiTheme && _T("") != GetThemeFont(sHAnsiTheme, *oParam.oReader) )
						sFont = GetThemeFont(sHAnsiTheme, *oParam.oReader);
					else if( _T("") != sCs )
						sFont = sCs;
					else if( _T("") != sCsTheme && _T("") != GetThemeFont(sCsTheme, *oParam.oReader) )
						sFont = GetThemeFont(sCsTheme, *oParam.oReader);
					else if( _T("") != sEastAsia )
						sFont = sEastAsia;
					else if( _T("") != sEastAsiaTheme && _T("") != GetThemeFont(sEastAsiaTheme, *oParam.oReader) )
						sFont = GetThemeFont(sEastAsiaTheme, *oParam.oReader);


					if( _T("") != sFont )
					{
						RtfFont oCurFont;
						if( true == oParam.oDocument->m_oFontTable.GetFont( sFont, oCurFont ) )
							nFont = oCurFont.m_nID;
						else
						{
							oCurFont.m_sName = sFont;
							nFont = oParam.oDocument->m_oFontTable.GetCount() + 1;
							oCurFont.m_nID = nFont;
							oParam.oDocument->m_oFontTable.AddItem( oCurFont );
						}
					}

				}
			}
			return false;
		}
private: CString GetThemeFont( CString sTheme, OOXReader & oReader )
	 {
		CString sFont;
		if( _T("majorAscii") == sTheme )
			sFont = oReader.m_smajorAscii;
		else if( _T("majorBidi") == sTheme )
			sFont = oReader.m_smajorBidi;
		else if( _T("majorEastAsia") == sTheme )
			sFont = oReader.m_smajorEastAsia;
		else if( _T("majorHAnsi") == sTheme )
			sFont = oReader.m_smajorHAnsi;
		else if( _T("minorAscii") == sTheme )
			sFont = oReader.m_sminorAscii;
		else if( _T("minorBidi") == sTheme )
			sFont = oReader.m_sminorBidi;
		else if( _T("minorEastAsia") == sTheme )
			sFont = oReader.m_sminorEastAsia;
		else if( _T("minorHAnsi") == sTheme )
			sFont = oReader.m_sminorHAnsi;
		return sFont;
	 }
};
