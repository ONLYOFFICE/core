#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/XFProp.h>

namespace XLS
{;

class CFRecord;

class XFProps : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(XFProps)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD cprops;
	std::vector<XFProp> xfPropArray;
};

} // namespace XLS
