#pragma once

#include "BiffStructure.h"
#include <Binary/CFRecordType.h>
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class FrtHeaderOld : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(FrtHeaderOld)
public:
	FrtHeaderOld();
	FrtHeaderOld(const CFRecordType::TypeId type);

	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	CFRecordType::TypeId rt;
	FrtFlags grbitFrt;
};

} // namespace XLS

