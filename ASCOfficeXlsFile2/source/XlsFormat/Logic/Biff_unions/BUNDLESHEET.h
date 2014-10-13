#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of BUNDLESHEET union of records 
class BUNDLESHEET: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(BUNDLESHEET)
public:
	BUNDLESHEET();
	~BUNDLESHEET();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

