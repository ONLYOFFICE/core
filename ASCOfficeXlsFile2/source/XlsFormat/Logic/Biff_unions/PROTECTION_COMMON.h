#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of PROTECTION_COMMON union of records 
class PROTECTION_COMMON: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PROTECTION)
public:
	PROTECTION_COMMON();
	~PROTECTION_COMMON();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePROTECTION_COMMON;
};

} // namespace XLS

