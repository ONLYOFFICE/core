#pragma once

#include <Logic/CompositeObject.h>

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

	static const ElementType	type = typeSHAPEPROPS;
	
};

} // namespace XLS

