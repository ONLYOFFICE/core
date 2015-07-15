#pragma once

#include <Logic/CompositeObject.h>

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
	
	static const ElementType	type = typeCUSTOMVIEW;

};

} // namespace XLS

