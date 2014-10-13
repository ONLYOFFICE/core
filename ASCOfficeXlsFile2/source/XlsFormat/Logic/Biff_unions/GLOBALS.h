#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of GLOBALS union of records 
class GLOBALS: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(GLOBALS)
public:
	GLOBALS(const bool is_dialog_sheet);
	~GLOBALS();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

private:
	bool is_dialog;
};

} // namespace XLS

