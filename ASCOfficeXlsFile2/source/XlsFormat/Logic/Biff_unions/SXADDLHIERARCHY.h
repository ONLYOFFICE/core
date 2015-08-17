#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SXADDLHIERARCHY union of records 
class SXADDLHIERARCHY: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLHIERARCHY)
public:
	SXADDLHIERARCHY();
	~SXADDLHIERARCHY();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLHIERARCHY;
	
};

} // namespace XLS

