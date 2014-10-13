#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of LD union of records 
class LD: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(LD)
public:
	LD();
	~LD();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

