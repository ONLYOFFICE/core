#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTPI union of records 
class PIVOTPI: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTPI)
public:
	PIVOTPI();
	~PIVOTPI();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTPI;
};

} // namespace XLS

