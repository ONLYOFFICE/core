#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXSRC union of records 
class SXSRC: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXSRC)
public:
	SXSRC();
	~SXSRC();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

