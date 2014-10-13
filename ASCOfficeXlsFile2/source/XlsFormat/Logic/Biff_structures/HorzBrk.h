#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BitMarkedStructs.h>

namespace XLS
{;

class CFRecord;

class HorzBrk : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(HorzBrk)
public:
	BiffStructurePtr clone();

	//virtual void setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	//virtual void getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	RwU row;
	unsigned __int16 colStart;
	unsigned __int16 colEnd;
};

typedef boost::shared_ptr<HorzBrk> HorzBrkPtr;

} // namespace XLS

