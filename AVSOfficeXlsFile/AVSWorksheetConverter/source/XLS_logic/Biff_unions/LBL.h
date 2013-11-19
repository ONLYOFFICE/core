#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of LBL union of records 
class LBL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(LBL)
public:
	LBL();
	~LBL();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

