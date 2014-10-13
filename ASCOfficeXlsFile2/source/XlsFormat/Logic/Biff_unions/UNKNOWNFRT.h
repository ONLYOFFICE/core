#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of UNKNOWNFRT union of records 
class UNKNOWNFRT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(UNKNOWNFRT)
public:
	UNKNOWNFRT();
	~UNKNOWNFRT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

