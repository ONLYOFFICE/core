#pragma once

#include "OOXTableReader.h"
#include "OOXParagraphReader.h"
#include "OOXtcPrReader.h"

#include "../RtfDocument.h"
#include "../RtfTable.h"

class OOXTableCellReader
{
private:
	OOX::Logic::CTc		*m_ooxTableCell;
public: 
	OOXTableCellReader(OOX::Logic::CTc *ooxTableCell)
	{
		m_ooxTableCell = ooxTableCell;
	}
	bool Parse( ReaderParameter oParam ,RtfTableCell& oOutputCell,  CcnfStyle oConditionalTableStyle, int nCurCell, int nCellCount, int nCurRow, int nRowCount )
	{
		if (m_ooxTableCell == NULL) return false;

		if( m_ooxTableCell->m_oTableCellProperties )
		{
			OOXtcPrReader oCellPropReader(m_ooxTableCell->m_oTableCellProperties);
			oCellPropReader.Parse( oParam, oOutputCell.m_oProperty, oConditionalTableStyle, nCurCell, nCellCount, nCurRow, nRowCount );//может поменяться на любой condition (firstRow)
		}
		else
		{
			RtfTableStylePtr oResultStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
			if( NULL != oResultStyle )
				oOutputCell.m_oProperty.Merge( oResultStyle->m_oCellProp );
			oConditionalTableStyle.ApplyTableStyleToCellBorder( oParam.poTableStyle, oOutputCell.m_oProperty, nCurCell, nCellCount, nCurRow, nRowCount );
		}

		for(int i = 0; i < m_ooxTableCell->m_arrItems.size(); i++ )
		{
			switch(m_ooxTableCell->m_arrItems[i]->getType())
			{
				case OOX::et_w_p:
				{
					OOX::Logic::CParagraph * pParagraph = static_cast<OOX::Logic::CParagraph*>(m_ooxTableCell->m_arrItems[i]);
			
					RtfParagraphPtr oNewParagraph( new RtfParagraph() );
					//применяем к новому параграфу default property
					oNewParagraph->m_oProperty = oParam.oRtf->m_oDefaultParagraphProp;
					oNewParagraph->m_oProperty.m_oCharProperty = oParam.oRtf->m_oDefaultCharProp;
					
					OOXParagraphReader oParagraphReader(pParagraph);
					bool bStartNewSection;
					oParagraphReader.Parse( oParam, (*oNewParagraph), oConditionalTableStyle, bStartNewSection );
					
					//ставим стиль таблицы
					if( NULL != oParam.poTableStyle )
						oNewParagraph->m_oProperty.m_nTableStyle = oParam.poTableStyle->m_nID;
					oNewParagraph->m_oProperty.m_nItap = oParam.oReader->m_nCurItap;
					oNewParagraph->m_oProperty.m_bInTable = true;
					
					oOutputCell.AddItem( oNewParagraph );
				}break;
				case OOX::et_w_tbl:
				{
					OOX::Logic::CTbl * pTbl = static_cast<OOX::Logic::CTbl*>(m_ooxTableCell->m_arrItems[i]);
					
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
					//OOXElementReader oElementReader(m_ooxTableCell->m_arrItems[i]);
					//ITextItemPtr *rtfElement = oElementReader.Parse( oParam);
					//oOutputCell.AddItem( rtfElement );
				}break;
			}
		}
		return true;
	}
};
