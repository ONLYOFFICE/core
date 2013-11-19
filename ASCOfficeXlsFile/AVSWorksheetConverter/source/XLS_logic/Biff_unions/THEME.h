#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of THEME union of records 
class THEME: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(THEME)
public:
	THEME();
	~THEME();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

