#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SXADDLQSI union of records 
class SXADDLQSI: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLQSI)
public:
	SXADDLQSI();
	~SXADDLQSI();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLQSI;
	
};

} // namespace XLS

