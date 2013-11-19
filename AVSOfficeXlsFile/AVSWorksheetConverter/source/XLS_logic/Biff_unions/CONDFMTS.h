#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of CONDFMTS union of records 
class CONDFMTS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CONDFMTS)
public:
	CONDFMTS();
	~CONDFMTS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

