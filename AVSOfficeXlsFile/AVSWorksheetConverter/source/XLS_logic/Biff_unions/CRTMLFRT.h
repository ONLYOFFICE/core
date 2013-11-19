#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of CRTMLFRT union of records 
class CRTMLFRT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CRTMLFRT)
public:
	CRTMLFRT();
	~CRTMLFRT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

