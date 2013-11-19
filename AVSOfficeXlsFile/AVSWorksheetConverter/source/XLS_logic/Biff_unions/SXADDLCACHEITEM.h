#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SXADDLCACHEITEM union of records 
class SXADDLCACHEITEM: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SXADDLCACHEITEM)
public:
	SXADDLCACHEITEM();
	~SXADDLCACHEITEM();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

