#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of AXM union of records 
class AXM: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AXM)
public:
	AXM();
	~AXM();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

