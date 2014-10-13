#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of SERIESDATA union of records 
class SERIESDATA: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SERIESDATA)
public:
	SERIESDATA();
	~SERIESDATA();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

