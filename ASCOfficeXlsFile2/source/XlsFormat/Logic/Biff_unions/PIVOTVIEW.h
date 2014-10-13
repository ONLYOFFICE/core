#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTVIEW union of records 
class PIVOTVIEW: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTVIEW)
public:
	PIVOTVIEW();
	~PIVOTVIEW();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

