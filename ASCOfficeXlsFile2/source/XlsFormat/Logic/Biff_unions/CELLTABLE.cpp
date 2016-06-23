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

#include "CELLTABLE.h"
#include <Logic/Biff_records/EntExU2.h>
#include <Logic/Biff_records/Row.h>
#include <Logic/Biff_unions/CELL.h>
#include <Logic/Biff_records/DBCell.h>

#include <simple_xml_writer.h>

namespace XLS
{

// This class is made a deriver of CompositeObject intentionally.
// This is an optimization step - to form a CELLTABLE that is divided into smaller groups
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

		XLS::GlobalWorkbookInfo::_sheet_size_info zero;
		XLS::GlobalWorkbookInfo::_sheet_size_info & sheet_info = global_info_->current_sheet >=0 ? 
											global_info_->sheet_size_info[global_info_->current_sheet - 1] : zero;
		
		int count, count_row = 0;
		
		if(proc.mandatory<Row>() == true)
		{
			count_row = count =  1 + proc.repeated<Row>(0, 0);
		
			while(count > 0)
			{
				Row* row = dynamic_cast<Row*>(elements_.front().get());
				if (row)
				{
					if (row->miyRw > 0 && std::abs(row->miyRw/20. - sheet_info.defaultRowHeight) > 0.001)
					{
						sheet_info.customRowsHeight.insert(std::pair<int, double>(row->rw, row->miyRw / 20.));
					}
				}
				m_rows.push_back(elements_.front());
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
				std::map<int, std::list<BaseObjectPtr>>::iterator it = m_cells.find(cell->RowNumber);
				if (it == m_cells.end())
				{
					std::list<BaseObjectPtr> c;
					c.push_back(elements_.front());
					m_cells.insert(std::pair<int, std::list<BaseObjectPtr>>(cell->RowNumber, c));
				}
				else
				{
					it->second.push_back(elements_.front());
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
	
	int serialize(std::wostream & stream);
	static const ElementType	type = typeCELL_GROUP;

//---------------------------------------------------------------------------
	std::map<int, std::list<BaseObjectPtr>> m_cells;
	std::list<BaseObjectPtr>				m_rows;
	std::list<BaseObjectPtr>				m_DBCells;

private:
	std::vector<CellRangeRef>& shared_formulas_locations_ref_;

	GlobalWorkbookInfoPtr global_info_;
};


struct _CompareRowCell
{
    bool operator()(XLS::BaseObjectPtr & first, XLS::BaseObjectPtr & second)
	{
		CELL * cell_1 = dynamic_cast<CELL *>(first.get());
		CELL * cell_2 = dynamic_cast<CELL *>(second.get());

		if (!cell_1 || !cell_2) 
			return true;

		if (cell_1->RowNumber < cell_2->RowNumber) 
			return true;
		else
			return false;
	}
}CompareRowCell;

struct _CompareColumnCell
{
    bool operator()(XLS::BaseObjectPtr & first, XLS::BaseObjectPtr & second)
	{
		CELL * cell_1 = dynamic_cast<CELL *>(first.get());
		CELL * cell_2 = dynamic_cast<CELL *>(second.get());

		if (!cell_1 || !cell_2) 
			return true;

		if (cell_1->ColumnNumber < cell_2->ColumnNumber) 
			return true;
		else
			return false;
	}
}CompareColumnCell;

int CELL_GROUP::serialize(std::wostream & stream)
{
	XLS::GlobalWorkbookInfo::_sheet_size_info zero;
	XLS::GlobalWorkbookInfo::_sheet_size_info & sheet_info = global_info_->current_sheet >=0 ? 
										global_info_->sheet_size_info[global_info_->current_sheet - 1] : zero;
	
	CP_XML_WRITER(stream)    
    {	
		std::list<XLS::BaseObjectPtr>::iterator current_row = m_rows.begin();
		
		for (std::map<int, std::list<BaseObjectPtr>>::iterator it_row = m_cells.begin(); it_row != m_cells.end(); it_row++)
		{
			it_row->second.sort(CompareColumnCell);
			
			Row * row = NULL;

			if (current_row != m_rows.end())
			{
				row = dynamic_cast<Row *>(current_row->get());

				while ((row) && (row->rw < it_row->first))
				{//skip cells
					row->serialize(stream);
					current_row++;
					row = dynamic_cast<Row *>(current_row->get());
				}
			}
			CP_XML_NODE(L"row")
			{	
				CP_XML_ATTR(L"r", it_row->first + 1);
				if ((row) && (row->rw  == it_row->first))
				{
					bool xf_set = true;
					if (row->fGhostDirty == false) xf_set = false;
					
					if (row->ixfe_val && xf_set)
					{
						int xf = row->ixfe_val >= global_info_->cellStyleXfs_count ? row->ixfe_val - global_info_->cellStyleXfs_count : row->ixfe_val;
						
						if (xf < global_info_->cellXfs_count && xf >= 0)
						{
							CP_XML_ATTR(L"s", xf);
							CP_XML_ATTR(L"customFormat", true);
						}
					}
					if (row->miyRw > 0/* && std::abs(row->miyRw/20. - sheet_info.defaultRowHeight) > 0.01*/)
					{
						CP_XML_ATTR(L"ht", row->miyRw / 20.);
						CP_XML_ATTR(L"customHeight", true);
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
					
					row = NULL;
					current_row++;
					if (current_row != m_rows.end())
						row = dynamic_cast<Row *>(current_row->get());

				}
				for ( std::list<BaseObjectPtr>::iterator it_cell = it_row->second.begin(); it_cell != it_row->second.end(); it_cell++)
				{
					(*it_cell)->serialize(CP_XML_STREAM());
				}
			}

		}
		while (current_row != m_rows.end())
		{//skip cells ... last rows
			(*current_row)->serialize(stream);
			current_row++;
		}	
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------


CELLTABLE::CELLTABLE(std::vector<CellRangeRef>& shared_formulas_locations_ref) : 
												 m_count_CELL_GROUP(0)
												,shared_formulas_locations_ref_(shared_formulas_locations_ref)
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
    CELL_GROUP cell_group1(shared_formulas_locations_ref_);
	if(!proc.mandatory(cell_group1))
	{
		return false;
	}
	
	CELL_GROUP cell_group2(shared_formulas_locations_ref_);
	m_count_CELL_GROUP = proc.repeated(cell_group2, 0, 0);

	proc.repeated<EntExU2>(0, 0);

	return true;
}

int CELLTABLE::serialize(std::wostream & stream)
{
	for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); it++)
	{
		it->get()->serialize(stream);
	}
	return 0;
}
} // namespace XLS

