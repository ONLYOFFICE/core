#pragma once

#include "BiffStructTagged.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class AddinUdf : public BiffStructTagged
{
	BASE_OBJECT_DEFINE_CLASS_NAME(AddinUdf)
public:
	BiffStructurePtr clone();

	virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	ShortXLUnicodeString udfName;
};

} // namespace XLS

