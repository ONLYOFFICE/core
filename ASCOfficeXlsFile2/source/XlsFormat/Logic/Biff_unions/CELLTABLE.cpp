
#include "CELLTABLE.h"
#include <Logic/Biff_records/EntExU2.h>
#include <Logic/Biff_records/Row.h>
#include <Logic/Biff_unions/CELL.h>
#include <Logic/Biff_records/DBCell.h>

#include <simple_xml_writer.h>

namespace XLS
{;

extern int cellStyleXfs_count;
// This class is made a deriver of CompositeObject intentionally.
// This is an optimization step - to form a CELLTABLE that is divided into smaller groups
class CELL_GROUP : public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CELL_GROUP)
public:
	CELL_GROUP(std::vector<CellRef>& shared_formulas_locations_ref) : 
							shared_formulas_locations_ref_(shared_formulas_locations_ref)
							, m_count_rows (0)
	{
	}

	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new CELL_GROUP(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		int count, count_row = 0;
		
		if(proc.mandatory<Row>() == true)
		{
			count_row = count =  1 + proc.repeated<Row>(0, 0);
		
			while(count > 0)
			{
				m_rows.insert(m_rows.begin(), elements_.back());
				elements_.pop_back();
				count--;
				m_count_rows++;
			}
		}	
		
		//------------------------------------------------------------------------------------------------------------------
		int count_cells = proc.repeated(CELL(shared_formulas_locations_ref_), 0, 0);


		count = proc.repeated<DBCell>(0, 0); // OpenOffice Calc stored files workaround (DBCell must be present at least once according to [MS-XLS])
		while(count > 0)
		{
			m_DBCells.insert(m_DBCells.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}	
		if (count_cells > 0 || count_row > 0)	return true;
		else									return false;
	};
	int serialize(std::wostream & stream);
	static const ElementType	type = typeCELL_GROUP;

	int							m_count_rows;
	std::list<BaseObjectPtr>	m_rows;
	std::list<BaseObjectPtr>	m_DBCells;

private:
	std::vector<CellRef>& shared_formulas_locations_ref_;
};

int CELL_GROUP::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		std::list<XLS::BaseObjectPtr>::iterator current_cell_start = elements_.begin();

		int current_row = 0;

		for (std::list<XLS::BaseObjectPtr>::iterator it_row = m_rows.begin(); it_row != m_rows.end(); it_row++)
		{
			Row * row = dynamic_cast<Row *>(it_row->get());
			
			if (row == NULL) continue;

			CP_XML_NODE(L"row")
			{		
				current_row = *row->rw.value();
				
				CP_XML_ATTR(L"r", current_row + 1);
				
				bool xf_set = true;
				if ((row->fGhostDirty.value()) && ( *row->fGhostDirty.value()== false)) xf_set = false;
				
				if (row->ixfe_val.value() && xf_set)
				{
					CP_XML_ATTR(L"s", row->ixfe_val - cellStyleXfs_count);
					CP_XML_ATTR(L"customFormat", true);
				}

				if (row->miyRw.value())
				{
					CP_XML_ATTR(L"ht", row->miyRw / 20.);
					CP_XML_ATTR(L"customHeight", true);
				}
				if ((row->iOutLevel.value()) && (row->iOutLevel > 0))
				{
					CP_XML_ATTR(L"outlineLevel", row->iOutLevel);
				}
				if ((row->fCollapsed.value()) && (row->fCollapsed))
				{
					CP_XML_ATTR(L"collapsed", row->fCollapsed);
				}
				std::list<XLS::BaseObjectPtr>::iterator it_cell = current_cell_start;
				while(true)
				{
					if (it_cell == elements_.end())
					{
						current_cell_start = it_cell;
						break;
					}
					CELL * cell = dynamic_cast<CELL *>(it_cell->get());

					if (cell == NULL) continue;

					if (cell->RowNumber >current_row)
					{
						current_cell_start = it_cell;
						break;
					}
					cell->serialize(CP_XML_STREAM());
					it_cell++;
				}
			}
		}

		if (current_row < 1 )//order_history.xls - нет описанных Row, тока €чейки
		{
			while(current_row ==0)
			{
				CP_XML_NODE(L"row")
				{	
					std::list<XLS::BaseObjectPtr>::iterator it_cell = current_cell_start;
					while(true)
					{
						if (it_cell == elements_.end())
						{
							current_cell_start = it_cell;
							current_row = -1;
							break;
						}
						CELL * cell = dynamic_cast<CELL *>(it_cell->get());

						if (cell == NULL) continue;

						if (current_row < 1) 
						{
							current_row = cell->RowNumber + 1;
							CP_XML_ATTR(L"r", current_row);
						}

						if (cell->RowNumber > current_row - 1)
						{
							current_cell_start = it_cell;
							current_row = 0;
							break;
						}
						cell->serialize(CP_XML_STREAM());
						it_cell++;
					}
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------


CELLTABLE::CELLTABLE(std::vector<CellRef>& shared_formulas_locations_ref) : 
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
	if(!proc.mandatory(CELL_GROUP(shared_formulas_locations_ref_)))
	{
		return false;
	}
	
	m_count_CELL_GROUP = proc.repeated(CELL_GROUP(shared_formulas_locations_ref_), 0, 0);

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

