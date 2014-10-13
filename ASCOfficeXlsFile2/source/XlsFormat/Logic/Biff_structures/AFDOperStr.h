#pragma once

#include <Logic/Biff_records/BiffRecord.h>
#include <Logic/Biff_structures/BiffString.h>


namespace XLS
{;

class AFDOperStr : public BiffStructure
{	
	BASE_OBJECT_DEFINE_CLASS_NAME(AFDOperStr)
public:
	BiffStructurePtr clone();

	AFDOperStr();
	AFDOperStr(bool bAutoFilter);
	~AFDOperStr();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	BIFF_BYTE cch;
	BIFF_BYTE fCompare;	

public:
	bool m_bAutoFilter;
};

} // namespace XLS