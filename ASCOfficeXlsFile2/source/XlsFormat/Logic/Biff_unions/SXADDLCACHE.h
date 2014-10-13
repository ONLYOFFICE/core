#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLCACHE union of records 
class SXADDLCACHE: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLCACHE)
public:
	SXADDLCACHE();
	~SXADDLCACHE();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

