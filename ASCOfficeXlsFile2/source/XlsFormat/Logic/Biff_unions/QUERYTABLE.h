#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of QUERYTABLE union of records 
class QUERYTABLE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(QUERYTABLE)
public:
	QUERYTABLE();
	~QUERYTABLE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeQUERYTABLE;
};

} // namespace XLS

