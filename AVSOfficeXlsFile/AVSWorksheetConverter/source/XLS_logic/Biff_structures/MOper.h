#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/SerAr.h>
#include <XLS_logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class MOper : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(MOper)
public:
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	ColByteU colLast;
	RwU rowLast;
	std::vector<SerArPtr> extOper;
};

} // namespace XLS

