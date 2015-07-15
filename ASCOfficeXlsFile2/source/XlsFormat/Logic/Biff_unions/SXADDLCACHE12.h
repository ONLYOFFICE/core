#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLCACHE12 union of records 
class SXADDLCACHE12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLCACHE12)
public:
	SXADDLCACHE12();
	~SXADDLCACHE12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLCACHE12;
	
};

} // namespace XLS

