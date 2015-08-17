#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of METADATA union of records 
class METADATA: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(METADATA)
public:
	METADATA();
	~METADATA();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeMETADATA;
};

} // namespace XLS

