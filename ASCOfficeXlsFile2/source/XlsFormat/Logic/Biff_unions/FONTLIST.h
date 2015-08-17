#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of FONTLIST union of records 
class FONTLIST: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FONTLIST)
public:
	FONTLIST();
	~FONTLIST();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeFONTLIST;
};

} // namespace XLS

