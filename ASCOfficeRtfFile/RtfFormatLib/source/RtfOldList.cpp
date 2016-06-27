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
                sResult.AppendFormat(_T("<w:lvlText w:val=\"%ls\"/>"), Utils::PrepareToXML( sText ).GetBuffer() );
			else
			{
				sResult.Append(_T("<w:lvlText w:val=\"") );
				sResult.AppendChar( 0xf0b7 );
				sResult.Append(_T("\"/>") );
			}
			if( false == sParProp.IsEmpty() )
                sResult.AppendFormat(_T("<w:pPr>%ls</w:pPr>"), sParProp.GetBuffer() );
			if( false == sCharProp.IsEmpty() )
                sResult.AppendFormat(_T("<w:rPr>%ls</w:rPr>"), sCharProp.GetBuffer() );
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
