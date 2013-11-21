#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class ISSTInf : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ISSTInf)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	DWORD ib;
	WORD cbOffset;
};

typedef boost::shared_ptr<ISSTInf> ISSTInfPtr;

} // namespace XLS

