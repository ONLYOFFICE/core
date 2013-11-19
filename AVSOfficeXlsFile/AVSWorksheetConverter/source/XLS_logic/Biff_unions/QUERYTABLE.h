#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of QUERYTABLE union of records 
class QUERYTABLE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(QUERYTABLE)
public:
	QUERYTABLE();
	~QUERYTABLE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

