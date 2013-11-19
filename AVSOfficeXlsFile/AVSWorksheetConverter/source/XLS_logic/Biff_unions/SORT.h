#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SORT union of records 
class SORT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SORT)
public:
	SORT();
	~SORT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

