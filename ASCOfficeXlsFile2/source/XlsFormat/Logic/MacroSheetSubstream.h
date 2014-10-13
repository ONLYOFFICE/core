#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of MacroSheetSubstream union of records 
class MacroSheetSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MacroSheetSubstream)
public:
	MacroSheetSubstream();
	~MacroSheetSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

