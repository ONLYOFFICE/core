#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of COLUMNS union of records 
class COLUMNS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(COLUMNS)
public:
	COLUMNS();
	~COLUMNS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeCOLUMNS;
};

} // namespace XLS

