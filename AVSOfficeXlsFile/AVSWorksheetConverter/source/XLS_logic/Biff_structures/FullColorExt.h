#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class FullColorExt : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FullColorExt)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	WORD xclrType;
	short nTintShade;
	DWORD xclrValue;
};

} // namespace XLS
