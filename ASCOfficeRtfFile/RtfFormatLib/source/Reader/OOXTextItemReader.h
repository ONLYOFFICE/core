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
#pragma once
#include "../RtfDocument.h"
#include "../RtfTable.h"

#include "OOXTableReader.h"
#include "OOXParagraphReader.h"

class OOXTextItemReader
{
private:
//	OOXParagraphReader	m_oParagraphReader; ///?????
//	OOXTableReader		m_oTableReader;

public: 
	TextItemContainerPtr m_oTextItems;

	OOXTextItemReader( )
	{
		m_oTextItems = TextItemContainerPtr( new TextItemContainer() );
	}
	bool Parse( OOX::WritingElement* ooxElement, ReaderParameter oParam )
	{
		switch(ooxElement->getType())
		{
			case OOX::et_w_p:
			{
				OOX::Logic::CParagraph * pParagraph = dynamic_cast<OOX::Logic::CParagraph*>(ooxElement);
				
				OOXParagraphReader	m_oParagraphReader(pParagraph);
				RtfParagraphPtr oNewParagraph( new RtfParagraph() );
				//применяем к новому параграфу default property
				oNewParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
				oNewParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
				oNewParagraph->m_oProperty.m_nItap = 0;

				if( true == m_oParagraphReader.Parse( oParam, (*oNewParagraph), CcnfStyle() ))
				{
					m_oTextItems->AddItem( oNewParagraph );
					if( true == oParam.oRtf->m_oStatusSection.start_new )
					{
						RtfSectionPtr oCurSection;
						if( true == oParam.oRtf->GetItem( oCurSection, oParam.oRtf->m_oStatusSection.number - 1) )
						{
							m_oTextItems = oCurSection;
						}
						oParam.oRtf->m_oStatusSection.start_new = false;
					}
				}
			}break;
			case OOX::et_w_tbl:
			{
				OOX::Logic::CTbl * pTbl = dynamic_cast<OOX::Logic::CTbl*>(ooxElement);
				RtfTablePtr oNewTable( new RtfTable() );
				
				OOXTableReader		oTableReader(pTbl);
				oParam.oReader->m_nCurItap = 1;
				if( true == oTableReader.Parse( oParam, (*oNewTable)) )
				{
					m_oTextItems->AddItem( oNewTable );
				}
				oParam.oReader->m_nCurItap = 0;
			}break;
			case OOX::et_w_sdt:
			{
				OOX::Logic::CSdt * pSdt = dynamic_cast<OOX::Logic::CSdt*>(ooxElement);
				if( pSdt->m_oSdtEndPr.IsInit())
				{
					//todo
				}
				if( pSdt->m_oSdtContent.IsInit() )
				{
					Parse( pSdt->m_oSdtContent.GetPointer(), oParam );
				}
			}break;
		}
		return true;
	}
};