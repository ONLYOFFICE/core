#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/FactoidType.h>
#include <Logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class PropertyBagStore : public XLS::BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(PropertyBagStore)
public:
	XLS::BiffStructurePtr clone();

	static const XLS::ElementType	type = XLS::typePropertyBagStore;
	
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

	std::vector<FactoidType> factoidTypes;
	unsigned short cbHdr;
	unsigned short sVer;
	std::vector<PBString> stringTable;
};

} // namespace OSHARED

