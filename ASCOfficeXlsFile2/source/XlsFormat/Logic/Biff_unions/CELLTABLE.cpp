#include "precompiled_xls.h"
#include "CELLTABLE.h"
#include <Logic/Biff_records/EntExU2.h>
#include <Logic/Biff_records/Row.h>
#include <Logic/Biff_unions/CELL.h>
#include <Logic/Biff_records/DBCell.h>

#include <simple_xml_writer.h>

namespace XLS
{;

// This class is made a deriver of CompositeObject intentionally.
// This is an optimization step - to form a CELLTABLE that is divided into smaller groups
class CELL_GROUP : public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CELL_GROUP)
public:
	CELL_GROUP(std::vector<CellRef>& shared_formulas_locations_ref) : m_rowCount(0), 
									shared_formulas_locations_ref_(shared_formulas_locations_ref) {}

	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new CELL_GROUP(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<Row>())
		{
			return false;
		}
		m_rowCount = proc.repeated<Row>(0, 0);
	
//------------------------------------------------------------------------------------------------------------------
		int count = proc.repeated(CELL(shared_formulas_locations_ref_), 0, 0);
		while(count > 0)
		{
			m_CELL_formulas_locations_ref.insert(m_CELL_formulas_locations_ref.begin(), elements_.back());
			elements_.pop_back();
			count--;
		}	

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

	std::vector<BaseObjectPtr>	m_CELL_formulas_locations_ref;
	std::vector<BaseObjectPtr>	m_DBCells;
	long						m_rowCount;
private:
	std::vector<CellRef>& shared_formulas_locations_ref_;
};

int CELL_GROUP::serialize(std::wostream & stream)
{
	for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); it++)
	{
		it->get()->serialize(stream);
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

