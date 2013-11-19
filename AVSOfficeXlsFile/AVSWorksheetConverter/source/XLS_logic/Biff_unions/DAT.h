#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DAT union of records 
class DAT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DAT)
public:
	DAT();
	~DAT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

