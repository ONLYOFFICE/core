#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;

class CFRecord;

class SqRefU : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SqRefU)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const CellRef getLocationFirstCell() const;

private:
	_bstr_t sqref;
};

} // namespace XLS

