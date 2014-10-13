#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of GELFRAME union of records 
class GELFRAME: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GELFRAME)
public:
	GELFRAME();
	~GELFRAME();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

