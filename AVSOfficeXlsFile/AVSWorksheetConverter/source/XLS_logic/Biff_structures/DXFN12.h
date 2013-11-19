#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/DXFN.h>
#include <XLS_logic/Biff_structures/XFExtNoFRT.h>

namespace XLS
{;

class CFRecord;

class DXFN12 : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFN12)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	DWORD cbDxf;
	DXFNPtr dxfn;
	XFExtNoFRTPtr xfext;
};

} // namespace XLS

