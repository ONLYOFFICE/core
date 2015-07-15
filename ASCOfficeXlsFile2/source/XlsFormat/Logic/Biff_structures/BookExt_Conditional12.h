#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class BookExt_Conditional12 : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(BookExt_Conditional12)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool fPublishedBookItems;
	bool fShowPivotChartFilter;
};

} // namespace XLS

