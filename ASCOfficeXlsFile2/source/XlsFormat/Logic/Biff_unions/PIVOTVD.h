#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{


// Logical representation of PIVOTVD union of records 
class PIVOTVD: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTVD)
public:
	PIVOTVD();
	~PIVOTVD();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTVD;
};

} // namespace XLS

