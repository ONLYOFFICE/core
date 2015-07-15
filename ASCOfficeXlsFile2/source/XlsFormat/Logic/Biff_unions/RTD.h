#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of RTD union of records 
class RTD: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RTD)
public:
	RTD();
	~RTD();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeRTD;
};

} // namespace XLS

