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

#include "RtfBookmark.h"

#include "Writer/OOXWriter.h"
#include "Writer/OOXFootnoteWriter.h"

#include "Utils.h"

CString RtfBookmarkStart::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;

	sResult += L"{\\*\\bkmkstart";
	if( PROP_DEF != nFirstColumn )
		sResult.AppendFormat(L"\\bkmkcolf%d", nFirstColumn);
	if( PROP_DEF != nLastColumn )
		sResult.AppendFormat(L"\\bkmkcoll%d", nLastColumn);
	
	sResult += L" ";
    RtfCharProperty* pCharProperty = NULL;
    sResult += RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, pCharProperty );
	sResult += L"}";
	return sResult;
}
CString RtfBookmarkStart::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	//ATLASSERT( false == m_sName.IsEmpty() );
	sResult += L"<w:bookmarkStart";
	
	OOXWriter	* poOOXWriter	= static_cast<OOXWriter*>	( oRenderParameter.poWriter );
	RtfDocument	* poDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	
	std::map<CString, int>::iterator pPair = poOOXWriter->m_aBookmarksId.find( m_sName );
	
	int nID;
	if( poOOXWriter->m_aBookmarksId.end() != pPair )
		nID = pPair->second;
	else
	{
		nID = poDocument->m_oIdGenerator.Generate_BookmarkNumber();
		poOOXWriter->m_aBookmarksId[m_sName] =  nID;
	}
	sResult.AppendFormat(L" w:id =\"%d\"", nID);
	if( PROP_DEF != nFirstColumn )
		sResult.AppendFormat(L" w:colFirst =\"%d\"", nID);
	if( PROP_DEF != nLastColumn )
		sResult.AppendFormat(L" w:colLast =\"%d\"", nID);
    sResult += L" w:name =\"" + Utils::PrepareToXML( m_sName ) + L"\"";
	sResult += L"/>";
	return sResult;
}
CString RtfBookmarkEnd::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;

	sResult += L"{\\*\\bkmkend";
	sResult += L" ";

    RtfCharProperty* pCharProperty = NULL;
    sResult += RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, pCharProperty );

    sResult += L"}";
	
	return sResult;
}
CString RtfBookmarkEnd::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;

	sResult  += L"<w:bookmarkEnd";

	OOXWriter* poOOXWriter = static_cast<OOXWriter*>( oRenderParameter.poWriter );
	RtfDocument* poDocument = static_cast<RtfDocument*>(oRenderParameter.poDocument);
	
	std::map<CString, int>::iterator pPair = poOOXWriter->m_aBookmarksId.find( m_sName );
	int nID;
	if( poOOXWriter->m_aBookmarksId.end() != pPair )
		nID = pPair->second;
	else
	{
		nID = poDocument->m_oIdGenerator.Generate_BookmarkNumber();
		poOOXWriter->m_aBookmarksId[m_sName] = nID;
	}
	sResult.AppendFormat(L" w:id =\"%d\"", nID);
	sResult += L"/>";
	return sResult;
}
CString RtfFootnote::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	
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
CString RtfFootnote::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
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
		CString srPr = m_oCharProp.RenderToOOX( oRenderParameter );

		if( false == srPr.IsEmpty() )
		{
			sResult += L"<w:rPr>";
            sResult += srPr;
			sResult += L"</w:rPr>";
		}
		sResult.AppendFormat( L"<w:endnoteReference  w:id=\"%d\"/>", nID );
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
		CString srPr = m_oCharProp.RenderToOOX( oRenderParameter );

		if( false == srPr.IsEmpty() )
		{
			sResult += L"<w:rPr>";
            sResult += srPr;
            sResult += L"</w:rPr>";
		}
		sResult.AppendFormat( L"<w:footnoteReference w:id=\"%d\"/>", nID );
		sResult += L"</w:r>";
	}

	return sResult;
}
