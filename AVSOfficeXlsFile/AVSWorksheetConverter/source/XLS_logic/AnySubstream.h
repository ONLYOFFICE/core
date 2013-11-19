#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of AnySubstream union of records 
class AnySubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AnySubstream)
public:
	AnySubstream();
	~AnySubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

