#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DAT union of records 
class DAT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DAT)
public:
	DAT();
	~DAT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeDAT;
};

} // namespace XLS

