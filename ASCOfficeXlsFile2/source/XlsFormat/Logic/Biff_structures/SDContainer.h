#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/SecurityDescriptor.h>

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

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	SecurityDescriptor sd;
};

} // namespace XLS

