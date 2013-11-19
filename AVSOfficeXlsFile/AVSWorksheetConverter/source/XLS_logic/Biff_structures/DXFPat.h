#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class DXFPat : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFPat)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	FillPattern fls;
	BYTE icvForeground;
	BYTE icvBackground;
};

} // namespace XLS

