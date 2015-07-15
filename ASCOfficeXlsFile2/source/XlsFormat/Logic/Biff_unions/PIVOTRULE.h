#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTRULE union of records 
class PIVOTRULE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTRULE)
public:
	PIVOTRULE();
	~PIVOTRULE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTRULE;
};

} // namespace XLS

