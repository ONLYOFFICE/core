#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SHAPEPROPS union of records 
class SHAPEPROPS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SHAPEPROPS)
public:
	SHAPEPROPS();
	~SHAPEPROPS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

