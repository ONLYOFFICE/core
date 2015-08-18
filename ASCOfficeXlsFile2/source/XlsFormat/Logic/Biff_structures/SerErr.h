#pragma once

#include "SerAr.h"
#include "BitMarkedStructs.h"

namespace XLS
{

class SerErr : public SerAr
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SerErr)
public:
	SerErr();
	SerErr(const std::wstring& word);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

private:
    const SerType fixed_type = typeSerErr;
	BErr err;
};


} // namespace XLS
