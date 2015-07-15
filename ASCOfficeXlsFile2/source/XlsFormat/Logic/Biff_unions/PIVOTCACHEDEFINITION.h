#pragma once

#include <Logic/CompositeObject.h>

namespace XLS
{;


// Logical representation of PIVOTCACHEDEFINITION union of records 
class PIVOTCACHEDEFINITION: public CompositeObject
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PIVOTCACHEDEFINITION)
public:
	PIVOTCACHEDEFINITION();
	~PIVOTCACHEDEFINITION();

	BaseObjectPtr clone();

	virtual const bool loadContent(BinProcessor& proc);

	static const ElementType	type = typePIVOTCACHEDEFINITION;
};

} // namespace XLS

