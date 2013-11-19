#pragma once

#include "BiffStructure.h"
#include "BiffString.h"

namespace XLS
{;

class CFRecord;

class RevItab : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(RevItab)
public:
	RevItab();
	RevItab(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE type;
	WORD tabid;
	XLUnicodeString sheet;
};

} // namespace XLS

