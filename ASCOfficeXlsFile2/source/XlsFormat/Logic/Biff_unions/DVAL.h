#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of DVAL union of records 
class DVAL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DVAL)
public:
	DVAL();
	~DVAL();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeDVAL;
};

} // namespace XLS

