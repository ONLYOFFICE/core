#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of AXM union of records 
class AXM: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AXM)
public:
	AXM();
	~AXM();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	BaseObjectPtr m_YMult;
	BaseObjectPtr m_ATTACHEDLABEL;

};

} // namespace XLS

