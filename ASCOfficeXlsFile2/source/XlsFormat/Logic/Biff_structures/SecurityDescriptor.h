#pragma once

#include "BiffStructure.h"

namespace XLS
{;

class CFRecord;

class SecurityDescriptor : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SecurityDescriptor)
public:
	SecurityDescriptor();
	SecurityDescriptor(CFRecord& record);
	BiffStructurePtr clone();

	static const ElementType	type = typeSecurityDescriptor;

	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	SECURITY_DESCRIPTOR sd;
};

} // namespace XLS

