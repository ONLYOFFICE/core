#pragma once

#include "BiffStructTagged.h"
#include <XLS_bin/CFRecordType.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class FrtHeader : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FrtHeader)
public:
	FrtHeader();
	FrtHeader(const CFRecordType::TypeId type);

	BiffStructurePtr clone();

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
private:
	CFRecordType::TypeId rt;
	FrtFlags grbitFrt;
};

typedef boost::shared_ptr<FrtHeader> FrtHeaderPtr;

} // namespace XLS

