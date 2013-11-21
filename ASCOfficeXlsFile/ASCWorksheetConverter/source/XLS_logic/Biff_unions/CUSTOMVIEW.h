#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of CUSTOMVIEW union of records 
class CUSTOMVIEW: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CUSTOMVIEW)
public:
	CUSTOMVIEW();
	~CUSTOMVIEW();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

