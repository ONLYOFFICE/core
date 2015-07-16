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

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	std::vector<FactoidType> factoidTypes;
	unsigned short cbHdr;
	unsigned short sVer;
	std::vector<PBString> stringTable;
};

} // namespace OSHARED

