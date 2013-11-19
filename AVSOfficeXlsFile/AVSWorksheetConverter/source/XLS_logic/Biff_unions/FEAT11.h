#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of FEAT11 union of records 
class FEAT11: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FEAT11)
public:
	FEAT11();
	~FEAT11();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

