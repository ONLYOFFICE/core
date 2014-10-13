#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of AUTOFILTER union of records 
class AUTOFILTER: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AUTOFILTER)
public:
	AUTOFILTER();
	~AUTOFILTER();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

