
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
	CELL_GROUP(std::vector<CellRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref) {}

	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new CELL_GROUP(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		int count = 0;
		if(!proc.mandatory<Row>())
		{
			return false;
		}
		count = 1 + proc.repeated<Row>(0, 0);
	
		while(count > 0)
		{
			m_rows.insert(m_rows.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}	
		
		//------------------------------------------------------------------------------------------------------------------
		count = proc.repeated(CELL(shared_formulas_locations_ref_), 0, 0);


		count = proc.repeated<DBCell>(0, 0); // OpenOffice Calc stored files workaround (DBCell must be present at least once according to [MS-XLS])
		while(count > 0)
		{
			m_DBCells.insert(m_DBCells.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}			
		return true;
	};
	int serialize(std::wostream & stream);
	static const ElementType	type = typeCELL_GROUP;

	std::list<BaseObjectPtr>	m_rows;
	std::list<BaseObjectPtr>	m_DBCells;
	//elements_ = cells
private:
	std::vector<CellRef>& shared_formulas_locations_ref_;
};

int CELL_GROUP::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		std::list<XLS::BaseObjectPtr>::iterator current_cell_start = elements_.begin();

		int current_row = 1;

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
					CP_XML_ATTR(L"s", *row->ixfe_val.value() - cellStyleXfs_count);
					CP_XML_ATTR(L"customFormat", true);
				}

				for (std::list<XLS::BaseObjectPtr>::iterator it_cell = current_cell_start; it_cell != elements_.end(); it_cell++)
				{
					CELL * cell = dynamic_cast<CELL *>(it_cell->get());

					if (cell == NULL) continue;

					if (cell->RowNumber >current_row)
					{
						current_cell_start = it_cell;
						break;
					}
					cell->serialize(CP_XML_STREAM());
			
				}
			}
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------


CELLTABLE::CELLTABLE(std::vector<CellRef>& shared_formulas_locations_ref) : m_count_CELL_GROUP(0), 
													shared_formulas_locations_ref_(shared_formulas_locations_ref)
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

