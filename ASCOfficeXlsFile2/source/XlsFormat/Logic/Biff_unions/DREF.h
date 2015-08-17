#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of DREF union of records 
class DREF: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DREF)
public:
	DREF();
	~DREF();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeDREF;
};

} // namespace XLS

