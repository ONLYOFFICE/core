#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of TABLESTYLES union of records 
class TABLESTYLES: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(TABLESTYLES)
public:
	TABLESTYLES();
	~TABLESTYLES();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeTABLESTYLES;
	
};

} // namespace XLS

