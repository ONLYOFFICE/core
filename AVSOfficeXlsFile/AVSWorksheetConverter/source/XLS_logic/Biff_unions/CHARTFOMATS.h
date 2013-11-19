#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of CHARTFOMATS union of records 
class CHARTFOMATS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CHARTFOMATS)
public:
	CHARTFOMATS();
	~CHARTFOMATS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

