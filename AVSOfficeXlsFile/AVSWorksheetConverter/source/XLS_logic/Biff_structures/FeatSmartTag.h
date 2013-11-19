#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/FactoidData.h>
//#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class FeatSmartTag : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(FeatSmartTag)
public:
	FeatSmartTag();
	FeatSmartTag(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	DWORD hashValue;
	std::vector<FactoidData> rgFactoid;
};

} // namespace XLS

