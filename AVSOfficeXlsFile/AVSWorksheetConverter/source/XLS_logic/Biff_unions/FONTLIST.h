#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of FONTLIST union of records 
class FONTLIST: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FONTLIST)
public:
	FONTLIST();
	~FONTLIST();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

