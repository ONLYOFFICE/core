#pragma once

#include "Ptg.h"
#include "BiffString.h"
#include "RevItab.h"

namespace XLS
{;

class CFRecord;

class RevExtern : public Ptg
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RevExtern)
public:
	RevExtern();
	RevExtern(CFRecord& record);
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record); // No type info
	virtual void store(CFRecord& record); // No type info

private:
	VirtualPath book;
	RevItab itabFirst;
	RevItab itabLast;
};

typedef boost::shared_ptr<RevExtern> RevExternPtr;

} // namespace XLS

