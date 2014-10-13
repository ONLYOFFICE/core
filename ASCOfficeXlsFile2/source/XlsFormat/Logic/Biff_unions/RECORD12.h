#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of RECORD12 union of records 
class RECORD12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RECORD12)
public:
	RECORD12();
	~RECORD12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

