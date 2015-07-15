#pragma once

#include "BiffStructure.h"
//#include <Logic/Biff_structures/Phs.h>
//#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class DXFProt : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(DXFProt)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool fLocked;
	bool fHidden;
};

} // namespace XLS

