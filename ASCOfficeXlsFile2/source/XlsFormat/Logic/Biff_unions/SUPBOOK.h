#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SUPBOOK union of records 
class SUPBOOK: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SUPBOOK)
public:
	SUPBOOK();
	~SUPBOOK();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSUPBOOK;
	
};

} // namespace XLS

