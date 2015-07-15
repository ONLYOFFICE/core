#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;

class CellRef;

// Logical representation of SHFMLA_SET union
// This union is not present in specs but is intended to encapsulate data about all shared formulas in a worksheet
class SHFMLA_SET: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SHFMLA_SET)
public:
	SHFMLA_SET(std::vector<CellRef>& shared_formulas_locations_ref);
	~SHFMLA_SET();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSHFMLA_SET;

	std::vector<CellRef>& shared_formulas_locations_ref_;
};

} // namespace XLS

