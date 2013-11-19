#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/MOper.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class ExternOleDdeLink : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(ExternOleDdeLink)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	DWORD lStgName;
	ShortXLUnicodeString linkName;
	MOper moper;
};

} // namespace XLS

