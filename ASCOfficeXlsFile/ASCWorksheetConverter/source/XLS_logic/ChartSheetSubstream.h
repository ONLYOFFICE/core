#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of ChartSheetSubstream union of records 
class ChartSheetSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ChartSheetSubstream)
public:
	ChartSheetSubstream();
	~ChartSheetSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

