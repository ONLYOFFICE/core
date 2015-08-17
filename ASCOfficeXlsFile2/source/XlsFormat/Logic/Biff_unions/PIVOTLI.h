#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of PIVOTLI union of records 
class PIVOTLI: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTLI)
public:
	PIVOTLI();
	~PIVOTLI();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTLI;
};

} // namespace XLS

