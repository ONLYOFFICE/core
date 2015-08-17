#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of DOCROUTE union of records 
class DOCROUTE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DOCROUTE)
public:
	DOCROUTE();
	~DOCROUTE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeDOCROUTE;
};

} // namespace XLS

