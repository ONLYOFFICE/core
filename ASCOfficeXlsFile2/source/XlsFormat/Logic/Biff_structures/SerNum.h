#pragma once

#include "SerAr.h"

namespace XLS
{;

class CFRecord;

class SerNum : public SerAr
{
	BASE_OBJECT_DEFINE_CLASS_NAME(SerNum)
public:
	SerNum();
	SerNum(const std::wstring& word);
	BiffStructurePtr clone();
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);
	virtual const std::wstring toString() const;

	static const unsigned char fixed_type = 0x01;
private:
	double xnum;
};

} // namespace XLS
