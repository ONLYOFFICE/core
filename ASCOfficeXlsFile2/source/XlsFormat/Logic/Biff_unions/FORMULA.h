#pragma once

#include <Logic/CompositeObject.h>
#include <Logic/Biff_structures/CellRef.h>

namespace XLS
{;

class CellRef;

// Logical representation of FORMULA union of records 
class FORMULA: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FORMULA)
public:
	FORMULA(std::vector<CellRef>& shared_formulas_locations_ref);
	~FORMULA();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
	static const ElementType	type = typeFORMULA;

	const CellRef getLocation() const;

	CellRef location;

	std::vector<CellRef>& shared_formulas_locations_ref_;
};

} // namespace XLS

