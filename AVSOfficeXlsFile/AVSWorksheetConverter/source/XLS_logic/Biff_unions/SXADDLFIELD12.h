#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLFIELD12 union of records 
class SXADDLFIELD12: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLFIELD12)
public:
	SXADDLFIELD12();
	~SXADDLFIELD12();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

