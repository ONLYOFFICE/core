#pragma once

#include <Logic/CompositeObject.h>

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

	static const ElementType	type = typeChartSheetSubstream;
	
};

} // namespace XLS

