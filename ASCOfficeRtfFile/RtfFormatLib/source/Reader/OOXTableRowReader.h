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

#include "OOXTableCellReader.h"
#include "OOXtrPrReader.h"

#include "../RtfDocument.h"
#include "../RtfTableRow.h"

class OOXTableRowReader
{
private:
	OOX::Logic::CTr				*m_ooxRowTable;
	OOX::Logic::CTableProperty	*m_ooxTableProps;
public: 
	OOXTableRowReader(OOX::Logic::CTr *ooxRowTable, OOX::Logic::CTableProperty* ooxTableProps)
	{
		m_ooxRowTable	= ooxRowTable;
		m_ooxTableProps	= ooxTableProps;
	}
	
	bool Parse( ReaderParameter oParam ,RtfTableRow& oOutputRow, int nCurRow, int nRowCount)
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
};
