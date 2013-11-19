#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class DXFNumUsr : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(DXFNumUsr)
public:
	DXFNumUsr();
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD cb;
	XLUnicodeString fmt;
};

} // namespace XLS

