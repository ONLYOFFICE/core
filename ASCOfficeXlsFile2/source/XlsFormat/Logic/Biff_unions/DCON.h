#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DCON union of records 
class DCON: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DCON)
public:
	DCON();
	~DCON();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

