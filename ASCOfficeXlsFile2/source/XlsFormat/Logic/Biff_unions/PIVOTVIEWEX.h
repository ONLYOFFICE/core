#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTVIEWEX union of records 
class PIVOTVIEWEX: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTVIEWEX)
public:
	PIVOTVIEWEX();
	~PIVOTVIEWEX();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

