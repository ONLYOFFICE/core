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
				poTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oResultStyle );
				poTableStyle->m_oTableProp = oOutputTable.m_oProperty; //TableProperty ставим как уже прочитали чтобы не терять порядок применения свойст
				//например индент последовательно затирает друг друга в стилях, numbering, просто в свойствах
			}
		}
	}
	else if( true == bExistTablPr )
	{
		RtfTableStylePtr poTableStyle = RtfTableStylePtr( new RtfTableStyle() );
		poTableStyle->m_oTableProp.Merge( oOutputTable.m_oProperty );
	}

	if( m_ooxTable->m_oTblGrid.IsInit() )
	{
		for( int i = 0; i < m_ooxTable->m_oTblGrid->m_arrGridCol.size(); i++ )
		{
			if (m_ooxTable->m_oTblGrid->m_arrGridCol[i] && m_ooxTable->m_oTblGrid->m_arrGridCol[i]->m_oW.IsInit())
			oOutputTable.m_aTableGrid.push_back( m_ooxTable->m_oTblGrid->m_arrGridCol[i]->m_oW->ToTwips() );
		}
	}

	long nRowCount = m_ooxTable->m_nCountRow;

	for( int i = 0; i < m_ooxTable->m_arrItems.size(); i++ )
	{
		if (m_ooxTable->m_arrItems[i] == NULL) continue;
		if (m_ooxTable->m_arrItems[i]->getType() != OOX::et_w_tr) continue;

		RtfTableRowPtr oNewRow( new RtfTableRow() );
		//применяем свойства таблицы к каждому row
		//т.к. в RTF нет свойств таблиц и все свойства записываются в свойства row
		(*((RtfTableProperty*)&oNewRow->m_oProperty)).Merge( oOutputTable.m_oProperty );
		
		OOX::Logic::CTr *ooxRow = dynamic_cast<OOX::Logic::CTr *>(m_ooxTable->m_arrItems[i]);
		
		OOXTableRowReader oRowReader(ooxRow);
		oRowReader.Parse( oParam, *oNewRow, i, nRowCount );
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
							boost::shared_ptr<RtfParagraph> oCurParagraph = boost::shared_static_cast<RtfParagraph,ITextItem>( pCurTextItem );
							oCurParagraph->m_oProperty.m_oFrame.ApplyParagraphProp( oOutputTable.m_oProperty );
						}
					}
				}
			}
	}
}