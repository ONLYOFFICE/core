#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class SecurityDescriptor : public BiffStructure
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SecurityDescriptor)
public:
	SecurityDescriptor();
	SecurityDescriptor(CFRecord& record);
	BiffStructurePtr clone();

	//virtual void toXML(BiffStructurePtr & parent);
	//virtual const bool fromXML(BiffStructurePtr & parent);
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

private:
	SECURITY_DESCRIPTOR sd;
};

} // namespace XLS

