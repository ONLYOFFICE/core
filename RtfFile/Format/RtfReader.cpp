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
#include "RtfReader.h"
#include "../OOXml/Writer/OOXWriter.h"
#include "DestinationCommand.h"

#include "../../Common/MS-LCID.h"

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
	m_oTok = oReader.m_oLex.NextCurToken();
	
	if (m_oTok.Type == m_oTok.None)
		m_oTok = oReader.m_oLex.NextToken();

	while (m_oTok.Type != RtfToken::Eof && false == m_bStopReader)
	{
		switch (m_oTok.Type)
		{
		case RtfToken::GroupStart:
		{
			ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
			PushState(oReader);
		}break;
		case RtfToken::GroupEnd:
		{
			ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);

			PopState(oDocument, oReader);
		}break;
		case RtfToken::Keyword:
		{
			ExecuteTextInternal2(oDocument, oReader, m_oTok.Key, m_nSkipChars);
			if (m_oTok.Key == "u")
			{
				ExecuteText(oDocument, oReader, ExecuteTextInternal(oDocument, oReader, m_oTok.Key, m_oTok.HasParameter, m_oTok.Parameter, m_nSkipChars));
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
			sResult += wchar_t(nPar);
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
	if (oReader.m_oState->m_sCurText.length() > 0)
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
	//удаляем символы вслед за юникодом
	if (nSkipChars > 0)
	{
		if (nSkipChars >= (int)sResult.length())
		{
			//nSkipChars -= nLength;//vedomost.rtf
			sResult.clear();
		}
		else
		{
			sResult = sResult.substr(nSkipChars);
		}
		nSkipChars = 0;
	}
	if ("u" == sKey)
	{
		//надо правильно установить m_nSkipChars по значению \ucN
		nSkipChars = oReader.m_oState->m_nUnicodeClean;
	}
}
void RtfAbstractReader::ExecuteTextInternalSkipChars(std::wstring & sResult, RtfReader& oReader, std::string & sKey, int& nSkipChars)
{
	//удаляем символы вслед за юникодом
	if (nSkipChars > 0)
	{
		if (nSkipChars >= (int)sResult.length())
		{
			//nSkipChars -= nLength;//vedomost.rtf
			sResult.clear();
		}
		else
		{
			sResult = sResult.substr(nSkipChars);
		}
		nSkipChars = 0;
	}
	if ("u" == sKey)
	{
		//надо правильно установить m_nSkipChars по значению \ucN
		nSkipChars = oReader.m_oState->m_nUnicodeClean;
	}
}
std::wstring RtfAbstractReader::ExecuteTextInternalCodePage( std::string& sCharString, RtfDocument& oDocument, RtfReader& oReader)
{
    std::wstring sResult;

    if( false == sCharString.empty())
    {
		if (sCharString == "*") return L"*";

        int nCodepage = -1;

        //применяем параметры codepage от текущего шрифта todo associated fonts.
        RtfFont oFont;
        if( true == oDocument.m_oFontTable.GetFont( oReader.m_oState->m_oCharProp.m_nFont, oFont ) && !m_bUseGlobalCodepage)
        {
            if( PROP_DEF != oFont.m_nCharset && oFont.m_nCharset > 1)
            {
                nCodepage = RtfUtility::CharsetToCodepage( oFont.m_nCharset );
            }
            else if( PROP_DEF != oFont.m_nCodePage )
            {
                nCodepage = oFont.m_nCodePage;
            }
        }
        //от настроек документа
        if( -1 == nCodepage && RtfDocumentProperty::cp_none != oDocument.m_oProperty.m_eCodePage )
        {
            switch ( oDocument.m_oProperty.m_eCodePage )
            {
            case RtfDocumentProperty::cp_ansi:
                {
                    if( PROP_DEF != oDocument.m_oProperty.m_nAnsiCodePage )
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
        //если ничего нет ставим ANSI или default from user
        if( -1 == nCodepage )
		{
            nCodepage = CP_ACP;
		}
		if (nCodepage == CP_ACP && oDocument.m_nUserLCID > 0)
		{
			nCodepage = msLCID2DefCodePage(oDocument.m_nUserLCID);
		}

        sResult = RtfUtility::convert_string_icu(sCharString.begin(), sCharString.end(), nCodepage);

		//if (!sCharString.empty() && sResult.empty())
		//{
		//	//code page not support in icu !!!
		//	sResult = RtfUtility::convert_string(sCharString.begin(), sCharString.end(), nCodepage); .. to UnicodeConverter
		//}
    }
    return sResult;
}
