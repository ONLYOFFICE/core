#pragma once

#include "Ptg.h"
#include "RevLblName.h"

namespace XLS
{;

class CFRecord;

class RevNameTabid : public Ptg
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevNameTabid)
public:
	RevNameTabid();
	RevNameTabid(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record); // No type info
	virtual void store(CFRecord& record); // No type info

	const std::wstring toString() const;

private:
	unsigned __int16 tabid;
	RevLblName name;
};

typedef boost::shared_ptr<RevNameTabid> RevNameTabidPtr;

} // namespace XLS

