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

#include "CELLTABLE.h"
#include "CELL.h"

#include "../Biff_records/EntExU2.h"
#include "../Biff_records/Row.h"
#include "../Biff_records/DBCell.h"
#include "../Biff_records/MulBlank.h"
#include "../Biff_records/MulRk.h"

namespace XLS
{

class CELL_GROUP : public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CELL_GROUP)
public:
	CELL_GROUP(std::vector<CellRangeRef>& shared_formulas_locations_ref) : 
							shared_formulas_locations_ref_(shared_formulas_locations_ref)
	{
	}

	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new CELL_GROUP(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		global_info_ = proc.getGlobalWorkbookInfo();
		int index_sheet_info_ = global_info_->current_sheet - 1;

		GlobalWorkbookInfo::_sheet_info & sheet_info = global_info_->sheets_info[index_sheet_info_];
		
		int count, count_row = 0;
		
		bool bRow = false;

		bRow = (global_info_->Version == 0x0200) ? proc.mandatory<Row_BIFF2>() : proc.mandatory<Row>();
		
		if (bRow == true)
		{
			count_row = count =  1 + ((global_info_->Version == 0x0200) ? proc.repeated<Row_BIFF2>(0, 0) : proc.repeated<Row>(0, 0));
		
			while(count > 0)
			{
				Row* row = dynamic_cast<Row*>(elements_.front().get());
				if (row)
				{
					if (row->miyRw > 0 && std::abs(row->miyRw/20. - sheet_info.defaultRowHeight) > 0.001)
					{
						sheet_info.customRowsHeight.insert(std::make_pair(row->rw, row->miyRw / 20.));
					}
				}
				std::map<int, GlobalWorkbookInfo::_row_info>::iterator pFindRow = sheet_info.mapRows.find(row->rw);
				if (pFindRow == sheet_info.mapRows.end())
				{
					GlobalWorkbookInfo::_row_info row_info;

					row_info.row_info = elements_.front();
					sheet_info.mapRows.insert(std::make_pair(row->rw, row_info));
				}
				else
				{
					if (!pFindRow->second.row_info)
					{
						pFindRow->second.row_info = elements_.front();
					}
					else
					{
						Row* row_find = dynamic_cast<Row*>(pFindRow->second.row_info.get());
						if ((row_find) && (false == row_find->bValid) && row->bValid)
						{
							pFindRow->second.row_info = elements_.front();
						}

					}
				}

				elements_.pop_front();
				count--;				
			}
		}	
		
		//------------------------------------------------------------------------------------------------------------------
        CELL cell(shared_formulas_locations_ref_);

        int count_cells = count = proc.repeated(cell, 0, 0);

		while(count > 0)
		{
			CELL * cell = dynamic_cast<CELL *>(elements_.front().get());
			if (cell)
			{
				std::map<int, GlobalWorkbookInfo::_row_info>::iterator pFindRow = sheet_info.mapRows.find(cell->RowNumber);
				if (pFindRow == sheet_info.mapRows.end())
				{
					GlobalWorkbookInfo::_row_info row_info;
					sheet_info.mapRows.insert(std::make_pair(cell->RowNumber, row_info));

					pFindRow = sheet_info.mapRows.find(cell->RowNumber);
				}
				std::map<int, BaseObjectPtr>::iterator pFindCell = pFindRow->second.mapCells.find(cell->ColumnNumber);

				if (pFindCell != pFindRow->second.mapCells.end())
				{
					CELL* cell_prev = dynamic_cast<CELL *>(pFindCell->second.get());

					pFindCell->second = elements_.front();
				}
				else
				{
					pFindRow->second.mapCells.insert(std::make_pair(cell->ColumnNumber, elements_.front()));
				}
			}
			elements_.pop_front();
			count--;				
		}
		count = proc.repeated<DBCell>(0, 0); 
		// OpenOffice Calc stored files workaround (DBCell must be present at least once according to [MS-XLS])
		while(count > 0)
		{
			m_DBCells.insert(m_DBCells.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}	
		if (count_cells > 0 || count_row > 0)	return true;
		else									return false;
	}
	
	static const ElementType type = typeCELL_GROUP;

//---------------------------------------------------------------------------
	std::list<BaseObjectPtr> m_DBCells;

private:
	std::vector<CellRangeRef>& shared_formulas_locations_ref_;

	GlobalWorkbookInfoPtr global_info_;
};
//-----------------------------------------------------------------------------------------------------------------
CELLTABLE::CELLTABLE(std::vector<CellRangeRef>& shared_formulas_locations_ref) : 
											isConcatinate_(false), shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
}


CELLTABLE::~CELLTABLE()
{
}

BaseObjectPtr CELLTABLE::clone()
{
	return BaseObjectPtr(new CELLTABLE(*this));
}


// CELLTABLE = 1*(1*Row *CELL 1*DBCell) *EntExU2
const bool CELLTABLE::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();
	index_sheet_info_ = global_info_->current_sheet - 1;

	GlobalWorkbookInfo::_sheet_info zero;
	while (index_sheet_info_ >= global_info_->sheets_info.size())
	{
		global_info_->sheets_info.push_back(zero);
	}

	CELL_GROUP cell_group(shared_formulas_locations_ref_);
	if(!proc.mandatory(cell_group))
	{
		return false;
	}
		
	int count = 1 + proc.repeated(cell_group, 0, 0);
	
	while(!elements_.empty())
	{
		//m_arCELLGROUP.insert(m_arCELLGROUP.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	
	count = proc.repeated<EntExU2>(0, 0);
	while(count > 0)
	{
		m_arEntExU2.insert(m_arEntExU2.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	
	return true;
}
int CELLTABLE::serialize(std::wostream & stream)
{
	GlobalWorkbookInfo::_sheet_info & sheet_info = global_info_->sheets_info[index_sheet_info_];

	CP_XML_WRITER(stream)    
    {	
		for (std::map<int, GlobalWorkbookInfo::_row_info>::iterator it_row = sheet_info.mapRows.begin(); it_row != sheet_info.mapRows.end(); it_row++)
		{
			Row* row = dynamic_cast<Row*>(it_row->second.row_info.get());
			
			if (row && it_row->second.mapCells.empty())
			{
				it_row->second.row_info->serialize(stream);
			}
			else
			{
				CP_XML_NODE(L"row")
				{	
					CP_XML_ATTR(L"r", it_row->first + 1);
					
					if (row)
					{
						bool xf_set = true;
						if (row->fGhostDirty == false) xf_set = false;
						
						if (row->colMic >= 0 && row->colMac > row->colMic)
						{
							CP_XML_ATTR(L"spans", std::to_wstring(row->colMic + 1) + L":" + std::to_wstring(row->colMac));  //zero based & one based
						}
						if (xf_set)
						{
							int xf = row->ixfe_val >= global_info_->cellStyleXfs_count ? row->ixfe_val - global_info_->cellStyleXfs_count : -1/*row->ixfe_val*/;
							
							if (xf < global_info_->cellXfs_count && xf >= 0)
							{
								CP_XML_ATTR(L"s", xf);
								CP_XML_ATTR(L"customFormat", true);
							}
						}
						if (row->miyRw > 0 && row->miyRw < 0x8000 && row->bValid &&
							((row->fUnsynced && row->fGhostDirty) || !row->fGhostDirty))
			//v8_14A_1b13.xls //Department_Sales_and_Stock_Monthly_Recap_Store_778_2019-09-03.xls
			//Уведомления об ознакомлении.xls
						{
							CP_XML_ATTR(L"ht", row->miyRw / 20.);
							if (row->fUnsynced)
							{
								CP_XML_ATTR(L"customHeight", true);
							}
							else
							{
							}
						}
						if (row->iOutLevel > 0)
						{
							CP_XML_ATTR(L"outlineLevel", row->iOutLevel);
						}
						if (row->fCollapsed)
						{
							CP_XML_ATTR(L"collapsed", row->fCollapsed);
						}
						if (row->fExAsc)
						{
							CP_XML_ATTR(L"thickTop", true);
						}
						if (row->fExDes)
						{
							CP_XML_ATTR(L"thickBot", true);
						}
						if (row->fDyZero)
						{
							CP_XML_ATTR(L"hidden", true);
						}						
					}
					for ( std::map<int, BaseObjectPtr>::iterator it_cell = it_row->second.mapCells.begin(); it_cell != it_row->second.mapCells.end(); it_cell++)
					{
						//if (isConcatinate_)
						{
							CELL* cell = dynamic_cast<CELL*>((it_cell->second).get());

							MulBlank	*mulblank	= dynamic_cast<MulBlank*>(cell->elements_.begin()->get());
							MulRk		*mulrk		= dynamic_cast<MulRk*>(cell->elements_.begin()->get());

							if (mulblank || mulrk)
							{
								std::map<int, BaseObjectPtr>::iterator it_next_cell =  it_cell; it_next_cell++;
								if (it_next_cell != it_row->second.mapCells.end())
								{
									CELL* cell_next = dynamic_cast<CELL*>((it_next_cell->second).get());

									if (mulblank)	mulblank->colLast = (std::min)((int)mulblank->colLast, cell_next->ColumnNumber - 1);
									if (mulrk)		mulrk->colLast = (std::min)((int)mulrk->colLast, cell_next->ColumnNumber - 1);
								}
							}
						}
						(it_cell->second)->serialize(CP_XML_STREAM());
					}
				}
			}
		}

	}
	return 0;
}
} // namespace XLS

