#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of OBJ union of records 
class OBJ: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(OBJ)
public:
	OBJ();
	~OBJ();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

