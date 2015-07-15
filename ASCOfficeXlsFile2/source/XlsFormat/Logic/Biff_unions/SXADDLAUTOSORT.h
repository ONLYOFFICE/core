#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLAUTOSORT union of records 
class SXADDLAUTOSORT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLAUTOSORT)
public:
	SXADDLAUTOSORT();
	~SXADDLAUTOSORT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLAUTOSORT;
	
};

} // namespace XLS

