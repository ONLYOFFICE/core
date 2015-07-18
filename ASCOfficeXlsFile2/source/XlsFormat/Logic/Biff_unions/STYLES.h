#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of STYLES union of records 
class STYLES : public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(STYLES)
public:
	STYLES(){styles_count = 0;}
	~STYLES(){}

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSTYLES;

	int serialize(std::wostream & stream);

	int styles_count;
	
};

} // namespace XLS

