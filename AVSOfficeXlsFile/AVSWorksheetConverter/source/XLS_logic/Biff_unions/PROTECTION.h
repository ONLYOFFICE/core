#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PROTECTION union of records 
class PROTECTION: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PROTECTION)
public:
	PROTECTION();
	~PROTECTION();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

