#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTFRT9 union of records 
class PIVOTFRT9: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTFRT9)
public:
	PIVOTFRT9();
	~PIVOTFRT9();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

