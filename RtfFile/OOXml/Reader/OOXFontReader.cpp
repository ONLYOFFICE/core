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

#include "OOXFontReader.h"

OOXFontReader::OOXFontReader(OOX::CFont* ooxFont)
{
	m_ooxFont = ooxFont;
}
bool OOXFontReader::Parse( ReaderParameter oParam, RtfFont& oOutputFont)
{
	if (!m_ooxFont) return false;

	std::wstring sFontName = m_ooxFont->m_sName;
	if( sFontName.length() < 1 ) return false;

	oOutputFont.m_sName = sFontName;
	oOutputFont.m_nID = oParam.oRtf->m_oFontTable.GetCount() + 1;

	if( m_ooxFont->m_oAltName.IsInit() )
		oOutputFont.m_sAltName = m_ooxFont->m_oAltName.get2();
	else if(m_ooxFont->m_oPanose.IsInit() )
		oOutputFont.m_sPanose = m_ooxFont->m_oPanose->ToString();
	else if( m_ooxFont->m_oFamily.IsInit() )
	{
		switch(m_ooxFont->m_oFamily->GetValue())
		{
		case SimpleTypes::fontfamilyDecorative	: oOutputFont.m_eFontFamily = RtfFont::ff_fdecor;		break;
		case SimpleTypes::fontfamilyModern		: oOutputFont.m_eFontFamily = RtfFont::ff_fmodern;		break;
		case SimpleTypes::fontfamilyRoman		: oOutputFont.m_eFontFamily = RtfFont::ff_froman;		break;
		case SimpleTypes::fontfamilyScript		: oOutputFont.m_eFontFamily = RtfFont::ff_fscript;		break;
		case SimpleTypes::fontfamilySwiss		: oOutputFont.m_eFontFamily = RtfFont::ff_fswiss;		break;
		case SimpleTypes::fontfamilyAuto		:
		default									: oOutputFont.m_eFontFamily =RtfFont::ff_fnil;			break;
		}
	}
	else if( m_ooxFont->m_oPitch.IsInit())
	{
		switch(m_ooxFont->m_oPitch->GetValue())
		{
		case SimpleTypes::pitchFixed    : oOutputFont.m_nPitch = 1;	break;
		case SimpleTypes::pitchVariable : oOutputFont.m_nPitch = 2;	break;
		case SimpleTypes::pitchDefault  :
		default							: oOutputFont.m_nPitch = 0;	break;
		}
	}
	else if( m_ooxFont->m_oCharset.IsInit() )
	{
		oOutputFont.m_nCharset = m_ooxFont->m_oCharset->GetValue();
	}
	return true;
}

OOXFontReader2::OOXFontReader2(ComplexTypes::Word::CFonts * ooxFont)
{
	m_ooxFont		= ooxFont;
}
bool OOXFontReader2::Parse( ReaderParameter oParam, int& nFont)
{
	if (!m_ooxFont) return false;

	std::wstring sAscii, sAsciiTheme, sCs, sCsTheme, sEastAsia, sEastAsiaTheme, sHAnsi, sHAnsiTheme;

	if (m_ooxFont->m_sAscii.IsInit())			sAscii = m_ooxFont->m_sAscii.get2();
	if (m_ooxFont->m_oAsciiTheme.IsInit())		sAsciiTheme = m_ooxFont->m_oAsciiTheme->ToString();

	if (m_ooxFont->m_sCs.IsInit())				sCs = m_ooxFont->m_sCs.get2();
	if (m_ooxFont->m_oCsTheme.IsInit())			sCsTheme = m_ooxFont->m_oCsTheme->ToString();

	if (m_ooxFont->m_sEastAsia.IsInit())		sEastAsia = m_ooxFont->m_sEastAsia.get2();
	if (m_ooxFont->m_oEastAsiaTheme.IsInit())	sEastAsiaTheme = m_ooxFont->m_oEastAsiaTheme->ToString();

	if (m_ooxFont->m_sHAnsi.IsInit())			sHAnsi = m_ooxFont->m_sHAnsi.get2();
	if (m_ooxFont->m_oHAnsiTheme.IsInit())		sHAnsiTheme = m_ooxFont->m_oHAnsiTheme->ToString();

	std::wstring sFont;
	std::wstring sTempFont;

	bool bFont = false;
	if( !sAscii.empty() )
	{
		bFont = true;
		sFont = sAscii;
	}
	else if( !sAsciiTheme.empty() && !GetThemeFont(sAsciiTheme, *oParam.oReader).empty() )
	{
		bFont = true;
		sFont = GetThemeFont(sAsciiTheme, *oParam.oReader);
	}
	else if( !sHAnsi.empty() )
	{
		bFont = true;
		sFont = sHAnsi;
	}
	else if( !sHAnsiTheme.empty() && !GetThemeFont(sHAnsiTheme, *oParam.oReader).empty() )
	{
		bFont = true;
		sFont = GetThemeFont(sHAnsiTheme, *oParam.oReader);
	}
	else if( !sCs.empty()  )
		sFont = sCs;
	else if( !sCsTheme.empty()  && !GetThemeFont(sCsTheme, *oParam.oReader).empty() )
		sFont = GetThemeFont(sCsTheme, *oParam.oReader);
	else if( !sEastAsia.empty() )
		sFont = sEastAsia;
	else if( !sEastAsiaTheme.empty() && !GetThemeFont(sEastAsiaTheme, *oParam.oReader).empty() )
		sFont = GetThemeFont(sEastAsiaTheme, *oParam.oReader);


	if( !sFont.empty() )
	{
		RtfFont oCurFont;
		if( true == oParam.oRtf->m_oFontTable.GetFont( sFont, oCurFont ) )
		{
			if (bFont)
				nFont = oCurFont.m_nID;
		}
		else
		{
			oCurFont.m_sName = sFont;
			oCurFont.m_nID = oParam.oRtf->m_oFontTable.GetCount() + 1;
			oParam.oRtf->m_oFontTable.AddItem( oCurFont );

			if (bFont)
				nFont = oCurFont.m_nID;
		}
	}
	return true;
}
std::wstring OOXFontReader2::GetThemeFont( std::wstring sTheme, OOXReader & oReader )
{
	std::wstring sFont;
	if		( L"majorAscii"		== sTheme )	sFont = oReader.m_smajorAscii;
	else if ( L"majorBidi"		== sTheme )	sFont = oReader.m_smajorBidi;
	else if ( L"majorEastAsia"	== sTheme )	sFont = oReader.m_smajorEastAsia;
	else if ( L"majorHAnsi"		== sTheme )	sFont = oReader.m_smajorHAnsi;
	else if ( L"minorAscii"		== sTheme )	sFont = oReader.m_sminorAscii;
	else if ( L"minorBidi"		== sTheme )	sFont = oReader.m_sminorBidi;
	else if ( L"minorEastAsia"	== sTheme )	sFont = oReader.m_sminorEastAsia;
	else if ( L"minorHAnsi"		== sTheme )	sFont = oReader.m_sminorHAnsi;

	return sFont;
}

OOXFontReader3::OOXFontReader3(PPTX::Logic::TextFont * asciiFont, PPTX::Logic::TextFont * asianFont, PPTX::Logic::TextFont * csFont)
{
	m_asciiFont	= asciiFont;
	m_asianFont	= asianFont;
	m_csFont	= csFont;
}
bool OOXFontReader3::Parse( ReaderParameter oParam, int& nFont)
{
	if (!m_asciiFont && !m_csFont && !m_asianFont) return false;

	std::wstring sAscii, sCs, sEastAsia;

	if (m_asciiFont)	sAscii		= m_asciiFont->typeface;
	if (m_csFont)		sCs			= m_csFont->typeface;
	if (m_asianFont)	sEastAsia	= m_asianFont->typeface;

	std::wstring sFont;
	std::wstring sTempFont;

	if( !sAscii.empty() )		sFont = sAscii;
	else if( !sCs.empty() )			sFont = sCs;
	else if( !sEastAsia.empty() )	sFont = sEastAsia;


	if( !sFont.empty() )
	{
		RtfFont oCurFont;
		if( true == oParam.oRtf->m_oFontTable.GetFont( sFont, oCurFont ) )
			nFont = oCurFont.m_nID;
		else
		{
			nFont = oParam.oRtf->m_oFontTable.GetCount() + 1;
			oCurFont.m_nID		= nFont;
			oCurFont.m_sName	= sFont;

			//if( !sAscii.empty() )
			//{
			//	if (m_asciiFont->m_oPanose.IsInit())
			//		oCurFont.m_sPanose	= m_asciiFont->m_oPanose->GetValue();
			//	if (m_asciiFont->charset.IsInit())
			//		oCurFont.m_nCharset	= m_asciiFont->charset.get();
			//	oCurFont.m_nPitch	= m_asciiFont->m_oPitchFamily.GetValue();
			//}
			//else if( !sCs.empty() )
			//{
			//	if (m_csFont->m_oPanose.IsInit())
			//		oCurFont.m_sPanose	= m_csFont->m_oPanose->GetValue();
			//	oCurFont.m_nCharset = m_csFont->m_oCharset.GetValue();
			//	oCurFont.m_nPitch	= m_csFont->m_oPitchFamily.GetValue();
			//}
			//else if( !sEastAsia.empty() )
			//{
			//	if (m_asianFont->m_oPanose.IsInit())
			//		oCurFont.m_sPanose	= m_asianFont->m_oPanose->GetValue();
			//	oCurFont.m_nCharset = m_asianFont->m_oCharset.GetValue();
			//	oCurFont.m_nPitch	= m_asianFont->m_oPitchFamily.GetValue();
			//}
			oParam.oRtf->m_oFontTable.AddItem( oCurFont );
		}
	}
	return true;
}
std::wstring OOXFontReader3::GetThemeFont( std::wstring sTheme, OOXReader & oReader )
{
	std::wstring sFont;
	if		( L"majorAscii"		== sTheme )	sFont = oReader.m_smajorAscii;
	else if ( L"majorBidi"		== sTheme )	sFont = oReader.m_smajorBidi;
	else if ( L"majorEastAsia"	== sTheme )	sFont = oReader.m_smajorEastAsia;
	else if ( L"majorHAnsi"		== sTheme )	sFont = oReader.m_smajorHAnsi;
	else if ( L"minorAscii"		== sTheme )	sFont = oReader.m_sminorAscii;
	else if ( L"minorBidi"		== sTheme )	sFont = oReader.m_sminorBidi;
	else if ( L"minorEastAsia"	== sTheme )	sFont = oReader.m_sminorEastAsia;
	else if ( L"minorHAnsi"		== sTheme )	sFont = oReader.m_sminorHAnsi;

	return sFont;
}
