#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of CRT union of records 
class CRT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CRT)
public:
	CRT();
	~CRT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeCRT;
};

} // namespace XLS

