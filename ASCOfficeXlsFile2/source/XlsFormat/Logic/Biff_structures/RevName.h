#pragma once

#include "Ptg.h"
#include "BiffString.h"
#include "RevNameTabid.h"
#include "RevNamePly.h"

namespace XLS
{;

class CFRecord;

class RevName : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevName)
public:
	RevName();
	RevName(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record); // No type info
	virtual void store(CFRecord& record); // No type info

private:
	VirtualPath book;
	unsigned char fExtern;
	RevNameTabid name;
	RevNamePly externName;
};

typedef boost::shared_ptr<RevName> RevNamePtr;


} // namespace XLS

