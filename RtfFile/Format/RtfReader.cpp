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
#include "RtfReader.h"
#include "../OOXml/Writer/OOXWriter.h"
#include "DestinationCommand.h"

RtfReader::ReaderState::ReaderState()
{
	m_bControlPresent = false;
	m_nUnicodeClean = 1;
	m_oCharProp.SetDefaultRtf();
	m_oParagraphProp.SetDefaultRtf();
	m_oRowProperty.SetDefaultRtf();
	m_oCellProperty.SetDefaultRtf();
	m_oCurOldList.SetDefault();
}

RtfReader::~RtfReader()
{
}
long RtfReader::GetProgress()
{
	return (long)( g_cdMaxPercent * m_oLex.GetProgress());
}
void RtfReader::Stop()
{
	m_oLex.CloseSource();
}
RtfReader::RtfReader(RtfDocument& oDocument, std::wstring sFilename ) : m_oDocument(oDocument), m_sFilename(sFilename)
{
	m_oState = ReaderStatePtr(new ReaderState());
	m_nFootnote = PROP_DEF;
	m_nDefFont = PROP_DEF;
	m_nDefLang = PROP_DEF;
	m_nDefLangAsian = PROP_DEF;
	m_convertationManager = NULL;
}
bool RtfReader::Load()
{
	if (false == m_oLex.SetSource(m_sFilename)) return false;
	
	RtfNormalReader oNormalReader( m_oDocument, (*this) );
	oNormalReader.Parse( m_oDocument, (*this) );
	m_oLex.CloseSource();
    return true;
} 
void RtfReader::PushState()
{
	ReaderStatePtr psaveNew = ReaderStatePtr(new ReaderState());
	psaveNew -> m_nUnicodeClean		= m_oState->m_nUnicodeClean;
	psaveNew -> m_oCharProp			= m_oState->m_oCharProp;
	psaveNew -> m_oParagraphProp	= m_oState->m_oParagraphProp;
	psaveNew -> m_oRowProperty		= m_oState->m_oRowProperty;
	psaveNew -> m_oCellProperty		= m_oState->m_oCellProperty;
	psaveNew -> m_oCurOldList		= m_oState->m_oCurOldList;
	//psaveNew -> m_oSectionProp	= m_oState->m_oSectionProp;
	psaveNew -> m_pSaveState		= m_oState;
	m_oState = psaveNew;

	if( PROP_DEF == m_oState->m_oCharProp.m_nFont )
		m_oState->m_oCharProp.m_nFont = m_nDefFont;
	
	if (PROP_DEF == m_oState->m_oCharProp.m_nLanguage)
		m_oState->m_oCharProp.m_nLanguage = m_nDefLang;
	
	if (PROP_DEF == m_oState->m_oCharProp.m_nLanguageAsian)
		m_oState->m_oCharProp.m_nLanguageAsian = m_nDefLangAsian;
}
void RtfReader::PopState()
{
	if( 0 != m_oState->m_pSaveState )
		m_oState = m_oState->m_pSaveState;
}

//---------------------------------------------------------------------------------------------------------------------------------
RtfAbstractReader::RtfAbstractReader()
{
	m_bCanStartNewReader = false;
	m_bSkip = false;
	m_nSkipChars = 0;
	m_nCurGroups = 1;
	m_oFileWriter = NULL;
	m_bStopReader = false;

	m_bUseGlobalCodepage = false;
}
void RtfAbstractReader::PushState(RtfReader& oReader)
{
	oReader.PushState();
	m_nCurGroups++;
	m_bCanStartNewReader = true;
}
void RtfAbstractReader::PopState(RtfDocument& oDocument, RtfReader& oReader)
{
	if( m_nCurGroups > 0 )
		m_nCurGroups--;
	else
		;//ASSERT(false);
	if( m_nCurGroups == 0 )
	{
		m_bStopReader = true;
		ExitReader( oDocument, oReader );
	}
	oReader.PopState();
	if( m_nCurGroups == 0 )
		ExitReader2( oDocument, oReader );
}
bool RtfAbstractReader::StartSubReader( RtfAbstractReader& poNewReader, RtfDocument& oDocument, RtfReader& oReader  )
{
	if( true == m_bCanStartNewReader )
	{
		m_bCanStartNewReader = false;
		m_nCurGroups--;

		poNewReader.m_bSkip = m_bSkip;
		return poNewReader.Parse(oDocument, oReader);
	}
	return false;
}
void RtfAbstractReader::Skip( RtfDocument& oDocument, RtfReader& oReader )
{
	int cGroup = 1;
	while( cGroup >= 1 )
	{
		m_oTok = oReader.m_oLex.NextToken();
		if(m_oTok.Type == RtfToken::GroupStart)
			cGroup++;
		else if(m_oTok.Type == RtfToken::GroupEnd)
			cGroup--;
		else if(m_oTok.Type == RtfToken::Eof)
			break;
	}
	PopState( oDocument, oReader );
}
bool RtfAbstractReader::ExecuteCommand( RtfDocument& oDocument, RtfReader& oReader, std::string sKey, bool bHasPar, int nPar )
{
	return true;
}
void RtfAbstractReader::ExecuteText( RtfDocument& oDocument, RtfReader& oReader, std::wstring oText )
{
}
void RtfAbstractReader::ExitReader( RtfDocument& oDocument, RtfReader& oReader )
{
}
void RtfAbstractReader::ExitReader2( RtfDocument& oDocument, RtfReader& oReader )
{
}
bool RtfAbstractReader::RtfAbstractReader::Parse(RtfDocument& oDocument, RtfReader& oReader)
{
	NFileWriter::CBufferedFileWriter* poOldWriter = oReader.m_oLex.m_oFileWriter;
	oReader.m_oLex.m_oFileWriter = m_oFileWriter;

	int res = 0;
	unsigned short nLastHigh = 0;

	// only for 4 bytes wchar_t, returns true if high surrogate
	auto handle_u32wchar_surrogate = [&nLastHigh] (std::wstring& res) -> bool {
		if (res[0] >= 0xD800 && res[0] <= 0xDBFF) // high
		{
			nLastHigh = res[0];
			return true;
		}
		if (nLastHigh && res[0] >= 0xDC00 && res[0] <= 0xDFFF) // low
			res[0] = 0x10000 + ((wchar_t)(nLastHigh - 0xD800) << 10) + (res[0] - 0xDC00);

		nLastHigh = 0;
		return false;
	};

	m_oTok = oReader.m_oLex.NextCurToken();
	
	if (m_oTok.Type == m_oTok.None)
		m_oTok = oReader.m_oLex.NextToken();

	while (m_oTok.Type != RtfToken::Eof && false == m_bStopReader)
	{
		switch (m_oTok.Type)
		{
		case RtfToken::GroupStart:
		{
			nLastHigh = 0;
			ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
			PushState(oReader);
		}break;
		case RtfToken::GroupEnd:
		{
			nLastHigh = 0;
			ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
			PopState(oDocument, oReader);
		}break;
		case RtfToken::Keyword:
		{
			ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
			if (m_oTok.Key == "u")
			{
				std::wstring sRes = ExecuteTextInternal(oDocument, oReader, m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter, m_nSkipChars);
				if (sizeof(wchar_t) == 4)
					if (!sRes.empty() && handle_u32wchar_surrogate(sRes))
						break;
				ExecuteText(oDocument, oReader, std::move(sRes));
				break;
			}
			else
			{
				if (true == m_bSkip)
				{
					if (false == ExecuteCommand(oDocument, oReader, m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter))
						Skip(oDocument, oReader);
					m_bSkip = false;
				}
				else
					ExecuteCommand(oDocument, oReader, m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter);
			}
			if (true == m_bCanStartNewReader)
				m_bCanStartNewReader = false;
		}break;
		case RtfToken::Control:
		{
			if (m_oTok.Key == "42")
				m_bSkip = true;
			if (m_oTok.Key == "39" && true == m_oTok.HasParameter)
			{
				oReader.m_oState->m_sCurText += m_oTok.Parameter;
				oReader.m_oState->m_bControlPresent = true;
			}
			if (m_oTok.Key == "32" && false == m_oTok.HasParameter)
			{
				oReader.m_oState->m_sCurText += " ";
				oReader.m_oState->m_bControlPresent = true;
			}
            if (m_oTok.Key == "par" && false == m_oTok.HasParameter)
            {
                oReader.m_oState->m_sCurText += "\n";
                oReader.m_oState->m_bControlPresent = true;
            }
		}break;
		case RtfToken::Text:
		{
			oReader.m_oState->m_sCurText += m_oTok.Key;
		}break;

		}
		if (false == m_bStopReader)
			m_oTok = oReader.m_oLex.NextToken();
	}

	oReader.m_oLex.m_oFileWriter = poOldWriter;

	return true;
}
std::wstring RtfAbstractReader::ExecuteTextInternal(RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, bool bHasPar, int nPar, int& nSkipChars)
{
	std::wstring sResult;

	if ("u" == sKey)
	{
		if (true == bHasPar)
		{
			int codepoint = (nPar < 0) ? (nPar + 65536) : nPar;
			sResult += wchar_t(codepoint);
		}
	}
	else
	{
		std::string sCharString;
		if ("39" == sKey)
		{
			if (true == bHasPar)
				sCharString += (char)nPar;
		}
		else
			sCharString = sKey;

		sResult = ExecuteTextInternalCodePage(sCharString, oDocument, oReader);
	}
	ExecuteTextInternalSkipChars(sResult, oReader, sKey, nSkipChars);
	return sResult;
}
void RtfAbstractReader::ExecuteTextInternal2(RtfDocument& oDocument, RtfReader& oReader, std::string & sKey, int& nSkipChars)
{
	if (false == oReader.m_oState->m_sCurText.empty())
	{
		std::string str;
		ExecuteTextInternalSkipChars(oReader.m_oState->m_sCurText, oReader, str, nSkipChars);
		
		std::wstring sResult = ExecuteTextInternalCodePage(oReader.m_oState->m_sCurText, oDocument, oReader);
		
		oReader.m_oState->m_sCurText.erase();
		oReader.m_oState->m_bControlPresent = false;
		
		if (false == sResult.empty())
		{
			ExecuteText(oDocument, oReader, sResult);
		}
	}
}
void RtfAbstractReader::ExecuteTextInternalSkipChars(std::string & sResult, RtfReader& oReader, std::string & sKey, int& nSkipChars)
{
	//remove characters following unicode
	if (nSkipChars > 0)
	{
		if (nSkipChars >= (int)sResult.length())
		{
			nSkipChars -= (int)sResult.length();
			sResult.clear();
		}
		else
		{
			sResult = sResult.substr(nSkipChars);
			nSkipChars = 0;
		}
	}
	if ("u" == sKey)
	{
		//need to correctly set m_nSkipChars based on \\ucN value
		nSkipChars = oReader.m_oState->m_nUnicodeClean;
	}
}
void RtfAbstractReader::ExecuteTextInternalSkipChars(std::wstring & sResult, RtfReader& oReader, std::string & sKey, int& nSkipChars)
{
	//remove characters following unicode
	if (nSkipChars > 0 && "u" != sKey)
	{
		if (nSkipChars >= (int)sResult.length())
		{
			nSkipChars -= (int)sResult.length();
			sResult.clear();
		}
		else
		{
			sResult = sResult.substr(nSkipChars);
			nSkipChars = 0;
		}
	}
	if ("u" == sKey)
	{
		//need to correctly set m_nSkipChars based on \\ucN value
		nSkipChars = oReader.m_oState->m_nUnicodeClean;
	}
}
std::wstring RtfAbstractReader::ExecuteTextInternalCodePage( std::string& sCharString, RtfDocument& oDocument, RtfReader& oReader)
{
	if (sCharString.empty()) return L"";
	if (sCharString == "*") return L"*";
	
	std::wstring sResult;

	int nCodepage = -1;

	//apply codepage parameters from the current font todo associated fonts.
	RtfFont oFont;
	if ((!m_bUseGlobalCodepage) && (true == oDocument.m_oFontTable.GetFont(oReader.m_oState->m_oCharProp.m_nFont, oFont)))
	{
		if (PROP_DEF != oFont.m_nCodePage)
		{
			nCodepage = oFont.m_nCodePage;
		}
		else if ((PROP_DEF != oFont.m_nCharset  && oFont.m_nCharset > 2)
			&& (PROP_DEF == oDocument.m_oProperty.m_nAnsiCodePage || 0 == oDocument.m_oProperty.m_nAnsiCodePage || 1252 == oDocument.m_oProperty.m_nAnsiCodePage))
		{
			nCodepage = RtfUtility::CharsetToCodepage(oFont.m_nCharset);
		}
	}
	//from document settings
	if (-1 == nCodepage && RtfDocumentProperty::cp_none != oDocument.m_oProperty.m_eCodePage)
	{
		switch (oDocument.m_oProperty.m_eCodePage)
		{
		case RtfDocumentProperty::cp_ansi:
		{
			if (PROP_DEF != oDocument.m_oProperty.m_nAnsiCodePage)
			{
				nCodepage = oDocument.m_oProperty.m_nAnsiCodePage;
			}
			else
				nCodepage = CP_ACP;
			break;
		}
		case RtfDocumentProperty::cp_mac:   nCodepage = CP_MACCP;   break; //?? todooo
		case RtfDocumentProperty::cp_pc:    nCodepage = 437;        break; //ms dos latin us
		case RtfDocumentProperty::cp_pca:   nCodepage = 850;        break; //ms dos latin eu
		}
	}
	//if nothing else, set ANSI or default from user
	if (-1 == nCodepage)
	{
		nCodepage = CP_ACP;
	}
	if ((nCodepage == CP_ACP || nCodepage == 1252)&& oDocument.m_nUserLCID > 0)
	{
		nCodepage = oDocument.m_lcidConverter.get_codepage(oDocument.m_nUserLCID);
	}
	if (m_bUseGlobalCodepage && nCodepage == 0 && PROP_DEF != oDocument.m_oProperty.m_nDefLang )
	{
		nCodepage = oDocument.m_lcidConverter.get_codepage(oDocument.m_oProperty.m_nDefLang);
	}

	if (m_bUseGlobalCodepage && nCodepage == 0)
	{
		sResult = std::wstring(sCharString.begin(), sCharString.end());
	}
	else
	{
		sResult = RtfUtility::convert_string_icu(sCharString.begin(), sCharString.end(), nCodepage);
	}

	//if (!sCharString.empty() && sResult.empty())
	//{
	//	//code page not support in icu !!!
	//	sResult = RtfUtility::convert_string(sCharString.begin(), sCharString.end(), nCodepage); .. to UnicodeConverter
	//}
    return sResult;
}
