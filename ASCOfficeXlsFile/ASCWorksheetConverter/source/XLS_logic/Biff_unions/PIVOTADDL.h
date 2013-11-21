#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTADDL union of records 
class PIVOTADDL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTADDL)
public:
	PIVOTADDL();
	~PIVOTADDL();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

