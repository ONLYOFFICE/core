#pragma once

#include <XLS_logic/Biff_records/BiffRecord.h>
#include <XLS_logic/Biff_structures/BiffString.h>
#include <XLS_logic/Biff_structures/DXFN.h>
#include <XLS_logic/Biff_structures/XFExtNoFRT.h>

namespace XLS
{;

class DXFN12List: public BiffStructTagged
{	
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFN12List)
public:
	BiffStructurePtr clone();

	DXFN12List();
	~DXFN12List();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	DXFN dxfn;
	XFExtNoFRT xfext;
};

typedef boost::shared_ptr<DXFN12List> DXFN12ListPtr;

} // namespace XLS