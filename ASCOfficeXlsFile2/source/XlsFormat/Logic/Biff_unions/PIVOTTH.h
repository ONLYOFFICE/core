#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTTH union of records 
class PIVOTTH: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTTH)
public:
	PIVOTTH();
	~PIVOTTH();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

