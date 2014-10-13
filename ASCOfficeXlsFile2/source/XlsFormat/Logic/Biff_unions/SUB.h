#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SUB union of records 
class SUB: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SUB)
public:
	SUB();
	~SUB();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

