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
#pragma once

#include "OOXTableReader.h"
#include "OOXParagraphReader.h"
#include "OOXtcPrReader.h"

#include "../RtfDocument.h"
#include "../RtfTable.h"

class OOXTableCellReader
{
private:
	OOX::Logic::CTc						*m_ooxTableCell;
	OOX::Logic::CTableProperty			*m_ooxTableProps;
public: 
	OOXTableCellReader(OOX::Logic::CTc *ooxTableCell, OOX::Logic::CTableProperty* ooxTableProps)
	{
		m_ooxTableCell	= ooxTableCell;
		m_ooxTableProps	= ooxTableProps;
	}
	bool Parse( ReaderParameter oParam ,RtfTableCell& oOutputCell,  CcnfStyle oConditionalTableStyle, int nCurCell, int nCellCount, int nCurRow, int nRowCount )
	{
		if (m_ooxTableCell == NULL) return false;

		if( m_ooxTableCell->m_pTableCellProperties )
		{
			OOXtcPrReader oCellPropReader(m_ooxTableCell->m_pTableCellProperties, m_ooxTableProps);
			oCellPropReader.Parse( oParam, oOutputCell.m_oProperty, oConditionalTableStyle, nCurCell, nCellCount, nCurRow, nRowCount );//может поменяться на любой condition (firstRow)
		}
		else
		{
			RtfTableStylePtr oResultStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
			if( NULL != oResultStyle )
				oOutputCell.m_oProperty.Merge( oResultStyle->m_oCellProp );
			oConditionalTableStyle.ApplyTableStyleToCellBorder( oParam.poTableStyle, oOutputCell.m_oProperty, nCurCell, nCellCount, nCurRow, nRowCount );
		}

        for (std::vector<OOX::WritingElement*>::iterator it = m_ooxTableCell->m_arrItems.begin(); it != m_ooxTableCell->m_arrItems.end(); ++it)
		{
			switch((*it)->getType())
			{
				case OOX::et_w_p:
				{
					OOX::Logic::CParagraph * pParagraph = dynamic_cast<OOX::Logic::CParagraph*>(*it);
			
					RtfParagraphPtr oNewParagraph( new RtfParagraph() );
					//применяем к новому параграфу default property
					oNewParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
					oNewParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
					
					OOXParagraphReader oParagraphReader(pParagraph);
					oParagraphReader.Parse( oParam, (*oNewParagraph), oConditionalTableStyle );
					
					//ставим стиль таблицы
					if( NULL != oParam.poTableStyle )
						oNewParagraph->m_oProperty.m_nTableStyle = oParam.poTableStyle->m_nID;
					oNewParagraph->m_oProperty.m_nItap = oParam.oReader->m_nCurItap;
					oNewParagraph->m_oProperty.m_bInTable = true;
					
					oOutputCell.AddItem( oNewParagraph );
				}break;
				case OOX::et_w_tbl:
				{
					OOX::Logic::CTbl * pTbl = dynamic_cast<OOX::Logic::CTbl*>(*it);
					
					oParam.oReader->m_nCurItap ++ ;
					RtfTablePtr oNewTabel( new RtfTable() );
					OOXTableReader oTableReader(pTbl);
					oTableReader.Parse( oParam, *oNewTabel);
					oOutputCell.AddItem( oNewTabel );
					oParam.oReader->m_nCurItap -- ;
				}break;
				default:
				{
					//todooo - универсальный риадер
					//OOXElementReader oElementReader((*it));
					//ITextItemPtr *rtfElement = oElementReader.Parse( oParam);
					//oOutputCell.AddItem( rtfElement );
				}break;
			}
		}
		return true;
	}
};
