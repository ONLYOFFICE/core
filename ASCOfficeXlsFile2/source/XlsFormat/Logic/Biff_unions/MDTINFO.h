#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of MDTINFO union of records 
class MDTINFO: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDTINFO)
public:
	MDTINFO();
	~MDTINFO();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

