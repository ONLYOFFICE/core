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
#include "OOXtblPrReader.h"
#include "OOXTableRowReader.h"
#include "OOXTableReader.h"

bool OOXTableReader::Parse( ReaderParameter oParam, RtfTable& oOutputTable )
{
	if (m_ooxTable == NULL) return false;

	bool bExistTablPr = false;
	//сначала читаем свойства
	if(m_ooxTable->m_oTableProperties )
	{
		OOXtblPrReader otblPrReader(m_ooxTable->m_oTableProperties);
		otblPrReader.Parse( oParam, oOutputTable.m_oProperty); 
		bExistTablPr = true;
	}

	//формируем внешний стиль для вложенных элементов
	RtfTableStylePtr poTableStyle;
	if( PROP_DEF != oOutputTable.m_oProperty.m_nStyle )
	{
		RtfStylePtr oTempStyle;
		if( true == oParam.oRtf->m_oStyleTable.GetStyle(oOutputTable.m_oProperty.m_nStyle, oTempStyle) )
		{
			RtfStylePtr oResultStyle = oParam.oRtf->m_oStyleTable.GetStyleResulting( oTempStyle );
			if( RtfStyle::stTable == oResultStyle->m_eType )
			{
				poTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oResultStyle );
				
				poTableStyle->m_oTableProp = oOutputTable.m_oProperty; 
				//TableProperty ставим как уже прочитали чтобы не терять порядок применения свойст
				//например индент последовательно затирает друг друга в стилях, numbering, просто в свойствах
				//затирает свойства и на First, Last .... todoooo
			}
		}
	}
	else if( true == bExistTablPr )
	{
		RtfTableStylePtr poTableStyle = RtfTableStylePtr( new RtfTableStyle() );
		poTableStyle->m_oTableProp.Merge( oOutputTable.m_oProperty ); // будут использованы ниже
	}

	if( m_ooxTable->m_oTblGrid.IsInit() )
	{
		for( int i = 0; i < m_ooxTable->m_oTblGrid->m_arrGridCol.size(); i++ )
		{
			if (m_ooxTable->m_oTblGrid->m_arrGridCol[i] && m_ooxTable->m_oTblGrid->m_arrGridCol[i]->m_oW.IsInit())
			oOutputTable.m_aTableGrid.push_back( m_ooxTable->m_oTblGrid->m_arrGridCol[i]->m_oW->ToTwips() );
		}
	}

	long nRowCount = m_ooxTable->m_nCountRow, nCurRow = 0;

	for( int i = 0; i < m_ooxTable->m_arrItems.size(); i++ )
	{
		if (m_ooxTable->m_arrItems[i] == NULL) continue;
		if (m_ooxTable->m_arrItems[i]->getType() != OOX::et_w_tr) continue;

		ReaderParameter newParam	= oParam;
		newParam.poTableStyle		= poTableStyle;

		RtfTableRowPtr oNewRow( new RtfTableRow() );
		//применяем свойства таблицы к каждому row
		//т.к. в RTF нет свойств таблиц и все свойства записываются в свойства row
		(*((RtfTableProperty*)&oNewRow->m_oProperty)).Merge( oOutputTable.m_oProperty );
		
		OOX::Logic::CTr *ooxRow = dynamic_cast<OOX::Logic::CTr *>(m_ooxTable->m_arrItems[i]);
		
		OOXTableRowReader oRowReader(ooxRow);
		oRowReader.Parse( newParam, *oNewRow, nCurRow++, nRowCount );
		oOutputTable.AddItem( oNewRow );
	}
	oOutputTable.CalculateCellx( *oParam.oRtf );
	ApplyParagraphProperty( oOutputTable );
	return true;
}
//применяет свойства параграфа связанные с положением
void OOXTableReader::ApplyParagraphProperty( RtfTable& oOutputTable )
{
	for( int i = 0; i < oOutputTable.GetCount(); i++ )
	{
		boost::shared_ptr<RtfTableRow> oCurRow ;
		oOutputTable.GetItem(oCurRow,i);
		if( oCurRow )
			for( int j = 0; j < oCurRow->GetCount(); j++ )
			{
				boost::shared_ptr<RtfTableCell> oCurCell ;
				oCurRow->GetItem( oCurCell, j );
				if( oCurCell )
				{
					boost::shared_ptr<ITextItem> pCurTextItem;
					for( int k = 0; k < oCurCell->GetCount(); k++ )
					{
						oCurCell->GetItem(pCurTextItem, k);
						if( NULL != pCurTextItem && pCurTextItem->GetType() == TYPE_RTF_PARAGRAPH )
						{
							boost::shared_ptr<RtfParagraph> oCurParagraph = boost::static_pointer_cast<RtfParagraph,ITextItem>( pCurTextItem );
							oCurParagraph->m_oProperty.m_oFrame.ApplyParagraphProp( oOutputTable.m_oProperty );
						}
					}
				}
			}
	}
}