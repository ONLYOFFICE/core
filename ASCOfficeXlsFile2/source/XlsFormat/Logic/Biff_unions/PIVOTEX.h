#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTEX union of records 
class PIVOTEX: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTEX)
public:
	PIVOTEX();
	~PIVOTEX();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

