#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_records/AutoFilter.h>


namespace XLS
{;

class CFRecord;

class Feat11FdaAutoFilter : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Feat11FdaAutoFilter)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	BIFF_DWORD cbAutoFilter;
	AutoFilter recAutoFilter;
};

typedef boost::shared_ptr<Feat11FdaAutoFilter> Feat11FdaAutoFilterPtr;

} // namespace XLS

