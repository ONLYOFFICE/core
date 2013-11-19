#pragma once

#include <XLS_logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTFRT union of records 
class PIVOTFRT: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTFRT)
public:
	PIVOTFRT();
	~PIVOTFRT();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

};

} // namespace XLS

