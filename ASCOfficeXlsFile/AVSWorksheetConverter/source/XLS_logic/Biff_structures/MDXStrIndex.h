#pragma once

#include "BiffStructTagged.h"

namespace XLS
{;

class CFRecord;

class MDXStrIndex : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MDXStrIndex)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	DWORD index;
};

typedef boost::shared_ptr<MDXStrIndex> MDXStrIndexPtr;

} // namespace XLS

