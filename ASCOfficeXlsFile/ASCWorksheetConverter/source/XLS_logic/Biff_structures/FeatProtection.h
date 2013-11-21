#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/SDContainer.h>
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class FeatProtection : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FeatProtection)
public:
	FeatProtection();
	FeatProtection(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	bool fSD;
	DWORD wPassword;
	XLUnicodeString stTitle;
	SDContainer sdContainer;
	bool sdContainer_set;
};

} // namespace XLS

