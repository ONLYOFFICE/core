#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLGROUP union of records 
class SXADDLGROUP: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLGROUP)
public:
	SXADDLGROUP();
	~SXADDLGROUP();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

