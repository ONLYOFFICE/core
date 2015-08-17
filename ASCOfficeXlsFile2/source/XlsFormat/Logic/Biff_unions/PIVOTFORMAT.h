#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of PIVOTFORMAT union of records 
class PIVOTFORMAT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTFORMAT)
public:
	PIVOTFORMAT();
	~PIVOTFORMAT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTFORMAT;
};

} // namespace XLS

