#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{

class CellRef;

// Logical representation of CELLTABLE union of records 
class CELLTABLE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CELLTABLE)
public:
	CELLTABLE(std::vector<CellRangeRef>& shared_formulas_locations_ref);
	~CELLTABLE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeCELLTABLE;

	int serialize(std::wostream & stream);

	std::vector<CellRangeRef>& shared_formulas_locations_ref_;

    int m_count_CELL_GROUP;
};

} // namespace XLS

