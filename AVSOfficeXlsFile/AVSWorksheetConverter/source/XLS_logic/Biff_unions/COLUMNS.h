#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of COLUMNS union of records 
class COLUMNS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(COLUMNS)
public:
	COLUMNS();
	~COLUMNS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

