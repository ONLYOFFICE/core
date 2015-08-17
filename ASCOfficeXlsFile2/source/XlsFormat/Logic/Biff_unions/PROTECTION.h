#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of PROTECTION union of records 
class PROTECTION: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PROTECTION)
public:
	PROTECTION();
	~PROTECTION();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePROTECTION;
};

} // namespace XLS

