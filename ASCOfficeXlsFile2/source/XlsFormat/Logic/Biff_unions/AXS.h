#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of AXS union of records 
class AXS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AXS)
public:
	AXS();
	~AXS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

