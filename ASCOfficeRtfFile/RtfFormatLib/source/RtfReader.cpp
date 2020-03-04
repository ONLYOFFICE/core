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
#include "Writer/OOXWriter.h"
#include "DestinationCommand.h"

#include "../../../Common/MS-LCID.h"

RtfReader::RtfReader(RtfDocument& oDocument, std::wstring sFilename ) : m_oDocument(oDocument), m_sFilename(sFilename)
{
	m_oState = ReaderStatePtr(new ReaderState());
	m_nFootnote = PROP_DEF;
	m_nDefFont = PROP_DEF;
	m_convertationManager = NULL;
}
bool RtfReader::Load()
{
	m_oLex.SetSource( m_sFilename );
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

std::wstring RtfAbstractReader::ExecuteTextInternalCodePage( std::string& sCharString, RtfDocument& oDocument, RtfReader& oReader)
{
    std::wstring sResult;

    if( false == sCharString.empty())
    {
        int         nCodepage = -1;

        //применяем параметры codepage от текущего шрифта todo associated fonts.
        RtfFont oFont;
        if( true == oDocument.m_oFontTable.GetFont( oReader.m_oState->m_oCharProp.m_nFont, oFont ) && !m_bUseGlobalCodepage)
        {
            if( PROP_DEF != oFont.m_nCharset )
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

		if (!sCharString.empty() && sResult.empty())
		{
			//code page not support in icu !!!
			sResult = RtfUtility::convert_string(sCharString.begin(), sCharString.end(), nCodepage);
		}
    }
    return sResult;
}
