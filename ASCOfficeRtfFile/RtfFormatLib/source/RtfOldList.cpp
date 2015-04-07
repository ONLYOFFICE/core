#include "RtfParagraph.h"
#include "Writer/OOXWriter.h"

CString RtfOldList::RenderToRtf(RenderParameter oRenderParameter)
{
	CString sResult;
	if( NULL != m_oText )
		sResult.Append( m_oText->RenderToRtf( oRenderParameter ) );
	return sResult;
}
CString RtfOldList::RenderToOOX(RenderParameter oRenderParameter)
{
	CString sResult;
	if( true == CanConvertToNumbering() )
	{//сохраняем как список
		if( RENDER_TO_OOX_PARAM_OLDLIST_ABS == oRenderParameter.nType )
		{
			CString sCharProp = m_oLevelText->m_oProperty.m_oCharProperty.RenderToOOX( oRenderParameter );
			CString sParProp = m_oLevelText->m_oProperty.RenderToOOX( oRenderParameter );

			RenderParameter oNewParameter = oRenderParameter;
			oNewParameter.nType = RENDER_TO_OOX_PARAM_PLAIN;
			CString sText = m_oLevelText->RenderToOOX( oNewParameter );

			sResult.AppendFormat( _T("<w:abstractNum w:abstractNumId=\"%d\">"), m_nLs );
			sResult.Append(_T("<w:multiLevelType w:val=\"singleLevel\" />"));
			sResult.Append(_T("<w:lvl w:ilvl=\"0\">"));
			sResult.Append(_T("<w:numFmt w:val=\"bullet\"/>"));
			if( false == sText.IsEmpty() )
				sResult.AppendFormat(_T("<w:lvlText w:val=\"%ls\"/>"), Utils::PrepareToXML( sText ) );
			else
			{
				sResult.Append(_T("<w:lvlText w:val=\"") );
				sResult.AppendChar( 0xf0b7 );
				sResult.Append(_T("\"/>") );
			}
			if( false == sParProp.IsEmpty() )
				sResult.AppendFormat(_T("<w:pPr>%ls</w:pPr>"), sParProp );
			if( false == sCharProp.IsEmpty() )
				sResult.AppendFormat(_T("<w:rPr>%ls</w:rPr>"), sCharProp );
			if( lj_none != m_eLevelJust )
			{
				switch( m_eLevelJust )
				{
					case lj_left: sResult.Append(_T("<w:lvlJc w:val=\"left\"/>"));break;
					case lj_center: sResult.Append(_T("<w:lvlJc w:val=\"center\"/>"));break;
					case lj_right: sResult.Append(_T("<w:lvlJc w:val=\"right\"/>"));break;
				}
			}
			else
				sResult.Append(_T("<w:lvlJc w:val=\"left\"/>"));
			sResult.Append(_T("</w:lvl>"));
			sResult.Append( _T("</w:abstractNum>") );
		}
		else if( RENDER_TO_OOX_PARAM_OLDLIST_OVR == oRenderParameter.nType )
			sResult.AppendFormat(_T("<w:num w:numId=\"%d\"><w:abstractNumId w:val=\"%d\" /></w:num>"), m_nLs, m_nLs );
		else
		{
			if( PROP_DEF != m_nLs && PROP_DEF != m_nIlvl )
				sResult.AppendFormat(_T("<w:numPr><w:ilvl w:val=\"%d\"/><w:numId w:val=\"%d\"/></w:numPr>"), m_nIlvl, m_nLs );
		}
	}
	return sResult;
}
bool RtfOldList::CanConvertToNumbering()
{
	return lt_blt == m_eLevelType;
}
void RtfOldList::SetDefault()
{
	m_eLevelType = lt_none;
	m_nLevelType = PROP_DEF;
	m_eLevelJust = lj_none;
	m_oLevelText = RtfParagraphPtr( new RtfParagraph() );
	m_oLevelText->m_oProperty.m_nIndLeft = 720;
	m_oLevelText->m_oProperty.m_nIndFirstLine = -360;

	m_nLs = PROP_DEF;
	m_nIlvl = PROP_DEF;

	m_oText = RtfParagraphPtr( new RtfParagraph() );
}
bool RtfOldList::operator==( const RtfOldList& oOldList )
{
	if( false == (m_eLevelType == oOldList.m_eLevelType && m_nLevelType == oOldList.m_nLevelType ) )
		return false;
	if( false == (m_oLevelText->m_oProperty.m_oCharProperty == oOldList.m_oLevelText->m_oProperty.m_oCharProperty ) )
		return false;
	RenderParameter oParam;
	oParam.nType = RENDER_TO_OOX_PARAM_PLAIN;
	CString sText1 = m_oLevelText->RenderToOOX( oParam );
	CString sText2 = oOldList.m_oLevelText->RenderToOOX( oParam );
	if( sText1 != sText2 )
		return false;
	return  true;
}