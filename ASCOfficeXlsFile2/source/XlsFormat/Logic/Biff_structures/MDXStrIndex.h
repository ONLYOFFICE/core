#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class MDXStrIndex : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXStrIndex)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	unsigned int index;
};

typedef boost::shared_ptr<MDXStrIndex> MDXStrIndexPtr;

} // namespace XLS

