#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of FNGROUPS union of records 
class FNGROUPS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FNGROUPS)
public:
	FNGROUPS();
	~FNGROUPS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeFNGROUPS;
};

} // namespace XLS

