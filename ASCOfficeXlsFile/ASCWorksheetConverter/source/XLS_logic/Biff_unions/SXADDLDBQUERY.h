#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLDBQUERY union of records 
class SXADDLDBQUERY: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLDBQUERY)
public:
	SXADDLDBQUERY();
	~SXADDLDBQUERY();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

