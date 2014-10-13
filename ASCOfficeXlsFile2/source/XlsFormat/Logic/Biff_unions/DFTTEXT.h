#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of DFTTEXT union of records 
class DFTTEXT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DFTTEXT)
public:
	DFTTEXT();
	~DFTTEXT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

