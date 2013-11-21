#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTCORE union of records 
class PIVOTCORE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTCORE)
public:
	PIVOTCORE();
	~PIVOTCORE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

