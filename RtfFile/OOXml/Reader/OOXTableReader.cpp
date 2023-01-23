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
#include "OOXtblPrReader.h"
#include "OOXTableRowReader.h"
#include "OOXTableReader.h"
#include "OOXParagraphReader.h"

#include "../../../OOXML/DocxFormat/Logic/Table.h"
#include "../../../OOXML/DocxFormat/Logic/Paragraph.h"

OOXtrPrReader::OOXtrPrReader(OOX::Logic::CTableRowProperties*	ooxTableRowProps)
{
	m_ooxTableRowProps = ooxTableRowProps;
}
bool OOXtrPrReader::Parse( ReaderParameter oParam , RtfRowProperty& oOutputProperty, CcnfStyle& oConditionalTableStyle)
{
	if (m_ooxTableRowProps == NULL) return false;

	//ищем cnfStyle  и применяем внешний стиль
	if( NULL != oParam.poTableStyle )
	{
		if( m_ooxTableRowProps->m_oCnfStyle.IsInit() )
		{
			CcnfStyle ocnfStyle;
			OOXcnfStyleReader ocnfStyleReader(m_ooxTableRowProps->m_oCnfStyle.GetPointer());
			ocnfStyleReader.Parse( oParam, ocnfStyle );

			oConditionalTableStyle.Merge( ocnfStyle );
		}
	}
	RtfTableStylePtr oResultStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
	if( NULL != oResultStyle )
		oOutputProperty.Merge( oResultStyle->m_oRowProp );

	if (m_ooxTableRowProps->m_oTblHeader.IsInit() )
		oOutputProperty.m_bIsHeader = m_ooxTableRowProps->m_oTblHeader->m_oVal.ToBool() ? 1 : 0;
	
//todooo непонятнка
//		if (m_ooxTableRowProps->m_oCantSplit.IsInit() )
//             oOutputProperty.m_bIsHeader= m_ooxTableRowProps->m_oCantSplit->m_oVal.ToBool() ? 1 : 0;
	
	if (m_ooxTableRowProps->m_oJc.IsInit() && m_ooxTableRowProps->m_oJc->m_oVal.IsInit())
	{
		switch(m_ooxTableRowProps->m_oJc->m_oVal->GetValue())
		{
			case SimpleTypes::jctableCenter : oOutputProperty.m_eJust = RtfRowProperty::rj_trqc; break;
			case SimpleTypes::jctableEnd    : oOutputProperty.m_eJust = RtfRowProperty::rj_trqr; break;
			case SimpleTypes::jctableStart  : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
			case SimpleTypes::jctableLeft   : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
			case SimpleTypes::jctableRight  : oOutputProperty.m_eJust = RtfRowProperty::rj_trqr; break;
		}
	}

	if( m_ooxTableRowProps->m_oTblHeight.IsInit() && m_ooxTableRowProps->m_oTblHeight->m_oVal.IsInit())
	{
		oOutputProperty.m_nHeight = m_ooxTableRowProps->m_oTblHeight->m_oVal->ToTwips();
		
		if (m_ooxTableRowProps->m_oTblHeight->m_oHRule.IsInit() && m_ooxTableRowProps->m_oTblHeight->m_oHRule->GetValue() == SimpleTypes::heightruleExact)
			oOutputProperty.m_nHeight = -oOutputProperty.m_nHeight;
	}

	OOXtcPrReader::Parse(m_ooxTableRowProps->m_oWBefore.GetPointer(), oOutputProperty.m_eWidthStartInvCellUnit, oOutputProperty.m_nWidthStartInvCell);
	OOXtcPrReader::Parse(m_ooxTableRowProps->m_oWAfter.GetPointer(), oOutputProperty.m_eWidthEndInvCellUnit, oOutputProperty.m_nWidthEndInvCell);

	if( m_ooxTableRowProps->m_oGridBefore.IsInit() && m_ooxTableRowProps->m_oGridBefore->m_oVal.IsInit())
		oOutputProperty.m_nGridBefore = *m_ooxTableRowProps->m_oGridBefore->m_oVal;

	if( m_ooxTableRowProps->m_oGridAfter.IsInit() && m_ooxTableRowProps->m_oGridAfter->m_oVal.IsInit())
		oOutputProperty.m_nGridAfter = *m_ooxTableRowProps->m_oGridAfter->m_oVal;
	
	if (m_ooxTableRowProps->m_oTrPrChange.IsInit())
	{
		if (m_ooxTableRowProps->m_oTrPrChange->m_sAuthor.IsInit())
			oOutputProperty.m_nTrAuth = oParam.oRtf->m_oRevisionTable.AddAuthor( m_ooxTableRowProps->m_oTrPrChange->m_sAuthor.get2() ) + 1;
		
		if (m_ooxTableRowProps->m_oTrPrChange->m_oDate.IsInit())
        {
            std::wstring sVal = m_ooxTableRowProps->m_oTrPrChange->m_oDate->GetValue();
            oOutputProperty.m_nTrDate = RtfUtility::convertDateTime( sVal);
        }
		
		RtfRowPropertyPtr props ( new RtfRowProperty() );
		OOXtrPrReader oTrReader(m_ooxTableRowProps->m_oTrPrChange->m_pTrPr.GetPointer());
	
		CcnfStyle style;
		if (oTrReader.Parse( oParam, *props.get(), style))
		{
			oOutputProperty.m_pOldRowProperty = props;
		}

	}
	return true;
}

OOXTableReader::OOXTableReader(OOX::Logic::CTbl *ooxTable)
{
	m_ooxTable = ooxTable;
}
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
		RtfTableStylePtr poTableStyle ( new RtfTableStyle() );
		poTableStyle->m_oTableProp.Merge( oOutputTable.m_oProperty ); // будут использованы ниже
	}

	if( m_ooxTable->m_oTblGrid.IsInit() )
	{
		for (size_t i = 0; i < m_ooxTable->m_oTblGrid->m_arrGridCol.size(); i++ )
		{
			if (m_ooxTable->m_oTblGrid->m_arrGridCol[i] && m_ooxTable->m_oTblGrid->m_arrGridCol[i]->m_oW.IsInit())
			{
				oOutputTable.m_aTableGrid.push_back( m_ooxTable->m_oTblGrid->m_arrGridCol[i]->m_oW->ToTwips() );
			}
		}
	}

	long nRowCount = m_ooxTable->m_nCountRow, nCurRow = 0;

    for (size_t i =0; i < m_ooxTable->m_arrItems.size(); ++i)
	{	
		if ( m_ooxTable->m_arrItems[i] == NULL) continue;
		if ( m_ooxTable->m_arrItems[i]->getType() != OOX::et_w_tr) continue;

		ReaderParameter newParam	= oParam;
		newParam.poTableStyle		= poTableStyle;

		RtfTableRowPtr oNewRow ( new RtfTableRow() );
		//применяем свойства таблицы к каждому row
		//т.к. в RTF нет свойств таблиц и все свойства записываются в свойства row
		(*((RtfTableProperty*)&oNewRow->m_oProperty)).Merge( oOutputTable.m_oProperty );
		
		OOX::Logic::CTr *ooxRow = dynamic_cast<OOX::Logic::CTr *>(m_ooxTable->m_arrItems[i]);
		
		OOXTableRowReader oRowReader(ooxRow, m_ooxTable->m_oTableProperties);
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
	for (int i = 0; i < oOutputTable.GetCount(); i++ )
	{
		boost::shared_ptr<RtfTableRow> oCurRow ;
		oOutputTable.GetItem(oCurRow,i);
		if( oCurRow )
			for (int j = 0; j < oCurRow->GetCount(); j++ )
			{
				boost::shared_ptr<RtfTableCell> oCurCell ;
				oCurRow->GetItem( oCurCell, j );
				if( oCurCell )
				{
					boost::shared_ptr<ITextItem> pCurTextItem;
					for (int k = 0; k < oCurCell->GetCount(); k++ )
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

OOXTableRowReader::OOXTableRowReader(OOX::Logic::CTr *ooxRowTable, OOX::Logic::CTableProperty* ooxTableProps)
{
    m_ooxRowTable	= ooxRowTable;
    m_ooxTableProps	= ooxTableProps;
}
bool OOXTableRowReader::Parse( ReaderParameter oParam, RtfTableRow& oOutputRow, int nCurRow, int nRowCount)
{
    if (m_ooxRowTable == NULL) return false;

    CcnfStyle oConditionStyle;

    if (nCurRow == 0 && oOutputRow.m_oProperty.m_bAutoFirstRow == 1)
        oConditionStyle.bFirstRow = true;

    if (nCurRow == nRowCount - 1 && oOutputRow.m_oProperty.m_bAutoLastRow == 1)
        oConditionStyle.bLastRow = true;

    //сначала применяем свойства
    if( m_ooxRowTable->m_pTableRowProperties )
    {
        OOXtrPrReader otrPrReader(m_ooxRowTable->m_pTableRowProperties);
        otrPrReader.Parse( oParam, oOutputRow.m_oProperty, oConditionStyle);// может поменяться на любой condition(first row)
    }

    int nCellCount = m_ooxRowTable->m_nCountCell, nCurCell = 0;

    for (size_t i = 0; i < m_ooxRowTable->m_arrItems.size(); ++i)
    {
        if ( m_ooxRowTable->m_arrItems[i] == NULL )		continue;
        if ( m_ooxRowTable->m_arrItems[i]->getType() != OOX::et_w_tc) continue;//todooo bookmarks

        RtfTableCellPtr oNewCell( new RtfTableCell() );

        OOX::Logic::CTc *ooxCell = NULL;

        if (nCurCell < (int)m_ooxRowTable->m_arrItems.size())
        {
            ooxCell = dynamic_cast<OOX::Logic::CTc *>(m_ooxRowTable->m_arrItems[i]);
        }

        OOXTableCellReader oCellReader(ooxCell, m_ooxTableProps );
        oCellReader.Parse( oParam, *oNewCell, oConditionStyle, nCurCell++, nCurRow, nCellCount, nRowCount );
        //добавляем cell
        oOutputRow.AddItem(oNewCell);
        //свойства cell в row
        oOutputRow.m_oProperty.AddItem( oNewCell->m_oProperty );
    }
    return true;
}

OOXTableCellReader::OOXTableCellReader(OOX::Logic::CTc *ooxTableCell, OOX::Logic::CTableProperty* ooxTableProps)
{
    m_ooxTableCell	= ooxTableCell;
    m_ooxTableProps	= ooxTableProps;
}
bool OOXTableCellReader::Parse( ReaderParameter oParam ,RtfTableCell& oOutputCell,  CcnfStyle oConditionalTableStyle, int nCurCell, int nCellCount, int nCurRow, int nRowCount )
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
                oNewParagraph->m_oProperty.m_bInTable = 1;

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
