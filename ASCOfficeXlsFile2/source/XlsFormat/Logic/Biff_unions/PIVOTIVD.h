#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTIVD union of records 
class PIVOTIVD: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTIVD)
public:
	PIVOTIVD();
	~PIVOTIVD();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTIVD;
};

} // namespace XLS

