#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of AI union of records 
class AI: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AI)
public:
	AI();
	~AI();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

