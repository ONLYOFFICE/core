#pragma once

#include "BiffStructure.h"
#include <XLS_logic/Biff_structures/SecurityDescriptor.h>

namespace XLS
{;

class CFRecord;

class SDContainer : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SDContainer)
public:
	SDContainer();
	SDContainer(CFRecord& record);
	BiffStructurePtr clone();

	virtual void toXML(MSXML2::IXMLDOMElementPtr parent);
	virtual const bool fromXML(MSXML2::IXMLDOMElementPtr parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	SecurityDescriptor sd;
};

} // namespace XLS

