#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SORTANDFILTER union of records 
class SORTANDFILTER: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SORTANDFILTER)
public:
	SORTANDFILTER();
	~SORTANDFILTER();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

