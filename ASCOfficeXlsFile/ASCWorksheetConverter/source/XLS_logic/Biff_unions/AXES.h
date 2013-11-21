#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of AXES union of records 
class AXES: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AXES)
public:
	AXES();
	~AXES();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
};

} // namespace XLS

