#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of CONDFMT union of records 
class CONDFMT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CONDFMT)
public:
	CONDFMT();
	~CONDFMT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

