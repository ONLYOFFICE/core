#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of WINDOW union of records 
class WINDOW: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(WINDOW)
public:
	WINDOW();
	~WINDOW();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

