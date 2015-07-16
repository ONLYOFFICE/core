#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;
class MacroSheetSubstream;
typedef boost::shared_ptr<MacroSheetSubstream>			MacroSheetSubstreamPtr;
// Logical representation of MacroSheetSubstream union of records 
class MacroSheetSubstream: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MacroSheetSubstream)
public:
	MacroSheetSubstream();
	~MacroSheetSubstream();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeMacroSheetSubstream;
	
};

} // namespace XLS

