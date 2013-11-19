#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/CFColor.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class SheetExtOptional : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SheetExtOptional)
public:
	SheetExtOptional();
	SheetExtOptional(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	BYTE icvPlain12;
	bool fCondFmtCalc;
	bool fNotPublished;
	CFColor color;
};

} // namespace XLS

