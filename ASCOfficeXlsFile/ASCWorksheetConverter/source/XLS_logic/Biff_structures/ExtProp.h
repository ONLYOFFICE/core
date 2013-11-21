#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/FullColorExt.h>
#include <XLS_logic/Biff_structures/XFExtGradient.h>

namespace XLS
{;

class CFRecord;

class ExtProp : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ExtProp)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD extType;
	WORD cb;
	struct extPropData_Tag
	{
		FullColorExt color;
		XFExtGradient gradient_fill;
		BYTE font_scheme;
		WORD indent_level;

	} extPropData;
};

typedef boost::shared_ptr<ExtProp> ExtPropPtr;

} // namespace XLS
