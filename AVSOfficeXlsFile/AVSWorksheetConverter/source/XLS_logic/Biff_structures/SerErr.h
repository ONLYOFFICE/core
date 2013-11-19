#pragma once

#include "SerAr.h"
#include "BitMarkedStructs.h"

namespace XLS
{;

class CFRecord;

class SerErr : public SerAr
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SerErr)
public:
	SerErr();
	SerErr(const std::wstring& word);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

	static const BYTE fixed_type = 0x10;
private:
	BErr err;
};

} // namespace XLS
