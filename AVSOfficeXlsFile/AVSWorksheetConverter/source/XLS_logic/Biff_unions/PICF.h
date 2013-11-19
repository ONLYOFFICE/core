#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PICF union of records 
class PICF: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PICF)
public:
	PICF();
	~PICF();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

