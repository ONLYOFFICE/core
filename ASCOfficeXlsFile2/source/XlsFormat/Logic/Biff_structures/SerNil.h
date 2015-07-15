#pragma once

#include "SerAr.h"

namespace XLS
{;

class CFRecord;

class SerNil : public SerAr
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SerNil)
public:
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;
	static const unsigned char fixed_type = 0x00;
};

} // namespace XLS
