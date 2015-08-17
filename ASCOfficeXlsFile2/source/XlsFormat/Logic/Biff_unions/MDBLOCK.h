#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of MDBLOCK union of records 
class MDBLOCK: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDBLOCK)
public:
	MDBLOCK();
	~MDBLOCK();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeMDBLOCK;
};

} // namespace XLS

