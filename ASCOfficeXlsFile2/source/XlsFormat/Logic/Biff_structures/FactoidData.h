#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/PropertyBag.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class CFRecord;

class FactoidData : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FactoidData)
public:
	FactoidData();
	FactoidData(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeFactoidData;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	bool fDelete;
	bool fXMLBased;
	OSHARED::PropertyBag propertyBag;
};

} // namespace XLS

