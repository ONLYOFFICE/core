#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of TEXTOBJECT union of records 
class TEXTOBJECT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(TEXTOBJECT)
public:
	TEXTOBJECT();
	~TEXTOBJECT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeTEXTOBJECT;
	
};

} // namespace XLS

