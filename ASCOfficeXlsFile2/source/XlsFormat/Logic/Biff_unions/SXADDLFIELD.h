#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SXADDLFIELD union of records 
class SXADDLFIELD: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLFIELD)
public:
	SXADDLFIELD();
	~SXADDLFIELD();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLFIELD;
	
};

} // namespace XLS

