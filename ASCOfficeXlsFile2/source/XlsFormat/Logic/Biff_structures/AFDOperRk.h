#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class AFDOperRk: public BiffStructure
{	
	BASE_OBJECT_DEFINE_CLASS_NAME(AFDOperRk)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);	

public:
	BIFF_DWORD rk;
};

} // namespace XLS