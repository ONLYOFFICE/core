#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTLI union of records 
class PIVOTLI: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTLI)
public:
	PIVOTLI();
	~PIVOTLI();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

