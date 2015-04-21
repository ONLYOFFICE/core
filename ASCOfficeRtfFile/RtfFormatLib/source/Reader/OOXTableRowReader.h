#pragma once

#include "OOXTableCellReader.h"
#include "OOXtrPrReader.h"

#include "../RtfDocument.h"
#include "../RtfTableRow.h"

class OOXTableRowReader
{
private:
	OOX::Logic::CTr *m_ooxRowTable;
public: 
	OOXTableRowReader(OOX::Logic::CTr *ooxRowTable)
	{
		m_ooxRowTable = ooxRowTable;
	}
	
	bool Parse( ReaderParameter oParam ,RtfTableRow& oOutputRow, int nCurRow, int nRowCount)
	{
		if (m_ooxRowTable == NULL) return false;

		CcnfStyle oConditionStyle;
		//с начала применяем свойства
		if( m_ooxRowTable->m_oTableRowProperties )
		{
			OOXtrPrReader otrPrReader(m_ooxRowTable->m_oTableRowProperties);
			otrPrReader.Parse( oParam, oOutputRow.m_oProperty, oConditionStyle);// может поменяться на любой condition(first row)
		}

		int nCellCount = m_ooxRowTable->m_nCountCell;

		for( int nCurCell = 0; nCurCell < nCellCount; nCurCell++)
		{
			RtfTableCellPtr oNewCell( new RtfTableCell() );

            OOX::Logic::CTc *ooxCell = NULL;

            if (nCurCell < m_ooxRowTable->m_arrItems.size())
                ooxCell = dynamic_cast<OOX::Logic::CTc *>(m_ooxRowTable->m_arrItems[nCurCell]);

			if (ooxCell)
			{
				OOXTableCellReader oCellReader(ooxCell);
				oCellReader.Parse( oParam, *oNewCell, oConditionStyle, nCurCell, nCurRow, nCellCount, nRowCount );
				//добавляем cell
				oOutputRow.AddItem(oNewCell);
				//свойства cell в row
				oOutputRow.m_oProperty.AddItem( oNewCell->m_oProperty );
			}
		}
		return true;
	}
};
