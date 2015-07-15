#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class RevLblName : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(RevLblName)
public:
	RevLblName();
	RevLblName(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	//virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	const std::wstring toString() const;

private:
	std::wstring  st;
};

} // namespace XLS

