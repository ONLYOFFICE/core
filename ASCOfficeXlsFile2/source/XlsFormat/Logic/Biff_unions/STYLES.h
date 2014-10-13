#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of STYLES union of records 
class STYLES: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(STYLES)
public:
	STYLES();
	~STYLES();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

