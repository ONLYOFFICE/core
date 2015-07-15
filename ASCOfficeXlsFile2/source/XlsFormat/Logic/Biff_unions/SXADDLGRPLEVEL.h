#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLGRPLEVEL union of records 
class SXADDLGRPLEVEL: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLGRPLEVEL)
public:
	SXADDLGRPLEVEL();
	~SXADDLGRPLEVEL();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLGRPLEVEL;
	
};

} // namespace XLS

