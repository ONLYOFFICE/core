#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of FRAME union of records 
class FRAME: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FRAME)
public:
	FRAME();
	~FRAME();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

