#pragma once

#include <XLS_logic/Biff_records/BiffRecord.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class ControlInfo : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ControlInfo)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

public:
	BIFF_BYTE fDefault;
	BIFF_BYTE fHelp;
	BIFF_BYTE fCancel;
	BIFF_BYTE fDismiss;
	BIFF_WORD accel1;
};

} // namespace XLS

