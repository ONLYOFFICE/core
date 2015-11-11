#pragma once

#include <Logic/CompositeObject.h>
#include <Logic/Biff_structures/CellRef.h>
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{

class CellRef;

// Logical representation of FORMULA union of records 
class FORMULA: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FORMULA)
public:
	FORMULA(std::vector<CellRangeRef>& shared_formulas_locations_ref);
	~FORMULA();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
	static const ElementType	type = typeFORMULA;

	int serialize(std::wostream & stream);

	const CellRef getLocation() const;

	BaseObjectPtr		m_Formula;
	BaseObjectPtr		m_TableFormula;
	BaseObjectPtr		m_ArrayFormula;
	BaseObjectPtr		m_SharedFormula;

	CellRef location;

	std::vector<CellRangeRef>&	shared_formulas_locations_ref_;

	int					m_sharedIndex;
};

} // namespace XLS

