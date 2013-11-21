#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/FactoidType.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace OSHARED
{;

class CFRecord;

class PropertyBagStore : public XLS::BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(PropertyBagStore)
public:
	XLS::BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(XLS::CFRecord& record);
	virtual void store(XLS::CFRecord& record);

private:
	std::vector<FactoidType> factoidTypes;
	WORD cbHdr;
	WORD sVer;
	std::vector<PBString> stringTable;
};

} // namespace OSHARED

