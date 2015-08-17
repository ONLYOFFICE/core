#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SXADDLFIELD12 union of records 
class SXADDLFIELD12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLFIELD12)
public:
	SXADDLFIELD12();
	~SXADDLFIELD12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);
	
	static const ElementType	type = typeSXADDLFIELD12;
	
};

} // namespace XLS

