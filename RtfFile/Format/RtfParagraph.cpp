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
#include "RtfParagraph.h"
#include "RtfWriter.h"

#include "../OOXml/Writer/OOXWriter.h"

bool RtfParagraph::IsValid()
{
	return m_IsValid;
}
int RtfParagraph::GetType( )
{
	return TYPE_RTF_PARAGRAPH;
}
RtfParagraph::RtfParagraph()
{
}
void RtfParagraph::SetValid(bool val)
{
	m_IsValid = val;
}

int RtfParagraph::AddItem( IDocumentElementPtr piRend )
{
	m_IsValid = true;

	if( TYPE_RTF_CHAR == piRend->GetType() )
	{
		if( m_aArray.size() > 0 && TYPE_RTF_CHAR == m_aArray[ m_aArray.size() - 1 ]->GetType() )//merge two texts with identical properties
		{
            RtfCharPtr oCurChar = boost::static_pointer_cast<RtfChar,IDocumentElement>( piRend );
            RtfCharPtr oPrevChar = boost::static_pointer_cast<RtfChar,IDocumentElement>( m_aArray[ m_aArray.size() - 1 ] );
			if( oCurChar->m_oProperty == oPrevChar->m_oProperty )
			{
				oPrevChar->AddText( oCurChar->GetText() );
				return (int)m_aArray.size() - 1;
			}
		}				
	}
	ItemContainer< IDocumentElementPtr >::AddItem( piRend );
	return (int)m_aArray.size() - 1;
}

std::wstring RtfParagraph::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult ;
	if( RENDER_TO_RTF_PARAM_CHAR == oRenderParameter.nType )
	{
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i]->RenderToRtf( oRenderParameter );
		}
	}
	else
	{
		sResult += L"\\pard\\plain";
		sResult += m_oProperty.RenderToRtf( oRenderParameter ) ;
		
		if( NULL != m_oOldList )
			sResult += m_oOldList->RenderToRtf( oRenderParameter ) ;

		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i]->RenderToRtf( oRenderParameter );
		}
		
		sResult += m_oProperty.m_oCharProperty.RenderToRtf( oRenderParameter );


		//if( RENDER_TO_RTF_PARAM_NO_PAR != oRenderParameter.nValue )
		//	sResult += T("\\par";
	}
	return sResult;
}

std::wstring RtfParagraph::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	pRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		pOOXWriter		= static_cast<OOXWriter*>(oRenderParameter.poWriter);

	std::wstring sResult ;

	if (RENDER_TO_OOX_PARAM_PLAIN == oRenderParameter.nType )
	{
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i]->RenderToOOX(oRenderParameter);
		}
	}
	else if (RENDER_TO_OOX_PARAM_RUN == oRenderParameter.nType ||
			 RENDER_TO_OOX_PARAM_FIELD == oRenderParameter.nType)
	{
		for (size_t i = 0; i < m_aArray.size(); i++ )
		{
			sResult += m_aArray[i]->RenderToOOX(oRenderParameter);
		}
	}
	else if (RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType )
	{
		if (m_aArray.size() < 1)
		{
			sResult += L"<w:rPr>";
			sResult += m_oProperty.m_oCharProperty.RenderToOOX(oRenderParameter);
			sResult += L"</w:rPr>";
		}
		else
		{
			for (size_t i = 0; i < m_aArray.size(); i++ )
			{
				sResult += m_aArray[i]->RenderToOOX(oRenderParameter);
			}
		}
	}
	else
	{		
		RenderParameter oNewParam = oRenderParameter;
		oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;
		
		bool bCanConvertToNumbering = false;
		if( NULL != m_oOldList )
			bCanConvertToNumbering = m_oOldList->CanConvertToNumbering();

		sResult += L"<w:p";
		if (oRenderParameter.nType == RENDER_TO_OOX_PARAM_COMMENT)
		{
			std::wstring sParaId = XmlUtils::ToString(++pOOXWriter->m_nextParaId, L"%08X");
			sResult += L" w14:paraId=\"" + sParaId + L"\" w14:textId=\"" + sParaId + L"\"";
		}
		sResult += L"><w:pPr>";
		
		m_oProperty.m_bList = (NULL != m_oOldList) || m_oProperty.m_bList;
		
		bool bRenderProps = false;
		if ( PROP_DEF != m_oProperty.m_nTableStyle && m_oProperty.m_bInTable > 0)
		{
			RtfStylePtr oCurStyle;
			if( true == pRtfDocument->m_oStyleTable.GetStyle( m_oProperty.m_nTableStyle, oCurStyle ) )
			{
				RtfParagraphStyle* oCurParaStyle = dynamic_cast<RtfParagraphStyle*>(oCurStyle.get());

				if (oCurParaStyle)
				{
					RtfParagraphProperty newProps;
					newProps.Merge(oCurParaStyle->m_oParProp);
					newProps.Merge(m_oProperty);
					
					sResult += newProps.RenderToOOX(oRenderParameter);
					bRenderProps = true;
				}
			}
		}
		if (false == bRenderProps)
			sResult += m_oProperty.RenderToOOX(oRenderParameter);

		if( NULL != m_oOldList )
		{
			//for OldList
			if( true == bCanConvertToNumbering )
				sResult += m_oOldList->RenderToOOX( oRenderParameter );
		}
		sResult += L"</w:pPr>";

		if( NULL != m_oOldList)
		{
			//for OldList
			if( false == bCanConvertToNumbering && NULL != m_oOldList->m_oText)
			{
				RtfCharProperty oCharProp = m_oProperty.m_oCharProperty;
				if( NULL != m_oOldList->m_oText )
					oCharProp.m_nFont = m_oOldList->m_oText->m_oProperty.m_oCharProperty.m_nFont;

				oNewParam = oRenderParameter;
				oNewParam.nType = RENDER_TO_OOX_PARAM_TEXT;

				for( int i = 0; i < m_oOldList->m_oText->GetCount(); i++ )
				{
					sResult += L"<w:r>";
						sResult += L"<w:rPr>";
							sResult += oCharProp.RenderToOOX(oRenderParameter);
						sResult += L"</w:rPr>";
					sResult += m_oOldList->m_oText->m_aArray[ i ]->RenderToOOX(oNewParam);
					sResult += L"</w:r>";
				}
			}
		}
		if (oRenderParameter.nType == RENDER_TO_OOX_PARAM_COMMENT)
		{
			sResult += L"<w:r>";
				sResult += L"<w:rPr>";
					sResult += m_oProperty.m_oCharProperty.RenderToOOX(oRenderParameter);
				sResult += L"</w:rPr>";
			sResult += L"<w:annotationRef/>";
			sResult += L"</w:r>";
		}
		oNewParam.nType = RENDER_TO_OOX_PARAM_RUN;

		// in RTF a run without an explicit \cf keeps the default (auto) color and does NOT inherit
		// the paragraph style color; in OOXML an omitted <w:color> inherits it. Pass the style fore
		// color down so runs without a color can block that inheritance with <w:color w:val="auto"/>.
		if (PROP_DEF != m_oProperty.m_nStyle)
		{
			RtfStylePtr oParaStyle;
			_INT32 nStyleId = m_oProperty.m_nStyle;
			for (int nGuard = 0; nGuard < 32 && PROP_DEF != nStyleId && pRtfDocument->m_oStyleTable.GetStyle(nStyleId, oParaStyle); ++nGuard)
			{
				RtfParagraphStyle* pParaStyle = dynamic_cast<RtfParagraphStyle*>(oParaStyle.get());
				if (NULL == pParaStyle) break;
				if (PROP_DEF != pParaStyle->m_oCharProp.m_nForeColor)
				{
					oNewParam.nParaStyleForeColor = pParaStyle->m_oCharProp.m_nForeColor;
					break;
				}
				nStyleId = (pParaStyle->m_nBasedOn != nStyleId) ? pParaStyle->m_nBasedOn : PROP_DEF;
			}
		}

        std::wstring ParagraphContent;
		for (size_t i = 0; i < m_aArray.size(); i++)
		{
			ParagraphContent += m_aArray[i]->RenderToOOX(oNewParam);
		}

        if (!ParagraphContent.empty())
			sResult += ParagraphContent;

		sResult += L"</w:p>";
	}
	return sResult;
}


