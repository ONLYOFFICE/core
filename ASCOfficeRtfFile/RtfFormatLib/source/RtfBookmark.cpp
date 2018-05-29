/*
 * (c) Copyright Ascensio System SIA 2010-2018
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

#include "RtfBookmark.h"

#include "Writer/OOXWriter.h"
#include "Writer/OOXFootnoteWriter.h"
#include "Writer/OOXCommentsWriter.h"

#include "Utils.h"

std::wstring RtfBookmarkStart::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;

	sResult += L"{\\*\\bkmkstart";
	if( PROP_DEF != nFirstColumn )
        sResult += L"\\bkmkcolf" + std::to_wstring( nFirstColumn);
	if( PROP_DEF != nLastColumn )
        sResult += L"\\bkmkcoll" + std::to_wstring( nLastColumn);
	
	sResult += L" ";
    RtfCharProperty* pCharProperty = NULL;
    sResult += RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, pCharProperty );
	sResult += L"}";
	return sResult;
}
std::wstring RtfBookmarkStart::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	sResult += L"<w:bookmarkStart";
	
	OOXWriter	* poOOXWriter	= static_cast<OOXWriter*>	( oRenderParameter.poWriter );
	RtfDocument	* poDocument	= static_cast<RtfDocument*>	( oRenderParameter.poDocument );
	
    std::map<std::wstring, int>::iterator pPair = poOOXWriter->m_aBookmarksId.find( m_sName );
	
	int nID;
	if( poOOXWriter->m_aBookmarksId.end() != pPair )
		nID = pPair->second;
	else
	{
		nID = poDocument->m_oIdGenerator.Generate_BookmarkNumber();
		poOOXWriter->m_aBookmarksId[m_sName] =  nID;
	}
    sResult += L" w:id =\"" + std::to_wstring(nID) + L"\"";
	if( PROP_DEF != nFirstColumn )
        sResult += L" w:colFirst =\"" + std::to_wstring(nID) + L"\"";
	if( PROP_DEF != nLastColumn )
        sResult += L" w:colLast =\"" + std::to_wstring(nID) + L"\"";

    sResult += L" w:name =\"" + XmlUtils::EncodeXmlString( m_sName ) + L"\"";
	sResult += L"/>";
	return sResult;
}
std::wstring RtfBookmarkEnd::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;

	sResult += L"{\\*\\bkmkend";
	sResult += L" ";

    RtfCharProperty* pCharProperty = NULL;
    sResult += RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, pCharProperty );

    sResult += L"}";
	
	return sResult;
}
std::wstring RtfBookmarkEnd::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;

	sResult  += L"<w:bookmarkEnd";

	OOXWriter* poOOXWriter = static_cast<OOXWriter*>( oRenderParameter.poWriter );
	RtfDocument* poDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	
    std::map<std::wstring, int>::iterator pPair = poOOXWriter->m_aBookmarksId.find( m_sName );
	int nID;
	if( poOOXWriter->m_aBookmarksId.end() != pPair )
		nID = pPair->second;
	else
	{
		nID = poDocument->m_oIdGenerator.Generate_BookmarkNumber();
		poOOXWriter->m_aBookmarksId[m_sName] = nID;
	}
    sResult += L" w:id =\"" + std::to_wstring(nID) + L"\"";
	sResult += L"/>";
	return sResult;
}

std::wstring RtfAnnotElem::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	
	if (m_nType == 1)		sResult += L"{\\*\\atrfstart " + m_sValue + L"}";
	else if (m_nType = 2)	sResult += L"{\\*\\atrfend " + m_sValue + L"}";
	else if (m_nType = 3)	sResult += L"{\\*\\atnref " + m_sValue + L"}";

	return sResult;
}

std::wstring RtfAnnotElem::RenderToOOX(RenderParameter oRenderParameter)
{
	if (m_nType > 3 || m_nType < 1) return L"";

    std::wstring sResult;
	
 	OOXWriter* poOOXWriter = static_cast<OOXWriter*> (oRenderParameter.poWriter);	
	OOXCommentsWriter* poCommentsWriter = static_cast<OOXCommentsWriter*>( poOOXWriter->m_poCommentsWriter );

	std::map<std::wstring, int>::iterator pFind = poCommentsWriter->m_mapRefs.find(m_sValue);

	int id = -1;
	if (pFind == poCommentsWriter->m_mapRefs.end())
	{
		id  = poCommentsWriter->m_mapRefs.size() ;//+ 1;
		poCommentsWriter->m_mapRefs.insert(std::make_pair(m_sValue, id));
	}
	else
	{
		id = pFind->second;
	}
		 if	(m_nType == 1)	sResult += L"<w:commentRangeStart w:id=\"" + std::to_wstring(id) + L"\"/>";
	else if (m_nType == 3)	sResult += L"<w:commentReference w:id=\"" + std::to_wstring(id) + L"\"/>";
	else if (m_nType == 2)
	{
		sResult += L"<w:commentRangeEnd w:id=\"" + std::to_wstring(id) + L"\"/>";
		sResult += L"<w:r><w:commentReference w:id=\"" + std::to_wstring(id) + L"\"/></w:r>";
	}

	return sResult;
}

std::wstring RtfAnnotation::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	
	if (m_oRef)
	{
		if (m_oRef->m_nType == 1)		sResult += L"{\\*\\atrfstart " + m_oRef->m_sValue + L"}";
		else if (m_oRef->m_nType = 2)	sResult += L"{\\*\\atrfend " + m_oRef->m_sValue + L"}";
		else if (m_oRef->m_nType = 3)	sResult += L"{\\*\\atnref " + m_oRef->m_sValue + L"}";
	}
	return sResult;
}

std::wstring RtfAnnotation::RenderToOOX(RenderParameter oRenderParameter)
{
 	OOXWriter* poOOXWriter = static_cast<OOXWriter*> (oRenderParameter.poWriter);		
	OOXCommentsWriter* poCommentsWriter = static_cast<OOXCommentsWriter*>( poOOXWriter->m_poCommentsWriter );
  
	std::wstring sResult;
	
	sResult += L"<w:comment";
	int id = -1;
	if (m_oRef)
	{
		std::map<std::wstring, int>::iterator pFind = poCommentsWriter->m_mapRefs.find(m_oRef->m_sValue);

		if (pFind == poCommentsWriter->m_mapRefs.end())
		{
			id  = poCommentsWriter->m_mapRefs.size();// + 1;
			poCommentsWriter->m_mapRefs.insert(std::make_pair(m_oRef->m_sValue, id));
		}
		else
		{
			id = pFind->second;
		}
		sResult += L" w:id=\"" + std::to_wstring(id) + L"\"";
	}
	sResult += L" w:author=\"Elena S\"";
	if (m_oDate)
	{
		int nValue = boost::lexical_cast<int>(m_oDate->m_sValue);
		sResult += L" w:date=\"" + RtfUtility::convertDateTime(nValue) + L"\"";
	}
	sResult += L" w:initials=\"ES\"";
	sResult += L">";
	if (m_oContent)
	{
		RenderParameter oNewParameter = oRenderParameter;
		
		oNewParameter.nType = RENDER_TO_OOX_PARAM_COMMENT;
		oNewParameter.poRels = poCommentsWriter->m_oRelsWriter.get();

		sResult += m_oContent->RenderToOOX(oNewParameter);
	}
	sResult += L"</w:comment>";

	std::wstring sParaId = XmlUtils::IntToString(poOOXWriter->m_nextParaId, L"%08X");//last para id in comment
	poCommentsWriter->AddComment(id, sResult, sParaId, m_oParent ? boost::lexical_cast<int>(m_oParent->m_sValue) : 0);

	return L"";
}

std::wstring RtfFootnote::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	
	sResult += L"{";
	sResult += m_oCharProp.RenderToRtf( oRenderParameter );
	sResult += L"\\chftn";
	sResult += L"{\\footnote";
	
	if( true == m_bEndNote )
		sResult += L"\\ftnalt";
	
	for( int i = 0; i < m_oContent->GetCount(); i++ )
		sResult += m_oContent->RenderToRtf(  oRenderParameter  );
	
	sResult += L"}";
	sResult += L"}";
	
	return sResult;
}
std::wstring RtfFootnote::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	OOXWriter* poOOXWriter = static_cast<OOXWriter*>(oRenderParameter.poWriter);
	RtfDocument* poDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	if( true == m_bEndNote )
	{
		int nID = poDocument->m_oIdGenerator.Generate_EndnoteNumber();
		OOXEndnoteWriter* poEndnoteWriter = static_cast<OOXEndnoteWriter*>( poOOXWriter->m_poEndnoteWriter );
		
		RenderParameter oNewParameter = oRenderParameter;
		oNewParameter.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		oNewParameter.poRels = poEndnoteWriter->m_oRelsWriter.get();
		
		poEndnoteWriter->AddEndnote( L"", nID, m_oContent->RenderToOOX(oNewParameter) );
		
		sResult += L"<w:r>";
        std::wstring srPr = m_oCharProp.RenderToOOX( oRenderParameter );

        if( false == srPr.empty() )
		{
			sResult += L"<w:rPr>";
            sResult += srPr;
			sResult += L"</w:rPr>";
		}
        sResult += L"<w:endnoteReference  w:id=\"" + std::to_wstring(nID) + L"\"/>";
		sResult += L"</w:r>";
	}
	else
	{
		int nID = poDocument->m_oIdGenerator.Generate_FootnoteNumber();
		OOXFootnoteWriter* poFootnoteWriter = static_cast<OOXFootnoteWriter*>( poOOXWriter->m_poFootnoteWriter );
		
		RenderParameter oNewParameter = oRenderParameter;
		oNewParameter.nType		= RENDER_TO_OOX_PARAM_UNKNOWN;
		oNewParameter.poRels	= poFootnoteWriter->m_oRelsWriter.get();
		
		poFootnoteWriter->AddFootnote( L"", nID, m_oContent->RenderToOOX(oNewParameter) );
		
		sResult += L"<w:r>";
        std::wstring srPr = m_oCharProp.RenderToOOX( oRenderParameter );

        if( false == srPr.empty() )
		{
			sResult += L"<w:rPr>";
            sResult += srPr;
            sResult += L"</w:rPr>";
		}
        sResult += L"<w:footnoteReference w:id=\"" + std::to_wstring(nID) + L"\"/>";
		sResult += L"</w:r>";
	}

	return sResult;
}
