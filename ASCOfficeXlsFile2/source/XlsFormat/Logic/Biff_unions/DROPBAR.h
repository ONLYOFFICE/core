#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DROPBAR union of records 
class DROPBAR: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DROPBAR)
public:
	DROPBAR();
	~DROPBAR();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

