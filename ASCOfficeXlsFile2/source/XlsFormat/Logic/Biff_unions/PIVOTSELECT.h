#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of PIVOTSELECT union of records 
class PIVOTSELECT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTSELECT)
public:
	PIVOTSELECT();
	~PIVOTSELECT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTSELECT;
};

} // namespace XLS

