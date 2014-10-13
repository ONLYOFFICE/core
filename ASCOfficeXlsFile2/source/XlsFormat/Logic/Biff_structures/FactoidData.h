#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/PropertyBag.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class FactoidData : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FactoidData)
public:
	FactoidData();
	FactoidData(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool fDelete;
	bool fXMLBased;
	OSHARED::PropertyBag propertyBag;
};

} // namespace XLS

