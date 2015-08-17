#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{

class CellRef;

// Logical representation of CELL union of records 
class CELL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CELL)
public:
	CELL(std::vector<CellRef>& shared_formulas_locations_ref);
	~CELL();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	int serialize(std::wostream & stream);

	static const ElementType	type = typeCELL;

	long RowNumber;

	std::vector<CellRef>& shared_formulas_locations_ref_;
};

} // namespace XLS

