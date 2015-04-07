
#include "RtfBookmark.h"

#include "Writer/OOXWriter.h"
#include "Writer/OOXFootnoteWriter.h"

#include "Utils.h"

CString RtfBookmarkStart::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	//ATLASSERT( false == m_sName.IsEmpty() );
	sResult.Append(_T("{\\*\\bkmkstart"));
	if( PROP_DEF != nFirstColumn )
		sResult.AppendFormat(_T("\\bkmkcolf%d"), nFirstColumn);
	if( PROP_DEF != nLastColumn )
		sResult.AppendFormat(_T("\\bkmkcoll%d"), nLastColumn);
	sResult.Append(_T(" "));
	sResult.Append( RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, NULL ) );
	sResult.Append(_T("}"));
	return sResult;
}
CString RtfBookmarkStart::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	//ATLASSERT( false == m_sName.IsEmpty() );
	sResult.Append(_T("<w:bookmarkStart"));
	
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
	sResult.AppendFormat(_T(" w:id =\"%d\""), nID);
	if( PROP_DEF != nFirstColumn )
		sResult.AppendFormat(_T(" w:colFirst =\"%d\""), nID);
	if( PROP_DEF != nLastColumn )
		sResult.AppendFormat(_T(" w:colLast =\"%d\""), nID);
	sResult.AppendFormat(_T(" w:name =\"%ls\""), Utils::PrepareToXML( m_sName ) );
	sResult.Append(_T("/>"));
	return sResult;
}
CString RtfBookmarkEnd::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	//ATLASSERT( false == m_sName.IsEmpty() );
	sResult.Append(_T("{\\*\\bkmkend"));
	sResult.Append(_T(" "));
	sResult.Append( RtfChar::renderRtfText( m_sName, oRenderParameter.poDocument, NULL ) );
	sResult.Append(_T("}"));
	return sResult;
}
CString RtfBookmarkEnd::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	//ATLASSERT( false == m_sName.IsEmpty() );
	sResult.Append(_T("<w:bookmarkEnd"));

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
	sResult.AppendFormat(_T(" w:id =\"%d\""), nID);
	sResult.Append(_T("/>"));
	return sResult;
}
CString RtfFootnote::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	sResult.Append(_T("{"));
	sResult.Append( m_oCharProp.RenderToRtf( oRenderParameter ) );
	sResult.Append(_T("\\chftn"));
	sResult.Append(_T("{\\footnote"));
	if( true == m_bEndNote )
		sResult.Append(_T("\\ftnalt"));
	for( int i = 0; i < m_oContent->GetCount(); i++ )
		sResult.Append( m_oContent->RenderToRtf(  oRenderParameter  ) );
	sResult.Append(_T("}"));
	sResult.Append(_T("}"));
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
		poEndnoteWriter->AddEndnote( _T(""), nID, m_oContent->RenderToOOX(oNewParameter) );
		sResult.Append( _T("<w:r>") );
		CString srPr = m_oCharProp.RenderToOOX( oRenderParameter );
		if( false == srPr.IsEmpty() )
			sResult.AppendFormat( _T("<w:rPr>%ls</w:rPr>"), srPr );
		sResult.AppendFormat( _T("<w:endnoteReference  w:id=\"%d\"/>"), nID );
		sResult.Append( _T("</w:r>") );
	}
	else
	{
		int nID = poDocument->m_oIdGenerator.Generate_FootnoteNumber();
		OOXFootnoteWriter* poFootnoteWriter = static_cast<OOXFootnoteWriter*>( poOOXWriter->m_poFootnoteWriter );
		RenderParameter oNewParameter = oRenderParameter;
		oNewParameter.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		oNewParameter.poRels = poFootnoteWriter->m_oRelsWriter.get();
		poFootnoteWriter->AddFootnote( _T(""), nID, m_oContent->RenderToOOX(oNewParameter) );
		sResult.Append( _T("<w:r>") );
		CString srPr = m_oCharProp.RenderToOOX( oRenderParameter );
		if( false == srPr.IsEmpty() )
			sResult.AppendFormat( _T("<w:rPr>%ls</w:rPr>"), srPr );
		sResult.AppendFormat( _T("<w:footnoteReference w:id=\"%d\"/>"), nID );
		sResult.Append( _T("</w:r>") );
	}

	return sResult;
}