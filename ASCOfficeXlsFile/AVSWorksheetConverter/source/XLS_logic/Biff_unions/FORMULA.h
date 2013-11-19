#pragma once

#include <XLS_logic/CompositeObject.h>
#include <XLS_logic/Biff_structures/CellRef.h>

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
	const CellRef getLocation() const;

private:
	CellRef location;

private:
	std::vector<CellRef>& shared_formulas_locations_ref_;
};

} // namespace XLS

