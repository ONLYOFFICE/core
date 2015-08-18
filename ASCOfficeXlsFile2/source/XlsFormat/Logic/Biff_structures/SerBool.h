#pragma once

#include "SerAr.h"

namespace XLS
{

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

private:
    const SerType fixed_type = typeSerBool;
    unsigned char f;
};

} // namespace XLS
