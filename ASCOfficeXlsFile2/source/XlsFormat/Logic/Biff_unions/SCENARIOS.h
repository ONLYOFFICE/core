#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SCENARIOS union of records 
class SCENARIOS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SCENARIOS)
public:
	SCENARIOS();
	~SCENARIOS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSCENARIOS;
};

} // namespace XLS

