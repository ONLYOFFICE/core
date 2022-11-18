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

#include "RtfParagraph.h"
#include "../OOXml/Writer/OOXWriter.h"

std::wstring RtfOldList::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( NULL != m_oText )
		sResult += m_oText->RenderToRtf( oRenderParameter );
	return sResult;
}
std::wstring RtfOldList::RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	if( true == CanConvertToNumbering() )
	{//сохраняем как список
		if( RENDER_TO_OOX_PARAM_OLDLIST_ABS == oRenderParameter.nType )
		{
            std::wstring sCharProp	= m_oLevelText->m_oProperty.m_oCharProperty.RenderToOOX( oRenderParameter );
            std::wstring sParProp	= m_oLevelText->m_oProperty.RenderToOOX( oRenderParameter );

			RenderParameter oNewParameter = oRenderParameter;
			oNewParameter.nType = RENDER_TO_OOX_PARAM_PLAIN;
            std::wstring sText = m_oLevelText->RenderToOOX( oNewParameter );

            sResult += L"<w:abstractNum w:abstractNumId=\"" + std::to_wstring(m_nLs) + L"\">";
			sResult += L"<w:multiLevelType w:val=\"singleLevel\" />";
			sResult += L"<w:lvl w:ilvl=\"0\">";
			sResult += L"<w:numFmt w:val=\"bullet\"/>";
			
			if(!sText.empty() )
			{
				// В символьном шрифте обрезать надо, в других случаях - нет
				if (/*bIsSymbol && */(sText[0] & 0xF000) != 0)
				{
					sText[0] &= 0x00FF;
				}
				sResult += L"<w:lvlText w:val=\"" + XmlUtils::EncodeXmlString( sText ) + L"\"/>";
			}
			else
			{
				sResult += L"<w:lvlText w:val=\"";
                sResult += L'\xF0B7' ;
				sResult += L"\"/>";
			}
			
			if( !sParProp.empty() )
                sResult += L"<w:pPr>" + sParProp + L"</w:pPr>";
			
			if( !sCharProp.empty() )
                sResult += L"<w:rPr>" + sCharProp + L"</w:rPr>";
			
			if( lj_none != m_eLevelJust )
			{
				switch( m_eLevelJust )
				{
					case lj_left:	sResult += L"<w:lvlJc w:val=\"left\"/>";	break;
					case lj_center: sResult += L"<w:lvlJc w:val=\"center\"/>";	break;
					case lj_right:	sResult += L"<w:lvlJc w:val=\"right\"/>";	break;
				}
			}
			else
				sResult += L"<w:lvlJc w:val=\"left\"/>";
			
			sResult += L"</w:lvl>";
			sResult += L"</w:abstractNum>";
		}
		else if( RENDER_TO_OOX_PARAM_OLDLIST_OVR == oRenderParameter.nType )
		{
            sResult += L"<w:num w:numId=\"" + std::to_wstring(m_nLs) +
                    L"\"><w:abstractNumId w:val=\"" + std::to_wstring(m_nLs) + L"\"/></w:num>";
		}
		else
		{
			if( PROP_DEF != m_nLs && PROP_DEF != m_nIlvl )
                sResult += L"<w:numPr><w:ilvl w:val=\"" + std::to_wstring(m_nLs) +
                        L"\"/><w:numId w:val=\"" + std::to_wstring(m_nLs) + L"\"/></w:numPr>";
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
    std::wstring sText1 = m_oLevelText->RenderToOOX( oParam );
    std::wstring sText2 = oOldList.m_oLevelText->RenderToOOX( oParam );
	if( sText1 != sText2 )
		return false;
	return  true;
}
