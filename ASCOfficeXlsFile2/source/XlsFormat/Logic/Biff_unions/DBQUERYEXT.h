#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DBQUERYEXT union of records 
class DBQUERYEXT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DBQUERYEXT)
public:
	DBQUERYEXT();
	~DBQUERYEXT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

