#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of HLINK union of records 
class HLINK: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(HLINK)
public:
	HLINK();
	~HLINK();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typeHLINK;

	BaseObjectPtr m_HLink;
	BaseObjectPtr m_HLinkTooltip;
};

} // namespace XLS

