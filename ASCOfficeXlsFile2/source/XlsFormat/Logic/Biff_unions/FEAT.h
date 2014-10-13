#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of FEAT union of records 
class FEAT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FEAT)
public:
	FEAT();
	~FEAT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

