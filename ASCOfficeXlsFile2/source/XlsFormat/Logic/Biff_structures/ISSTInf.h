#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class ISSTInf : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ISSTInf)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned int ib;
	unsigned __int16 cbOffset;
};

typedef boost::shared_ptr<ISSTInf> ISSTInfPtr;

} // namespace XLS

