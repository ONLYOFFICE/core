#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/SecurityDescriptor.h>

namespace XLS
{

class CFRecord;

class SDContainer : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SDContainer)
public:
	SDContainer();
	SDContainer(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeSDContainer;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	SecurityDescriptor sd;
};

} // namespace XLS

