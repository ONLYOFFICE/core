#pragma once

#include "SerAr.h"

namespace XLS
{;

class CFRecord;

class SerBool : public SerAr
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SerBool)
public:
	SerBool();
	SerBool(const std::wstring& word); // Accepts only "TRUE" or "FALSE"
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

	static const unsigned char fixed_type = 0x04;
private:
	unsigned char f;
};

} // namespace XLS
