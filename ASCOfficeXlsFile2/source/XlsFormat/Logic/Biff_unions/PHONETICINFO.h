#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of PHONETICINFO union of records 
class PHONETICINFO: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PHONETICINFO)
public:
	PHONETICINFO();
	~PHONETICINFO();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePHONETICINFO;
};

} // namespace XLS

