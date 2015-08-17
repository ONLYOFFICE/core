#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of SERIESFORMAT union of records 
class SERIESFORMAT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SERIESFORMAT)
public:
	SERIESFORMAT();
	~SERIESFORMAT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeSERIESFORMAT;
	
};

} // namespace XLS

