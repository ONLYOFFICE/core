#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of MDXSTR union of records 
class MDXSTR: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXSTR)
public:
	MDXSTR();
	~MDXSTR();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeMDXSTR;
};

} // namespace XLS

