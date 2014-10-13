#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SS union of records 
class SS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SS)
public:
	SS();
	~SS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

