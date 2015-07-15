#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXTBL union of records 
class SXTBL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXTBL)
public:
	SXTBL();
	~SXTBL();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXTBL;
	
};

} // namespace XLS

