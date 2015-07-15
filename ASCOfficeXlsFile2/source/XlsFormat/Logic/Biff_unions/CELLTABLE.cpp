#include "precompiled_xls.h"
#include "CELLTABLE.h"
#include <Logic/Biff_records/EntExU2.h>
#include <Logic/Biff_records/Row.h>
#include <Logic/Biff_unions/CELL.h>
#include <Logic/Biff_records/DBCell.h>

namespace XLS
{;


CELLTABLE::CELLTABLE(std::vector<CellRef>& shared_formulas_locations_ref) : shared_formulas_locations_ref_(shared_formulas_locations_ref)
{
}


CELLTABLE::~CELLTABLE()
{
}


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
		if(!proc.mandatory<Row>())
		{
			return false;
		}
		proc.repeated<Row>(0, 0);
		proc.repeated(CELL(shared_formulas_locations_ref_), 0, 0);
		proc.repeated<DBCell>(0, 0); // OpenOffice Calc stored files workaround (DBCell must be present at least once according to [MS-XLS])
		return true;
	};
	
	static const ElementType	type = typeCELL_GROUP;

private:
	std::vector<CellRef>& shared_formulas_locations_ref_;
};


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
	proc.repeated(CELL_GROUP(shared_formulas_locations_ref_), 0, 0);
	proc.repeated<EntExU2>(0, 0);

	return true;
}

} // namespace XLS

