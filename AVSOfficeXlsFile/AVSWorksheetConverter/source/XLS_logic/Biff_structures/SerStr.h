#pragma once

#include "SerAr.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;

class CFRecord;

class SerStr : public SerAr
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SerStr)
public:
	SerStr();
	SerStr(const std::wstring& word);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

	static const BYTE fixed_type = 0x02;
private:
	XLUnicodeString string_;
};

} // namespace XLS
