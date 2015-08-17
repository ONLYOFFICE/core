#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SXADDLDBQUERY union of records 
class SXADDLDBQUERY: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLDBQUERY)
public:
	SXADDLDBQUERY();
	~SXADDLDBQUERY();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSXADDLDBQUERY;
	
};

} // namespace XLS

