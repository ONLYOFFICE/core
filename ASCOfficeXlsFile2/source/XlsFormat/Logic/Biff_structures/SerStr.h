#pragma once

#include "SerAr.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{

class SerStr : public SerAr
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(SerStr)
public:
	SerStr();
	SerStr(const std::wstring& word);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

private:
    const SerType fixed_type = typeSerStr;
    XLUnicodeString string_;
};



} // namespace XLS
