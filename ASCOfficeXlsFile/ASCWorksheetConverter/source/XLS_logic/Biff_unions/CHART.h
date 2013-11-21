#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of CHART union of records 
class CHART: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(CHART)
public:
	CHART();
	~CHART();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

