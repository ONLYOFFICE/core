#pragma once

#include "SerAr.h"

namespace XLS
{

class SerNil : public SerAr
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SerNil)
public:
    SerNil();
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

};

} // namespace XLS
