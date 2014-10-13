#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLCACHEFIELD union of records 
class SXADDLCACHEFIELD: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLCACHEFIELD)
public:
	SXADDLCACHEFIELD();
	~SXADDLCACHEFIELD();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

