#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of FORMATTING union of records 
class FORMATTING: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FORMATTING)
public:
	FORMATTING();
	~FORMATTING();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

