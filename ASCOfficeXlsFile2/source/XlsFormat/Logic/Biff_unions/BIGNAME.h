#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of BIGNAME union of records 
class BIGNAME: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(BIGNAME)
public:
	BIGNAME();
	~BIGNAME();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

