#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DBQUERY union of records 
class DBQUERY: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DBQUERY)
public:
	DBQUERY();
	~DBQUERY();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

