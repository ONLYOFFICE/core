#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PRFILTER union of records 
class PRFILTER: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PRFILTER)
public:
	PRFILTER();
	~PRFILTER();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

