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
#pragma once

#include "OOXReader.h"
#include "OOXReaderBasic.h"

#include "../RtfDocument.h"

class OOXFontReader
{
private:
	OOX::CFont* m_ooxFont;
public: 
	OOXFontReader(OOX::CFont* ooxFont)
	{
		m_ooxFont = ooxFont;
	}
	bool Parse( ReaderParameter oParam, RtfFont& oOutputFont)
	{
		if (!m_ooxFont) return false;

		CString sFontName = m_ooxFont->m_sName;
		if( sFontName.GetLength() < 1 ) return false;

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
};

class OOXFontReader2
{
private:
	ComplexTypes::Word::CFonts * m_ooxFont;
public: 
	OOXFontReader2(ComplexTypes::Word::CFonts * ooxFont)
	{
		m_ooxFont = ooxFont;
	}
	bool Parse( ReaderParameter oParam, int& nFont)
	{
		if (!m_ooxFont) return false;

		CString sAscii, sAsciiTheme, sCs, sCsTheme, sEastAsia, sEastAsiaTheme, sHAnsi, sHAnsiTheme;
		
		if (m_ooxFont->m_sAscii.IsInit())			sAscii = m_ooxFont->m_sAscii.get2();
		if (m_ooxFont->m_oAsciiTheme.IsInit())		sAsciiTheme = m_ooxFont->m_oAsciiTheme->ToString();

		if (m_ooxFont->m_sCs.IsInit())				sCs = m_ooxFont->m_sCs.get2();
		if (m_ooxFont->m_oCsTheme.IsInit())			sCsTheme = m_ooxFont->m_oCsTheme->ToString();

		if (m_ooxFont->m_sEastAsia.IsInit())		sEastAsia = m_ooxFont->m_sEastAsia.get2();
		if (m_ooxFont->m_oEastAsiaTheme.IsInit())	sEastAsiaTheme = m_ooxFont->m_oEastAsiaTheme->ToString();

		if (m_ooxFont->m_sHAnsi.IsInit())			sHAnsi = m_ooxFont->m_sHAnsi.get2();
		if (m_ooxFont->m_oHAnsiTheme.IsInit())		sHAnsiTheme = m_ooxFont->m_oHAnsiTheme->ToString();

		CString sFont;	
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
			if( true == oParam.oRtf->m_oFontTable.GetFont( sFont, oCurFont ) )
				nFont = oCurFont.m_nID;
			else
			{
				oCurFont.m_sName = sFont;
				nFont = oParam.oRtf->m_oFontTable.GetCount() + 1;
				oCurFont.m_nID = nFont;
				oParam.oRtf->m_oFontTable.AddItem( oCurFont );
			}
		}
		return true;
	}
private: 
	CString GetThemeFont( CString sTheme, OOXReader & oReader )
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
